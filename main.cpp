#include "dialog.h"
#include <QApplication>
#include "searchengine.h"
#include "CharString.h"
#include "searchengine.h"
#include "WebsiteProcessor.h"
#include "convert_qstr_str.h"
#include <QDebug>
#include <iostream>
#include "hash.h"
#include <string>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();



    //WebsiteProcessor websiteProcessor;//网页信息处理类
    //websiteProcessor.initDictionary();//初始化词典
    //websiteProcessor.extractInfo(urlFile, extractResultFile, true);//浏览整个网页，去除无用词
    //CharString url("http://bbs.cehome.com/thread-1150391-1-1.html");
    //websiteProcessor.processHtml(url, std::cout, true);
    //CharString origin("&#26377;&#20852;&#36259;&#36827;&#31354;&#38388;312436742&#30475;&#29031;&#29255;&#65292;&#32431;&#2;");
    //qDebug() << charStr2qstr( UnicodeToChinese(origin) );
    Hash hash(3);

    std::string string[10] = {"Hello", "Hi", "Wah", "No", "ouh", "AO", "Bug"};
    for(int i=0; i<7; i++){
        hash.Insert(CharString(string[i]));
    }
    hash.debug();
    return a.exec();
}
