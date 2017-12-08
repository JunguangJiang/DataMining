#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "searchengine.h"
#include <QStandardItem>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:

    void on_queryButton_clicked();

    void on_detailedListView_doubleClicked(const QModelIndex &index);

private:
    Ui::Dialog *ui;
    QStandardItemModel *standardItemModel;
    SearchEngine* searchEngine;

};

#endif // DIALOG_H
