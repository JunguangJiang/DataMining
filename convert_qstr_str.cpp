#include "convert_qstr_str.h"

std::string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}

CharString qstr2charStr(const QString qstr)
{
    return CharString(qstr2str(qstr));
}

QString str2qstr(const std::string& str)
{
    return QString::fromLocal8Bit(str.data());
}

QString charStr2qstr(const CharString& str)
{
    return str2qstr(str.data());
}
