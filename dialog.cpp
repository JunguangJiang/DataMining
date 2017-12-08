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
    searchEngine = new SearchEngine(10);//����Ȩ��Ϊ10
    searchEngine->initDictionary();//��ʼ���ʵ�
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
    CharString line(qstr2str(temp));//���������һ�����
    std::vector<CharString> sentences = line.split(CharString(" "));//���տո�����з�
    std::shared_ptr<DocList> docList = searchEngine->searchSentences(sentences,std::cout);//�õ��ĵ�����
    ui->resultEdit->setText(str2qstr("Ϊ���ҵ���ؽ����")+QString::number(docList->Size())+str2qstr("��"));//��ʾ�����Ľ������
    standardItemModel->clear();//�������ԭ�ȵ��б�
    for(auto p = docList->first(); docList->isValid(p); p = docList->next(p)){//�����ĵ�����
        int DocID = p->DocID; QString title;//�������ĵ��ı���
        title += (str2qstr("�ĵ� ")+QString::number(DocID)+str2qstr(" : "));
        title += str2qstr(searchEngine->getNthRecord(DocID).title.data());
        QStandardItem *item = new QStandardItem(title);
        standardItemModel->appendRow(item);//�����뵽
    }
    ui->detailedListView->setModel(standardItemModel);//�б���ʾ��
}


void Dialog::on_detailedListView_doubleClicked(const QModelIndex &index)
{
    ui->detailedResultEdit->clear();//��վ���������ʾ��
    int DocID = index.data().toString().split(" ").at(1).toInt();//�õ����������һ�ж�Ӧ���ĵ����
    CharString detailedText = searchEngine->getNthRecord(DocID).content;//�õ���Ӧ���ĵ�����
    ui->detailedResultEdit->appendPlainText(str2qstr(detailedText.data()));//��ʾ�ĵ�����
}
