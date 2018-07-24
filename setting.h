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
#ifndef SETTING_H
#define SETTING_H

#include <QColorDialog>
#include <QDesktopWidget>
#include "ui_setting.h"
#include "tools.h"


namespace Ui {
    class Setting;
}
class Setting : public QDialog, public Ui::Setting
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    QFont fontprog,fontchapter,fontchapterTr,fontBook;
    QColorDialog colorQ,colorTr,colorBook;
    bool mostRestart;

private slots:

    void ProgFont();
    void QuranFont();
    void trFont();
    void bookFont();
    void bookColor();
    void QuranColor();
    void trColor();
    void on_pushButtonSet_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonPathDatabase_clicked();
    void on_comb_lang_activated(const QString &arg1);



    void on_pushButtonPathAudio_clicked();

    void on_Btn_ChangeColor_clicked();

    void setFontColorPer();

    void on_Btn_ChangeFont_clicked();

private:
tools tool;
};

#endif // SETTING_H
