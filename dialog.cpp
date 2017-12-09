#include "dialog.h"
#include "ui_dialog.h"
#include "searchengine.h"
#include "CharString.h"
#include <QString>
#include <iostream>
#include <QDebug>
#include "convert_qstr_str.h"
#include "WebsiteProcessor.h"
#include "parameter.h"
#include <QFileInfo>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    standardItemModel = new QStandardItemModel(this);
    searchEngine = new SearchEngine(10);//设置权重为10
    searchEngine->initDictionary();//初始化词典
    QFileInfo downloadResultFile (DownloadResultFile);
    if(!downloadResultFile.isFile() ){//如果之前没有下载网页源码
        qDebug() << DownloadResultFile << " doesn't exist";
        WebsiteProcessor websiteProcessor;//则需要先将所有的网页源码下载到本地
        websiteProcessor.initDictionary();//初始化词典
        websiteProcessor.extractInfo(CharString(URLFile), CharString(DownloadResultFile), true);//浏览整个网页，去除无用词
    }
    searchEngine->buildInvertedFile(CharString(DownloadResultFile), CharString(InvertedDocumentFile));

    //CharString queryFile("query.txt");
    //CharString resultFile("result.txt");
    //searchEngine->query(queryFile, resultFile);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_queryButton_clicked()
{
    QString temp = ui->wordsInputEdit->text();
    CharString line(qstr2str(temp));//从输入读入一组句子
    std::vector<CharString> sentences = line.split(CharString(" "));//按照空格对其切分
    docList = searchEngine->searchSentences(sentences,std::cout);//得到文档链表
    ui->resultEdit->setText(str2qstr("为您找到相关结果共")+QString::number(docList->Size())+str2qstr("个"));//显示搜索的结果个数
    standardItemModel->clear();//首先清空原先的列表
    for(auto p = docList->first(); docList->isValid(p); p = docList->next(p)){//遍历文档链表
        qDebug() << "docList total weight "<< docList->totalWeight(p);
        int DocID = p->DocID; QString title;//将所有文档的标题
        title += (str2qstr("文档 ")+QString::number(DocID)+str2qstr(" : "));
        title += str2qstr(searchEngine->getNthRecord(DocID).title.data());
        QStandardItem *item = new QStandardItem(title);
        standardItemModel->appendRow(item);//都加入到
    }
    ui->detailedListView->setModel(standardItemModel);//列表显示中
}


void Dialog::on_detailedListView_doubleClicked(const QModelIndex &index)
{
    ui->detailedResultEdit->clear();//清空具体内容显示框
    int DocID = index.data().toString().split(" ").at(1).toInt();//得到被点击的那一行对应的文档编号
    Record record = searchEngine->getNthRecord(DocID);//得到对应的记录

    ui->detailedResultEdit->setOpenExternalLinks(false);
    ui->detailedResultEdit->setTextColor(Qt::blue);
    ui->detailedResultEdit->setFontWeight(QFont::Bold);
    ui->detailedResultEdit->setAlignment(Qt::AlignHCenter);
    ui->detailedResultEdit->setFontPointSize(15);
    ui->detailedResultEdit->append(str2qstr("标题:") + str2qstr(record.title.data()));//显示标题

    ui->detailedResultEdit->setTextColor(Qt::gray);
    ui->detailedResultEdit->setFontWeight(QFont::Normal);
    ui->detailedResultEdit->setFontPointSize(10);
    ui->detailedResultEdit->append(str2qstr("发帖人:")+str2qstr(record.userName.data()));//显示发帖人
    ui->detailedResultEdit->append(str2qstr("发帖日期:")+str2qstr(record.date.data()));//显示发帖日期

    ui->detailedResultEdit->setTextColor(Qt::black);
    ui->detailedResultEdit->append(str2qstr(""));
    ui->detailedResultEdit->setAlignment(Qt::AlignLeft);
    ui->detailedResultEdit->append(str2qstr(record.content.data()));//显示文档内容
    ui->detailedResultEdit->append(str2qstr(""));

    ui->detailedResultEdit->setOpenExternalLinks(true);
    ui->detailedResultEdit->setTextColor(Qt::gray);
    QString urlLink =  ("<style> a {text-decoration: none} </style> "
                        "<a href=\"" + charStr2qstr(record.url)+ "\" >"+
                        str2qstr("来源")+charStr2qstr( record.url)+
                        "</a>");//设置网页的链接
    ui->detailedResultEdit->append(urlLink);

    QString temp = ui->wordsInputEdit->text(); CharString line(qstr2str(temp));//从输入读入一组句子
    std::vector<CharString> sentences = line.split(CharString(" "));//按照空格对其切分
    CharStringLink wordList = searchEngine->getWordListFromSentence(sentences);//分词得到一组关键词

    QTextDocument *document = ui->detailedResultEdit->document();
    for(auto p = wordList.first(); wordList.isValid(p); p = wordList.next(p)){
        QString search_text = str2qstr(p->data.data());//对每个关键词,都进行一次高亮
        qDebug() << search_text;
        QTextCursor highlight_cursor(document);
        QTextCursor cursor(document);
        QTextCharFormat color_format(highlight_cursor.charFormat());
        color_format.setForeground(Qt::red);color_format.setFontPointSize(14);
        cursor.beginEditBlock();
        while(!highlight_cursor.isNull() && !highlight_cursor.atEnd()){
            highlight_cursor = document->find(search_text, highlight_cursor, QTextDocument::FindCaseSensitively);
            if(!highlight_cursor.isNull()){
                highlight_cursor.mergeCharFormat(color_format);
            }
        }
        cursor.endEditBlock();
    }
}
