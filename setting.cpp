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


#include "setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent)

{
    setupUi(this);
    setWindowIcon(QIcon(":/images/app.png"));

    mostRestart=false;
    QFont defultfont = tool.ReadSettings("ProgFont","Tahoma,8,-1,5,50,0,0,0,0,0").value<QFont>();
#ifdef  Q_OS_LINUX
   defultfont = QFont();
 #endif
    QList<QWidget*> allWidget =findChildren<QWidget*>();
    foreach(QWidget* wid,allWidget)
            wid->setFont(defultfont);


    lineEditPathDatabase->setReadOnly(true);
    colorQ.setCurrentColor(tool.ReadSettings("QuranColor",QColor()).toString());
    colorBook.setCurrentColor(tool.ReadSettings("bookColor",QColor()).toString());
    fontBook=tool.ReadSettings("Bookfont",QFont()).value<QFont>();

    colorTr.setCurrentColor(tool.ReadSettings("trColor",QColor()).toString());
    fontchapterTr=tool.ReadSettings("trfont", QFont()).value<QFont>();
    fontchapter=tool.ReadSettings("Quranfont",QFont()).value<QFont>();
    fontprog=tool.ReadSettings("ProgFont", QFont()).value<QFont>();

    connect(radioButtonProgFC,SIGNAL(clicked()),this,SLOT(setFontColorPer()));
    connect(radioButtonQuranFC,SIGNAL(clicked()),this,SLOT(setFontColorPer()));
    connect(radioButtonTrFC,SIGNAL(clicked()),this,SLOT(setFontColorPer()));
    setFontColorPer();



}

void Setting::ProgFont(){
    bool ok;
     fontprog= QFontDialog::getFont(&ok,font(),this);
     setFontColorPer();

}
void Setting::QuranFont(){
    bool ok;
    fontchapter =  QFontDialog::getFont(&ok,tool.ReadSettings("Quranfont",QFont()).value<QFont>(),this);
    setFontColorPer();

}
void Setting::bookFont()
{
    bool ok;
    fontBook=  QFontDialog::getFont(&ok,tool.ReadSettings("bookfont",fontprog).value<QFont>(),this);
    setFontColorPer();
}
void Setting::trFont()
{
    bool ok;
    fontchapterTr=  QFontDialog::getFont(&ok,tool.ReadSettings("trfont",fontprog).value<QFont>(),this);
    setFontColorPer();
}
void Setting::QuranColor()
{
     colorQ.exec();
    setFontColorPer();
}

void Setting::bookColor()
{
     colorBook.exec();
    setFontColorPer();
}
void Setting::trColor()
{
    colorTr.exec();
   setFontColorPer();

}

void Setting::on_pushButtonSet_clicked()
{
    accept();
}



void Setting::on_pushButtonCancel_clicked()
{
    reject();
}

void Setting::on_pushButtonPathDatabase_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,tool.trlang("DataBase Dir"),tool.pathData);
    if(dir!="")
        lineEditPathDatabase->setText(dir);
    mostRestart=true;
}

void Setting::on_comb_lang_activated(const QString &arg1)
{
    mostRestart=true;

}


void Setting::on_pushButtonPathAudio_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,tool.trlang("Audio Dir"),tool.pathAudio);
    if(dir!="")
        lineEditPathAudio->setText(dir);
    mostRestart=true;
}

void Setting::on_Btn_ChangeColor_clicked()
{
      if(radioButtonQuranFC->isChecked())
        QuranColor();
    else
        trColor();

}

void Setting::setFontColorPer()
{
    QFont pFont;
    QColor  pColor;

    if(radioButtonProgFC->isChecked())
    {
        pFont =fontprog;
        pColor =Qt::black;
    }
    else  if(radioButtonQuranFC->isChecked())
    {
        pFont =fontchapter;
        pColor =colorQ.currentColor();
    }
    else
    {
        pFont =fontchapterTr;
        pColor =colorTr.currentColor();
    }
    QTextCharFormat fmt;
    fmt.setForeground(pColor);
    fmt.setFont(pFont);
    textEditFontPerviwe->selectAll();
    QTextCursor cursor = textEditFontPerviwe->textCursor();
    cursor.mergeCharFormat(fmt);
    textEditFontPerviwe->mergeCurrentCharFormat(fmt);
    cursor.clearSelection();
    textEditFontPerviwe->setTextCursor(cursor);
}

void Setting::on_Btn_ChangeFont_clicked()
{
    if(radioButtonProgFC->isChecked())
        ProgFont();
    else  if(radioButtonQuranFC->isChecked())
        QuranFont();
    else
        trFont();

}
