/***************************************************************************
 *  This file is part of Al-Anvar, Quran study tool for simply browsing    *
 *   and researching on the Holy Quran (Koran) and other Islamic books.    *
 *                                                                         *
 *  Copyright (C) 2010-2011 by S.Hosein Hoseini                            *
 *     E-Mail: <etrate14@gmail.com>,                                       *
 *     WWW: <http://sourceforge.net/projects/al-anvar/>                    *
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 3 of the License,         *
 *  (at your option) any later version                                     *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details                            *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program; if not, see http://www.gnu.org/licenses/      *
 *                                                                         *
 ***************************************************************************/
#ifndef DBTREEVIEW_H
#define DBTREEVIEW_H

#include "dbmodel.h"
#include "adddirdialog.h"
#include <QToolBar>
#include <QCompleter>
/*
CREATE TABLE IF NOT EXISTS Books (id NUMERIC, Name TEXT, Parent_ID NUMERIC, Type NUMERIC, rating NUMERIC, ordering NUMERIC, Date DATE, BookId NUMERIC,peroperty TEXT);
CREATE TABLE IF NOT EXISTS BooksValue (id NUMERIC, idText NUMERIC, Text TEXT, Page TEXT,BookId NUMERIC);
CREATE INDEX IF NOT EXISTS Book_rating ON Books (rating  ASC);
CREATE INDEX IF NOT EXISTS BooksValue_bookid ON BooksValue(BookId ASC);
CREATE INDEX IF NOT EXISTS BooksValue_id ON BooksValue(id ASC);
CREATE INDEX IF NOT EXISTS BooksValue_idText ON BooksValue(idText ASC);
CREATE INDEX IF NOT EXISTS Books_Date ON Books(Date ASC);
CREATE INDEX IF NOT EXISTS Books_Id ON Books(id ASC);
CREATE INDEX IF NOT EXISTS Books_Name ON Books(Name ASC);
CREATE INDEX IF NOT EXISTS Books_Ordering ON Books(ordering ASC);
CREATE INDEX IF NOT EXISTS Books_Parent_Id ON Books(Parent_ID ASC);
CREATE INDEX IF NOT EXISTS Books_bookid ON Books (BookId  ASC);
CREATE INDEX IF NOT EXISTS Books_type ON Books (Type  ASC);
*/
class DbTreeView : public QTreeView
{
   Q_OBJECT
    public:
    bool justFindInExpanded;
    explicit DbTreeView(DataBase *database,QWidget *parent,bool istrash,QString tableName_,DBModel *treeModel,bool editeable =true)
        :QTreeView(parent)
    {

        setContextMenuPolicy(Qt::ActionsContextMenu);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        if(editeable)
        setEditTriggers(QAbstractItemView::DoubleClicked);
        else
        setEditTriggers(QAbstractItemView::NoEditTriggers);

        setAutoExpandDelay(1000);
        setAlternatingRowColors(true);
        setHeaderHidden(true);
        db = database;
        isTrash=istrash;
        treeModel_=treeModel;
        tableName=tableName_;
        isediteable=editeable;
        needToUpdate=false;
        justFindInExpanded=false;


        actionAddsub = new QAction(QIcon(":/images/add.png"),db->trlang("Add Subject"),parent);
        actionAddsubF= new QAction(QIcon(":/images/folder.png"),db->trlang("add folder"),parent);
        actionSetToTopLevel = new QAction(QIcon(":/images/toTop.png"),db->trlang("Set Top Level"),parent);
        actionMoveToTrash = new QAction(QIcon(":/images/delete.png"),db->trlang("Move To Trash"),parent);
        actionMerge = new QAction(db->trlang("Merge Subject"),parent);
        actionSelectToMove = new QAction(QIcon(":/images/ok.png"),db->trlang("Select for Move"),parent);
        actionOrdering  = new QAction(QIcon(":/images/order.png"),db->trlang("Edit Ordering"),parent);
        actionDeletePermantly   = new QAction(QIcon(":/images/delete.png"),db->trlang("Delete Permantly Selecteds"),parent);
        actionShowTrash = new QAction(QIcon(":/images/toTrash.png"),db->trlang("Show Trash"),parent);
        actionRestore = new QAction(db->trlang("Restore Seleteds"),parent);
        actionEmptyTrash= new QAction(db->trlang("EmptyTrash "),parent);
        actionMoveSub = new QAction(QIcon(":/images/move.png"),db->trlang("Move Subject"),parent);
        updateTree = new QAction(QIcon(":/images/update.png"),db->trlang("update tree"),parent);
        actionXml= new QAction(QIcon(":/images/exportXml.png"),db->trlang("export all to xml"),parent);
        actionXmlSelected= new QAction(QIcon(":/images/exportXml.png"),db->trlang("export selected to xml"),parent);

        SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
        SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);
        TitleIcon= QIcon(":/images/file.png");
       actionMoveSub->setEnabled(false);
       actionMerge->setEnabled(false);
        if(editeable && !isTrash){
       addAction(actionAddsub);
       addAction(actionAddsubF);
       addAction(actionSetToTopLevel);
       addAction(actionSelectToMove);
       addAction(actionMoveSub);
       addAction(actionMerge);
       addAction(actionOrdering);
       addAction(updateTree);
       addAction(actionMoveToTrash);
       addAction(actionShowTrash);
       addAction(actionXmlSelected);
       addAction(actionXml);



        }else if(isTrash){

            addAction(actionRestore);
            addAction(actionDeletePermantly);
            addAction(actionEmptyTrash);

        }

        actionMerge->setVisible(false);


       setModel(treeModel_);
       completer = new QCompleter;
       completer->setModel(db->getMode("Select Name From "+tableName+" Where Type =1 ;"));
       hiddenColumn();
             findLable = new QLabel(db->trlang("Find"));
             lineEdit = new QLineEdit;
             lineEdit->setCompleter(completer);
             find = new QHBoxLayout;
             tb_find = new QToolBar(this);
             nextfind = new QAction(QIcon(":/images/next-icon.png"),db->trlang("Next"), this);
             previousfind = new QAction(QIcon(":/images/previous-icon.png"),db->trlang("Previous"), this);

       tb_find->setVisible(false);
       tb_find->addAction(nextfind);
       tb_find->addAction(previousfind);
       find->addWidget(findLable);
       find->addWidget(lineEdit);
       find->addWidget(tb_find);
       connect(treeModel_,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(editItem(QModelIndex)));
       connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(chengeMove(QModelIndex)));
        actionAddsub->setData(false);
        actionAddsubF->setData(true);

       connect(actionAddsub,SIGNAL(triggered()),this,SLOT(addDBItem()));
       connect(actionAddsubF,SIGNAL(triggered()),this,SLOT(addDBItem()));

       connect(actionSetToTopLevel,SIGNAL(triggered()),this,SLOT(setToTopLevel()));
       connect(actionMoveToTrash,SIGNAL(triggered()),this,SLOT(MoveToTrashDBItem()));
       connect(actionMerge,SIGNAL(triggered()),this,SLOT(mergeDBItem()));
       connect(actionSelectToMove,SIGNAL(triggered()),this,SLOT(choiseToMoveDBItem()));
       connect(actionMoveSub,SIGNAL(triggered()),this,SLOT(moveDBItem()));
       connect(actionOrdering,SIGNAL(triggered()),this,SLOT(SetOrdering()));
       connect(treeModel_,SIGNAL(updated()),this,SLOT(hiddenColumn()));
       connect(lineEdit, SIGNAL(textEdited(QString)),this, SLOT(findTree()));
       connect(nextfind, SIGNAL(triggered()),this, SLOT(findTreeNext()));
       connect(previousfind, SIGNAL(triggered()),this, SLOT(findTreePrevious()));
       connect(actionDeletePermantly, SIGNAL(triggered()),this, SLOT(deletePermantly()));
       connect(actionRestore, SIGNAL(triggered()),this, SLOT(restore()));
       connect(actionEmptyTrash, SIGNAL(triggered()),this, SLOT(emptyTrash()));
       connect(updateTree, SIGNAL(triggered()),this, SLOT(upTree()));
       connect(actionXml, SIGNAL(triggered()),this, SLOT(exportXml()));
       connect(actionXmlSelected, SIGNAL(triggered()),this, SLOT(exportXmlSelected()));

       connect(actionShowTrash, SIGNAL(triggered()),this, SLOT(showTrash()));
       connect(this, SIGNAL(expanded(QModelIndex)),this,SLOT(addChildOnExpanded(QModelIndex)));



    }

    QModelIndex getCurrentIndex(int col,QModelIndex parent)
    {
       return treeModel_->index(parent.row(),col,parent.parent());
    }
    QString getCurrentText(int col,QModelIndex parent){

        return treeModel_->data(treeModel_->index(parent.row(),col,parent.parent())).toString();

    }
    void updateItemAdd(QString id ,QString  parent_id,QString  book_id,QString Type, QString txt,QString section,bool setCurrent=true)
    {
        QList<QStandardItem*> items;
        QStandardItem *itemName = new  QStandardItem(TitleIcon,txt);
        QStandardItem *itemId = new  QStandardItem(id);
        QStandardItem *itemParentId = new  QStandardItem(parent_id);
        QStandardItem *SubjectRoot = new  QStandardItem("1");
        QStandardItem *SubjectType = new  QStandardItem(Type);
        QStandardItem *SubjectBookId = new  QStandardItem(book_id);
               if(Type=="0")
         itemName->setIcon(SubjectIcon);
         itemName->setFlags( Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
         items.append(itemName);
         items.append(itemId);
         items.append(itemParentId);
         items.append(SubjectRoot);
         items.append(SubjectType);
         items.append(SubjectBookId);
         treeModel_->getChild(itemName,itemId);

         //
        // if(Checkablebool && Type=="1")
        //     itemName->setCheckState(Qt::Unchecked);

         if ( section =="title")
         treeModel_->itemFromIndex(getCurrentIndex(0,currentIndex()))->parent()->appendRow(items);

         else if ( section =="parent")
         treeModel_->itemFromIndex(getCurrentIndex(0,currentIndex()))->appendRow(items);

          else if ( section =="top")
         treeModel_->appendRow(items);
         if(setCurrent){

         QList<QStandardItem*> indexes = treeModel_->findItems(itemName->text(),Qt::MatchExactly | Qt::MatchRecursive);
         foreach(QStandardItem *item,indexes)
            {
              if(getCurrentText(1,item->index())==itemId->text())
                 setCurrentIndex(item->index());
             }
         }
    }
    QModelIndexList getSelectedIndex()
    {
      return selectedIndexes();
    }

public slots:
    void exportXml()
    {
        treeModel_->getXMl();
    }
    void exportXmlSelected()
    {
        QModelIndexList items;
        items = selectedIndexes();
        QStringList idList;
        foreach ( QModelIndex item,items)
            idList << getCurrentText(1,item);

        treeModel_->getXMl(idList);
    }
    void addChildOnExpanded(QModelIndex index)
    {
        QStandardItem *item= treeModel_->itemFromIndex(index);
        for(int i =0;i<item->rowCount();i++)
        {
        if(item->child(i)->rowCount()==0)
          treeModel_->getChild(item->child(i),item->child(i,1));

        }
    }
    void hiddenColumn(){

            setColumnHidden(1,true);
            setColumnHidden(2,true);
            setColumnHidden(3,true);
            setColumnHidden(4,true);
            setColumnHidden(5,true);
            setColumnHidden(6,true);

        }
    void choiseToMoveDBItem()
   {
       IdSubjectForMove.clear();
       IdSubjectForMove = selectedIndexes();
       if(IdSubjectForMove.count()<1)
           return;
        actionMoveSub->setEnabled(true);
        actionMerge->setEnabled(true);

   }
    void getChildsId()
   {

      treeModel_->listChildIds.clear();
      treeModel_->getChildsId(getCurrentText(1,currentIndex()));
      QMessageBox::StandardButton sb;

      sb = QMessageBox::warning(0, db->trlang("Warning"),treeModel_->listChildIds.join("\n"),
                                 QMessageBox::Yes | QMessageBox::No);
      if (sb == QMessageBox::No)
          return;
      else
          return;

   }
    void chengeMove(QModelIndex)
    {

      QString text;
            if(getCurrentText(4,currentIndex())=="0")
                text= getCurrentText(0,currentIndex());
            else
                text= getCurrentText(0,currentIndex().parent());

            if(text=="")
                text = db->trlang("first list");

          actionMoveSub->setText(db->trlang("Move Subject")+text);

    }
    void moveDBItem()
   {
        if(IdSubjectForMove.count()<1)
            return;
        bool mostUp=false;
         QString itemsAlter,MoveAlter,Parent_ID;
         foreach ( QModelIndex item,IdSubjectForMove)
             itemsAlter += "\n"+getCurrentText(0,item);

         if(getCurrentText(4,currentIndex())=="0")
         {
             MoveAlter= getCurrentText(0,currentIndex());
              Parent_ID=getCurrentText(1,currentIndex());
         }else
         {
             MoveAlter= getCurrentText(0,currentIndex().parent());
             Parent_ID=db->getDatastr("Select Parent_Id From "+tableName+" Where Id ='"+getCurrentText(1,currentIndex())+"';");


         }

         if(MoveAlter==""){
                    MoveAlter = db->trlang("first list");
                    mostUp=true;
                }

              QMessageBox::StandardButton sb;
              sb = QMessageBox::warning(0, db->trlang("Warning"),db->trlang("Those Subjects:")+itemsAlter+"\n"+db->trlang("will Move to ")+"\n"+MoveAlter,
                                         QMessageBox::Yes | QMessageBox::No);
              if (sb == QMessageBox::No)
                  return;


           foreach (QModelIndex item,IdSubjectForMove){
               int order= db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='"+Parent_ID+"';").toInt()+1;
               db->insertsql("UPDATE "+tableName+" SET Parent_ID='"+Parent_ID+"' ,ordering="+QString::number(order)+" WHERE id='"+getCurrentText(1,item)+"';");

               QString idt    = getCurrentText(1,item);
               QString typet  =getCurrentText(4,item);
               QString txt    = getCurrentText(0,item);
               QString parent = Parent_ID;
               if(!mostUp)
                 updateItemAdd(idt,parent,"0",typet,txt,"parent",false);
           }
           for(int i=IdSubjectForMove.size();i>0 ;i=i--)
                 treeModel_->removeRow(IdSubjectForMove.at(i-1).row(),IdSubjectForMove.at(i-1).parent());

       actionMoveSub->setEnabled(false);
       actionMerge->setEnabled(false);
       IdSubjectForMove.clear();
       if(mostUp)
           treeModel_->updateTree();

   }
    void mergeDBItem()
   {
       if(getCurrentText(4,currentIndex())!="1")
           return;

        if(IdSubjectForMove.count()<1)
            return;

         QString itemsAlter,MoveAlter;

         foreach (QModelIndex item,IdSubjectForMove)
             if(getCurrentText(4,item)=="1")
           itemsAlter += "\n"+getCurrentText(0,item);

         MoveAlter = db->getDatastr("Select Name From "+tableName+" Where Id ='"+getCurrentText(1,currentIndex())+"';");
             QMessageBox::StandardButton sb;
             sb = QMessageBox::warning(0, db->trlang("Warning"),db->trlang("Those Subjects:")+itemsAlter+"\n"+db->trlang("will merge to ")+"\n"+MoveAlter,
                                        QMessageBox::Yes | QMessageBox::No);
             if (sb == QMessageBox::No)
                 return;

       QString item_ID = getCurrentText(1,currentIndex());
       foreach (QModelIndex item,IdSubjectForMove){

       if(getCurrentText(4,item)=="1")
         {
                 db->insertsql("UPDATE "+tableName+"Value  SET idText='"+item_ID+"' WHERE  idText='"+getCurrentText(1,item)+"';");
                 db->insertsql("DELETE FROM "+tableName+" WHERE id='"+getCurrentText(1,item)+"' AND Type ='1' ");

           }
         }
       for(int i=IdSubjectForMove.size();i>0 ;i=i--)
             treeModel_->removeRow(IdSubjectForMove.at(i-1).row(),IdSubjectForMove.at(i-1).parent());

       actionMoveSub->setEnabled(false);
       actionMerge->setEnabled(false);

       IdSubjectForMove.clear();
   }
    void SetOrdering()
   {
        EditOrdering *Ordering = new EditOrdering(db,"Select id,name,ordering From "+tableName+" Where Parent_ID='"+getCurrentText(2,currentIndex())+"' ORDER BY ordering ;",tableName);
             QTableWidgetItem *itemn = new QTableWidgetItem(db->trlang("Title"));
                       QTableWidgetItem *itemo = new QTableWidgetItem(db->trlang("Ordering"));
                       Ordering->view->setHorizontalHeaderItem(1,itemn);
                       Ordering->view->setHorizontalHeaderItem(2,itemo);
                       Ordering->setWindowTitle(db->trlang("Edit Ordering"));
                       Ordering->okButton->setText(db->trlang("Ok"));
                          if(db->trlang("Align")=="Right")
                            Ordering->setLayoutDirection(Qt::RightToLeft);
                         else
                            Ordering->setLayoutDirection(Qt::LeftToRight);


             if(Ordering->exec())
               treeModel_->updateTree();
  }
    void upTree()
    {
          treeModel_->updateTree();
    }
    void restore()
    {
        QModelIndexList items;
        items = selectedIndexes();
        if(items.count()<1)
            return;
        foreach (QModelIndex item,items)
            db->insertsql("UPDATE "+tableName+" SET Parent_ID= '0' WHERE id='"+getCurrentText(1,item)+"' And Parent_ID='0101010101' ;");


              treeModel_->updateTree();
              needToUpdate=true;
    }
    void emptyTrash()
    {
        selectAll();
        deletePermantly();
       }
    void showTrash()
    {
        QDialog *trashForm = new QDialog;
        DBModel *trashModel = new DBModel(db,trashForm,true,tableName,false);
        QPushButton *okButton = new QPushButton(db->trlang("update tree"),trashForm);
              QPushButton *closeButton = new QPushButton(db->trlang("Close"),trashForm);

        connect(okButton,SIGNAL(clicked()),trashForm,SLOT(accept()));
        connect(closeButton,SIGNAL(clicked()),trashForm,SLOT(reject()));
       QVBoxLayout *mainLau = new QVBoxLayout;
       QHBoxLayout *btnLay = new QHBoxLayout;
       btnLay->addWidget(okButton);
       btnLay->addWidget(closeButton);
       btnLay->addStretch();
       DbTreeView *trashTree= new DbTreeView(db,trashForm,true,tableName,trashModel);
       mainLau->addWidget(trashTree);
       mainLau->addLayout(btnLay);
       trashForm->setLayout(mainLau);
        trashForm->setMinimumWidth(250);
        trashForm->setMinimumHeight(250);
        if(trashForm->exec()){
                  if(trashTree->needToUpdate)
                  treeModel_->updateTree();
                }
    }
    void deletePermantly()
    {
        QModelIndexList items;
        items = selectedIndexes();
        QString itemsAlter;
        foreach ( QModelIndex item,items)
            itemsAlter += "\n"+getCurrentText(0,item);
        if(items.count()<1)
            return;
        QMessageBox::StandardButton sb;
              sb = QMessageBox::critical(0, db->trlang("Warning"),db->trlang("Are you sure to Remove those Subjects:")+itemsAlter,
                                         QMessageBox::Yes | QMessageBox::No);
              if (sb == QMessageBox::No)
                  return;

        QStringList delSql;

        foreach (QModelIndex item,items)
        {
            treeModel_->listChildIds.clear();
            treeModel_->getChildsId(getCurrentText(1,item));
            delSql <<"DELETE FROM "+tableName+"  WHERE id='"+getCurrentText(1,item)+"';";
            foreach(QString id,treeModel_->listChildIds)
               delSql <<"DELETE FROM "+tableName+"  WHERE id='"+id+"';" <<"DELETE FROM "+tableName+"Value  WHERE idText='"+id+"';";
        }
        for(int i=items.size();i>0 ;i=i--)
              treeModel_->removeRow(items.at(i-1).row(),items.at(i-1).parent());

        db->insertsqls(delSql);
        needToUpdate=false;
    }

    QString getParentTitle(QString id)
    {
      return   db->getDatastr("Select Name From "+tableName+" Where Id ="+id);
    }

    void addDBItem()
      {


      QString state,IDADD,SqlRepor,idparent;
           AddDirDialog addSub;
           addSub.setFont(font());
           if(db->trlang("Align")=="Right")
               addSub.setLayoutDirection(Qt::RightToLeft);
               else
               addSub.setLayoutDirection(Qt::LeftToRight);

               addSub.setWindowTitle(db->trlang("Add Subject"));
               addSub.pushButtonCancel->setText(db->trlang("Cancel"));
               addSub.pushButtonOk->setText(db->trlang("Ok"));
               addSub.setfrist->setText(db->trlang("Set Top Level"));
               addSub.asDir->setText(db->trlang("Make As Folder"));


       QAction *action = qobject_cast<QAction *>(sender());
       if(action)
            addSub.asDir->setChecked(action->data().toBool());

           addSub.nameText->setFocus();

      addSub.nameText->setFocus();

          QString parentIdGet=db->trlang("child of");
          addSub.setfrist->setEnabled(currentIndex().isValid());
          addSub.setfrist->setChecked(currentIndex().isValid());

          addSub.asDir->setVisible(false);

          if (currentIndex().isValid()){
           if (getCurrentText(4,currentIndex())=="0")
               parentIdGet=db->trlang("child of")+getParentTitle(getCurrentText(1,currentIndex()));
           else if(getCurrentText(4,currentIndex())=="1")
                parentIdGet=db->trlang("child of")+getParentTitle(getCurrentText(2,currentIndex()));
          }
             addSub.setfrist->setText(parentIdGet);

      QString Parent_Id,Book_Id;
          if (addSub.exec()) {


              Book_Id="0";
              if (!addSub.setfrist->isChecked()){
                  Parent_Id="0";
                  Book_Id="0";
                  idparent="0";
                  state ="top";
              }  else if (currentIndex().isValid()){


                  if (getCurrentText(4,currentIndex())=="0"){
                        Parent_Id=getCurrentText(1,currentIndex());
                      //  Book_Id=getCurrentText(5,currentIndex());
                        idparent=getCurrentText(1,currentIndex());
                         state ="parent";
                     }  else if  (getCurrentText(4,currentIndex())=="1" && getCurrentText(2,currentIndex())!="0"){
                       Parent_Id=getCurrentText(2,currentIndex());
                     //  Book_Id=getCurrentText(5,currentIndex());
                         idparent= getCurrentText(2,currentIndex());
                         state ="title";
                   }else if(getCurrentText(4,currentIndex())=="1"){
                      Parent_Id=getCurrentText(2,currentIndex());
                    //  Book_Id=getCurrentText(5,currentIndex());
                       idparent= getCurrentText(2,currentIndex());
                       state ="top";
                  }
              } else{
                   Parent_Id="0";
                    idparent="0";
                    Book_Id="0";

             state ="top";
                   }
              QString Type;
              if(addSub.asDir->isChecked())
                  Type ="0";
              else
                  Type ="1";
              QStringList DiaTitles;
              if(addSub.radioButton->isChecked())
                    DiaTitles << addSub.nameText->text();
              else if(addSub.radioButton_2->isChecked())
                  DiaTitles = addSub.plainTextEdit->toPlainText().split("\n",QString::SkipEmptyParts);
              else
              {

                  foreach(QString str,addSub.textFiles)
                   {

                      QFileInfo file(str);
                      DiaTitles << file.fileName();
                  }
              }
              for(int i=0;i<DiaTitles.size();i++)
              {
              IDADD = tool.GetNodeIdCode();
              QString DiaTitle = DiaTitles.at(i);
              QString ordering = QString::number(db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='"+Parent_Id+"';").toInt()+1);
              QString sql= QString("INSERT INTO %1(id,Name,Parent_ID,Type,ordering,Date,BookId) VALUES('%2','%3','%4',%5,%6,DATETIME('NOW'),'%7');").arg(tableName)
                      .arg(IDADD).arg(DiaTitle.trimmed()).arg(Parent_Id).arg(Type).arg(ordering).arg(Book_Id);
             SqlRepor = db->insertsql(sql,"saved");
             if (SqlRepor !="Not" && currentIndex().isValid())
                  updateItemAdd(IDADD,idparent,Book_Id,Type,DiaTitles.at(i).trimmed(),state);

            if(!addSub.asDir->isChecked()){
                 QString Pageid=tool.GetNodeIdCode();
                     db->insertsql("INSERT INTO "+tableName+"Value(id,idText,BookId) VALUES('"+Pageid+"','"+IDADD+"','"+Book_Id+"')");
            }
         }
              if(addSub.radioButton_2->isChecked())
                  treeModel_->updateTree();
              else if(!currentIndex().isValid()&& Parent_Id=="0")
                     treeModel_->updateTree();

       }
     }
      void editItem(QModelIndex index)
       {
       QString id =   treeModel_->data(treeModel_->index(index.row(),1,index.parent())).toString();
       QStringList f,v;
       f << "Name";
       v<< treeModel_->data(treeModel_->index(index.row(),0,index.parent())).toString();
          if(getCurrentText(1,index)!="1288" && getCurrentText(1,index)!="1289")
            {
              db->update(tableName,f,v,"id",id);
              emit edited();
             }
       }

    void setToTopLevel()
    {
       QList<QModelIndex> itemsIndex = selectedIndexes();

      foreach (QModelIndex item,itemsIndex){

          int order= db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='0' ").toInt()+1;
          db->insertsql("UPDATE "+tableName+" SET Parent_ID= '0' ,ordering="+QString::number(order)+" WHERE ID='"+getCurrentText(1,item)+"';");
          QString idt = getCurrentText(1,item);
          QString text = getCurrentText(0,item);

          QString typet=getCurrentText(4,item);
           updateItemAdd(idt,"0","0",typet,text,"top");
      }
     for(int i=itemsIndex.size();i>0 ;i=i--)
            treeModel_->removeRow(itemsIndex.at(i-1).row(),itemsIndex.at(i-1).parent());

      }


    void MoveToTrashDBItem(){

          QModelIndexList items;
          items = selectedIndexes();
          QString itemsAlter;
          foreach ( QModelIndex item,items)
              itemsAlter += "\n"+getCurrentText(0,item);

          if(items.count()<1)
              return;
          QMessageBox::StandardButton sb;
                   sb = QMessageBox::critical(0, db->trlang("Warning"),db->trlang("Are you sure to Remove those Subjects:")+itemsAlter,
                                              QMessageBox::Yes | QMessageBox::No);
                   if (sb == QMessageBox::No)
                       return;

          foreach (QModelIndex item,items)
          {
                  db->insertsql("UPDATE "+tableName+" SET Parent_ID= '0101010101' WHERE id='"+getCurrentText(1,item)+"' ;");
          }
          for(int i=items.size();i>0 ;i=i--)
          {
                 treeModel_->removeRow(items.at(i-1).row(),items.at(i-1).parent());
          }



      }
    void findTree(){
         if(lineEdit->text()=="")
             return;

        findlist=  treeModel_->findItems(lineEdit->text(), Qt::MatchContains | Qt::MatchRecursive);
        currentFindList=0;
        setCurrentIndexInFind();


   }
    void setCurrentIndexInFind()
    {
        if(findlist.count()>currentFindList)
        {
            if(!justFindInExpanded or getCurrentText(2,findlist.at(currentFindList)->index())=="0")
                setCurrentIndex(findlist.at(currentFindList)->index());
            else
            {
                if(isExpanded(findlist.at(currentFindList)->index().parent()))
                                setCurrentIndex(findlist.at(currentFindList)->index());
            }

        }
    }
    void findTreeNext(){
       if(lineEdit->text()=="")return;

      if (currentFindList+1==findlist.count()){
            currentFindList=0;
            setCurrentIndexInFind();
       }else if(findlist.count()>currentFindList+1){
            currentFindList++;
            setCurrentIndexInFind();
       }

   }
    void findTreePrevious(){
       if(lineEdit->text()=="")
           return;

       if(currentFindList>0)
       currentFindList=currentFindList-1;
       else
       currentFindList=0;

        if(findlist.count()>currentFindList)
            setCurrentIndexInFind();

   }


protected:

signals:
    void edited();


public:

   QAction *actionMoveSub,
    *actionAddsub,*actionAddsubF,
    *actionSetToTopLevel,
    *actionMoveToTrash,
    *actionSelectToMove,
    *actionOrdering,
    *actionMerge,
    *nextfind,*updateTree,*actionXml,*actionXmlSelected,
    *previousfind,*actionDeletePermantly,*actionRestore,*actionShowTrash,*actionEmptyTrash;
   tools tool;
   QIcon SubjectIcon,TitleIcon;
   DBModel *treeModel_;
   DataBase *db;
   QString tableName;
   QModelIndexList IdSubjectForMove;
   QLabel *findLable;
   QLineEdit *lineEdit;
   QHBoxLayout *find;
   QList<QStandardItem*> findlist;
   int currentFindList;
   QToolBar *tb_find;
   bool isediteable,isTrash,needToUpdate;
   QCompleter *completer;

};

class DbTreeViewSubJectQuran : public QTreeView
{
   Q_OBJECT
    public:
    bool justFindInExpanded;
    explicit DbTreeViewSubJectQuran(DataBase *database,QWidget *parent,bool istrash,QString tableName_,DBModelSubJectQuran *treeModel,bool editeable =true)
        :QTreeView(parent)
    {

        setContextMenuPolicy(Qt::ActionsContextMenu);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        if(editeable)
        setEditTriggers(QAbstractItemView::DoubleClicked);
        else
        setEditTriggers(QAbstractItemView::NoEditTriggers);

        setAutoExpandDelay(1000);
        setAlternatingRowColors(true);
        setHeaderHidden(true);
        db = database;
        isTrash=istrash;
        treeModel_=treeModel;
        tableName=tableName_;
        isediteable=editeable;
        needToUpdate=false;
        justFindInExpanded=false;

        actionAddsub = new QAction(QIcon(":/images/add.png"),db->trlang("Add Subject"),parent);
        actionAddsubF= new QAction(QIcon(":/images/folder.png"),db->trlang("add folder"),parent);
        actionSetToTopLevel = new QAction(QIcon(":/images/toTop.png"),db->trlang("Set Top Level"),parent);
        actionMoveToTrash = new QAction(QIcon(":/images/delete.png"),db->trlang("Move To Trash"),parent);
        actionMerge = new QAction(db->trlang("Merge Subject"),parent);
        actionSelectToMove = new QAction(QIcon(":/images/ok.png"),db->trlang("Select for Move"),parent);
        actionOrdering  = new QAction(QIcon(":/images/order.png"),db->trlang("Edit Ordering"),parent);
        actionDeletePermantly   = new QAction(QIcon(":/images/delete.png"),db->trlang("Delete Permantly Selecteds"),parent);
        actionShowTrash = new QAction(QIcon(":/images/toTrash.png"),db->trlang("Show Trash"),parent);
        actionRestore = new QAction(db->trlang("Restore Seleteds"),parent);
        actionEmptyTrash= new QAction(db->trlang("EmptyTrash "),parent);
        actionMoveSub = new QAction(QIcon(":/images/move.png"),db->trlang("Move Subject"),parent);
        updateTree = new QAction(QIcon(":/images/update.png"),db->trlang("update tree"),parent);
        actionXml= new QAction(QIcon(":/images/exportXml.png"),db->trlang("export all to xml"),parent);
        actionXmlSelected= new QAction(QIcon(":/images/exportXml.png"),db->trlang("export selected to xml"),parent);

        SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
        SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);
        TitleIcon= QIcon(":/images/file.png");
       actionMoveSub->setEnabled(false);
       actionMerge->setEnabled(false);
        if(editeable && !isTrash){
       addAction(actionAddsub);
       addAction(actionAddsubF);
       addAction(actionSetToTopLevel);
       addAction(actionSelectToMove);
       addAction(actionMoveSub);
       addAction(actionMerge);
       addAction(actionOrdering);
       addAction(updateTree);
       addAction(actionMoveToTrash);
       addAction(actionShowTrash);
       addAction(actionXmlSelected);
       addAction(actionXml);



        }else if(isTrash){

            addAction(actionRestore);
            addAction(actionDeletePermantly);
            addAction(actionEmptyTrash);

        }

        actionMerge->setVisible(true);


       setModel(treeModel_);
       completer = new QCompleter;
       completer->setModel(db->getMode("Select Name From "+tableName+" Where Type =1 ;"));
       hiddenColumn();
       findLable = new QLabel(db->trlang("Find"));
       lineEdit = new QLineEdit;
       lineEdit->setCompleter(completer);
       find = new QHBoxLayout;
       tb_find = new QToolBar(this);
       nextfind = new QAction(QIcon(":/images/next-icon.png"),db->trlang("Next"), this);
       previousfind = new QAction(QIcon(":/images/previous-icon.png"),db->trlang("Previous"), this);

       tb_find->setVisible(false);
       tb_find->addAction(nextfind);
       tb_find->addAction(previousfind);
       find->addWidget(findLable);
       find->addWidget(lineEdit);
       find->addWidget(tb_find);
       connect(treeModel_,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(editItem(QModelIndex)));
       connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(chengeMove(QModelIndex)));
        actionAddsub->setData(false);
        actionAddsubF->setData(true);

       connect(actionAddsub,SIGNAL(triggered()),this,SLOT(addDBItem()));
       connect(actionAddsubF,SIGNAL(triggered()),this,SLOT(addDBItem()));

       connect(actionSetToTopLevel,SIGNAL(triggered()),this,SLOT(setToTopLevel()));
       connect(actionMoveToTrash,SIGNAL(triggered()),this,SLOT(MoveToTrashDBItem()));
       connect(actionMerge,SIGNAL(triggered()),this,SLOT(mergeDBItem()));
       connect(actionSelectToMove,SIGNAL(triggered()),this,SLOT(choiseToMoveDBItem()));
       connect(actionMoveSub,SIGNAL(triggered()),this,SLOT(moveDBItem()));
       connect(actionOrdering,SIGNAL(triggered()),this,SLOT(SetOrdering()));
       connect(treeModel_,SIGNAL(updated()),this,SLOT(hiddenColumn()));
       connect(lineEdit, SIGNAL(textEdited(QString)),this, SLOT(findTree()));
       connect(nextfind, SIGNAL(triggered()),this, SLOT(findTreeNext()));
       connect(previousfind, SIGNAL(triggered()),this, SLOT(findTreePrevious()));
       connect(actionDeletePermantly, SIGNAL(triggered()),this, SLOT(deletePermantly()));
       connect(actionRestore, SIGNAL(triggered()),this, SLOT(restore()));
       connect(actionEmptyTrash, SIGNAL(triggered()),this, SLOT(emptyTrash()));
       connect(updateTree, SIGNAL(triggered()),this, SLOT(upTree()));
       connect(actionXml, SIGNAL(triggered()),this, SLOT(exportXml()));
       connect(actionXmlSelected, SIGNAL(triggered()),this, SLOT(exportXmlSelected()));
       connect(lineEdit, SIGNAL(returnPressed()),this, SLOT(addNotFound()));






       connect(actionShowTrash, SIGNAL(triggered()),this, SLOT(showTrash()));
       connect(this, SIGNAL(expanded(QModelIndex)),this,SLOT(addChildOnExpanded(QModelIndex)));



    }

    QModelIndex getCurrentIndex(int col,QModelIndex parent)
    {
       return treeModel_->index(parent.row(),col,parent.parent());
    }
    QString getCurrentText(int col,QModelIndex parent){

        return treeModel_->data(treeModel_->index(parent.row(),col,parent.parent())).toString();

    }
    void updateItemAdd(QString id ,QString  parent_id,QString  book_id,QString Type, QString txt,QString section,bool setCurrent=true)
    {
        QList<QStandardItem*> items;
        QStandardItem *itemName = new  QStandardItem(TitleIcon,txt);
        QStandardItem *itemId = new  QStandardItem(id);
        QStandardItem *itemParentId = new  QStandardItem(parent_id);
        QStandardItem *SubjectRoot = new  QStandardItem("1");
        QStandardItem *SubjectType = new  QStandardItem(Type);
        QStandardItem *SubjectBookId = new  QStandardItem(book_id);
               if(Type=="0")
         itemName->setIcon(SubjectIcon);
         itemName->setFlags( Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
         items.append(itemName);
         items.append(itemId);
         items.append(itemParentId);
         items.append(SubjectRoot);
         items.append(SubjectType);
         items.append(SubjectBookId);
         treeModel_->getChild(itemName,itemId);

         //
        // if(Checkablebool && Type=="1")
        //     itemName->setCheckState(Qt::Unchecked);

         if ( section =="title")
         treeModel_->itemFromIndex(getCurrentIndex(0,currentIndex()))->parent()->appendRow(items);

         else if ( section =="parent")
         treeModel_->itemFromIndex(getCurrentIndex(0,currentIndex()))->appendRow(items);

          else if ( section =="top")
         treeModel_->appendRow(items);
         if(setCurrent){

         QList<QStandardItem*> indexes = treeModel_->findItems(itemName->text(),Qt::MatchExactly | Qt::MatchRecursive);
         foreach(QStandardItem *item,indexes)
            {
              if(getCurrentText(1,item->index())==itemId->text())
                 setCurrentIndex(item->index());
             }
         }
    }
    QModelIndexList getSelectedIndex()
    {
      return selectedIndexes();
    }


public slots:
    void exportXml()
    {
        treeModel_->getXMl();
    }
    void exportXmlSelected()
    {
        QModelIndexList items;
        items = selectedIndexes();
        QStringList idList;
        foreach ( QModelIndex item,items)
            idList << getCurrentText(1,item);

        treeModel_->getXMl(idList);
    }
    void addChildOnExpanded(QModelIndex index)
    {
        QStandardItem *item= treeModel_->itemFromIndex(index);
        for(int i =0;i<item->rowCount();i++)
        {
        if(item->child(i)->rowCount()==0)
          treeModel_->getChild(item->child(i),item->child(i,1));

        }
    }
    void hiddenColumn(){

            setColumnHidden(1,true);
            setColumnHidden(2,true);
            setColumnHidden(3,true);
            setColumnHidden(4,true);
            setColumnHidden(5,true);
        }
    void choiseToMoveDBItem()
   {
       IdSubjectForMove.clear();
       IdSubjectForMove = selectedIndexes();
       if(IdSubjectForMove.count()<1)
           return;
        actionMoveSub->setEnabled(true);
        actionMerge->setEnabled(true);

   }
    void getChildsId()
   {

      treeModel_->listChildIds.clear();
      treeModel_->getChildsId(getCurrentText(1,currentIndex()));
      QMessageBox::StandardButton sb;

      sb = QMessageBox::warning(0, db->trlang("Warning"),treeModel_->listChildIds.join("\n"),
                                 QMessageBox::Yes | QMessageBox::No);
      if (sb == QMessageBox::No)
          return;
      else
          return;

   }
    void chengeMove(QModelIndex)
    {
        QString text;
        if(getCurrentText(4,currentIndex())=="0")
            text= getCurrentText(0,currentIndex());
        else
            text= getCurrentText(0,currentIndex().parent());

        if(text=="")
            text = db->trlang("first list");

      actionMoveSub->setText(db->trlang("Move Subject")+text);
    }
    void moveDBItem()
   {
        if(IdSubjectForMove.count()<1)
            return;
        bool mostUp=false;
         QString itemsAlter,MoveAlter,Parent_ID;
         foreach ( QModelIndex item,IdSubjectForMove)
             itemsAlter += "\n"+getCurrentText(0,item);

         if(getCurrentText(4,currentIndex())=="0")
         {
             MoveAlter= getCurrentText(0,currentIndex());
              Parent_ID=getCurrentText(1,currentIndex());
         }else
         {
             MoveAlter= getCurrentText(0,currentIndex().parent());
             Parent_ID=db->getDatastr("Select Parent_Id From "+tableName+" Where Id ='"+getCurrentText(1,currentIndex())+"';");


         }

         if(MoveAlter==""){
             MoveAlter = db->trlang("first list");
             mostUp=true;
         }

       QMessageBox::StandardButton sb;
       sb = QMessageBox::warning(0, db->trlang("Warning"),db->trlang("Those Subjects:")+itemsAlter+"\n"+db->trlang("will Move to ")+"\n"+MoveAlter,
                                  QMessageBox::Yes | QMessageBox::No);
       if (sb == QMessageBox::No)
           return;


           foreach (QModelIndex item,IdSubjectForMove){
               int order= db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='"+Parent_ID+"';").toInt()+1;
               db->insertsql("UPDATE "+tableName+" SET Parent_ID='"+Parent_ID+"' ,ordering="+QString::number(order)+" WHERE id='"+getCurrentText(1,item)+"';");

               QString idt    = getCurrentText(1,item);
               QString typet  =getCurrentText(4,item);
               QString txt    = getCurrentText(0,item);
               QString parent = Parent_ID;
               if(!mostUp)
                 updateItemAdd(idt,parent,"0",typet,txt,"parent",false);
           }
           for(int i=IdSubjectForMove.size();i>0 ;i=i--)
                 treeModel_->removeRow(IdSubjectForMove.at(i-1).row(),IdSubjectForMove.at(i-1).parent());

       actionMoveSub->setEnabled(false);
       actionMerge->setEnabled(false);
       IdSubjectForMove.clear();
       if(mostUp)
           treeModel_->updateTree();

   }
    void mergeDBItem()
   {
       if(getCurrentText(4,currentIndex())!="1")
           return;

        if(IdSubjectForMove.count()<1)
            return;

         QString itemsAlter,MoveAlter;

         foreach (QModelIndex item,IdSubjectForMove)
             if(getCurrentText(4,item)=="1")
           itemsAlter += "\n"+getCurrentText(0,item);

          MoveAlter = db->getDatastr("Select Name From "+tableName+" Where Id ='"+getCurrentText(1,currentIndex())+"';");
       QMessageBox::StandardButton sb;
       sb = QMessageBox::warning(0, db->trlang("Warning"),db->trlang("Those Subjects:")+itemsAlter+"\n"+db->trlang("will merge to ")+"\n"+MoveAlter,
                                  QMessageBox::Yes | QMessageBox::No);
       if (sb == QMessageBox::No)
           return;

       QString item_ID = getCurrentText(1,currentIndex());
       foreach (QModelIndex item,IdSubjectForMove){

       if(getCurrentText(4,item)=="1")
         {
                 db->insertsql("UPDATE Subjects  SET ID_SUBJECT='"+item_ID+"' WHERE  ID_SUBJECT='"+getCurrentText(1,item)+"';");
                 db->insertsql("DELETE FROM "+tableName+" WHERE id='"+getCurrentText(1,item)+"' AND Type ='1' ");

           }
         }
       for(int i=IdSubjectForMove.size();i>0 ;i=i--)
             treeModel_->removeRow(IdSubjectForMove.at(i-1).row(),IdSubjectForMove.at(i-1).parent());

       actionMoveSub->setEnabled(false);
       actionMerge->setEnabled(false);

       IdSubjectForMove.clear();
   }
    void SetOrdering()
   {
       EditOrdering *Ordering = new EditOrdering(db,"Select id,name,ordering From "+tableName+" Where Parent_ID='"+getCurrentText(2,currentIndex())+"' ORDER BY ordering ;",tableName);
       QTableWidgetItem *itemn = new QTableWidgetItem(db->trlang("Title"));
                 QTableWidgetItem *itemo = new QTableWidgetItem(db->trlang("Ordering"));
                 Ordering->view->setHorizontalHeaderItem(1,itemn);
                 Ordering->view->setHorizontalHeaderItem(2,itemo);
                 Ordering->setWindowTitle(db->trlang("Edit Ordering"));
                 Ordering->okButton->setText(db->trlang("Ok"));
                    if(db->trlang("Align")=="Right")
                      Ordering->setLayoutDirection(Qt::RightToLeft);
                   else
                      Ordering->setLayoutDirection(Qt::LeftToRight);


       if(Ordering->exec())
         treeModel_->updateTree();
   }
    void upTree()
    {
          treeModel_->updateTree();
    }
    void restore()
    {
        QModelIndexList items;
        items = selectedIndexes();
        if(items.count()<1)
            return;
        foreach (QModelIndex item,items)
            db->insertsql("UPDATE "+tableName+" SET Parent_ID= '0' WHERE id='"+getCurrentText(1,item)+"' And Parent_ID='0101010101' ;");


              treeModel_->updateTree();
              needToUpdate=true;
    }
    void emptyTrash()
    {
        selectAll();
        deletePermantly();
       }
    void showTrash()
    {
        QDialog *trashForm = new QDialog;
        DBModelSubJectQuran *trashModel = new DBModelSubJectQuran(db,trashForm,true,tableName,false);
        QPushButton *okButton = new QPushButton(db->trlang("update tree"),trashForm);
        QPushButton *closeButton = new QPushButton(db->trlang("Close"),trashForm);

        connect(okButton,SIGNAL(clicked()),trashForm,SLOT(accept()));
        connect(closeButton,SIGNAL(clicked()),trashForm,SLOT(reject()));
       QVBoxLayout *mainLau = new QVBoxLayout;
       QHBoxLayout *btnLay = new QHBoxLayout;
       btnLay->addWidget(okButton);
       btnLay->addWidget(closeButton);
       btnLay->addStretch();
       DbTreeViewSubJectQuran *trashTree= new DbTreeViewSubJectQuran(db,trashForm,true,tableName,trashModel);
       mainLau->addWidget(trashTree);
       mainLau->addLayout(btnLay);
       trashForm->setLayout(mainLau);
        trashForm->setMinimumWidth(250);
        trashForm->setMinimumHeight(250);
        if(trashForm->exec()){
                  if(trashTree->needToUpdate)
                  treeModel_->updateTree();
                }
    }
    void deletePermantly()
    {
        QModelIndexList items;
        items = selectedIndexes();
        QString itemsAlter;
        foreach ( QModelIndex item,items)
            itemsAlter += "\n"+getCurrentText(0,item);
        if(items.count()<1)
            return;
        QMessageBox::StandardButton sb;
        sb = QMessageBox::critical(0, db->trlang("Warning"),db->trlang("Are you sure to Remove those Subjects:")+itemsAlter,
                                   QMessageBox::Yes | QMessageBox::No);
        if (sb == QMessageBox::No)
            return;

        QStringList delSql;

        foreach (QModelIndex item,items)
        {
            treeModel_->listChildIds.clear();
            treeModel_->getChildsId(getCurrentText(1,item));
            delSql <<"DELETE FROM "+tableName+"  WHERE id='"+getCurrentText(1,item)+"';";
            foreach(QString id,treeModel_->listChildIds)
               delSql <<"DELETE FROM "+tableName+"  WHERE id='"+id+"';" <<"DELETE FROM  Subjects  WHERE ID_SUBJECT='"+id+"';";

        }
        for(int i=items.size();i>0 ;i=i--)
              treeModel_->removeRow(items.at(i-1).row(),items.at(i-1).parent());

        db->insertsqls(delSql);
        needToUpdate=false;
    }

    QString getParentTitle(QString id)
    {
      return   db->getDatastr("Select Name From "+tableName+" Where Id ="+id);
    }

    void addDBItem()
      {

      QString state,IDADD,SqlRepor,idparent;
      AddDirDialog addSub;
      addSub.setFont(font());
      if(db->trlang("Align")=="Right")
          addSub.setLayoutDirection(Qt::RightToLeft);
          else
          addSub.setLayoutDirection(Qt::LeftToRight);

          addSub.setWindowTitle(db->trlang("Add Subject"));
          addSub.pushButtonCancel->setText(db->trlang("Cancel"));
          addSub.pushButtonOk->setText(db->trlang("Ok"));
          addSub.setfrist->setText(db->trlang("Set Top Level"));
          addSub.asDir->setText(db->trlang("Make As Folder"));

       QAction *action = qobject_cast<QAction *>(sender());
       if(action)
            addSub.asDir->setChecked(action->data().toBool());



      addSub.nameText->setFocus();

      QString parentIdGet=db->trlang("child of");
      addSub.setfrist->setEnabled(currentIndex().isValid());
      addSub.setfrist->setChecked(currentIndex().isValid());

      addSub.asDir->setVisible(false);

      if (currentIndex().isValid()){
       if (getCurrentText(4,currentIndex())=="0")
           parentIdGet=db->trlang("child of")+getParentTitle(getCurrentText(1,currentIndex()));
       else if(getCurrentText(4,currentIndex())=="1")
            parentIdGet=db->trlang("child of")+getParentTitle(getCurrentText(2,currentIndex()));
      }
         addSub.setfrist->setText(parentIdGet);

      QString Parent_Id,Book_Id;
          if (addSub.exec()) {


              Book_Id="0";
              if (!addSub.setfrist->isChecked()){
                  Parent_Id="0";
                  Book_Id="0";
                  idparent="0";
                  state ="top";
              }  else if (currentIndex().isValid()){


                  if (getCurrentText(4,currentIndex())=="0"){
                        Parent_Id=getCurrentText(1,currentIndex());
                      //  Book_Id=getCurrentText(5,currentIndex());
                        idparent=getCurrentText(1,currentIndex());
                         state ="parent";
                     }  else if  (getCurrentText(4,currentIndex())=="1" && getCurrentText(2,currentIndex())!="0"){
                       Parent_Id=getCurrentText(2,currentIndex());
                     //  Book_Id=getCurrentText(5,currentIndex());
                         idparent= getCurrentText(2,currentIndex());
                         state ="title";
                   }else if(getCurrentText(4,currentIndex())=="1"){
                      Parent_Id=getCurrentText(2,currentIndex());
                    //  Book_Id=getCurrentText(5,currentIndex());
                       idparent= getCurrentText(2,currentIndex());
                       state ="top";
                  }
              } else{
                   Parent_Id="0";
                    idparent="0";
                    Book_Id="0";

             state ="top";
                   }
              QString Type;
              if(addSub.asDir->isChecked())
                  Type ="0";
              else
                  Type ="1";
              QStringList DiaTitles;
              if(addSub.radioButton->isChecked())
                    DiaTitles << addSub.nameText->text();
              else if(addSub.radioButton_2->isChecked())
                  DiaTitles = addSub.plainTextEdit->toPlainText().split("\n",QString::SkipEmptyParts);
              else
              {

                  foreach(QString str,addSub.textFiles)
                   {

                      QFileInfo file(str);
                      DiaTitles << file.fileName();
                  }
              }
              for(int i=0;i<DiaTitles.size();i++)
              {
              IDADD = tool.GetNodeIdCode();
              QString DiaTitle = DiaTitles.at(i);
              QString ordering = QString::number(db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='"+Parent_Id+"';").toInt()+1);
              QString sql= QString("INSERT INTO %1 VALUES('%2','%3','%4',%5,'',%6,DATETIME('NOW'),'%7');").arg(tableName)
                      .arg(IDADD).arg(DiaTitle.trimmed()).arg(Parent_Id).arg(Type).arg(ordering).arg(Book_Id);
             SqlRepor = db->insertsql(sql,"saved");
             if (SqlRepor !="Not" && currentIndex().isValid())
                  updateItemAdd(IDADD,idparent,Book_Id,Type,DiaTitles.at(i).trimmed(),state);


         }
              if(addSub.radioButton_2->isChecked())
                  treeModel_->updateTree();
              else if(!currentIndex().isValid()&& Parent_Id=="0")
                     treeModel_->updateTree();

       }
     }
      void editItem(QModelIndex index)
       {
       QString id =   treeModel_->data(treeModel_->index(index.row(),1,index.parent())).toString();
       QStringList f,v;
       f << "Name";
       v<< treeModel_->data(treeModel_->index(index.row(),0,index.parent())).toString();
          if(getCurrentText(1,index)!="1288" && getCurrentText(1,index)!="1289")
            {
              db->update(tableName,f,v,"id",id);
              emit edited();
             }
       }

    void setToTopLevel()
    {
       QList<QModelIndex> itemsIndex = selectedIndexes();

      foreach (QModelIndex item,itemsIndex){

          int order= db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='0' ").toInt()+1;
          db->insertsql("UPDATE "+tableName+" SET Parent_ID= '0' ,ordering="+QString::number(order)+" WHERE ID='"+getCurrentText(1,item)+"';");
          QString idt = getCurrentText(1,item);
          QString text = getCurrentText(0,item);

          QString typet=getCurrentText(4,item);
           updateItemAdd(idt,"0","0",typet,text,"top");
      }
     for(int i=itemsIndex.size();i>0 ;i=i--)
            treeModel_->removeRow(itemsIndex.at(i-1).row(),itemsIndex.at(i-1).parent());

      }


    void MoveToTrashDBItem(){

          QModelIndexList items;
          items = selectedIndexes();
          QString itemsAlter;
          foreach ( QModelIndex item,items)
              itemsAlter += "\n"+getCurrentText(0,item);

          if(items.count()<1)
              return;
          QMessageBox::StandardButton sb;
          sb = QMessageBox::critical(0, db->trlang("Warning"),db->trlang("Are you sure to Remove those Subjects:")+itemsAlter,
                                     QMessageBox::Yes | QMessageBox::No);
          if (sb == QMessageBox::No)
              return;

          foreach (QModelIndex item,items)
          {
                  db->insertsql("UPDATE "+tableName+" SET Parent_ID= '0101010101' WHERE id='"+getCurrentText(1,item)+"' ;");
          }
          for(int i=items.size();i>0 ;i=i--)
          {
                 treeModel_->removeRow(items.at(i-1).row(),items.at(i-1).parent());
          }



      }
    void addNotFound()
      {
          if(lineEdit->text()=="")
              return;
          findlist=  treeModel_->findItems(lineEdit->text(),Qt::MatchExactly | Qt::MatchRecursive);
         if(findlist.count()>0)
             return;
         QMessageBox::StandardButton sb;
         sb = QMessageBox::information(0, db->trlang("Add subject"),db->trlang("Do you want to add this subject to list:")+"\n"+lineEdit->text(),
                                    QMessageBox::Yes | QMessageBox::No);
         if (sb == QMessageBox::No)
             return;

         QString IDADD = db->GetNodeIdCode();
         QString ordering =QString::number(db->getDatastr("Select MAX(ordering) From "+tableName+" Where Parent_Id='0';").toInt()+1);
         QString sql = "INSERT INTO "+tableName+" VALUES('"+IDADD+"','"+lineEdit->text()+"','0',1,'','"+ordering+"',DATETIME('NOW'),'0');";
        if (db->insertsql(sql) =="ok" && currentIndex().isValid())
            updateItemAdd(IDADD,"0","0","1",lineEdit->text().trimmed(),"top");


      }
    void findTree(){
         if(lineEdit->text()=="")
             return;

        findlist=  treeModel_->findItems(lineEdit->text(), Qt::MatchContains | Qt::MatchRecursive);
        currentFindList=0;
        setCurrentIndexInFind();


   }
    void setCurrentIndexInFind()
    {
        if(findlist.count()>currentFindList)
        {
            if(!justFindInExpanded or getCurrentText(2,findlist.at(currentFindList)->index())=="0")
                setCurrentIndex(findlist.at(currentFindList)->index());
            else
            {
                if(isExpanded(findlist.at(currentFindList)->index().parent()))
                                setCurrentIndex(findlist.at(currentFindList)->index());
            }

        }
    }
    void findTreeNext(){
       if(lineEdit->text()=="")return;

      if (currentFindList+1==findlist.count()){
            currentFindList=0;
            setCurrentIndexInFind();
       }else if(findlist.count()>currentFindList+1){
            currentFindList++;
            setCurrentIndexInFind();
       }

   }
    void findTreePrevious(){
       if(lineEdit->text()=="")
           return;

       if(currentFindList>0)
       currentFindList=currentFindList-1;
       else
       currentFindList=0;

        if(findlist.count()>currentFindList)
            setCurrentIndexInFind();

   }


protected:

signals:
    void edited();


public:

   QAction *actionMoveSub,
    *actionAddsub,*actionAddsubF,
    *actionSetToTopLevel,
    *actionMoveToTrash,
    *actionSelectToMove,
    *actionOrdering,
    *actionMerge,
    *nextfind,*updateTree,*actionXml,*actionXmlSelected,
    *previousfind,*actionDeletePermantly,*actionRestore,*actionShowTrash,*actionEmptyTrash;
   tools tool;
   QIcon SubjectIcon,TitleIcon;
   DBModelSubJectQuran *treeModel_;
   DataBase *db;
   QString tableName;
   QModelIndexList IdSubjectForMove;
   QLabel *findLable;
   QLineEdit *lineEdit;
   QHBoxLayout *find;
   QList<QStandardItem*> findlist;
   int currentFindList;
   QToolBar *tb_find;
   bool isediteable,isTrash,needToUpdate;
   QCompleter *completer;

};


class VerseTree :public QTreeView
{
Q_OBJECT
public:
    explicit VerseTree(QWidget *parent=0):QTreeView(parent)
    {
        setDragDropMode(QAbstractItemView::DropOnly);
        setAutoExpandDelay(1000);
        setAlternatingRowColors(true);
        setHeaderHidden(true);
    }
protected:

     void dropEvent(QDropEvent *e)
     {
         emit droped();
         e->accept();
     }


signals:
    void droped();

};
class QuranTreeView : public QTreeView
{
   Q_OBJECT
    public:
    DataBase *db;
    QuranModel *quranModel_;

    explicit QuranTreeView(DataBase *database,QuranModel *quranModel,QString Id,QWidget *parent=0)
        :QTreeView(parent)
    {
        db =database;
        quranModel_=quranModel;
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setAutoExpandDelay(1000);
        setAlternatingRowColors(true);
        setHeaderHidden(true);    
        setModel(quranModel);
        setColumnHidden(1,true);
        setColumnHidden(2,true);
        setMinimumWidth(169);
        setMaximumWidth(170);
        setCurrent(Id);


    }
    void setCurrent(QString Id)
    {
        QString chapterTxt =db->getDatastr(QString("Select Chapter From Quran Where id= %1 ;").arg(Id));
        QString VerseTxt =db->getDatastr(QString("Select Verse From Quran Where id= %1 ;").arg(Id));
        setCurrentIndex(quranModel_->index(VerseTxt.toInt()-1,0,quranModel_->index(chapterTxt.toInt()-1,0)));

    }
    QModelIndexList selected()
    {
     return selectedIndexes();
    }

};
#endif // DBTREEVIEW_H
