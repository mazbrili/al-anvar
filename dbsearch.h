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

#ifndef DBSEARCH_H
#define DBSEARCH_H


#include "database.h"


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QStringList keywordPatterns,QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
    {
        HighlightingRule rule;
        keywordFormat.setBackground(Qt::yellow);
        keywordFormat.setFontWeight(QFont::Bold);
           foreach (const QString &pattern, keywordPatterns) {
               rule.pattern = QRegExp(pattern);
               rule.format = keywordFormat;
               highlightingRules.append(rule);

           }

    }

protected:
    void highlightBlock(const QString &text)
    {
        foreach (const HighlightingRule &rule, highlightingRules) {
                QRegExp expression(rule.pattern);
                int index = expression.indexIn(text);
                while (index >= 0) {
                    int length = expression.matchedLength();
                    setFormat(index, length, rule.format);
                    index = expression.indexIn(text, index + length);
                }
            }

       setCurrentBlockState(0);

    }

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;

};
class DBSearch
{

public:
    tools tool;
     DBSearch(bool agErab=false,bool agHamza=false,bool boundary=true){

        setOptions(agErab,agHamza,boundary);

    }
    void setOptions(bool agErab=false,bool agHamza=false,bool boundary=true)
    {
        agErab_=agErab;
        agHamza_=agHamza;
        boundary_=boundary;

    }

    QString roolSearch(QString txt)
    {
        QString pattern;
        for(int i =0;i<txt.count();i++)
            pattern = pattern+"\\w*"+txt[i];

        return pattern;
    }
    QString clearErab(QString str)
    {

        str.replace(QString::fromUtf8("ْ"),"");
        str.replace(QString::fromUtf8("ٌ"),"");
        str.replace(QString::fromUtf8("ٍ"),"");
        str.replace(QString::fromUtf8("ً"),"");
        str.replace(QString::fromUtf8("ُ"),"");
        str.replace(QString::fromUtf8("ِ"),"");
        str.replace(QString::fromUtf8("َ"),"");
        str.replace(QString::fromUtf8("ّ"),"");
        str.replace(QString::fromUtf8("‌َ"),"");
        str.replace(QString::fromUtf8("‌ٰ"),"");
        str.replace(QString::fromUtf8("‌ٓ"),"");

        return str;

    }
    QString alifHamza(QString txt){
       // SELECT replace( replace( replace( replace( replace( replace( replace( replace( replace( replace(replace(arabic,
        //'ْ',''),'ٌ',''),'ٍ',''),'ً',''),'ُ',''),'ِ',''),'َ',''),'ّ',''),'ٓ',''),'ٰ',''),'ْ','')From Quran;
//SELECT replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( replace( arabic, 'ْ', '' ) , 'ۖ ', '' ) , ' ۚ', '' ) , ' ۗ', '' ) , 'ٌ', '' ) , 'ٍ', '' ) , 'ً', '' ) , 'ُ', '' ) , 'ِ', '' ) , 'َ', '' ) , 'ّ', '' ) , 'ٓ', '' ) , 'ٰ', '' ) , 'ْ', '' ) , 'ؤ', 'و' ) , 'ئ', '' ) , 'ی', 'ي' ) , 'إ', 'ا' ) , 'أ', 'ا' ) , 'آ', 'ا' ) , 'ة', 'ه' ) , 'ک', 'ك' )  FROM Quran


        QString addtxt;
        txt = clearErab(txt);
        txt.replace(QString::fromUtf8("«"),"^");
        txt.replace(QString::fromUtf8("»"),"$");

        txt.replace(QString::fromUtf8("ی"),QString::fromUtf8("ي"));
        txt.replace(QString::fromUtf8("ک"),QString::fromUtf8("ك"));
       if(!agErab_){
           QStringList arabics;
           arabics << QString::fromUtf8("ض") << QString::fromUtf8("ص") << QString::fromUtf8("ث") << QString::fromUtf8("ق") << QString::fromUtf8("ف")
                   << QString::fromUtf8("غ") << QString::fromUtf8("ع") << QString::fromUtf8("ه") << QString::fromUtf8("ة") << QString::fromUtf8("خ")
                   << QString::fromUtf8("ح") << QString::fromUtf8("ج") << QString::fromUtf8("چ") << QString::fromUtf8("ش") << QString::fromUtf8("س")
                   << QString::fromUtf8("ی") << QString::fromUtf8("ي") << QString::fromUtf8("ب") << QString::fromUtf8("ل") << QString::fromUtf8("ا")
                   << QString::fromUtf8("آ") << QString::fromUtf8("أ") << QString::fromUtf8("إ") << QString::fromUtf8("ئ") << QString::fromUtf8("ت")
                   << QString::fromUtf8("ن") << QString::fromUtf8("م") << QString::fromUtf8("ک") << QString::fromUtf8("ك") << QString::fromUtf8("گ")
                   << QString::fromUtf8("ظ") << QString::fromUtf8("ط") << QString::fromUtf8("ز") << QString::fromUtf8("ر") << QString::fromUtf8("ذ")
                   << QString::fromUtf8("د") << QString::fromUtf8("پ") << QString::fromUtf8("و") << QString::fromUtf8("ؤ") << QString::fromUtf8("ژ");

           if(txt.contains("#")){
               txt.replace("#","");
               addtxt=roolSearch(txt);
           }else{
          for(int i =0;i<txt.count();i++)
          {
                QString set=txt[i]+"";
              if(arabics.contains(set))
            addtxt = addtxt+txt[i]+QString::fromUtf8("(ّ|َ|ِ|ُ|ً|ٍ|ٌ|ْ|ٰ)")+"*";
              else
            addtxt = addtxt+txt[i];
          }
        }

       }else
            addtxt =txt;

      if(!agHamza_){

            addtxt.replace(QString::fromUtf8("و"),QString::fromUtf8("(و|ؤ)"));
            addtxt.replace(QString::fromUtf8("ا"),QString::fromUtf8("(آ|أ|إ|ا)"));
            addtxt.replace(QString::fromUtf8("ك"),QString::fromUtf8("(ك|ک)"));
            addtxt.replace(QString::fromUtf8("ي"),QString::fromUtf8("(ى|ي|ی)"));
            addtxt.replace(QString::fromUtf8("ه"),QString::fromUtf8("(ه|ة)"));
       }else{
           addtxt.replace(QString::fromUtf8("ك"),QString::fromUtf8("(ك|ک)"));
           addtxt.replace(QString::fromUtf8("ي"),QString::fromUtf8("(ى|ي|ی)"));

       }

     return addtxt;

       }


    QString getSearchPattern(QString unWanted,QString getOr,QString andall,QString andafter){

        if(andall.isEmpty() or andafter.isEmpty())
        return unWanted+getOr+andall+andafter;

        return unWanted+getOr+andall+".+"+andafter;
    }
    QString getAnd(const QString &arg1)
    {
        QString str = arg1.trimmed();
        str.replace("+",",");
        str.replace("/",",");
        str.replace(";",",");
        QStringList list = str.split(",",QString::SkipEmptyParts);

        QStringList textList;
        foreach(QString rep,list){
            if(boundary_)
            rep ="\\b"+rep.trimmed()+"\\b";
            textList.append(alifHamza(rep.trimmed()));
              highlightPattern.append(alifHamza(rep.trimmed()));
             }
        QString ret;
        if(textList.size()==1)
           ret= textList.at(0);


        if(textList.size()==2)
           ret= QString("^(?=.*%1)(?=.*%2)").arg(textList.at(0)).arg(textList.at(1));
        if(textList.size()==3)
            ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2));
        if(textList.size()==4)
             ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3));
         if(textList.size()==5)
             ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4));
         if(textList.size()==6)
             ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)(?=.*%6)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4)).arg(textList.at(5));
         if(textList.size()==7)
             ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)(?=.*%6)(?=.*%7)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4)).arg(textList.at(5)).arg(textList.at(6));
         if(textList.size()==8)
             ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)(?=.*%6)(?=.*%7)(?=.*%8)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4)).arg(textList.at(5)).arg(textList.at(6)).arg(textList.at(7));


        return ret;
    }
    QString getUnWanted(const QString &arg1)
    {
        QString str = arg1.trimmed();
        str.replace("+",",");
        str.replace("/",",");
        str.replace(";",",");
        str.replace(".",",");

        QString retStr;
        QStringList list = str.split(",",QString::SkipEmptyParts);
        QStringList unWanted;
        foreach(QString rep,list){
          if(boundary_)
            rep ="\\b"+rep.trimmed()+"\\b";
            unWanted.append(alifHamza(rep.trimmed()));
        }
        if(unWanted.size()>0)
           retStr = QString("^((?!%1).)*$").arg(unWanted.join("|"));
               //  "^((?!"++").)*";


    return retStr;

    }
    QString getAndAfter(const QString &arg1)
    {
        QString str = arg1.trimmed();
        str.replace("+",",");
        str.replace("/",",");
        str.replace(";",",");

        QString retStr;
        QStringList list = str.split(",",QString::SkipEmptyParts);
        QStringList listb;
        foreach(QString rep,list){
         if(boundary_)
            rep ="\\b"+rep.trimmed()+"\\b";
            listb.append(alifHamza(rep.trimmed()));
            highlightPattern.append(alifHamza(rep.trimmed()));

         }
        if(listb.size()==1)
            retStr+=listb.at(0);
         else
            retStr+=listb.join(".+");


        return retStr;

    }
    QString getOr(const QString &arg1)
    {
        QString str = arg1.trimmed();
        str.replace("+","|");
        str.replace("/","|");
        str.replace(";","|");
        QString retStr;
        QStringList list = str.split("|",QString::SkipEmptyParts);
        QStringList listb;
        foreach(QString rep,list){
         if(boundary_)
            rep ="\\b"+rep.trimmed()+"\\b";
            listb.append(alifHamza(rep.trimmed()));
            highlightPattern.append(alifHamza(rep.trimmed()));

         }
        if(listb.size()==1)
            retStr+=listb.at(0);
         else
            retStr+=listb.join("|");


        return retStr;

    }


    bool agErab_,agHamza_,boundary_;
    DataBase *db;
    QStringList highlightPattern;

signals:

public slots:

};

#endif // DBSEARCH_H
