#include "editordering.h"

EditOrdering::EditOrdering(DataBase *database,QString sql_,QString tbName_,QWidget *parent) :
    QDialog(parent)

{
    setupUi(this);
    db = database;
    tbName=tbName_;
    sql=sql_;
    upTable(sql);
    connect(view, SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(setsort(QTableWidgetItem *)));

}


void EditOrdering::on_okButton_clicked()
{
    accept();
}

void EditOrdering::setsort(QTableWidgetItem *item){
    if(item->column()==2){
        db->insertsql("UPDATE "+tbName+" SET ordering='"+item->text()+"' WHERE id='"+view->item(item->row(),0)->text()+"' ;");
    }else if(item->column()==1){
        db->insertsql("UPDATE "+tbName+" SET Name='"+item->text()+"' WHERE id='"+view->item(item->row(),0)->text()+"' ;");
    }
}
void EditOrdering::upTable(QString sql){

    QList <QStringList> li= db->getListData(sql);
      if(li.size()>0){
    view->setColumnCount(li.size());
    view->setRowCount(li.at(0).size());
    view->setColumnHidden(0,true);

    for (int c =0;c<li.size();c++){
        for (int r=0;r<li.at(c).size();r++){
            QTableWidgetItem *item = new QTableWidgetItem(li.at(c).at(r));
            view->setItem(r,c,item);

        }
      }
    }

}

void EditOrdering::on_pushButton_clicked()
{
    reject();
}
