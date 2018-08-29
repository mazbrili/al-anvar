/***************************************************************************
 *  This file is part of Al-Anvar,a Quran study tool for simply browsing   *
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

#ifndef ANVAR_H
#define ANVAR_H

#include <QMainWindow>
#include "setting.h"
#include "quranwidget.h"
#include "ayeexplor.h"
#include "addons.h"
#include "intro.h"
#include "search.h"


namespace Ui {
    class anvar;
}
class anvar : public QMainWindow
{
    Q_OBJECT

public:
    explicit anvar(QWidget *parent = 0);
    void SetLanguage();
    void addTab(QString title,QString mainId,QString type,int verse=0);
    void connectToDataBase();
    void SetTree(QString sortBy);
    QString checkLatestVersion();

    ~anvar();

private slots:
    void openAyeExplor(QString,int);
    void restartApp();
    void installAddons(QStringList fileNames,bool report);
    void BackupOfSubjectAndComment();
    bool BackupOfSubjectAndComment(QStringList table,QString BackupName);
    void installAddonsFormFile();
    void TreeEventHizb();
    void TreeEventJuz();
    void TreeEventQuran();
    void SubjectEvent();
    void showOptions();
    void AcAbout();
    void AcHelp();
    void AcSave();
    void AcPrint();
    void AcPdf();
    void on_tabWidgetQuran_tabCloseRequested(int index);
    void goToQuranTree(int value);
    void showHideLayoutChapter();
    void on_dockWidget_visibilityChanged(bool visible);
    void showAddonsePage();
    void on_actionHomePage_triggered();
    void on_actionNewsLetter_triggered();
    void on_comboBoxSortBy_activated(int index);
    void on_actionUpdates_triggered();
    void on_actionSearch_triggered();
    void showRelated(QString,QString);
    void openLastPage();
    void OpenReseachTools();
    void gotoTopics();
    void bookmarkEvent(QListWidgetItem*);
    void quranSearch();
    void rootSearch(QModelIndex);
    void showQuranFromSearch(QString);
    void showQuranFromSearchRoot(QString);
    void filterRoot(QString);
    void getValue(QVariant);
    void getValueRoot(QVariant);

    void addJavaScriptObject();
    void addJavaScriptObjectRoot();
    void on_comboBoxLimit_activated();
    void on_comboBoxLimitRoot_activated();

private:
    Ui::anvar *ui;
    DataBase *db;
    DbTreeViewSubJectQuran *dtree_;
    DBModelSubJectQuran *treeModel;
    tools tool;
    int nextId,limitSearch,allrecordSearch,allrecordSearchRoot;
    QStringList trTableSearch,trNameSearch,chapterList,tabListTitle,searchHistory;
    QuranWidget *quranWidget;
    Setting *stp;
    Addons *addonsForm;
    QuranModel *quranModel;
    AyeExplor *ayeExp;
    QList<QStringList> QuranRoot;
    DBSearch serPat;
    Intro intro;
    bool firstAddons,firstAyeExp;
    QSortFilterProxyModel *rootSearchModelSF;
    QString translationTable,searchText, fontType,startQuery,sampleText,rootstartQuery;
    JsObject * sampleQObject,*sampleQObjectRoot;


protected:
    virtual void closeEvent(QCloseEvent *e);

  signals:
       void findVerse(QString str);



};

#endif // ANVAR_H
