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
#include <BackendCapabilities>
#include <MediaSource>
#include <MediaObject>
#include <AudioOutput>
#include <VolumeSlider>
#include <VideoPlayer>
#include <VideoWidget>
#include <SeekSlider>
#include <SeekSlider>
#include <VolumeSlider>
#include "./sendermail/SmtpMime"
class tools : public QObject
{
    Q_OBJECT
public:
    QString  pathUser,pathAudio,pathData,defultPath;
    QHash<QString, QString> langValue;
    QStringList highlightPattern;

    QString getDirection(QString txt);
    QString getHtmlTag_(QString sampleTxtToGetDirection,bool sql=true,QString feild="Arabic",QString font="noorehira",int size=18,QString more="" /*background-image: url(qrc:/data/imapes/bookmark.png); */);
    QString getHtmlTagFeild(QString sampleTxtToGetDirection,QString feild="Arabic",QString type = "QuranFont",QString more="");
    QString getHtmlTagString(QString str,QString type,QString more="");
    QStringList getHtmlTagStringList(QStringList list,QString type,QString more="");
    QString getHtmlTagStringSampleTxt(QString sampleTxtToGetDirection,QString str,QString type,QString more="" );
    QStringList getHtmlTagStringListSampleTxt(QString sampleTxtToGetDirection,QStringList list,QString type,QString more="" /*background-image: url(qrc:/data/imapes/bookmark.png); */);

    QString setTextStyle(QString trTable,QString feild="Arabic",QString type="arabictext",QString moreField="",QString more="");
    QString clearErab(QString str);
    QString roolSearch(QString txt);
    QString alifHamza(QString txt,bool agErab,bool agHamza);
    QString getAnd(const QString &arg1,bool boundary,bool agErab,bool agHamza);
    QString getUnWanted(QString arg1,bool boundary,bool agErab,bool agHamza);
    QString getAndAfter( QString arg1,bool boundary,bool agErab,bool agHamza);
    QString getSearchPattern(QString unWanted,QString andall,QString andafter,bool boundary,bool agErab,bool agHamza);
    QString highlighSearch(QString value,int words=10,QString tag="<b><font color='red'>",QString tagEnd = "</font></b>");

    explicit tools(QObject *parent = 0);
    void WriteSettings(QString key , QVariant Value);
    QVariant ReadSettings(QString key , QVariant DefultValue);
    QStringList setRecent(QString name,const QString &text,int MaxRecent);
    QString arabicNumToEnglish(QString num);
    QStringList ReadFromTextFile(QString name,QString character="UTF-8");
     void WriteToText(QString name , QString str);
     void WriteToText(QString name , QStringList list);
     void Zip(QString filename , QString zipfilename);
     void UnZip(QString zipfilename , QString filename);
     void ZipOpen(QString OpenedData="");
     void UnZipList();
     void ZipList();
     QString Fitterdir (QString dir,int Type = 0);
     int randInt(int low, int high);
     QString DataTime(QString format="yyMMddHHmmss");
     QStringList GetLang(QString dir);
     void GetLangValue();
     QString trlang(QString key);
     QString GetNodeIdCode();
     QString GetNodeIdCodeCheachNotSame(QStringList list);
     QString GetNodeIdCodeCheachNotSame(QString list);
     static QString toJalali(QString gregorianDate);
     bool sendMail(QString user,QString pass,QString subject ,QString str);
     QString currentJalaliDate();
     static QString toGregorian(QString jalaliDate);
     static QString getJalaliDayName(QString jalaliDate);
     static QString toJalaliDateTime(QString gregorianDate);
private:
     static void gregorian_to_jalali(/* output */ int *j_y, int *j_m, int *j_d, int  g_y, int  g_m, int  g_d);
     static void  jalali_to_gregorian(/* output */ int *g_y, int *g_m, int *g_d, int  j_y, int  j_m, int  j_d);
};




#endif // TOOLS_H
