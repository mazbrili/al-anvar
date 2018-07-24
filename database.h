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


class DataBase :public tools
{
public:

    DataBase(  QString dir)
        {

          if(!open(dir)){
              QMessageBox::critical(0,"DataBase","DataBase Not Found..");
              WriteSettings("DataBaseDir",defultPath);
              open(dir);
          }


        }
    static void sqlite3_regexp(sqlite3_context *context, int argc,
       sqlite3_value **argv){
           Q_ASSERT( argc==2 );

              const unsigned char* pattern_utf8 = sqlite3_value_text(argv[0]);
              const unsigned char* lhs_utf8 = sqlite3_value_text(argv[1]);

              if( !lhs_utf8 || !pattern_utf8 ) return;

              QString pattern = QString::fromUtf8( (const char*)pattern_utf8 );
              QString lhs = QString::fromUtf8( (const char*)lhs_utf8 );

              QRegExp regexp ( pattern, Qt::CaseSensitive, QRegExp::RegExp2 );
              bool contains = lhs.contains( regexp );

              sqlite3_result_int( context, contains?1:0 );
       }

  bool open(QString dbname){

        bool ok=false;
        int  err;
        err = sqlite3_open(dbname.toUtf8(), &_db);
        if (err) {
           errList << QString(sqlite3_errmsg(_db));
          sqlite3_close(_db);
          _db = 0;
          return false;
        }
       if (_db){
       if (SQLITE_OK==sqlite3_exec(_db,"PRAGMA empty_result_callbacks = ON;",
                                     NULL,NULL,NULL)){
       if (SQLITE_OK==sqlite3_exec(_db,"PRAGMA show_datatypes = ON;",
                                     NULL,NULL,NULL)){
           ok=true;
         }
       }
     }
       sqlite3_create_function(_db, "regexp", 2, SQLITE_UTF8, 0,sqlite3_regexp, 0, 0);
        return ok;

 }
  bool isOpen ()
   {
       return _db!=0;
   }
  void close (){
       if (_db)
       {

         //  "is open"
           ;
       }
    sqlite3_close(_db);
      _db = 0;
      err.clear();
      errList.clear();
   }

  QStringList  getTables()
   {
     return getData("SELECT name FROM sqlite_master WHERE type='table' ;");
   }
  QStringList  getfieldName(QString query)
    {
        char *zErrMsg;
        char **result;
        int rc;
        int nrow,ncol;
        QStringList r;
        rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);
        if( rc == SQLITE_OK ){
              for(int i=0; i < ncol; ++i)
                 r << QString::fromUtf8(result[i]);  /* First row heading */
        }else
            errList <<  zErrMsg;
        sqlite3_free_table(result);

  return r;

    }
  QString getDatastr (QString query)
       {
       char *zErrMsg;
       char **result;
       int rc;
       int nrow,ncol;
       QString r;
       rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);

       if( rc == SQLITE_OK && nrow>0)
                  r = QString::fromUtf8(result[ncol]);
             else
                errList <<  zErrMsg;

            sqlite3_free_table(result);
     return r;
       }
  QStringList getDataSpilt (QString query)
   {

       char *zErrMsg;
       char **result;
       int rc;
       int nrow,ncol;
       QStringList mainList;
       rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);

       if( rc == SQLITE_OK ){


          for (int r=1 ;r<=nrow;r++ ){
              mainList << QString::fromUtf8(result[r*ncol+0])+"#-I?D-SPL?IT-#"+QString::fromUtf8(result[r*ncol+1]);

           }
   }else
       errList <<  zErrMsg;

       sqlite3_free_table(result);
       return mainList;

   }

  QStringList getData (QString query)
   {
          char *zErrMsg;
          char **result;
          int rc;
          int nrow,ncol;
          QStringList r;
          rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);

          if( rc == SQLITE_OK ){
                 for(int i=0; i < ncol*nrow; ++i)
                     r << QString::fromUtf8(result[ncol+i]);

                }else
                   errList <<  zErrMsg;

               sqlite3_free_table(result);

    return r;

   }
  QStringList getDatasame (QString query)
   {
          char *zErrMsg;
          char **result;
          int rc;
          int nrow,ncol;
          QStringList r;
          rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);

          if( rc == SQLITE_OK ){
                 for(int i=0; i < ncol*nrow; ++i){
                     if(!r.contains(QString::fromUtf8(result[ncol+i])))
                     r << QString::fromUtf8(result[ncol+i]);
                 }

                }else
                   errList <<  zErrMsg;

               sqlite3_free_table(result);

    return r;

   }

  QList<QStringList>  getListData(QString query)
    {
        char *zErrMsg;
        char **result;
        int rc;
        int nrow,ncol;
        QStringList tmpList;
        QList<QStringList>  mainList;
        rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);

        if( rc == SQLITE_OK ){

             for (int c = 0;c<ncol;c++ ){
                       for (int r=1 ;r<=nrow;r++ )
                          tmpList << QString::fromUtf8(result[r*ncol+c]);
                  mainList.append(tmpList);
                  tmpList.clear();
            }
    }else
        errList <<  zErrMsg;
        sqlite3_free_table(result);
        return mainList;
    }



  QStandardItemModel* getMode(QString query)
    {
      QStandardItemModel *model = new QStandardItemModel();

        char *zErrMsg;
        char **result;
        int rc;
        int nrow,ncol;
        rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);
        model->setColumnCount(ncol);
        model->setRowCount(nrow);
        if( rc == SQLITE_OK ){

             for (int c = 0;c<ncol;c++ ){
                       for (int r=1 ;r<=nrow;r++ )
                         {
                         QModelIndex index = model->index(r-1, c);
                         model->setData(index,QString::fromUtf8(result[r*ncol+c]));
                         }
            }
    }else
        errList <<  zErrMsg;

        sqlite3_free_table(result);
        return model;
    }
  QStandardItemModel* getModelTr(QString query)
    {
      QStandardItemModel *model = new QStandardItemModel();

        char *zErrMsg;
        char **result;
        int rc;
        int nrow,ncol;
        rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);
        model->setColumnCount(ncol);
        model->setRowCount(nrow);
        if( rc == SQLITE_OK ){

             for (int c = 0;c<ncol;c++ ){
                       for (int r=1 ;r<=nrow;r++ )
                         {
                         QModelIndex index = model->index(r-1, c);
                         model->setData(index,trlang(QString::fromUtf8(result[r*ncol+c])));
                         }
            }
    }else
        errList <<  zErrMsg;

        sqlite3_free_table(result);
        return model;
    }

  QSortFilterProxyModel* getProxyMode(QString query)
    {
        QSortFilterProxyModel *proxyModel= new QSortFilterProxyModel();
        proxyModel->setSourceModel(getMode(query));
        return proxyModel;
    }
  QString  insertsql(QString query,QString Alter="ok")
   {


       int err=0;
      err=sqlite3_exec(_db,query.toUtf8(),NULL,NULL,NULL);
      if (err == SQLITE_OK) {
          return Alter;

      }
      else{
              errList << QString(sqlite3_errmsg(_db));
              return QString(sqlite3_errmsg(_db));
       }

    }

  void   insertsqls(QStringList Sql,bool begin=true)
   {
      if(begin)
      insertsql("BEGIN TRANSACTION ;");
      foreach (QString str,Sql)
              insertsql(str);
       if(begin)
      insertsql("COMMIT;");
    }
  QString getsqlTable(QString tableName){

      return getDatastr("SELECT  sql FROM sqlite_master WHERE type='table' and name ='"+tableName+"' ;");

  }
  QList<QStringList>  getUpListData(QString query)
    {
        char *zErrMsg;
        char **result;
        int rc;
        int nrow,ncol;
        QStringList tmpList;
        QList<QStringList>  mainList;
        rc = sqlite3_get_table(_db,  query.toUtf8(),&result,&nrow,&ncol,&zErrMsg);

        if( rc == SQLITE_OK ){

             for (int r=1 ;r<=nrow;r++ ){
                       for (int c = 0;c<ncol;c++ ){
                           QString str =  QString::fromUtf8(result[r*ncol+c]);
                           str.replace("'","''");
                          tmpList <<str;
                       }
                  mainList.append(tmpList);
                  tmpList.clear();
            }
    }else
        errList <<  zErrMsg;
        sqlite3_free_table(result);
        return mainList;
    }

  QStringList backup(QStringList tables)
  {
      QStringList sql;
      sql <<"BEGIN TRANSACTION ;";

      foreach(QString table,tables){
          sql << getsqlTable(table)+";";
          QList<QStringList> result = getUpListData("Select *From "+table);
          foreach (QStringList list,result)
              sql << "INSERT INTO  "+table+" VALUES('"+list.join("','")+"');";

      }


      sql<<"COMMIT;";
      return sql;
  }

  QString  lastError(){
      if(errList.size()>0)
       return    errList.last();
      else
          return "no error";

   }
  QList<QStringList> select(QString table,QStringList filds,QString findType="=",QString findfild1="",QString text1="",QString findfild2="",QString text2="",QString findfild3="",QString text3=""){

   //   WriteToText("insert.txt","Select "+strinListToStr(filds)+" From  "+table+" ;");

       if(findfild3!="")
           return getListData("Select "+strinListToStr(filds)+" From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' AND "+findfild2+" "+findType+" '"+text2+"' AND "+findfild3+" "+findType+" '"+text3+"' ;");

       else if(findfild2!="")
           return getListData("Select "+strinListToStr(filds)+" From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' AND "+findfild2+" "+findType+" '"+text2+"' ;");

       else if(findfild1!="")
           return getListData("Select "+strinListToStr(filds)+" From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' ;");

      else
       return getListData("Select "+strinListToStr(filds)+" From  "+table+" ;");


  }
  QString insert(QString table,QStringList fild,QStringList value,bool cheackExist=false,QString cheackfild1="",QString cheacktext1="",QString cheackfild2="",QString cheacktext2="",QString cheackfild3="",QString cheacktext3=""){


      if(!cheackExist)
      return insertsql("INSERT INTO "+table+"("+strinListToStr(fild)+") VALUES("+strinListToStr(value,true)+");");
      else {

          if(!isAdded(table,cheackfild1,cheacktext1,cheackfild2,cheacktext2,cheackfild3,cheacktext3))
              return insertsql("INSERT INTO "+table+"("+strinListToStr(fild)+") VALUES("+strinListToStr(value,true)+");");
          else
              return trlang("added before");

      }


   }
  void begin()
  {
      insertsql("BEGIN TRANSACTION ;");
  }
  void commit()
  {
      insertsql("COMMIT;");
  }

  QString update(QString table,QStringList filds,QStringList value,QString findfild1,QString text1,QString findType="=",QString findfild2="",QString text2="",QString findfild3="",QString text3=""){
      //  WriteToText("update.txt","UPDATE "+table+" SET "+forupdate(filds,value)+"    Where "+findfild1+" "+findType+" '"+text1+"' ;"+insertsql("UPDATE "+table+" SET "+forupdate(filds,value)+"    Where "+findfild1+" "+findType+" '"+text1+"' ;"));

       if(findfild3!="")
           return insertsql("UPDATE "+table+" SET "+forupdate(filds,value)+"    Where "+findfild1+" "+findType+" '"+text1+"' AND "+findfild2+" "+findType+" '"+text2+"' AND "+findfild3+" "+findType+" '"+text3+"' ;");

       else if(findfild2!="")
           return insertsql("UPDATE "+table+" SET "+forupdate(filds,value)+"    Where "+findfild1+" "+findType+" '"+text1+"' AND "+findfild2+" "+findType+" '"+text2+"' ;");

       else
           return insertsql("UPDATE "+table+" SET "+forupdate(filds,value)+"    Where "+findfild1+" "+findType+" '"+text1+"' ;");

  }
  QString deleterow(QString table,QString findfild1,QString text1,QString findType="=",QString findfild2="",QString text2="",QString findfild3="",QString text3=""){
     //   WriteToText("delete.txt","DELETE  From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' ;"+insertsql("DELETE  From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' ;"));

      if(findfild3!="")
           return insertsql("DELETE  From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' AND "+findfild2+" "+findType+" '"+text2+"' AND "+findfild3+" "+findType+" '"+text3+"' ;");

       else if(findfild2!="")
           return insertsql("DELETE  From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' AND "+findfild2+" "+findType+" '"+text2+"' ;");

       else
           return insertsql("DELETE  From  "+table+" Where "+findfild1+" "+findType+" '"+text1+"' ;");


  }
  bool isAdded(QString table,QString fild1,QString text1,QString fild2="",QString text2="",QString fild3="",QString text3=""){

      QString sql;

      if(fild3!="")
          sql = "Select Count("+fild1+") From "+table+" Where "+fild1+"='"+text1+"' And "+fild2+"='"+text2+"' And "+fild3+"='"+text3+"' ;";
      else if(fild2!="")
          sql = "Select Count("+fild1+") From "+table+" Where "+fild1+"='"+text1+"' And "+fild2+"='"+text2+"'  ;";
      else
          sql = "Select Count("+fild1+") From "+table+" Where "+fild1+"='"+text1+"' ;";


    if (getDatastr(sql)=="0")
        return false;
    else
        return true;


  }
  QString strinListToStr(QStringList list,bool cution=false){

      QString str;

      if(cution){
      for(int i=0;i<list.size();i++){
          QString val = list.at(i);
          val.replace("'","''");

          if(i+1==list.size())
          str += "'"+val+"'";
          else
           str += "'"+val+"',";
      }
      }else{

          for(int i=0;i<list.size();i++){
              QString val = list.at(i);
              val.replace("'","''");

              if(i+1==list.size())
              str += val;
              else
               str +=val+",";
          }

      }
      return str;

  }
  QString forupdate(QStringList filds,QStringList value){

      QString str;
      for (int i=0;i<filds.size();i++){
          if(i+1==filds.size())
          str +=filds.at(i)+"='"+value.at(i)+"'";
          else
          str +=filds.at(i)+"='"+value.at(i)+"',";
      }

      return str;

  }
  void   Exportsqls(QStringList Sql,QString fileName)
  {
     WriteToText(fileName,"BEGIN TRANSACTION ;");
       foreach (QString str,Sql)
           WriteToText(fileName,str);
     WriteToText(fileName,"COMMIT;");
   }

  QString err;
  QStringList errList;
  sqlite3 * _db;

};

#endif // DATABASE_H
