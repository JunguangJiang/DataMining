#include "dialog.h"
#include <QApplication>
#include "searchengine.h"
#include "CharString.h"
#include "searchengine.h"
#include "WebsiteProcessor.h"
#include "convert_qstr_str.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();



    //WebsiteProcessor websiteProcessor;//��ҳ��Ϣ������
    //websiteProcessor.initDictionary();//��ʼ���ʵ�
    //websiteProcessor.extractInfo(urlFile, extractResultFile, true);//���������ҳ��ȥ�����ô�

    //CharString origin("&#26377;&#20852;&#36259;&#36827;&#31354;&#38388;312436742&#30475;&#29031;&#29255;&#65292;&#32431;&#2;");
    //qDebug() << charStr2qstr( UnicodeToChinese(origin) );

    return a.exec();
}
