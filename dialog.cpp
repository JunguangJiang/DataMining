#include "dialog.h"
#include "ui_dialog.h"
#include "searchengine.h"
#include "CharString.h"
#include <QString>
#include <iostream>
#include <QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    standardItemModel = new QStandardItemModel(this);
    searchEngine = new SearchEngine(10);//设置权重为10
    searchEngine->initDictionary();//初始化词典
    searchEngine->buildInvertedFile(CharString("output/result.csv"), CharString("output/invertedFile.txt"));

    //CharString queryFile("query.txt");
    //CharString resultFile("result.txt");
    //searchEngine->query(queryFile, resultFile);
}

Dialog::~Dialog()
{
    delete ui;
}

std::string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}

QString str2qstr(const std::string& str)
{
    return QString::fromLocal8Bit(str.data());
}

void Dialog::on_queryButton_clicked()
{
    QString temp = ui->wordsInputEdit->text();
    CharString line(qstr2str(temp));//从输入读入一组句子
    std::vector<CharString> sentences = line.split(CharString(" "));//按照空格对其切分
    std::shared_ptr<DocList> docList = searchEngine->searchSentences(sentences,std::cout);//得到文档链表
    ui->resultEdit->setText(str2qstr("为您找到相关结果共")+QString::number(docList->Size())+str2qstr("个"));//显示搜索的结果个数
    standardItemModel->clear();//首先清空原先的列表
    for(auto p = docList->first(); docList->isValid(p); p = docList->next(p)){//遍历文档链表
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
    CharString detailedText = searchEngine->getNthRecord(DocID).content;//得到对应的文档内容
    ui->detailedResultEdit->appendPlainText(str2qstr(detailedText.data()));//显示文档内容
}
