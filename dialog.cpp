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
    searchEngine = new SearchEngine(10);//����Ȩ��Ϊ10
    searchEngine->initDictionary();//��ʼ���ʵ�
    QFileInfo downloadResultFile (DownloadResultFile);
    if(!downloadResultFile.isFile() ){//���֮ǰû��������ҳԴ��
        qDebug() << DownloadResultFile << " doesn't exist";
        WebsiteProcessor websiteProcessor;//����Ҫ�Ƚ����е���ҳԴ�����ص�����
        websiteProcessor.initDictionary();//��ʼ���ʵ�
        websiteProcessor.extractInfo(CharString(URLFile), CharString(DownloadResultFile), true);//���������ҳ��ȥ�����ô�
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
    CharString line(qstr2str(temp));//���������һ�����
    std::vector<CharString> sentences = line.split(CharString(" "));//���տո�����з�
    docList = searchEngine->searchSentences(sentences,std::cout);//�õ��ĵ�����
    ui->resultEdit->setText(str2qstr("Ϊ���ҵ���ؽ����")+QString::number(docList->Size())+str2qstr("��"));//��ʾ�����Ľ������
    standardItemModel->clear();//�������ԭ�ȵ��б�
    for(auto p = docList->first(); docList->isValid(p); p = docList->next(p)){//�����ĵ�����
        qDebug() << "docList total weight "<< docList->totalWeight(p);
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
    Record record = searchEngine->getNthRecord(DocID);//�õ���Ӧ�ļ�¼

    ui->detailedResultEdit->setOpenExternalLinks(false);
    ui->detailedResultEdit->setTextColor(Qt::blue);
    ui->detailedResultEdit->setFontWeight(QFont::Bold);
    ui->detailedResultEdit->setAlignment(Qt::AlignHCenter);
    ui->detailedResultEdit->setFontPointSize(15);
    ui->detailedResultEdit->append(str2qstr("����:") + str2qstr(record.title.data()));//��ʾ����

    ui->detailedResultEdit->setTextColor(Qt::gray);
    ui->detailedResultEdit->setFontWeight(QFont::Normal);
    ui->detailedResultEdit->setFontPointSize(10);
    ui->detailedResultEdit->append(str2qstr("������:")+str2qstr(record.userName.data()));//��ʾ������
    ui->detailedResultEdit->append(str2qstr("��������:")+str2qstr(record.date.data()));//��ʾ��������

    ui->detailedResultEdit->setTextColor(Qt::black);
    ui->detailedResultEdit->append(str2qstr(""));
    ui->detailedResultEdit->setAlignment(Qt::AlignLeft);
    ui->detailedResultEdit->append(str2qstr(record.content.data()));//��ʾ�ĵ�����
    ui->detailedResultEdit->append(str2qstr(""));

    ui->detailedResultEdit->setOpenExternalLinks(true);
    ui->detailedResultEdit->setTextColor(Qt::gray);
    QString urlLink =  ("<style> a {text-decoration: none} </style> "
                        "<a href=\"" + charStr2qstr(record.url)+ "\" >"+
                        str2qstr("��Դ")+charStr2qstr( record.url)+
                        "</a>");//������ҳ������
    ui->detailedResultEdit->append(urlLink);

    QString temp = ui->wordsInputEdit->text(); CharString line(qstr2str(temp));//���������һ�����
    std::vector<CharString> sentences = line.split(CharString(" "));//���տո�����з�
    CharStringLink wordList = searchEngine->getWordListFromSentence(sentences);//�ִʵõ�һ��ؼ���

    QTextDocument *document = ui->detailedResultEdit->document();
    for(auto p = wordList.first(); wordList.isValid(p); p = wordList.next(p)){
        QString search_text = str2qstr(p->data.data());//��ÿ���ؼ���,������һ�θ���
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
