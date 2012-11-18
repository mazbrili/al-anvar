#ifndef ADDDIRDIALOG_H
#define ADDDIRDIALOG_H

#include <QDialog>
#include "ui_adddirdialog.h"

namespace Ui {
class AddDirDialog;
}

class AddDirDialog : public QDialog ,public Ui::AddDirDialog
{
    Q_OBJECT
    
public:
    QStringList textFiles,textValue;
    explicit AddDirDialog(QWidget *parent = 0);
    
private slots:

    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();



private:
};

#endif // ADDDIRDIALOG_H
