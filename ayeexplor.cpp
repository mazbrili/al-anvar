#include "ayeexplor.h"

AyeExplor::AyeExplor(DataBase *database,DBModelSubJectQuran *treeModel,QuranTreeView *quranTree,QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    db =  database ;

     defultfont = db->ReadSettings("ProgFont","Tahoma,8,-1,5,50,0,0,0,0,0").value<QFont>();
    #ifdef  Q_OS_LINUX
      defultfont = QFont();
  #endif
    QList<QWidget*> allWidget =findChildren<QWidget*>();
    foreach(QWidget* wid,allWidget)
            wid->setFont(defultfont);

    textExp->setTextColor(db->ReadSettings("trColor",QColor()).value<QColor>());
    textExp->setFont(db->ReadSettings("trfont",QFont()).value<QFont>());
    textComment->setTextColor(db->ReadSettings("trColor",QColor()).value<QColor>());
    textComment->setFont(db->ReadSettings("trfont",QFont()).value<QFont>());
    textTr->setTextColor(db->ReadSettings("trColor",QColor()).value<QColor>());
    textTr->setFont(db->ReadSettings("trfont",QFont()).value<QFont>());
    textTr->addAction(actionFindSelected);

     treeModel_ = treeModel;
     treeAye =quranTree ;
     model = new QStandardItemModel ;
     SubjectTree = new DbTreeViewSubJectQuran(db,this,false,"SubjectName",treeModel_);
     SubjectTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
     SubjectTree->setDragDropMode(QAbstractItemView::DragOnly);

     findExp =new FindReplace(this,textExp);
     findComment =new FindReplace(this,textComment);
     layoutFindComment->addWidget(findComment);
     layoutFindExp->addWidget(findExp);
     SubjectTree->tb_find->setVisible(true);
     layoutSubject->addLayout(SubjectTree->find);
     layoutSubject->addWidget(SubjectTree);    
     listLayout->addWidget(treeAye);

     verseSubjects= new VerseTree(this);

     verticalLayout_2->addWidget(verseSubjects);


     listTr=db->getListData("Select  *From Tr_Translation ;");
     listEx=db->getListData("Select  *From Explanation_Name ;");

     if(listTr.at(0).size()>0)
        tr_Combo->addItems(listTr.at(1));
     if(listEx.at(0).size()>0)
        Explanations_Combo->addItems(listEx.at(1));
     connect(textComment->document(),SIGNAL(modificationChanged(bool)),okButtonComment,SLOT(setEnabled(bool)));

     connect(okButtonComment, SIGNAL(clicked()),this, SLOT(saveComment()));
     connect(okButtonSimilar, SIGNAL(clicked()),this, SLOT(delitemsSimilar()));
     connect(Explanations_Combo, SIGNAL(currentIndexChanged(int)),this, SLOT(showValue(int)));
     connect(tr_Combo, SIGNAL(currentIndexChanged(int)),this, SLOT(showtr(int)));
     connect(SubjectTree,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(addToSubjects()));
     connect(verseSubjects,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(deleteSubject()));
     connect(verseSubjects,SIGNAL(droped()),this,SLOT(addToSubjectsDragDrop()));
     connect(treeAye, SIGNAL(clicked(QModelIndex)),this, SLOT(showAll()));
     tr_Combo->setCurrentIndex(db->ReadSettings("translation",0).toInt());
     table->addAction(actionFilter);
     connect(actionFilter, SIGNAL(triggered()),this, SLOT(filterWord()));
     connect(radioQuran, SIGNAL(clicked()),this, SLOT(showAye()));
     connect(radioTr, SIGNAL(clicked()),this, SLOT(showAye()));

     actionFilter->setText(db->trlang("Filter by current Similar Words"));
     radioQuran->setText(db->trlang("Qur'an Only"));
     trLable->setText(db->trlang("Qur'an Translations"));
     radioTr->setText(db->trlang("Translation Only"));
     label->setText(db->trlang("Show the Verses")+":");
     radioQuran->setChecked(db->ReadSettings("typeShowQuran",true).toBool());
     radioTr->setChecked(db->ReadSettings("typeShowQuran",false).toBool());





}

void AyeExplor::filterWord()
{
    if(actionFilter->isChecked())
    {
        actionFilter->setText(db->trlang("Delete filter and show all"));
        actionFilter->setIcon(QIcon(":/images/filterdelete.png"));

    QString filter = table->item(table->currentRow(),1)->text();
    for( int i = 0; i < table->rowCount(); ++i )
    {
        bool match = false;

          QTableWidgetItem *item = table->item( i,1 );

            if( item->text().contains(filter) )
              match = true;

        table->setRowHidden( i, !match );
    }
    }else{
        actionFilter->setText(db->trlang("Filter by current Similar Words"));
        actionFilter->setIcon(QIcon(":/images/filteradd.png"));


        for( int i = 0; i < table->rowCount(); ++i )
            table->setRowHidden( i, false );


    }

}

bool AyeExplor::maybeSave()
{
    if (!textComment->document()->isModified())
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, db->trlang("Save the Point"),
                               db->trlang("The Point has been modified.")+"\n"+
                                  db->trlang("Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return saveComment();
    else if (ret == QMessageBox::Cancel)
        return false;
    textComment->document()->setModified(false);
    return true;

}

bool AyeExplor::saveComment()
{
    QString ret;
    if(mostAdd){
    if(!textComment->toPlainText().isNull())
          ret=  db->insertsql("INSERT INTO Comment VALUES("+Id+",'"+textComment->toPlainText()+"');");

    }else
          ret= db->insertsql("UPDATE Comment SET Value='"+textComment->toPlainText()+"' Where id ="+Id+" ;");

    if(ret=="ok"){
    textComment->document()->setModified(false);
    treeAye->quranModel_->itemFromIndex(treeAye->currentIndex())->setIcon(QIcon(":/images/ok.png"));
    if(db->getDatastr("Select Value From Comment Where id = "+Id+";").trimmed()=="")
        treeAye->quranModel_->itemFromIndex(treeAye->currentIndex())->setIcon(QIcon(":/images/file.png"));


    return true;
    }else
    {
        QMessageBox::warning(this,db->trlang("Save the Point"),db->trlang("Point was not saved Successfully."));
        return false;

    }
}

void AyeExplor::showComment()
{

 if(db->getDatastr(" Select count(id) From Comment Where id ="+Id+" ;")=="0"){

         mostAdd=true;
         textComment->clear();

          }else{

           mostAdd=false;
           textComment->setText(db->getDatastr("Select Value From Comment Where id = "+Id+";"));
          }
    textComment->document()->setModified(false);

}

void AyeExplor::showtr(int i)
{
    if(i<0)
        return;

    textTr->setFont(db->ReadSettings("trfont",defultfont).value<QFont>());
    textTr->setTextColor(db->ReadSettings("trColor",QColor()).value<QColor>());

    QString chapter =db->getDatastr(QString("Select Chapter From Quran Where id= %1 ;").arg(Id));
    QString verse =db->getDatastr(QString("Select Verse From Quran Where id= %1 ;").arg(Id));
    QString chapterName = db->trlang(db->getDatastr(QString("Select descent From ChapterProperty Where id= %1 ;").arg(chapter)));



    QString tbName=listTr.at(2).at(i);
    QString str =db->getDatastr("Select tr From "+tbName+"  Where id ='"+Id+"' ;");
    textTr->setPlainText(QString(" %1 - %2 : %3").arg(chapterName).arg(chapter).arg(verse)+
            " "+str);
}

void AyeExplor::showAye()
{
    QString chapter =db->getDatastr(QString("Select Chapter From Quran Where id= %1 ;").arg(Id));
    QString verse =db->getDatastr(QString("Select Verse From Quran Where id= %1 ;").arg(Id));

      QString chapterName = db->trlang(db->getDatastr(QString("Select descent From ChapterProperty Where id= %1 ;").arg(chapter)));
      verseText = db->getDatastr("SELECT Arabic FROM Quran Where id="+Id);

      if (radioQuran->isChecked())
      {
          textTr->setFont(db->ReadSettings("Quranfont",defultfont).value<QFont>());
          textTr->setTextColor(db->ReadSettings("QuranColor",QColor()).value<QColor>());

              textTr->setPlainText(QString(" %1 - %2 : %3").arg(chapterName).arg(chapter).arg(verse)+
                      " "+verseText);
              tr_Combo->setEnabled(false);
              trLable->setEnabled(false);
              db->WriteSettings("typeShowQuran",true);

      }else{
          textTr->setFont(db->ReadSettings("trfont",defultfont).value<QFont>());
          textTr->setTextColor(db->ReadSettings("trColor",QColor()).value<QColor>());


      tr_Combo->setEnabled(true);
      trLable->setEnabled(true);
      showtr(tr_Combo->currentIndex());
      db->WriteSettings("typeShowQuran",true);

      }
}

void AyeExplor::showAll(){

       toolButtoNext->setFocus();
    if(maybeSave()){

    if(! treeAye->currentIndex().isValid())
        return;
    if(treeAye->model()->data(treeAye->model()->index(treeAye->currentIndex().row(),2,treeAye->currentIndex().parent())).toString()!="verse")
        return;

    Id=treeAye->model()->data(treeAye->model()->index(treeAye->currentIndex().row(),1,treeAye->currentIndex().parent())).toString();


        showAye();
        showComment();
        chengeItemExp();
        updateVerseTree();
        showSimilar();

        }else
        treeAye->setCurrent(Id);

}


void AyeExplor::chengeItemExp()
{
    showValue(Explanations_Combo->currentIndex());
}
void AyeExplor::showSimilarSelected()
{

    if(radioTr->isChecked())
        return;
    QString str =textTr->textCursor().selectedText();
    DBSearch ds;

    QStandardItemModel *item= db->getMode(tr("Select Chapter,Verse,Arabic From Quran Where Arabic REGEXP '%1'  ;").arg(ds.alifHamza(str)));
    table->setRowCount(item->rowCount());
    table->setColumnCount(3);
    QTableWidgetItem *verse = new  QTableWidgetItem(db->trlang("Verses"));
    QTableWidgetItem *sim = new  QTableWidgetItem(db->trlang("Similar Words"));
    QTableWidgetItem *address = new  QTableWidgetItem(db->trlang("Address"));
    verse->setTextAlignment(Qt::AlignCenter);
    sim->setTextAlignment(Qt::AlignCenter);

    table->setColumnWidth(0,width()-450);
    table->setColumnWidth(1,120);
    table->setColumnWidth(2,55);

    table->setHorizontalHeaderItem(0,verse);
    table->setHorizontalHeaderItem(1,sim);
    table->setHorizontalHeaderItem(2,address);
    okButtonSimilar->setVisible(false);
    for(int i=0;i<item->rowCount();i++)
    {


        QTableWidgetItem *verse = new  QTableWidgetItem;
        verse->setText(item->data(item->index(i,2)).toString());
        QTableWidgetItem *sim = new  QTableWidgetItem;
        sim->setText(str);
        QTableWidgetItem *versechapter = new  QTableWidgetItem;
        versechapter->setText(item->data(item->index(i,0)).toString()+"/"+item->data(item->index(i,1)).toString());
        table->setItem(i,0,verse);
        table->setItem(i,1,sim);
        table->setItem(i,2,versechapter);
    }



}
void AyeExplor::showSimilar()
{
    okButtonSimilar->setVisible(true);

    QStringList listId,listVerse,listWord;
    QStringList listban = db->getData("Select Word From unwantedWords;");
    QStringList listresults = db->getDataSpilt("Select id,Arabic From Quran Where id="+Id);
    QStringList listresultst = db->getDataSpilt("Select id,Arabic From Quran ;");
       for (int ii=0;ii<listresults.count();ii++){
         QStringList list = listresults.at(ii).split("#-I?D-SPL?IT-#").at(1).split(" ");
           for(int i=0; i<list.size()-1;i++){
             QString text = list.at(i).trimmed()+" "+list.at(i+1).trimmed();
             QRegExp reg(text);
                foreach (QString find,listresultst){
                   if(find.contains(reg) &&  listresults.at(ii)!=find && !listban.contains(text) && !listId.contains(find.split("#-I?D-SPL?IT-#").at(0)) ){
                      listId.append(find.split("#-I?D-SPL?IT-#").at(0));
                      listVerse.append(find.split("#-I?D-SPL?IT-#").at(1));
                      listWord.append(text);
               }
             }
           }
         }
   table->setRowCount(listVerse.size());
   table->setColumnCount(3);
   QTableWidgetItem *verse = new  QTableWidgetItem(db->trlang("Verses"));
   QTableWidgetItem *sim = new  QTableWidgetItem(db->trlang("Similar Words"));
   QTableWidgetItem *address = new  QTableWidgetItem(db->trlang("Address"));
   verse->setTextAlignment(Qt::AlignCenter);
   sim->setTextAlignment(Qt::AlignCenter);

   table->setColumnWidth(0,width()-450);
   table->setColumnWidth(1,120);
   table->setColumnWidth(2,55);

   table->setHorizontalHeaderItem(0,verse);
   table->setHorizontalHeaderItem(1,sim);
   table->setHorizontalHeaderItem(2,address);
 for(int i=0; i<listVerse.size();i++){
     QTableWidgetItem *verse = new  QTableWidgetItem;
     verse->setText(listVerse.at(i));
     QTableWidgetItem *sim = new  QTableWidgetItem;
     sim->setText(listWord.at(i));
     QTableWidgetItem *versechapter = new  QTableWidgetItem;
     versechapter->setText(db->getDatastr("Select Chapter From Quran Where id="+listId.at(i))+"/"+db->getDatastr("Select Verse From Quran Where id="+listId.at(i)));
     verse->setCheckState(Qt::Unchecked);
     table->setItem(i,0,verse);
     table->setItem(i,1,sim);
     table->setItem(i,2,versechapter);


  }

}
void AyeExplor::delitemsSimilar()
{
    for(int i=0;i<table->rowCount();i++)
    {
        if(table->item(i,0)->checkState()==Qt::Checked && db->getDatastr("Select count(id) From unwantedWords Where Word='"+table->item(i,1)->text()+"';").toInt()==0)
     db->insertsql("INSERT INTO unwantedWords(Word) VALUES('"+table->item(i,1)->text()+"');");
     }

}

void AyeExplor::showValue(int i)
{
   if (i<0)
       return;

    QString idEx=listEx.at(0).at(i);
    QString str =db->getDatastr("Select Value From Explanations  Where id_Verse LIKE '%["+Id+"]%' AND id_Name='"+idEx+"' ;");
    str.replace(QRegExp("\\n\\n\\n"),"\n");
    str.replace(QRegExp("\\n\\n"),"\n");
    textExp->setText(str);
    str = textExp->toPlainText();
    str.replace(QRegExp("\\n\\n\\n\\n\\n\\n"),"\n");
    str.replace(QRegExp("\\n\\n\\n\\n\\n"),"\n");
    str.replace(QRegExp("\\n\\n\\n\\n"),"\n");
    str.replace(QRegExp("\\n\\n\\n"),"\n");
    str.replace(QRegExp("\\n\\n"),"\n");
    textExp->setText(str);

    findExp->lineEditFind->setText(findExp->clearErab(verseText));
    findExp->findTypedText();
}


void AyeExplor::updateVerseTree()
{

    model = db->getModelTr("select name,subjects.id from subjectname left join subjects on (subjectname.id=subjects.id_subject) where subjects.id_verse ="+Id);
    verseSubjects->setModel(model);
    verseSubjects->setHeaderHidden(true);
    verseSubjects->setColumnHidden(1,true);
}

void AyeExplor::addToSubjects()
{
   QModelIndex item =SubjectTree->currentIndex();
   QModelIndexList indexs= treeAye->selected();

   foreach (QModelIndex index,indexs)
   {
   QString idVe =treeAye->model()->data(treeAye->model()->index(index.row(),1,index.parent())).toString();
     if (SubjectTree->getCurrentText(4,item)=="1" and db->getDatastr("Select count(id) From Subjects Where ID_SUBJECT ='"+SubjectTree->getCurrentText(1,item)+"' And ID_VERSE  = '"+idVe+"' ")=="0")
          {
         db->insertsql("INSERT INTO Subjects VALUES("+db->GetNodeIdCode()+","+SubjectTree->getCurrentText(1,item)+","+idVe+");");
         SubjectTree->treeModel_->itemFromIndex(item)->setIcon(QIcon(":/images/ok.png"));
          }

   }

 /*  for(int i=0;i<tree->rowCount();i++){
  if(tree->item(i,0)->checkState()==Qt::Checked){
    if(treeModel_->itemFromIndex(item)->checkState()==Qt::Checked){
      if (db->getDatastr("Select count(id) From Subjects Where ID_SUBJECT ='"+SubjectTree->getCurrentText(1,item)+"' And ID_VERSE  = '"+tree->item(i,1)->text()+"' ")=="0")
        db->insertsql("INSERT INTO Subjects VALUES("+db->GetNodeIdCode()+","+SubjectTree->getCurrentText(1,item)+","+tree->item(i,1)->text()+");");

   }
}
}
   */
    updateVerseTree();
}
void AyeExplor::addToSubjectsDragDrop()
{
   QModelIndexList itemsList =SubjectTree->getSelectedIndex();
   QModelIndexList indexs= treeAye->selected();

   foreach (QModelIndex item,itemsList)
   {
         foreach (QModelIndex index,indexs)
          {
         QString idVe =treeAye->model()->data(treeAye->model()->index(index.row(),1,index.parent())).toString();
           if (SubjectTree->getCurrentText(4,item)=="1" and db->getDatastr("Select count(id) From Subjects Where ID_SUBJECT ='"+SubjectTree->getCurrentText(1,item)+"' And ID_VERSE  = '"+idVe+"' ")=="0")
               {
                  db->insertsql("INSERT INTO Subjects VALUES("+db->GetNodeIdCode()+","+SubjectTree->getCurrentText(1,item)+","+idVe+");");
                  SubjectTree->treeModel_->itemFromIndex(item)->setIcon(QIcon(":/images/ok.png"));
               }

           }
   }

    updateVerseTree();
}

void AyeExplor::deleteSubject()
{
    QModelIndex item =verseSubjects->currentIndex();
    QMessageBox::StandardButton sb;
    sb = QMessageBox::warning(0, db->trlang("Warning"),db->trlang("Are you sure to Remove those Subjects:")+model->item(item.row(),0)->text(),
                               QMessageBox::Yes | QMessageBox::No);
    if (sb == QMessageBox::No)
        return;
    db->insertsql("DELETE FROM Subjects WHERE id="+model->item(item.row(),1)->text());   
    updateVerseTree();

}


void AyeExplor::on_toolButtonPrevious_clicked()
{
    int idN=Id.toInt()-1;
   if (idN>0)
       Id=QString::number(idN);
   else
       Id="6236";
    treeAye->setCurrent(Id);
    showAll();
}

void AyeExplor::on_toolButtoNext_clicked()
{
    int idN=Id.toInt()+1;
   if (idN<6236)
       Id=QString::number(idN);
   else
       Id="1";
    treeAye->setCurrent(Id);
    showAll();
}

void AyeExplor::on_actionFindSelected_triggered()
{
    showSimilarSelected();
}
