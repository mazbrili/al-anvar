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

#ifndef TOOLS_H
#define TOOLS_H

#include "sqlite3.h"
#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAbstractTextDocumentLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <phonon4qt5/phonon/BackendCapabilities>
#include <phonon4qt5/phonon/MediaSource>
#include <phonon4qt5/phonon/MediaObject>
#include <phonon4qt5/phonon/AudioOutput>
#include <phonon4qt5/phonon/VolumeSlider>
#include <phonon4qt5/phonon/VideoPlayer>
#include <phonon4qt5/phonon/VideoWidget>
#include <phonon4qt5/phonon/SeekSlider>
#include <phonon4qt5/phonon/SeekSlider>
#include <phonon4qt5/phonon/VolumeSlider>
#include "./sendermail/SmtpMime"
#include <QStandardPaths>


class tools : public QObject
{
    Q_OBJECT
public:
    QString  pathUser,pathAudio,pathData,defultPath;
    QHash<QString, QString> langValue;


    explicit tools(QObject *parent = 0)
        :QObject(parent)
    {

        bool cdMode=false;

      #ifdef  Q_OS_LINUX
        WriteSettings("ExternalPlayer",ReadSettings("ExternalPlayer","mplayer").toString());
      #endif
        defultPath  = QApplication::applicationDirPath();
      #ifdef  Q_OS_LINUX
        defultPath =QDir::homePath()+"/.al-anvar";
      #endif


        if(cdMode)
        {
            defultPath=QDir::homePath()+"/al-anvar";
      #ifdef  Q_OS_LINUX
            defultPath =QDir::homePath()+"/.al-anvar";
      #endif

        }
        pathUser= ReadSettings("UserDir",defultPath).toString();
        pathData= ReadSettings("DataBaseDir",defultPath).toString();

        QDir a_diruser = pathUser;
           if (! a_diruser.exists()){
              if(a_diruser.mkdir(pathUser))
                  WriteSettings("UserDir",pathUser);
           }else
               WriteSettings("UserDir",pathUser);


        QString pathAudio_;

            if(cdMode){
                   pathAudio_=QApplication::applicationDirPath()+"/Audio";
                   QFile databaseFileName(pathData+"/Data.db");
                     if(!databaseFileName.exists()){
                         QMessageBox::information(0,QString::fromUtf8("ایجاد بانک اطلاعات"),
                                                  QString::fromUtf8("برای انجام تغییرات در بانک اطلاعات مانند یادداشت گذاری وموضوع بندی شخصی لازم است بانک اطلاعات از لوح به رایانه شما منتقل شود چند لحظه شکیبا باشید تا این عملیات انجام گیرد . این کار تنها در اولین مرتبه ای که نرم افزار را اجرا میکنید  انجام خواهد گرفت"));
                           QFile::copy("Data.db",pathData+"/Data.db");
                     }
            }else
                   pathAudio_=ReadSettings("UserDir",pathUser).toString()+"/Audio";

            pathAudio=ReadSettings("AudioDir",pathAudio_).toString();


            QDir a_dir = pathAudio;
             WriteSettings("AudioDir",pathAudio);
         if (! a_dir.exists())
            a_dir.mkdir(pathAudio);

        bool internalavailable=false;
        QStringList availableMimeTypes= Phonon::BackendCapabilities::availableMimeTypes();
         if(availableMimeTypes.size()>0)
            internalavailable=true;
        WriteSettings("internalavailable",internalavailable);

       GetLangValue();

    }

    void makeBorder(QWidget *widgetSet,QHBoxLayout *hBox,QString nameDir=":/images")
    {
        QGridLayout *gridLayout =  new QGridLayout();
        gridLayout->setSpacing(0);

        makeBorder_(gridLayout,widgetSet,nameDir);
        hBox->addLayout(gridLayout);
    }
    void makeBorder(QWidget *widgetSet,QVBoxLayout *vBox,QString nameDir=":/images")
    {
        QGridLayout *gridLayout =  new QGridLayout();
        gridLayout->setSpacing(0);

        makeBorder_(gridLayout,widgetSet,nameDir);
        vBox->addLayout(gridLayout);
    }
    void makeBorder_(QGridLayout *gridLayout,QWidget *widgetSet,QString nameDir=":/images")
    {

        QString imgPath=nameDir;
        QPixmap pix(imgPath+"/top_right.png");
        QSize size(pix.width(),pix.height());
        QWidget *frame_top,*frame_topRight,*frame_left, *frame_right,*frame_bot,*frame_botRight,*frame_topLeft,*frame_botLeft;
        frame_top = new QWidget();
        frame_top->setStyleSheet("background-image: url("+imgPath+"/top.png);");
        gridLayout->addWidget(frame_top, 0, 1, 1, 1);
        frame_topRight = new QWidget();
        frame_topRight->setMinimumSize(size);
        frame_topRight->setStyleSheet("background-image: url("+imgPath+"/top_right.png);");
        gridLayout->addWidget(frame_topRight, 0, 0, 1, 1);
        frame_left = new QWidget();
        frame_left->setStyleSheet("background-image: url("+imgPath+"/left.png);");
        gridLayout->addWidget(frame_left, 1, 2, 1, 1);
        frame_right = new QWidget();
        frame_right->setStyleSheet("background-image: url("+imgPath+"/right.png);");
        gridLayout->addWidget(frame_right, 1, 0, 1, 1);
        frame_bot = new QWidget();
        frame_bot->setStyleSheet("background-image: url("+imgPath+"/bot.png);");
        gridLayout->addWidget(frame_bot, 2, 1, 1, 1);
        frame_botRight = new QWidget();
        frame_botRight->setMinimumSize(size);
        frame_botRight->setStyleSheet("background-image: url("+imgPath+"/bot_right.png);");
        gridLayout->addWidget(frame_botRight, 2, 0, 1, 1);
        frame_topLeft = new QWidget();
        frame_topLeft->setMinimumSize(size);
        frame_topLeft->setStyleSheet("background-image: url("+imgPath+"/top_left.png);");
        gridLayout->addWidget(frame_topLeft, 0, 2, 1, 1);
        frame_botLeft = new QWidget();
        frame_botLeft->setMinimumSize(size);
        frame_botLeft->setStyleSheet("background-image: url("+imgPath+"/bot_left.png);");
        if(trlang("Align")!="Right"){
            frame_botLeft->setStyleSheet("background-image: url("+imgPath+"/bot_right.png);");
            frame_topLeft->setStyleSheet("background-image: url("+imgPath+"/top_right.png);");
            frame_botRight->setStyleSheet("background-image: url("+imgPath+"/bot_left.png);");
            frame_topRight->setStyleSheet("background-image: url("+imgPath+"/top_left.png);");
            frame_left->setStyleSheet("background-image: url("+imgPath+"/right.png);");
            frame_right->setStyleSheet("background-image: url("+imgPath+"/left.png);");

         }
        gridLayout->addWidget(frame_botLeft, 2, 2, 1, 1);
        gridLayout->addWidget(widgetSet, 1, 1, 1, 1);

    }

    void WriteSettings(QString key , QVariant Value)
    {
        QSettings settings(defultPath+"/al-anvar.ini",QSettings::IniFormat);
        settings.setValue(key,Value);
    }
    QVariant ReadSettings(QString key , QVariant DefultValue)
    {
       QVariant setting;
        QSettings settings(defultPath+"/al-anvar.ini",QSettings::IniFormat);
        setting = settings.value(key,DefultValue);
        return setting;
    }

      QStringList setRecent(QString name,const QString &text,int MaxRecent){

         QStringList files = ReadSettings(name,"").toStringList();
         files.removeAll(text.toUtf8());
         files.prepend(text.toUtf8());
         while (files.size() > MaxRecent)
             files.removeLast();
     WriteSettings(name, files);
     return files;

     }
      QString arabicNumToEnglish(QString num){

         num.replace("۱","1");
         num.replace("۲","2");
         num.replace("۳","3");
         num.replace("۴","4");
         num.replace("۵","5");
         num.replace("۶","6");
         num.replace("۷","7");
         num.replace("۸","8");
         num.replace("۹","9");
         num.replace("۰","0");
         return num;
     }


     QStringList ReadFromTextFile(QString name,QString character="UTF-8")
     {
        QFile file(name);
            QStringList list;
             if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
               QTextStream in(&file);
        in.setCodec(character.toLatin1());
        while(!in.atEnd())
         {
            QString line =in.readLine();
            list <<line;
         }
       }
             file.close();
      return list;
     }
     void WriteToText(QString name , QString str)
     {
         QFile file(name);

         if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
              {
                    QTextStream out(&file);
                   out.setCodec("UTF-8");
                    out << str << endl;
                }
                    file.close();
     }
     void WriteToText(QString name , QStringList list)
     {
         QFile file(name);

         if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
              {
                    QTextStream out(&file);
                   out.setCodec("UTF-8");
                   foreach (QString str,list)
                       out << str << endl;
                }
                    file.close();
     }
     void Zip(QString filename , QString zipfilename)
     {

         QFile infile(filename);
         QFile outfile(zipfilename);
         infile.open(QIODevice::ReadOnly);
         outfile.open(QIODevice::WriteOnly);
         QByteArray uncompressedData = infile.readAll();
         QByteArray compressedData = qCompress(uncompressedData,9);
         outfile.write(compressedData);
         infile.close();
         outfile.close();

     }
     void UnZip(QString zipfilename , QString filename)
     {
         QFile infile(zipfilename);
         QFile outfile(filename);
         infile.open(QIODevice::ReadOnly);
         outfile.open(QIODevice::WriteOnly);
         QByteArray uncompressedData = infile.readAll();
         QByteArray compressedData = qUncompress(uncompressedData);
         outfile.write(compressedData);
         infile.close();
         outfile.close();

     }
     void ZipOpen(QString OpenedData=""){
         if (OpenedData=="")
          //OpenedData =  QFileDialog::getOpenFileName(0,"Open",QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
	     OpenedData =  QFileDialog::getOpenFileName(0,"Open",QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
	
         if (OpenedData.isNull()) return;
         Zip(OpenedData,pathData+"/temp.zip");
         QFile file(OpenedData);
         QString BaseDir=Fitterdir(OpenedData);
         QFileInfo fileInfo(file.fileName());
         QString filename =BaseDir+fileInfo.fileName()+".nvr" ;
          Zip(OpenedData,filename);
          file.close();

     }
     void UnZipList(){

          QStringList filelist =  QFileDialog::getOpenFileNames(0,"Open",QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

         if (filelist.isEmpty()) return;
         foreach(QString OpenedData,filelist){
         QFile file(OpenedData);
         QString BaseDir=Fitterdir(OpenedData);
         QFileInfo fileInfo(file.fileName());
         QString filename =BaseDir+fileInfo.baseName()+".db" ;
          UnZip(OpenedData,filename);
         }
     }
     void ZipList(){

         // QStringList filelist =  QFileDialog::getOpenFileNames(0,"Open",QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
         QStringList filelist =  QFileDialog::getOpenFileNames(0,"Open",QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
         if (filelist.isEmpty()) return;
         foreach(QString OpenedData,filelist){
         QFile file(OpenedData);
         QString BaseDir=Fitterdir(OpenedData);
         QFileInfo fileInfo(file.fileName());
         QString filename =BaseDir+fileInfo.baseName()+".nvr" ;
          Zip(OpenedData,filename);
         }
     }
     QString Fitterdir (QString dir,int Type = 0){
         QFileInfo info(dir);
         if (Type==1)
             return info.fileName();
         else if (Type==2)
             return info.completeSuffix();
         else
             return dir.replace(info.fileName(),"");
     }
     int randInt(int low, int high)
         {
                return qrand() % ((high + 1) - low) + low;

         }
     QString DataTime(QString format="yyMMddHHmmss"){
     QString datetime = QDateTime::currentDateTime().toString(format);
     return datetime;
     }
     QStringList GetLang(QString dir)
     {
         QDir lang(dir);
         QFileInfoList list = lang.entryInfoList(QDir::Files);
         QStringList langs;
          foreach (QFileInfo info ,list){
              if(info.completeSuffix()=="xml" or info.completeSuffix()=="XML"){
                  QFile filelang(":/language/"+info.fileName());
                  filelang.open(QIODevice::ReadOnly);
                  QXmlStreamReader xmlLang;
                  xmlLang.setDevice(&filelang);
                  bool end=false;
                  while (!xmlLang.atEnd() && !end)
                  {
                    xmlLang.readNext();
                      if (xmlLang.name() == "locale"){
                        if(xmlLang.attributes().value("VERSION").toString()=="AL-ANVAR-0.2.5")
                          langs << xmlLang.attributes().value("NAME").toString()+";"+info.baseName();
                        else
                            QMessageBox::warning(0,"Language","Invalid language file.");
                          end=true;
                     }
                  }
               }
            }
        return langs;
     }
     void GetLangValue()
     {
         QString fileName = ":/language/"+ReadSettings("lang","English").toString()+".xml";
         langValue.clear();
         QFile filelang(fileName);
         filelang.open(QIODevice::ReadOnly);
         QXmlStreamReader xmlLang;
         xmlLang.setDevice(&filelang);
         while (!xmlLang.atEnd())
         {
                xmlLang.readNext();

             if (xmlLang.name() == "text")
                 langValue.insert(xmlLang.attributes().value("id").toString(),xmlLang.attributes().value("value").toString());
         }
         filelang.close();
     }
     QString trlang(QString key)
     {


         if(langValue.value(key)=="")
             return key;
         else
             return langValue.value(key);
     }

     QString GetNodeIdCode()
     {
         QString datetime = QDateTime::currentDateTime().toString("MMddHHmmss");
         QTime time = QTime::currentTime();
         qsrand((uint)time.msec());
        return  datetime+QString::number(randInt(10,99));
     }
     QString GetNodeIdCodeCheachNotSame(QStringList list)
     {
         QString idCode = GetNodeIdCode();
         while (list.contains(idCode))
              idCode = GetNodeIdCode();
         return idCode;
     }
     QString GetNodeIdCodeCheachNotSame(QString list)
     {
         QString idCode = GetNodeIdCode();
         while (list==idCode)
              idCode = GetNodeIdCode();
         return idCode;
     }
     static QString toJalali(QString gregorianDate)
     {
         int y, m, d;
         QString currentDate;
         gregorian_to_jalali(&y,&m,&d,gregorianDate.left(4).toInt(),gregorianDate.mid(5,2).toInt(),gregorianDate.right(2).toInt());
         currentDate=QString("%1-%2-%3").arg(y).arg(m).arg(d);
         if(m<10) currentDate.insert(5,"0");
         if(d<10) currentDate.insert(8,"0");
         return currentDate;
     }
     bool sendMail(QString user,QString pass,QString subject ,QString str)
     {
         SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
           smtp.setUser(user);
           smtp.setPassword(pass);
           MimeMessage message;
           message.setSender(new EmailAddress(pass, ""));
           message.addRecipient(new EmailAddress("etrate14@gmail.com", ""));
           message.setSubject(subject);
           MimeText text;
           text.setText(str);
           message.addPart(&text);
           if(!smtp.connectToHost())
           {
               QMessageBox::warning(0,trlang("SendMail"),trlang("Connection Failed."));
               return false;
           }
           else if(!smtp.login())
           {
               QMessageBox::warning(0,trlang("SendMail"),trlang("Authentification Failed."));
               return false;

           }else if(!smtp.sendMail(message))
           {
               QMessageBox::warning(0,trlang("SendMail"),trlang("Mail sending failed."));
               return false;
           }else
           smtp.quit();
           return true;

     }



     QString currentJalaliDate(){
      return  toJalali(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
     }
     static QString toGregorian(QString jalaliDate)
     {
         int y, m, d;
         QString currentDate;
         jalali_to_gregorian(&y,&m,&d,jalaliDate.left(4).toInt(),jalaliDate.mid(5,2).toInt(),jalaliDate.right(2).toInt());
         currentDate=QString("%1-%2-%3").arg(y).arg(m).arg(d);
         if(m<10) currentDate.insert(5,"0");
         if(d<10) currentDate.insert(8,"0");
         return currentDate;
     }
     static QString getJalaliDayName(QString jalaliDate){

          QString roz = QDateTime::fromString(toGregorian(jalaliDate),"yyyy-MM-dd").toString("ddd");

          roz.replace("Sun",QString::fromUtf8("یک شنبه"));
          roz.replace("Mon",QString::fromUtf8("دو شنبه"));
          roz.replace("Tue",QString::fromUtf8("سه شنبه"));
          roz.replace("Wed",QString::fromUtf8("چهار شنبه"));
          roz.replace("Thu",QString::fromUtf8("پنج شنبه"));
          roz.replace("Fri",QString::fromUtf8("جمعه"));
          roz.replace("Sat",QString::fromUtf8("شنبه"));

          return roz;

     }
     static QString toJalaliDateTime(QString gregorianDate)
     {
         int y, m, d;
         QString currentDate;
         gregorian_to_jalali(&y,&m,&d,gregorianDate.left(4).toInt(),gregorianDate.mid(5,2).toInt(),gregorianDate.right(2).toInt());
         currentDate=QString("%1-%2-%3").arg(y).arg(m).arg(d);
         if(m<10) currentDate.insert(5,"0");
         if(d<10) currentDate.insert(8,"0");
         return currentDate.append(" , ").append(QTime::currentTime().toString("hh:mm"));
     }
 private:
     static void gregorian_to_jalali(/* output */ int *j_y, int *j_m, int *j_d, int  g_y, int  g_m, int  g_d){

             int g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
          int j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

             int gy, gm, gd;
             int jy, jm, jd;
             long g_day_no, j_day_no;
             int j_np;

             int i;

             gy = g_y-1600;
             gm = g_m-1;
             gd = g_d-1;

             g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
             for (i=0;i<gm;++i)
                g_day_no += g_days_in_month[i];
             if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
                /* leap and after Feb */
                ++g_day_no;
             g_day_no += gd;

             j_day_no = g_day_no-79;

             j_np = j_day_no / 12053;
             j_day_no %= 12053;

             jy = 979+33*j_np+4*(j_day_no/1461);
             j_day_no %= 1461;

             if (j_day_no >= 366) {
                jy += (j_day_no-1)/365;
                j_day_no = (j_day_no-1)%365;
             }

             for (i = 0; (i < 11) && (j_day_no >= j_days_in_month[i]); ++i) {
                j_day_no -= j_days_in_month[i];
             }
             jm = i+1;
             jd = j_day_no+1;
             *j_y = jy;
             *j_m = jm;
             *j_d = jd;
      }
     static void  jalali_to_gregorian(/* output */ int *g_y, int *g_m, int *g_d, int  j_y, int  j_m, int  j_d){
          int g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
          int j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

          int gy, gm, gd;
            int jy, jm, jd;
            long g_day_no, j_day_no;
            int leap;

            int i;

            jy = j_y-979;
            jm = j_m-1;
            jd = j_d-1;

            j_day_no = 365*jy + (jy/33)*8 + (jy%33+3)/4;
            for (i=0; i < jm; ++i)
               j_day_no += j_days_in_month[i];

            j_day_no += jd;

            g_day_no = j_day_no+79;

            gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
            g_day_no = g_day_no % 146097;

            leap = 1;
            if (g_day_no >= 36525) /* 36525 = 365*100 + 100/4 */
            {
               g_day_no--;
               gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
               g_day_no = g_day_no % 36524;

               if (g_day_no >= 365)
                  g_day_no++;
               else
                  leap = 0;
            }

            gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
            g_day_no %= 1461;

            if (g_day_no >= 366) {
               leap = 0;

               g_day_no--;
               gy += g_day_no/365;
               g_day_no = g_day_no % 365;
            }

            for (i = 0; g_day_no >= g_days_in_month[i] + (i == 1 && leap); i++)
               g_day_no -= g_days_in_month[i] + (i == 1 && leap);
            gm = i+1;
            gd = g_day_no+1;

            *g_y = gy;
            *g_m = gm;
            *g_d = gd;
      }
};




#endif // TOOLS_H
