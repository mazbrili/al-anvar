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
#include <QScrollBar>
#include <QToolBar>
#include <QProgressBar>
#include <QFormLayout>
#include <QStyleFactory>
#include <QPrintDialog>
#include <QPrinter>
#include <QFontDialog>
#include <QTextDocument>
#include <QTextEdit>
#include <QStandardPaths>
#include <QDesktopWidget>
#include "setting.h"
#include "quranwidget.h"
#include "helpWindow.h"
#include "ayeexplor.h"
#include "addons.h"


namespace Ui {
    class anvar;
}
class anvar : public QMainWindow
{
    Q_OBJECT

public:
    explicit anvar(QWidget *parent = 0);
    void SetLanguage();
    void addTab(QString title,QString sql,int verse=0,QString type="chapter");
    void connectToDataBase();
    void SetTree(QString sortBy);
    QString checkLatestVersion();



    ~anvar();

private slots:

    void doSearchQ();   
    void showResultQ(const QModelIndex &index);
    void showResultRoot(const QModelIndex &index);
    void findRoot();
    void findRootInList( QString str);
    void on_tableResultQ_doubleClicked(const QModelIndex &index);
    void openAyeExplor(QString,int);
    void bookTreeEvent();



    void restartApp();
    void finder(QString searchString);
    void installAddons(QStringList fileNames);
    void BackupOfSubjectAndComment();
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
    void goToVerseFromSearch(QString);
    void on_tabWidgetQuran_tabCloseRequested(int index);
    void on_tabWidgetQuran_currentChanged(int);
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

    void on_tableResultRoot_doubleClicked(const QModelIndex &index);

    void on_actionMoshafView_triggered();


private:
    Ui::anvar *ui;
    DataBase *db;
    DbTreeViewSubJectQuran *dtree_;
    DBModelSubJectQuran *treeModel;

    tools tool;
    int nextId;
    QStringList langvalue;
    QuranWidget *quranWidget;
    Setting *stp;
    QListWidget *listWidgetPrint;
    QStringList tabListTitle;
    QPointer<HelpWindow> helpWindow; 
    Addons *addonsForm;
    QuranModel *quranModel;
    AyeExplor *ayeExp;
    QList<QStringList> QuranRoot;
    DBSearch serPat;

    bool fristSearchQ,fristSearchRoot;


protected:
    virtual void closeEvent(QCloseEvent *e);

  signals:
       void findVerse(QString str);



};

#endif // ANVAR_H
