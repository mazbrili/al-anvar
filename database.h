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

#ifndef DATABASE_H
#define DATABASE_H

#include "tools.h"
#include "qmlstandarditemmodel.h"

class DataBase :public tools
{
public:

  DataBase(  QString dir);
  static void sqlite3_regexp(sqlite3_context *context, int argc,sqlite3_value **argv);
  bool open(QString dbname);
  bool isOpen ();
  void close ();
  QStringList  getTables();
  QStringList  getfieldName(QString query);
  QString getDatastr (QString query);
  QStringList getDataSpilt (QString query);
  QStringList getData (QString query);
  QStringList getDatasame (QString query);
  QList<QStringList>  getListData(QString query);
  QmlStandardItemModel* getQmlModel(QString query);
  void updateQmlModelHighlighSearch(QString query,QmlStandardItemModel *model,int colHighlight,QString searchText);
  void updateQmlModel(QString query,QmlStandardItemModel *model);
  void updateQmlModelSubject(QString query,QmlStandardItemModel *model,int filedTrLang,QString tag,int filedIco,int idFild);

  QStandardItemModel* getMode(QString query);
  QStandardItemModel* getModelTr(QString query);
  QSortFilterProxyModel* getProxyMode(QString query);
  QString insertsql(QString query,QString Alter="ok");
  void  insertsqls(QStringList Sql,bool begin=true);
  QString getsqlTable(QString tableName);
  QList<QStringList>  getUpListData(QString query);
  QStringList backup(QStringList tables);
  QString  lastError();
  QList<QStringList> select(QString table,QStringList filds,QString findType="=",QString findfild1="",QString text1="",QString findfild2="",QString text2="",QString findfild3="",QString text3="");
  QString insert(QString table,QStringList fild,QStringList value,bool cheackExist=false,QString cheackfild1="",QString cheacktext1="",QString cheackfild2="",QString cheacktext2="",QString cheackfild3="",QString cheacktext3="");
  void begin();
  void commit();
  QString update(QString table,QStringList filds,QStringList value,QString findfild1,QString text1,QString findType="=",QString findfild2="",QString text2="",QString findfild3="",QString text3="");
  QString deleterow(QString table,QString findfild1,QString text1,QString findType="=",QString findfild2="",QString text2="",QString findfild3="",QString text3="");
  bool isAdded(QString table,QString fild1,QString text1,QString fild2="",QString text2="",QString fild3="",QString text3="");
  QString strinListToStr(QStringList list,bool cution=false);
  QString forupdate(QStringList filds,QStringList value);
  void Exportsqls(QStringList Sql,QString fileName);
  QString err;
  QStringList errList;
  sqlite3 * _db;

};

#endif // DATABASE_H
