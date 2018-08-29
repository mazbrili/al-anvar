#include "langdialog.h"
#include "ui_langdialog.h"

LangDialog::LangDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LangDialog)
{
    ui->setupUi(this);
    QStringList com_LangItems =tool.GetLang(":/language/");

    foreach(QString str,com_LangItems)
      ui->comboBox->addItem(str.split(";").at(0),str.split(";").at(1));

    if(ui->comboBox->findText("English")>=0)
    ui->comboBox->setCurrentIndex(ui->comboBox->findText("English"));

}

LangDialog::~LangDialog()
{
    delete ui;
}


void LangDialog::on_comboBox_activated(int index)
{
    tool.WriteSettings("lang",ui->comboBox->itemData(index).toString());
    accept();
}
