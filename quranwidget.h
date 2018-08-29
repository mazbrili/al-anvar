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
#ifndef QURANWIDGET_H
#define QURANWIDGET_H

#include "database.h"
#include "player.h"
#include "findreplace.h"
#include "ayeexplor.h"
#include "ui_quranwidget.h"
#include "contactform.h"
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>


class JsObject : public QObject
{
    Q_OBJECT
public:
    explicit JsObject(QObject *parent = 0):
        QObject(parent)
    {
    }

public slots:
   void getValue(const QVariant& v)
   {
       emit sendValue(v);
   }
signals:
   void sendValue(QVariant);

};

namespace Ui {

    class QuranWidget;
}
class QuranWidget : public QWidget , public Ui::QuranWidget
{
    Q_OBJECT
public:
    QToolBar *toolBar;
    explicit QuranWidget(DataBase *database,QWidget *parent = 0);
    void setQuranPeroperty(QString id);
    void setTranslation();
public slots:

    void needExplanations();
    void PageChanged();
    void showJuz(QString text);
    void showHizb(QString text);
    void ChapterChanged();
    void VerseChanged();
    void goToVerseFromSearch(QString str);
    void AddToSubjects();
    void showDuplicatesWords();
    void AddToComment();
    void ShowExplanations();
    void PlayQuran();
    void similarVerses();
    void zoomIn();
    void zoomQut();
    bool finder(QString chapter,QString verse);
    bool finder(QString id);
    void addJavaScriptObject();
    void getValue(QVariant v);
    void showVerse();
    void addBookmark();


signals:
 void showRelated(QString,QString);
 void openAyeExplor(QString,int);

public:
  DataBase * db;
  DBModelSubJectQuran *treeModel;
  bool igChange;
  QHash<int,QString> versePeroperti;
  int currentRow;
  QString showType, mainTndex,trTableName, currentVerseId;
  QList<QStringList> listAudio;
  ExternalPlayer *exPl;
  InternalPlayer *inPl;
  QAction *acVerse,*acPlay,*acInter,*acSubject,*acComment,*acSimiler,*acMail,*acZoomIn,*acZoomOut,*acBookmark;
  QStringList  trName,trTable,listId;
  enum { MaxRecent = 12 };
  QAction *acSub[MaxRecent];
  JsObject* sampleQObject;

protected:
  void keyPressEvent (QKeyEvent *);


private slots:

  void on_comboBoxTr_activated(int index);
  void on_checkBoxNoShowQuran_clicked();
};

#endif // QURANWIDGET_H
