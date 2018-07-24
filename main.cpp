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
 * it's a test!!                                                                         *
 ***************************************************************************/

#include "anvar.h"
#include "tools.h"
#include "langdialog.h"
#include <QStandardPaths>


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    app.setApplicationName( "Al-Anvar" );
    QFontDatabase::addApplicationFont(":/redist/ScheherazadeRegOT.ttf"); 
    QFontDatabase::addApplicationFont(":/redist/noorehira.ttf");//noorehira
    if(!QFile::exists(QStandardPaths::StandardLocation(QStandardPaths::FontsLocation)+"/noorehira.ttf"))
    {
        QFile::copy(":/redist/noorehira.ttf",QStandardPaths::StandardLocation(QStandardPaths::FontsLocation)+"/noorehira.ttf");
        QFile::copy(":/redist/ScheherazadeRegOT.ttf",QStandardPaths::StandardLocation(QStandardPaths::FontsLocation)+"/ScheherazadeRegOT.ttf");
    }


       tools tool;
             app.setApplicationName("Al-anvar");
             app.setApplicationVersion("050");
             QDir a_dirlang = tool.pathUser+"/language";
                if (! a_dirlang.exists()){
                   a_dirlang.mkdir(tool.pathUser+"/language");
                   a_dirlang.mkdir(tool.pathUser+"/addonse");                

                    a_dirlang.mkdir(tool.pathUser+"/language/help");
                    QFile::copy(":/doc/al-anvar-manual.English.html",tool.pathUser+"/language/help/al-anvar-manual.English.html");
                    QFile::copy(":/doc/al-anvar-manual.Farsi.html",tool.pathUser+"/language/help/al-anvar-manual.Farsi.html");
                    QFile::copy(":addonse-0.5.0.xml",tool.pathUser+"/addonse-0.5.0.xml");

                }

 QFont font;

                //for CD
                /*
                if(!QFile::exists(QDesktopServices::storageLocation(QDesktopServices::FontsLocation)+"/irsans.ttf"))
                    QFile::copy(":/redist/irsans.ttf",QDesktopServices::storageLocation(QDesktopServices::FontsLocation)+"/irsans.ttf");

            QFontDatabase::addApplicationFont(":/redist/irsans.ttf");//Iranian Sans

            font.setFamily("Iranian Sans");
            font.setPixelSize(14);
            tool.WriteSettings("trfont",tool.ReadSettings("trfont",font).value<QFont>());
            font.setPixelSize(12);
            tool.WriteSettings("ProgFont",tool.ReadSettings("ProgFont",font).value<QFont>());

                  QFile databaseFileName(tool.pathData+"/Data.db");
                  if(!databaseFileName.exists())
                     QFile::copy("Data.db",tool.pathData+"/Data.db");
                     tool.WriteSettings("lang",tool.ReadSettings("lang","persian").toString());
                     tool.GetLangValue();

            tool.WriteSettings("ExternalPlayer",tool.ReadSettings("ExternalPlayer","C:\\Program Files\\Windows Media Player\\wmplayer.exe").toString());


                     */
                     //End

                     font.setFamily("noorehira");
                     font.setPixelSize(18);
                     tool.WriteSettings("Quranfont",tool.ReadSettings("Quranfont",font).value<QFont>());



             if(tool.ReadSettings("lang","").toString()=="")
            {
                 LangDialog langDi;
                 if(langDi.exec())
                     tool.GetLangValue();
            }
             QPixmap pixmap(":/images/logo.png");
             QLabel* aWidget = new QLabel(0, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::WindowStaysOnTopHint );
             aWidget->setAttribute(Qt::WA_TranslucentBackground);
             aWidget->setPixmap(pixmap);
             aWidget->setGeometry(QRect(aWidget->x(),aWidget->y(),650,401));
             QRect qRect(QApplication::desktop()->screenGeometry());
             int iXpos=qRect.width()/2-aWidget->width()/2;
             int iYpos=qRect.height()/2-aWidget->height()/2;
             aWidget->move(iXpos,iYpos);
             aWidget->show();
             if(tool.trlang("Align")=="Right")
             app.setLayoutDirection(Qt::RightToLeft);

             app.processEvents();
             anvar w;
             w.show();
                aWidget->hide();
             return app.exec();
}
