#ifndef CONVERT_QSTR_STR_H
#define CONVERT_QSTR_STR_H
#include <CharString.h>
#include <QString>
#include <string>
std::string qstr2str(const QString qstr);//QStringתstd::string
CharString qstr2charStr(const QString qstr);//QStringתCharString
QString str2qstr(const std::string& str);//std:stringתQString

QString charStr2qstr(const CharString& str);//CharStringתQString
#endif // CONVERT_QSTR_STR_H
