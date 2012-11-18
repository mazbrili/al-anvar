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

#ifndef ADDONS_H
#define ADDONS_H


#include <QMainWindow>
#include "database.h"
#include "Downloader.h"
#include "ui_addons.h"


namespace Ui {
    class Addons;
}

class Addons : public QMainWindow, public Ui::Addons
{
    Q_OBJECT

public:
    explicit Addons(DataBase *database,QWidget *parent = 0);
    void treeChargeAddons(QTreeWidget *view);

signals:
    void downlodFinishd(QStringList);

private slots:
    void on_pushButtonDownload_clicked();

    void on_pushButtonUpdate_clicked();

private:
    Downloader *downlod;  
    DataBase *db;
};

#endif // ADDONS_H
