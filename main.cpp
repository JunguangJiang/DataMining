#include "dialog.h"
#include <QApplication>
#include "searchengine.h"
#include "CharString.h"
#include "searchengine.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
