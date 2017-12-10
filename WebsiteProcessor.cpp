#include "WebsiteProcessor.h"
using namespace std;
#include <fstream>
#include <string>
#include "Stack.h"
#include <vector>
#include <QtNetwork/QNetworkAccessManager>
#include "convert_qstr_str.h"
#include <QtCore>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QObject>
#include <QTextCodec>
#include <QTextStream>
WebsiteProcessor::WebsiteProcessor(void)
{
}


WebsiteProcessor::~WebsiteProcessor(void)
{
}

void WebsiteProcessor::downloadWebsite(const CharString& url, const CharString& filename){ //下载url网页到本地文件，返回文件名
    QUrl qUrl ( charStr2qstr(url) );
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(qUrl));//发送下载网页源码的请求
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//当下载成功后，结束连接
    loop.exec();

    QFile file(charStr2qstr(filename));//打开本地文件
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Can not open the file " << charStr2qstr( filename );
        exit(-1);
    }
    QTextStream out(&file);
    QTextCodec *codec = QTextCodec::codecForName("GBK");//设置网页的编码为GBK编码
    out << codec->toUnicode(reply->readAll()) << endl;//将下载的网页源码写入本地文件
    file.close();
}

std::shared_ptr<CharString> WebsiteProcessor::downloadWebsite(const CharString& url){ //下载url网页,返回字符串
    QUrl qUrl ( charStr2qstr(url) );
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(qUrl));//发送下载网页源码的请求
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//当下载成功后，结束连接
    loop.exec();
    QTextCodec *codec = QTextCodec::codecForName("GBK");//设置网页的编码为GBK编码
    std::shared_ptr<CharString> sourceCode =
            std::make_shared<CharString>(qstr2str(codec->toUnicode(reply->readAll())));//将源码存到sourceCode中
    return sourceCode;
}


NodePosi WebsiteProcessor::readOnePairOfBracket(const CharString& string, int& i){
	//从字符串string的第i个位置开始读取一对匹配的括号，
	//并返回这对括号组成的节点
	NodePosi node(new Node());
	node->left = i; //左括号的位置
	node->right = string.indexOf(">", i+1);//从i+1的位置开始找">"，找到的位置即为右括号的位置
	i = node->right;//此后从右括号的右边开始找起
	
	CharString sub = string.subString(node->left, node->right+1);//求取原来字符串在上述左右括号间的子串
	if(sub[1] == '/'){
		node->matchingType = Right;
	}else if(sub[sub.size()-2] == '/' || sub[1] == '!'){
		node->matchingType = SelfMatched;
	}else{
		node->matchingType = Left;
		int j = sub.indexOf(" ", 1);//j指示子串中第一次出现空格的位置
		if(j < 0){//如果空格不存在，
			node->m_tag = sub.subString(1, sub.size()-1);//则两个括号之间只有标签的内容
		}else{//如果存在括号，
			int k1 = sub.indexOf("class=", j+1);//k1指示类class的位置
			if(k1 >= 0){//如果括号之间存在类，
				k1 = sub.indexOf("\"", k1+1);//k1指示左引号的位置
				int k2 = sub.indexOf("\"", k1+1);//k2指示右引号的位置
				node->m_class = sub.subString(k1+1, k2);//[k1+1, k2)就是引号中的内容
			}
			node->m_tag = sub.subString(1, j);//左括号与空格之间的为标签的内容
		}
	}
	return node;
}

CharString WebsiteProcessor::removeTag(const CharString& string){//求取一个字符串的文本信息，去除所有的标签信息
	CharString result;
	int right=0, left=0;//right标记上一个匹配的括号的右边位置+1，left标记当前匹配的括号的左边位置
	while(true){
		left = getFirstLeftBracket(string, left);//找到下一个匹配括号的左边位置
		if(left<0){//如果已经找到了尽头
			if(right < string.size()){
				result.concat(string.subString(right, string.size()) );
			}
			break;
		}
		NodePosi node = readOnePairOfBracket(string, left);
		//找到字符串中的第一个匹配的括号[node->left, node->right+1),同时left被更新到node->right+1

		if( right < node->left ){//[right, node->left)之间存在文本
			result.concat(string.subString(right, node->left));//将文本加入结果中
		}
		right = node->right+1;//right变为当前匹配括号的右边位置加一
	}
	return result;
}

CharString WebsiteProcessor::getOuterText(const CharString& string){//删除一个字符串中所有的标签，只保留最外层的文本
    CharString result;
    Stack<NodePosi> nodeStack;//存储所有节点类型为左界的节点，并且这些节点对应的右界都还没有扫描到
    int left = 0;
    for(int i=0; i<string.size(); ){
        i = getFirstLeftBracket(string, i);//找到第一个左括号
        if(i<0)//如果此时已经找不到多余的括号了，
            break;//则停止查找

        if(left < i && nodeStack.empty()){//如果此时[left,i)之间存在外层文本
            result.concat(string.subString(left, i));//则将其加入结果
        }

        NodePosi node = readOnePairOfBracket(string, i);
        //读入一对匹配的括号所构成的节点,并且i被自动更新到右括号右边的位置
        if(!node) break;//如果不存在node，则可退出循环
        left = i+1;//更新left的位置

        switch (node->matchingType)//根据节点的类型,若当前节点是
        {
        case SelfMatched://自匹配，不做处理
            break;
        case Left://左界
            nodeStack.push(node);
            break;
        case Right://右界
            if(!nodeStack.empty() && nodeStack.top()->m_tag == node->m_tag)//如果当前节点的标签和栈顶标签一致
                nodeStack.pop();//则将栈顶元素弹出
        }
    }

    if(left < string.size() && nodeStack.empty()){//如果此时[left,size)之间存在外层文本
        result.concat(string.subString(left, string.size()));//则将其加入结果
    }

    return result;
}

int WebsiteProcessor::getFirstLeftBracket(const CharString& string, int i){
	//获得字符串string从第i个位置起第一个左括号的位置
	return string.indexOf("<", i);
}

void WebsiteProcessor::processSourceCode(std::shared_ptr<const CharString> sourceCode, std::ofstream& out, bool removeUselessWords){
    //-----------------接下来对字符串string进行处理----------------------------------
    Stack<CharString> divClassStack;//存储当前位置外层的所有div的class
    divClassStack.push(CharString(""));//在栈底加一个哨兵

    Stack<NodePosi> nodeStack;//存储已经所有节点类型为左界的节点，并且这些节点对应的右界都还没有扫描到

    vector<shared_ptr<CharString>> z_a;//发帖大类，发帖小类、发帖标题
    vector<shared_ptr<CharString>> td_tf;//正文
    vector<shared_ptr<CharString>> authi_a;//发帖人
    vector<shared_ptr<CharString>> authi_em;//发帖日期
    vector<shared_ptr<CharString>> tszh1_a;//发帖分类、发帖标题

    Record record;//一条记录

    for(int i=0; i<sourceCode->size(); ){
        i = getFirstLeftBracket(*sourceCode, i);//找到第一个左括号
        if(i<0)//如果此时已经找不到多余的括号了，
            break;//则停止查找

        NodePosi node = readOnePairOfBracket(*sourceCode, i);
        //读入一对匹配的括号所构成的节点,并且i被自动更新到右括号右边的位置

        switch (node->matchingType)//根据节点的类型,若当前节点是
        {
        case SelfMatched://自匹配，不做处理
            break;
        case Left://左界
            if(node->m_tag == "div"){//若节点的标签是"div"
                divClassStack.push(node->m_class);//则记录节点的class
            }
            nodeStack.push(node);//并将节点压栈
            break;
        case Right://右界
            if(nodeStack.empty()){//假如栈中没有节点,说明出现问题
                std::cout << "error in  match"<<endl;
                break;
            }else if(nodeStack.top()->m_tag == "a"){//若节点的标签是"a"
                if(divClassStack.top() == "z"){//并且所在的div为z，则
                    shared_ptr<CharString> content(new CharString);//对应发帖大类、发帖小类、发帖标题
                    *content = sourceCode->subString(nodeStack.top()->right+1, node->left);//取得文本内容
                    z_a.push_back(content);//将其存储在z_a中
                }else if(divClassStack.top() == "ts z h1"){//并且所在的div为ts z h1,则
                    shared_ptr<CharString> content(new CharString);//对应发帖分类、发帖标题
                    *content = removeTag( sourceCode->subString(nodeStack.top()->right+1, node->left) );//只保留标签嵌套内的文本
                    tszh1_a.push_back(content);//将其存储在tszh1_a中
                }else if(divClassStack.top() == "authi"){//并且所在的div为authi，则
                    shared_ptr<CharString> content(new CharString);//对应发帖人
                    *content = sourceCode->subString(nodeStack.top()->right+1, node->left);//取得文本内容
                    authi_a.push_back(content);//将其存储在authi_a中
                }
            }else if(nodeStack.top()->m_tag == "td"){//如果是标签为td
                if(nodeStack.top()->m_class == "t_f"){//并且类型为t_f,则
                    shared_ptr<CharString> content(new CharString);//对应发帖内容
                    //std::cout << sourceCode->subString(nodeStack.top()->right+1, node->left) << std::endl;
                    //*content = getText( sourceCode->subString(nodeStack.top()->right+1, node->left) );//除去多余的标签内容
                    *content = getOuterText( sourceCode->subString(nodeStack.top()->right+1, node->left) );//只保留外层文本
                    content->removeSpace();//除去多余的空格
                    td_tf.push_back(content);//存入td_tf中
                }
            }else if(nodeStack.top()->m_tag == "em"){//如果标签是em
                if(divClassStack.top() == "authi"){//并且所在的div为authi，则
                    shared_ptr<CharString> content(new CharString);//对应发帖日期
                    *content = sourceCode->subString(nodeStack.top()->right+7, node->left-9);//截取日期段
                    authi_em.push_back(content);//将其存在authi_em中
                }
            }else if(nodeStack.top()->m_tag == "div"){//如果标签是div
                divClassStack.pop();//则最内层的div结束
            }
            nodeStack.pop();//节点出栈
            break;
        default:
            break;
        }
    }

    if(z_a.size()<5 || tszh1_a.empty() || authi_a.empty() || authi_em.empty()//对于无法获取信息的情况
        || td_tf.empty()//发帖内容
            ){
            out << endl;
            return;
    }
    record.category = *z_a[2];//大类
    record.subclass = *z_a[3];//小类
    record.title = *z_a[4];//标题
    record.type = *tszh1_a[0];//类型
    record.userName = *authi_a[0];//发帖人
    record.date = *authi_em[0];//发帖日期
    record.content = UnicodeToChinese(*td_tf[0]);//发帖内容

    //将结果输出到输出流
    out << record.category << "||" << record.subclass << "||" << record.title << "||"
        << record.content << "||" << record.userName << "||" << record.date << "||"
        << record.type << "||"
        << divideWords(record.title, removeUselessWords) << divideWords(record.content, removeUselessWords)//输出分词结果
        << endl;
}

void WebsiteProcessor::processHtml(const CharString& htmlText, std::ofstream& out, bool removeUselessWords){
	//处理本地文件htmlText，并将结果输出到out中 	
	
	ifstream outfile; //文件输出流
	unique_ptr<char> data( htmlText.data() );//字符串htmlText对应的字符数组
	outfile.open(data.get());//打开本地文件htmlText
	if(!outfile){
		cout << "文件打开失败"<< data.get()<<endl;
	}else{
		string temp( (istreambuf_iterator<char>(outfile)), istreambuf_iterator<char>() );
		//从文件读取数据到stl字符串temp
        //CharString string(temp);//然后将其转化成CharString
        std::shared_ptr<CharString> sourceCode = std::make_shared<CharString>(temp);
        processSourceCode(sourceCode, out, removeUselessWords);
        /*
        //-----------------接下来对字符串string进行处理----------------------------------
		
		Stack<CharString> divClassStack;//存储当前位置外层的所有div的class
		divClassStack.push(CharString(""));//在栈底加一个哨兵

		Stack<NodePosi> nodeStack;//存储已经所有节点类型为左界的节点，并且这些节点对应的右界都还没有扫描到

		vector<shared_ptr<CharString>> z_a;//发帖大类，发帖小类、发帖标题
		vector<shared_ptr<CharString>> td_tf;//正文
		vector<shared_ptr<CharString>> authi_a;//发帖人
		vector<shared_ptr<CharString>> authi_em;//发帖日期
		vector<shared_ptr<CharString>> tszh1_a;//发帖分类、发帖标题
		
		Record record;//一条记录
		record.url = htmlText;//修改记录的url

		for(int i=0; i<string.size(); ){
			i = getFirstLeftBracket(string, i);//找到第一个左括号
			if(i<0)//如果此时已经找不到多余的括号了，
				break;//则停止查找
			
			NodePosi node = readOnePairOfBracket(string, i);
			//读入一对匹配的括号所构成的节点,并且i被自动更新到右括号右边的位置
			
			switch (node->matchingType)//根据节点的类型,若当前节点是
			{
			case SelfMatched://自匹配，不做处理
				//cout << "self "<< node->m_tag << endl;
				break;
			case Left://左界
				if(node->m_tag == "div"){//若节点的标签是"div"
					divClassStack.push(node->m_class);//则记录节点的class
				}
				nodeStack.push(node);//并将节点压栈
				//cout << "<"<< node->m_tag << endl;
				break;
			case Right://右界
				//cout << ">" << node->m_tag << endl;
				if(nodeStack.empty()){//假如栈中没有节点,说明出现问题
					std::cout << "error in  match"<<endl;
					break;
				}else if(nodeStack.top()->m_tag == "a"){//若节点的标签是"a"
					if(divClassStack.top() == "z"){//并且所在的div为z，则
						shared_ptr<CharString> content(new CharString);//对应发帖大类、发帖小类、发帖标题
						*content = string.subString(nodeStack.top()->right+1, node->left);//取得文本内容
						z_a.push_back(content);//将其存储在z_a中
					}else if(divClassStack.top() == "ts z h1"){//并且所在的div为ts z h1,则
						shared_ptr<CharString> content(new CharString);//对应发帖分类、发帖标题
						*content = string.subString(nodeStack.top()->right+2, node->left-1);//此处将外侧的"[" "]"一并去除了,得到文本内容
						tszh1_a.push_back(content);//将其存储在tszh1_a中
					}else if(divClassStack.top() == "authi"){//并且所在的div为authi，则
						shared_ptr<CharString> content(new CharString);//对应发帖人
						*content = string.subString(nodeStack.top()->right+1, node->left);//取得文本内容
						authi_a.push_back(content);//将其存储在authi_a中
					}
				}else if(nodeStack.top()->m_tag == "td"){//如果是标签为td
					if(nodeStack.top()->m_class == "t_f"){//并且类型为t_f,则
						shared_ptr<CharString> content(new CharString);//对应发帖内容
						*content = getText( string.subString(nodeStack.top()->right+1, node->left) );//除去多余的标签内容
						content->removeSpace();//除去多余的空格
						td_tf.push_back(content);//存入td_tf中
					}
				}else if(nodeStack.top()->m_tag == "em"){//如果标签是em
					if(divClassStack.top() == "authi"){//并且所在的div为authi，则
						shared_ptr<CharString> content(new CharString);//对应发帖日期
						*content = string.subString(nodeStack.top()->right+7, node->left-9);//截取日期段
						authi_em.push_back(content);//将其存在authi_em中
					}
				}else if(nodeStack.top()->m_tag == "div"){//如果标签是div
					divClassStack.pop();//则最内层的div结束
				}
				nodeStack.pop();//节点出栈
				break;
			default:
				break;
			}
		}

		if(z_a.size()<5 || tszh1_a.empty() || authi_a.empty() || authi_em.empty()//对于无法获取信息的情况
			|| td_tf.empty()//发帖内容
				){
				out << endl;
				return;
		}
		record.category = *z_a[2];//大类
		record.subclass = *z_a[3];//小类
		record.title = *z_a[4];//标题
		record.type = *tszh1_a[0];//类型
		record.userName = *authi_a[0];//发帖人
		record.date = *authi_em[0];//发帖日期
        record.content = UnicodeToChinese(*td_tf[0]);//发帖内容
		
		//将结果输出到输出流
		out << record.category << "||" << record.subclass << "||" << record.title << "||"
			<< record.content << "||" << record.userName << "||" << record.date << "||" 
			<< record.type << "||" 
			<< divideWords(record.title, removeUselessWords) << divideWords(record.content, removeUselessWords)//输出分词结果
			<< endl;

            */
		outfile.close();//关闭文件
	}
}


void WebsiteProcessor::readURL(std::ifstream& in, CharString& url)//读入输入流in中的网页，得到网页的url
{
	string temp;
	getline(in, temp);//输入文件中的一行
	CharString line(temp);//将其转化为CharString
	int left = line.indexOf("\"",0);//找到左引号的位置
	int right = line.indexOf("\"", left+1);//找到右引号的位置
	line.subString(left+1, right, url);//截取引号之间的内容存到url中
}

void WebsiteProcessor::process(std::ifstream& in, std::ofstream& out, bool removeUselessWords){
	//处理输入流in中的所有网页，并将结果输出到输出流out中
    //如果removeUselessWords为true，则分词结果就会删除无用词；为false，则保留无用

	string temp;
	getline(in, temp);//读入的第一行的内容无用
	//输出第一行内容
	out << "\"序号\",\"网址\",\"发帖大类\",\"发帖小类\",\"发帖标题\"," ;
	out << "\"发帖内容\",\"发帖人\",\"发帖日期\",\"发帖类型\",\"分词结果\"";
	out << endl;
	int id = 1;

	while(!in.eof()){//一直输入到文件末尾
		CharString url;
		readURL(in, url);//读入网页的url
		if(in.eof()) break;
        CharString filename("temp.html");//本地文件
		cout << "downloading "<< url<<endl;
        //downloadWebsite(url, filename);//将网页源码下载到本地文件
        std::shared_ptr<const CharString> sourceCode = downloadWebsite(url);//将网页下载到字符串sourceCode
		out << id++ << "||" << url << "||" ;//先输出序号和网页url
		cout << "processing "<< url<<endl;
        processSourceCode(sourceCode, out, removeUselessWords);//然后处理源码，将处理后的信息输出到输出流
        //processHtml(filename, out, removeUselessWords);//然后处理本地文件filename，将处理后的信息输出到输出流
        cout << "finishing "<< id << " websites"<<endl;
	}
}

void WebsiteProcessor::extractInfo(const CharString& inputFile, const CharString& outputFile, bool removeUselessWords){
	//处理inputFile中的所有网页，将结果存储到outputFile中，失败的话返回false
	//如果removeUselessWords为true，则分词结果就会删除无用词；为false，则保留无用词
	ifstream in; in.open(inputFile.data());//打开存储网页url的文件
	if(!in){
		cout << "error in open " << inputFile <<endl;
		exit(-1);
	}
	ofstream out; out.open(outputFile.data());//打开提取信息存储的文件
	if(!out){
		cout << "error in open " << outputFile <<endl;
		exit(-1);
	}
	process(in, out, removeUselessWords);//进行处理
	in.close(); out.close();//关闭打开的文件
}

void WebsiteProcessor::initDictionary(){//初始化词库
	dictionary = make_shared<Dictionary>();//新建一个词库
	dictionary->init();//词库初始化
}

void WebsiteProcessor::initDictionary(const CharString& mainDictionary,const CharString& professionalTerm){//初始化词库
	dictionary = make_shared<Dictionary>();//新建一个词库
	dictionary->init(mainDictionary, professionalTerm);//词库初始化
}

CharStringLink WebsiteProcessor::divideWords(const CharString& sentence, bool removeUselessWords){//对句子进行分词
	//如果removeUselessWords为true，则分词结果就会删除无用词；为false，则保留无用词
	if(!dictionary){
		exit(-1);
	}
	return *dictionary->divideSentence(sentence, removeUselessWords);
}
