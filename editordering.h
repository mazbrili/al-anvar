#ifndef EDITORDERING_H
#define EDITORDERING_H

#include <QDialog>
#include "ui_editordering.h"
#include "database.h"

namespace Ui
{

class EditOrdering;

}

class EditOrdering : public QDialog ,public Ui::EditOrdering
{
    Q_OBJECT

public:
    DataBase *db;
    QString sql,tbName;
    explicit EditOrdering(DataBase *database,QString sql_,QString tbName_,QWidget *parent = 0);

private slots:

    void on_okButton_clicked();
    void upTable(QString sql);
    void setsort(QTableWidgetItem *item);

    void on_pushButton_clicked();

private:
};

#endif // EDITORDERING_H
