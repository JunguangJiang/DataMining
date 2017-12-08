#include "Record.h"
#include <ostream>
#include <istream>
using namespace std;

Record::Record(CharString title, CharString content,
               CharString userName, CharString date,
       CharString type, CharString url,
       CharString category, CharString subclass):
    title(title), content(content), userName(userName), date(date),
    type(type), url(url), category(category), subclass(subclass){}

Record::Record(const Record& rhs):
    title(rhs.title), content(rhs.content), userName(rhs.userName), date(rhs.date),
    type(rhs.type), url(rhs.url), category(rhs.category), subclass(rhs.subclass)
{}
