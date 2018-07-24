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
#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#include "tools.h"
#include <QFormLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QCompleter>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QCheckBox>
#include <QBoxLayout>
#include <QHBoxLayout>

class FindReplace : public QWidget
{
    Q_OBJECT
public:
    explicit FindReplace(QWidget *parent ,QTextEdit *textEdit_,bool replace=false):
        QWidget(parent)
    {
        textEdit =textEdit_;

        lineEditReplace = new QLineEdit(this);
        lineEditReplace->setVisible(replace);
        lineEditFind = new QLineEdit(this);
        findNextButton = new QAction(this);
        findPreviousButton = new QAction(this);

        matchCaseCheckbox = new QCheckBox(tool.trlang("Match Case"), this);

        QToolBar *tb_find = new QToolBar(this);

        tb_find->addAction(findNextButton);
        tb_find->addAction(findPreviousButton);


        connect(lineEditFind,SIGNAL(textChanged(QString)),this,SLOT(setTypedText(QString)));
        QHBoxLayout *mainLayout = new QHBoxLayout(this);//Main Layout
        toolButton = new QToolButton(this);
        toolButton->setAutoRaise(true);
        toolButton->setIcon(QIcon(":/images/delete.png"));
        toolButton->setShortcut(QKeySequence("Esc"));
        connect(toolButton, SIGNAL(clicked()), this, SLOT(deleteAddedWidget()));
        mainLayout->addWidget(toolButton);
        QFormLayout *verticalLayoutSub = new QFormLayout;//The layout that contains the other layouts
        //Find Layout Start
        QHBoxLayout *horizontalLayoutFind = new QHBoxLayout;
        QLabel *label = new QLabel(this);
        label->setText(tool.trlang("Search in current text"));

        lineEditFind->setObjectName("lineEditFind");
        findNextButton->setText(tool.trlang("Next"));
        findNextButton->setIcon(QIcon(":/images/next-icon.png"));
        connect(findNextButton, SIGNAL(triggered()), this, SLOT(findNext()));
        findPreviousButton->setText(tool.trlang("Previous"));
        findPreviousButton->setIcon(QIcon(":/images/previous-icon.png"));
        connect(findPreviousButton, SIGNAL(triggered()), this, SLOT(findPrevious()));
        connect(matchCaseCheckbox, SIGNAL(clicked(bool)), this, SLOT(setMatchCase(bool)));
        horizontalLayoutFind->addWidget(label);
        horizontalLayoutFind->addWidget(lineEditFind);
        horizontalLayoutFind->addWidget(tb_find);
        horizontalLayoutFind->addWidget(matchCaseCheckbox);
        verticalLayoutSub->addRow(horizontalLayoutFind);
        //Find Layout End

        if(replace)
        {
            QToolBar *tb_replace = new QToolBar(this);
            QAction*replaceButton,*replaceAllButton;
            replaceAllButton = new QAction(this);
            replaceButton = new QAction(this);
            tb_replace->addAction(replaceAllButton);
            tb_replace->addAction(replaceButton);

            //Find and Replace Layout Start
            QHBoxLayout *horizontalLayoutReplace = new QHBoxLayout;
            QLabel *label = new QLabel(this);
            label->setText("Replace:");
            lineEditReplace->setObjectName("lineEditReplace");
            replaceButton->setText("Replace");
            connect(replaceButton, SIGNAL(triggered()), this, SLOT(replace()));
            replaceAllButton->setText("Replace All");
            connect(replaceAllButton, SIGNAL(triggered()), this, SLOT(replaceAll()));
            horizontalLayoutReplace->addWidget(label);
            horizontalLayoutReplace->addWidget(lineEditReplace);
            horizontalLayoutReplace->addWidget(tb_replace);
            verticalLayoutSub->addRow(horizontalLayoutReplace);
            //Find and Replace Layout End
        }
        mainLayout->addLayout(verticalLayoutSub);
        setModelCompleter();

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
    void setModelCompleter(){
        QStringList list = textEdit->toPlainText().split(" ");
        list.removeDuplicates();
        QCompleter *completer  = new QCompleter(list,this);
        lineEditFind->setCompleter(completer);
    }


    QTextEdit *textEdit;
    QRegExp typedText;
    QLineEdit *lineEditFind,*lineEditReplace;
    QAction *findNextButton,*findPreviousButton;
    QCheckBox *matchCaseCheckbox;
    QToolButton *toolButton;
    bool matchCase;
    tools tool;


signals:


public slots:

    QString alifHamza(QString txt){

        QString addtxt;
        txt.replace(QString::fromUtf8("ی"),QString::fromUtf8("ي"));
        txt.replace(QString::fromUtf8("ک"),QString::fromUtf8("ك"));
           QStringList arabics;
           arabics << QString::fromUtf8("ض") << QString::fromUtf8("ص") << QString::fromUtf8("ث") << QString::fromUtf8("ق") << QString::fromUtf8("ف")
                   << QString::fromUtf8("غ") << QString::fromUtf8("ع") << QString::fromUtf8("ه") << QString::fromUtf8("ة") << QString::fromUtf8("خ")
                   << QString::fromUtf8("ح") << QString::fromUtf8("ج") << QString::fromUtf8("چ") << QString::fromUtf8("ش") << QString::fromUtf8("س")
                   << QString::fromUtf8("ی") << QString::fromUtf8("ي") << QString::fromUtf8("ب") << QString::fromUtf8("ل") << QString::fromUtf8("ا")
                   << QString::fromUtf8("آ") << QString::fromUtf8("أ") << QString::fromUtf8("إ") << QString::fromUtf8("ئ") << QString::fromUtf8("ت")
                   << QString::fromUtf8("ن") << QString::fromUtf8("م") << QString::fromUtf8("ک") << QString::fromUtf8("ك") << QString::fromUtf8("گ")
                   << QString::fromUtf8("ظ") << QString::fromUtf8("ط") << QString::fromUtf8("ز") << QString::fromUtf8("ر") << QString::fromUtf8("ذ")
                   << QString::fromUtf8("د") << QString::fromUtf8("پ") << QString::fromUtf8("و") << QString::fromUtf8("ؤ") << QString::fromUtf8("ژ");

          for(int i =0;i<txt.count();i++){
                QString set=txt[i]+"";
              if(arabics.contains(set))
            addtxt = addtxt+txt[i]+QString::fromUtf8("(ّ|َ|ِ|ُ|ً|ٍ|ٌ|ْ|ٰ)")+"*";
              else
            addtxt = addtxt+txt[i];

          }

            addtxt.replace(QString::fromUtf8("و"),QString::fromUtf8("(و|ؤ)"));
            addtxt.replace(QString::fromUtf8("ا"),QString::fromUtf8("(آ|أ|إ|ا)"));
            addtxt.replace(QString::fromUtf8("ك"),QString::fromUtf8("(ك|ک)"));
            addtxt.replace(QString::fromUtf8("ي"),QString::fromUtf8("(ى|ي|ی)"));
            addtxt.replace(QString::fromUtf8("ه"),QString::fromUtf8("(ه|ة)"));


     return addtxt;

       }

     void setTypedText(QString text)
    {
        text= clearErab(text);
        typedText.setPattern(alifHamza(text));
        findTypedText();
    }

    void deleteAddedWidget()
    {
       setVisible(false);
    }
    void findTypedText()
    {
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        if(matchCase)
            cursor = textEdit->document()->find(typedText, cursor, QTextDocument::FindCaseSensitively);
        else
            cursor = textEdit->document()->find(typedText, cursor);
        if(!cursor.isNull())
            textEdit->setTextCursor(cursor);
        if(typedText.isEmpty())
            cursor.clearSelection();
    }
    void findNext()
    {
        QTextCursor cursor = textEdit->textCursor();
        if(cursor.atEnd())
            cursor.movePosition(QTextCursor::Start);
        if(matchCase)
            cursor = textEdit->document()->find(typedText, cursor, QTextDocument::FindCaseSensitively);
        else
            cursor = textEdit->document()->find(typedText, cursor);
        if(!cursor.isNull())
            textEdit->setTextCursor(cursor);
    }
    void findPrevious()
    {
        QTextCursor cursor = textEdit->textCursor();
        if(cursor.selectionStart() == 0)
            cursor.movePosition(QTextCursor::End);
        if(matchCase)
            cursor = textEdit->document()->find(typedText, cursor, QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
        else
            cursor = textEdit->document()->find(typedText, cursor, QTextDocument::FindBackward);
        if(!cursor.isNull())
            textEdit->setTextCursor(cursor);
    }

    void replaceAll()
    {
        QTextCursor cursor = textEdit->textCursor();
        QString replacedText = textEdit->document()->toPlainText().replace(typedText, lineEditReplace->text());
        cursor.select(QTextCursor::Document);
        cursor.insertText(replacedText);
    }
    void replace()
    {
        QTextCursor cursor = textEdit->textCursor();
        if(cursor.hasSelection())
        {
            cursor.insertText(lineEditReplace->text());
            findNext();
        }
        else
        {
            findNext();
            if(cursor.hasSelection())
                cursor.insertText(lineEditReplace->text());
            findNext();
        }
    }

    void setMatchCase(bool boolCase)
    {
        matchCase = boolCase;
        findTypedText();
    }


};

#endif // FINDREPLACE_H

