#include "adddirdialog.h"
#include <QFileDialog>




AddDirDialog::AddDirDialog(QWidget *parent) :
    QDialog(parent)

{
   setupUi(this);
   plainTextEdit->setEnabled(false);

}


void AddDirDialog::on_radioButton_clicked()
{
  nameText->setEnabled(radioButton->isChecked());
  plainTextEdit->setEnabled(radioButton_2->isChecked());

}

void AddDirDialog::on_radioButton_2_clicked()
{
    nameText->setEnabled(radioButton->isChecked());
    plainTextEdit->setEnabled(radioButton_2->isChecked());

}

void AddDirDialog::on_pushButtonOk_clicked()
{
    accept();

}

void AddDirDialog::on_pushButtonCancel_clicked()
{
      reject();
}


