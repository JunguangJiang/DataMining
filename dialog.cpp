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

void Dialog::on_queryButton_clicked()
{
    QString temp = ui->wordsInputEdit->text();
    CharString line(qstr2str(temp));//从输入读入一组句子
    std::vector<CharString> sentences = line.split(CharString(" "));//按照空格对其切分
    searchEngine->searchSentences(sentences,std::cout);//暂时输出到命令行
}

