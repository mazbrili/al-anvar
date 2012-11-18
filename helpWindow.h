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

#ifndef HELPWINDOW_H
#define HELPWINDOW_H


#include <QMainWindow>
#include "findreplace.h"


class HelpWindow : public QMainWindow
{
    Q_OBJECT
public:
    HelpWindow( const QString home, QWidget* parent = 0, Qt::WFlags flags = 0) : QMainWindow( parent, flags )
    {

        setWindowTitle(tool.trlang("Help"));

    setWindowIcon(QIcon(":/images/app.png"));
     QStringList list= tool.ReadFromTextFile(home);
      text.clear();
     foreach(QString str,list)
         text+=str;
    view = new QTextBrowser(this);
        view->setText(text);
    QToolBar *toolBar = addToolBar("Navigation");
    QAction *Act;
    Act = new QAction(QIcon(":/images/home.png"), tr("Index"), this);
    connect(Act, SIGNAL(triggered()), this, SLOT(home()));
    toolBar->addAction(Act);
    findrep = new FindReplace(this,view,false);
    QWidget *mainWid = new QWidget;
    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->addWidget(view);
    mainLay->addWidget(findrep);
    findrep->setVisible(true);
    mainWid->setLayout(mainLay);
    setCentralWidget(mainWid);
    setUnifiedTitleAndToolBarOnMac(true);
    }
    ~HelpWindow(){}

protected slots:
    void adjustTitle(){}

private slots:

  void home(){
      view->setText(text);

  }

private:
    QTextBrowser *view;
    FindReplace *findrep;
    tools tool;
    QString text;

};

#endif //HELPWINDOW_H

