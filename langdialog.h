#ifndef LANGDIALOG_H
#define LANGDIALOG_H

#include <QDialog>
#include "tools.h"

namespace Ui {
    class LangDialog;
}

class LangDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LangDialog(QWidget *parent = 0);
    ~LangDialog();

private slots:

    void on_comboBox_activated(int index);

private:
    Ui::LangDialog *ui;
    tools tool;
};

#endif // LANGDIALOG_H
