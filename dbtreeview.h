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

class DbTreeView : public QTreeView
{
   Q_OBJECT
    public:
    bool justFindInExpanded;
    explicit DbTreeView(DataBase *database,QWidget *parent,bool istrash,QString tableName_,DBModel *treeModel,bool editeable =true)    ;
    QModelIndex getCurrentIndex(int col,QModelIndex parent);
    QString getCurrentText(int col,QModelIndex parent);
    void updateItemAdd(QString id ,QString  parent_id,QString  book_id,QString Type, QString txt,QString section,bool setCurrent=true);
    QModelIndexList getSelectedIndex();
public slots:
    void exportXml();
    void exportXmlSelected();
    void addChildOnExpanded(QModelIndex index);
    void hiddenColumn();
    void choiseToMoveDBItem();
    void getChildsId();
    void chengeMove(QModelIndex);
    void moveDBItem();
    void mergeDBItem();
    void SetOrdering();
    void upTree();
    void restore();
    void emptyTrash();
    void showTrash();
    void deletePermantly();
    QString getParentTitle(QString id);
    void addDBItem();
    void editItem(QModelIndex index);
    void setToTopLevel();
    void MoveToTrashDBItem();
    void findTree();
    void setCurrentIndexInFind();
    void findTreeNext();
    void findTreePrevious();

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
       ;
    QModelIndex getCurrentIndex(int col,QModelIndex parent);
    QString getCurrentText(int col,QModelIndex parent);
    void updateItemAdd(QString id ,QString  parent_id,QString  book_id,QString Type, QString txt,QString section,bool setCurrent=true);
    QModelIndexList getSelectedIndex();

public slots:
    void exportXml();
    void exportXmlSelected();
    void addChildOnExpanded(QModelIndex index);
    void hiddenColumn();
    void choiseToMoveDBItem();
    void getChildsId();
    void chengeMove(QModelIndex);
    void moveDBItem();
    void mergeDBItem();
    void SetOrdering();
    void upTree();
    void restore();
    void emptyTrash();
    void showTrash();
    void deletePermantly();
    QString getParentTitle(QString id);
    void addDBItem();
    void editItem(QModelIndex index);
    void setToTopLevel();

    void MoveToTrashDBItem();
    void addNotFound();
    void findTree();
    void setCurrentIndexInFind();
    void findTreeNext();
    void findTreePrevious();

protected:

signals:
    void edited();
    void addedItem(QModelIndex);


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
    explicit VerseTree(QWidget *parent=0);
protected:

     void dropEvent(QDropEvent *e);
signals:
    void droped();

};
class QuranTreeView : public QTreeView
{
   Q_OBJECT
    public:
    DataBase *db;
    QuranModel *quranModel_;

    explicit QuranTreeView(DataBase *database,QuranModel *quranModel,QString Id,QWidget *parent=0);
   void setCurrent(QString Id);
   QModelIndexList selected();
};
#endif // DBTREEVIEW_H
