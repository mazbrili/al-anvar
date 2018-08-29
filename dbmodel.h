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
#ifndef DBMODEL_H
#define DBMODEL_H


#include "database.h"
#include "dbsearch.h"
#include "editordering.h"


class DBModel : public QStandardItemModel
{
    Q_OBJECT
public:

    explicit DBModel(DataBase *database,QWidget *parent = 0,bool istrush=false,QString nameTable="BankNotes",bool leyzing=false,bool Checkable=false,bool updatable =true,QString fildOrder="ordering",bool sortAsc=true);
    void updateAsTreeWidget(QTreeWidget *treeWidget);
    void updateAsTreeWidget(QTreeWidget *treeWidget,QStringList idList);
    void getTreeWidgetItem(QString id,QString type,QString name ,QTreeWidgetItem *treeItem);
    void getXMl();
    void getXMl(QStringList idList);
    bool writeTreeToXml(QIODevice *device,QTreeWidget *treeWidget);
    void writeItem(QTreeWidgetItem *item);
    void updateTree();
    QString getCurrentText(int col,QModelIndex parent);
    void getParents(QString findTxt="0");
public slots:
    void getChild(QStandardItem *iname,QStandardItem *id);
    void getChildsId(QString id);

signals:
     void updated();
     void childFinded(QString);
public:
     DataBase *db;
     tools tool;
     QString tableName,Ordering,ascending;
     QIcon SubjectIcon,TitleIcon;
     QStringList listChildIds;
     bool Checkablebool,isediteable,isleyzing,isTrush;
     QXmlStreamWriter xml;


};

class DBModelSubJectQuran : public QStandardItemModel
{
    Q_OBJECT
public:


    explicit DBModelSubJectQuran(DataBase *database,QWidget *parent = 0,bool istrush=false,QString nameTable="BankNotes",bool leyzing=false,bool Checkable=false,bool updatable =true,QString fildOrder="ordering",bool sortAsc=true);
    void updateAsTreeWidget(QTreeWidget *treeWidget);
    void updateAsTreeWidget(QTreeWidget *treeWidget,QStringList idList);
    void getTreeWidgetItem(QString id,QString type,QString name ,QTreeWidgetItem *treeItem);
    void getXMl();
    void getXMl(QStringList idList);
    bool writeTreeToXml(QIODevice *device,QTreeWidget *treeWidget);
    void writeItem(QTreeWidgetItem *item);
    void updateTree();

    QString getCurrentText(int col,QModelIndex parent);
    void getParents(QString findTxt="0");
public slots:
    void getChild(QStandardItem *iname,QStandardItem *id);
    void getChildsId(QString id);
signals:
     void updated();
     void childFinded(QString);
public:
     DataBase *db;
     tools tool;
     QString tableName,Ordering,ascending;
     QIcon SubjectIcon,TitleIcon;
     QStringList listChildIds;
     bool Checkablebool,isediteable,isleyzing,isTrush;
     QXmlStreamWriter xml;


};
class QuranModel : public QStandardItemModel {

    Q_OBJECT
        public:
    DataBase *db;
    QIcon SubjectIcon,TitleIcon;

    explicit QuranModel(DataBase *database,QWidget *parent = 0);
    void getChild(QStandardItem *iname,QStandardItem *id);};


#endif // DBMODEL_H
