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


namespace Ui {

    class QuranWidget;
}
class QuranWidget : public QWidget , public Ui::QuranWidget
{
    Q_OBJECT
public:
    QToolBar *toolBar;
    explicit QuranWidget(DataBase *database,QWidget *parent = 0):
        QWidget(parent)
    {
       setupUi(this);
       db = database;
       chengeVerse=true;
       stopHighlight=false;
       acVerse= new QAction(0);
       acPlay= new QAction(QIcon(":images/audio.png"),db->trlang("Play"),0);
       acInter= new QAction(QIcon(":images/Explanations.png"),db->trlang("Interpretation"),0);
       acSubject= new QAction(QIcon(":images/tree.png"),db->trlang("Subject of the Verses"),0);
       acComment= new QAction(QIcon(":images/comment_show.png"),db->trlang("Add a Point"),0);
       acMail = new QAction(QIcon(":images/newsletter.png"),db->trlang("need Explanations"),0);
       acSimiler= new QAction(db->trlang("Similar Words"),0);
       acfind=new QAction(QIcon(":images/search.png"),db->trlang("Search in current text"),0);
       acPlay->setShortcut(Qt::CTRL + Qt::Key_Q);
       acInter->setShortcut(Qt::CTRL + Qt::Key_I);
       acSubject->setShortcut(Qt::CTRL + Qt::Key_T);
       acComment->setShortcut(Qt::CTRL + Qt::Key_N);
       acfind->setShortcut(QKeySequence("F2"));

       for (int i = 0; i < MaxRecent; ++i) {
          acSub[i] = new QAction(0);
          acSub[i]->setIcon(QIcon(":images/tag.png"));
          acSub[i]->setVisible(false);
          connect(acSub[i], SIGNAL(triggered()),
                  this, SLOT(similarVerses()));
      }

       connect(acPlay,SIGNAL(triggered()),this,SLOT(PlayQuran()));
       connect(acInter,SIGNAL(triggered()),this,SLOT(ShowExplanations()));
       connect(acSubject,SIGNAL(triggered()),this,SLOT(AddToSubjects()));
       connect(acComment,SIGNAL(triggered()),this,SLOT(AddToComment()));
       connect(acSimiler,SIGNAL(triggered()),this,SLOT(showDuplicatesWords()));
       connect(acfind, SIGNAL(triggered()), this, SLOT(showFindReplase()));
       connect(acMail, SIGNAL(triggered()), this, SLOT(needExplanations()));



       connect(listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(highlightCurrentLine()));
       listWidget->addAction(acVerse);
       QAction *separ = new QAction(0);
       separ->setSeparator(true);


       listWidget->addAction(separ);
       listWidget->addAction(acPlay);
       listWidget->addAction(acInter);
       listWidget->addAction(acSubject);
       listWidget->addAction(acComment);
       listWidget->addAction(acMail);
       listWidget->addAction(acSimiler);
       listWidget->addAction(acfind);

        toolBar = new QToolBar;
        toolBar->setIconSize(QSize(24,24));
        toolBar->setFixedWidth(165);
        toolBar->addAction(acPlay);
        toolBar->addAction(acInter);
        toolBar->addAction(acSubject);
        toolBar->addAction(acComment);
        toolBar->addAction(acfind);


       QAction *separ2 = new QAction(0);
       separ2->setSeparator(true);

       listWidget->addAction(separ2);


       QAction *sub = new QAction(0);
       sub->setText(db->trlang("Similar Verses")+":");

       listWidget->addAction(sub);


       for (int i = 0; i < MaxRecent; ++i)
       {

         listWidget->addAction(acSub[i]);
       }


        exPl=new ExternalPlayer(0);
        inPl=new InternalPlayer(0);
        exPl->setVisible(false);
        inPl->setVisible(false);

        playLayout->addWidget(inPl);
        playLayout->addWidget(exPl);

        if(db->trlang("Align")=="Right"){
       exPl->setLayoutDirection(Qt::RightToLeft);
       inPl->setLayoutDirection(Qt::RightToLeft);

        }
        connect(exPl,SIGNAL(verseChanged(QString,QString)),this,SLOT(finder(QString,QString)));
        connect(inPl,SIGNAL(verseChanged(QString,QString)),this,SLOT(finder(QString,QString)));


       // textChapter->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       // textChapter->setOpenLinks(false);
        //textChapter->setFrameShape(QFrame::NoFrame);
       //  findRep = new FindReplace(this,textChapter);
     //    findRep->setVisible(false);
         line_Chapter->setValue(1);
         line_Chapter->setMinimum(1);
         line_Chapter->setMaximum(114);
         line_Verse->setValue(1);
         line_Verse->setMinimum(1);
         line_Verse->setMaximum(290);
         label_Hizb->setValue(1);
         label_Hizb->setMinimum(1);
         label_Hizb->setMaximum(120);
         label_Juz->setValue(1);
         label_Juz->setMinimum(1);
         label_Juz->setMaximum(30);
         line_Page->setValue(1);
         line_Page->setMinimum(1);
         line_Page->setMaximum(604);

         line_Chapter->setInputMethodHints(Qt::ImhDigitsOnly);
         label_Hizb->setInputMethodHints(Qt::ImhDigitsOnly);
         line_Chapter->setInputMethodHints(Qt::ImhDigitsOnly);
         label_Juz->setInputMethodHints(Qt::ImhDigitsOnly);
         line_Page->setInputMethodHints(Qt::ImhDigitsOnly);

         lableChapter->setText(db->trlang("Chapter"));
         lableVerse->setText(db->trlang("Verse"));
         lablePage->setText(db->trlang("Page"));
         lableJuz->setText(db->trlang("Section"));
         lableHizb->setText(db->trlang("Hizb"));


         QToolButton *zoomInButton = new QToolButton(this);
         zoomInButton->setAutoRaise(true);
         zoomInButton->setIcon(QIcon(":/images/zoom-in.png"));
         connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
         QToolButton *zoomOutButton = new QToolButton(this);
         zoomOutButton->setAutoRaise(true);
         zoomOutButton->setIcon(QIcon(":/images/zoom-out.png"));
         connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomQut()));
         zoomOutButton->setVisible(false);
         zoomInButton->setVisible(false);






        actionsLayout->addWidget(toolBar);
        //findLay->addWidget(findRep);
        check_SameWord=db->ReadSettings("SameWord",false).toBool();

        igEvent=false;
           translationList = db->getListData("Select id,Name,TableName From Tr_Translation;");
           isJuz=false;
           defultfont = db->ReadSettings("ProgFont","Tahoma,8,-1,5,50,0,0,0,0,0").value<QFont>();
           #ifdef  Q_OS_LINUX
           defultfont = QFont();
          #endif
           nextId=0;
           subjectList = db->getListData("Select id,Name From SubjectName;");
           QuranRoot = db->getListData("SELECT *FROM quran_root ORDER BY id;");
            listAudio = db->getListData("Select Name,dir_Name,Url From Audio ;");

            foreach (QString str,listAudio.at(0))
            {
              exPl->comb_ghary->addItem(db->trlang(str));
              inPl->comb_ghary->addItem(db->trlang(str));
              inPl->comboBoxRep->addItem(db->trlang(str));


            }

            setTranslation();
            line_Chapter->setValue(1);
              connect(line_Chapter,SIGNAL(valueChanged(int)),this,SLOT(ChapterChangedLine()));
              connect(line_Verse, SIGNAL(valueChanged(int)), this, SLOT(VerseChangedLine()));
              connect(label_Juz, SIGNAL(valueChanged(QString)), this, SLOT(showJuz(QString)));
              connect(label_Hizb,SIGNAL(valueChanged(QString)),this,SLOT(showHizb(QString)));
              connect(line_Page,SIGNAL(valueChanged(int)),this,SLOT(PageChanged()));
  }



    void getQuranValue(QString sql,bool besm=true)
    {

        listWidget->clear();
        versePeroperti.clear();
        stopHighlight=true;
        check_subject=db->ReadSettings("iconsubject",true).toBool();
        check_Similer=db->ReadSettings("similiarVerse",false).toBool();
        check_audio=db->ReadSettings("iconaudio",true).toBool();
        check_comment=db->ReadSettings("iconcomment",true).toBool();
        check_explanation=db->ReadSettings("iconexplanation",true).toBool();
        radio_both=db->ReadSettings("showboth",true).toBool();
        radio_Quran=db->ReadSettings("showquran",false).toBool();
        radio_tr=db->ReadSettings("showtr",false).toBool();


        int alignAr=2;
        int alignTr=1;
        if(db->trlang("Align")=="Right")
        {
       alignAr=1;
       alignTr=2;
        }
        if(strTranslation.startsWith("fa_") || strTranslation.startsWith("ar_"))
            alignTr=alignAr;



        quranValueSql=sql;
        quranValueBsm=besm;
       if(strTranslation=="")
            sql.replace("?translation","");
        else
            sql.replace("?translation","Left Join "+strTranslation+" On ("+strTranslation+".id=quran.id)");
        mainQuranValue = db->getListData(sql);
        setQuranPeroperty(mainQuranValue.at(0).at(0));
        nextId=0;

                QFont formatArL,formatTrL,formatL;
                QTextCharFormat formatAr,formatTr,format;

                formatArL=db->ReadSettings("Quranfont",QFont()).value<QFont>();
                formatTrL=db->ReadSettings("trfont",QFont()).value<QFont>();
                formatL=db->ReadSettings("ProgFont",QFont()).value<QFont>();

                formatAr.setFont(db->ReadSettings("Quranfont",QFont()).value<QFont>());
                formatTr.setFont(db->ReadSettings("trfont",QFont()).value<QFont>());
                format.setFont(db->ReadSettings("ProgFont",QFont()).value<QFont>());
                formatAr.setForeground(db->ReadSettings("QuranColor",QColor()).value<QColor>());
                formatTr.setForeground(db->ReadSettings("trColor",QColor()).value<QColor>());



        if (!mainQuranValue.isEmpty()){
        for(int i=0;i<mainQuranValue.at(0).size();i++){


            //besmellah

            if(radio_Quran && mainQuranValue.at(3).at(i)=="1" && mainQuranValue.at(2).at(i)!="1" && mainQuranValue.at(2).at(i)!="9")
            {

                    QListWidgetItem  *item = new QListWidgetItem(db->getDatastr("Select Arabic From Quran Where id=1 ;"), listWidget);
                    item->setData(12,mainQuranValue.at(0).at(i)+";"+QString::number(i)+";"+mainQuranValue.at(2).at(i));
                     item->setFont(formatArL);
                     item->setTextAlignment(alignAr);

            }
            if(radio_both && mainQuranValue.at(3).at(i)=="1" && mainQuranValue.at(2).at(i)!="1" && mainQuranValue.at(2).at(i)!="9")
            {



                    QListWidgetItem  *item =   new QListWidgetItem(db->getDatastr("Select Arabic From Quran Where id=1 ;"), listWidget);
                    QListWidgetItem  *item2 =   new QListWidgetItem(db->getDatastr("Select tr From "+strTranslation+" Where id=1 ;"), listWidget);
                    item->setData(12,mainQuranValue.at(0).at(i)+";"+QString::number(i)+";"+mainQuranValue.at(2).at(i));
                    item->setFont(formatArL);
                    item2->setFont(formatTrL);
                    item->setTextAlignment(alignAr);
                    item2->setTextAlignment(alignTr);

            }
            if(radio_tr && mainQuranValue.at(3).at(i)=="1" && mainQuranValue.at(2).at(i)!="1" && mainQuranValue.at(2).at(i)!="9")
            {
                   QListWidgetItem  *item =    new QListWidgetItem(db->getDatastr("Select tr From "+strTranslation+" Where id=1 ;"), listWidget);
                  item->setData(12,mainQuranValue.at(0).at(i)+";"+QString::number(i)+";"+mainQuranValue.at(2).at(i));
                  item->setFont(formatTrL);
                  item->setTextAlignment(alignTr);
            }

          if(radio_both)
          {

                     QListWidgetItem  *item =  new QListWidgetItem(mainQuranValue.at(1).at(i), listWidget);
                     QListWidgetItem  *item2 =  new QListWidgetItem(mainQuranValue.at(4).at(i)+"["+mainQuranValue.at(2).at(i)+":"+mainQuranValue.at(3).at(i)+"]", listWidget);
                     item->setData(12,mainQuranValue.at(0).at(i)+";"+QString::number(i));
                     item->setFont(formatArL);
                     item2->setFont(formatTrL);
                     item->setTextAlignment(alignAr);
                     item2->setTextAlignment(alignTr);
          }

          if(radio_Quran )
          {

                  QListWidgetItem  *item =  new QListWidgetItem(mainQuranValue.at(1).at(i)+"["+mainQuranValue.at(2).at(i)+":"+mainQuranValue.at(3).at(i)+"]", listWidget);
                  item->setData(12,mainQuranValue.at(0).at(i)+";"+QString::number(i));
                  item->setFont(formatArL);
                  item->setTextAlignment(alignAr);
          }

          if(radio_tr )
          {
                QListWidgetItem  *item =  new QListWidgetItem(mainQuranValue.at(4).at(i)+"["+mainQuranValue.at(2).at(i)+":"+mainQuranValue.at(3).at(i)+"]", listWidget);
                item->setData(12,mainQuranValue.at(0).at(i)+";"+QString::number(i));
                item->setFont(formatTrL);
                item->setTextAlignment(alignTr);

          }

          if(i==50 || i==100 || i==150 || i==200 || i==230)
              QApplication::processEvents();

     }
    }

          stopHighlight=false;
          db->WriteSettings("lastChapter",line_Page->text());
              if(listWidget->count()>0)
                listWidget->setCurrentRow(0);
                highlightCurrentLine();

    }

    void setQuranPeroperty(QString id)
    {
         igEvent=true;
        line_Chapter->setValue(db->getDatastr("Select Chapter From Quran Where id="+id).toInt());
        line_Page->setValue(db->getDatastr("Select Page From Quran Where id="+id).toInt());
        label_Juz->setValue(db->getDatastr("Select Juz From Quran Where id="+id).toInt());
        label_Hizb->setValue(db->getDatastr("Select Hizb From Quran Where id="+id).toInt());
        isJuz=false;
        igEvent=false;


    }
    void setTranslation(){
        translationStr="";
        strTranslation="";
        int idtranslation =      db->ReadSettings("translation",0).toInt();
           translationStr =",tr";
           if(idtranslation>translationList.at(2).size()-1)
               idtranslation=0;
           strTranslation = translationList.at(2).at(idtranslation);
    }
public slots:

    void highlightCurrentLine()
    {
        if(stopHighlight)
            return;
        chengeVerse=false;

                  QListWidgetItem *item= listWidget->currentItem();
                  if(!item)
                      return;
                        QString value =item->data(12).toString();

                        if(value.split(";").size()>1)
                        {
                          currentBlock = value.split(";").at(0);
                          currentId = value.split(";").at(1);
                        }
                        if( currentBlock=="1")
                           currentBlock= db->getDatastr("Select Id From Quran Where Chapter ="+line_Chapter->text());


                        QString name =db->trlang(db->getDatastr("Select descent From Quran Left Join ChapterProperty on (Quran.Chapter=ChapterProperty.id)  Where Quran.id= "+currentBlock));
                        QString chapter = db->getDatastr("Select Chapter From Quran Where id= "+currentBlock);
                        QString verse = db->getDatastr("Select Verse From Quran Where id= "+currentBlock);
                        line_Chapter->setValue(chapter.toInt());
                        line_Verse->setValue(verse.toInt());
                        line_Page->setValue(db->getDatastr("Select Page From Quran Where id= "+currentBlock).toInt());
                        label_Hizb->setValue(db->getDatastr("Select Hizb From Quran Where id= "+currentBlock).toInt());
                        label_Juz->setValue(db->getDatastr("Select Juz From Quran Where id= "+currentBlock).toInt());
                        acVerse->setText(QString("%1: %2(%3) - %4: %5").arg(db->trlang("Chapter")).arg(name).arg(chapter).arg(db->trlang("Verse")).arg(verse));
                        lableMakki->setText(QString("%1(%2)-%3:%4-").arg(name).arg(chapter).arg(db->trlang("Verse")).arg(verse)+
                                    db->trlang(db->getDatastr("Select Nozol From ChapterProperty Where id="+line_Chapter->text()))
                                            );
                        if(db->getDatastr("select  count(id) from comment where id="+currentBlock)=="0")
                              acComment->setIcon(QIcon(":images/comment_add.png"));
                        else
                              acComment->setIcon(QIcon(":images/comment_show.png"));


                        QList<QStringList> subnamelist=db->getListData("Select SubjectName.id,Name From Subjects left Join SubjectName on (Subjects.ID_SUBject=SubjectName.id) Where ID_VERSE="+currentBlock);
                        if(subnamelist.size()>0)
                        {
                        for(int i=0;i<subnamelist.at(0).count();i++)
                         {
                            if(i<MaxRecent)
                             {
                             acSub[i]->setText(db->trlang(subnamelist.at(1).at(i)));
                             acSub[i]->setData(subnamelist.at(0).at(i));
                             acSub[i]->setVisible(true);
                            }
                         }
                        }

              chengeVerse=true;
    }

    void needExplanations()
    {

        ContactForm cotact;
        cotact.setWindowTitle(db->trlang(""));
        cotact.plainTextEdit->setPlainText(db->trlang("Tel Me More about this verse:")+"\n"+db->getDatastr("Select tr From "+strTranslation+" Where id ="+currentBlock)+
                                           "["+db->getDatastr("Select Chapter From Quran Where id ="+currentBlock)+
                                           ";"+db->getDatastr("Select Verse From Quran Where id ="+currentBlock)+"]");
        cotact.exec();
    }
    void ChapterChangedLine()
    {
        if(igEvent)
            return;
        line_Page->setFocus();
        ChapterChanged();
        line_Chapter->setFocus();
    }
    void VerseChangedLine()
    {
        if(igEvent)
            return;
        igEvent=true;
        VerseChanged();
        igEvent=false;


    }
    void PageChanged()
    {
        if(!chengeVerse)
                 return;
             if(igEvent)
                 return;

           getQuranValue("Select  Quran.id,Arabic,Chapter,Verse"+translationStr+" From Quran ?translation Where Quran.Page = "+ line_Page->text()+" ;",false);

    }
    void showJuz(QString text){
        if(!chengeVerse)
            return;
        if(igEvent)
            return;
        isJuz=true;
        line_Page->setFocus();
        getQuranValue("Select Quran.id,Arabic,Chapter,Verse"+translationStr+" From Quran  Left Join Juz On (Quran.Juz=Juz.id) ?translation  Where Quran.Juz ='"+text+"';");
        label_Juz->setFocus();
    }
    void showHizb(QString text){
        if(!chengeVerse)
            return;
        if(igEvent)
            return;
        isJuz=true;
        line_Page->setFocus();
      getQuranValue("Select Quran.id,Arabic,Chapter,Verse"+translationStr+" From Quran  Left Join Hizb On (Quran.Hizb=Hizb.id) ?translation  Where Quran.Hizb ='"+text+"';");
        label_Hizb->setFocus();
    }
    void ChapterChanged()
    {
        if(!chengeVerse)
            return;
        getQuranValue("Select  Quran.id,Arabic,Chapter,Verse"+translationStr+" From Quran ?translation Where Quran.Chapter = "+ line_Chapter->text()+" ;",true);

    }
    void VerseChanged()
    {
        if(!chengeVerse)
            return;
      int Chapterint = line_Chapter->value();
      int Verseint = line_Verse->value();
      QString str ="["+db->getDatastr("Select  chapter  From Quran  Where Chapter = "+ line_Chapter->text()+" and Verse = "+ line_Verse->text()+";")+":"+db->getDatastr("Select  Verse  From Quran  Where Chapter = "+  line_Chapter->text()+" and Verse = "+  line_Verse->text()+";")+"]";
       bool ok = finder(str);
      line_Page->setValue(db->getDatastr("Select Page From Quran   Where Chapter = "+  line_Chapter->text()+" and Verse = "+line_Verse->text()+";").toInt());
      label_Juz->setValue(db->getDatastr("Select Juz From Quran  Where Chapter = "+  line_Chapter->text()+" and Verse = "+ line_Verse->text()+";").toInt());
      label_Hizb->setValue(db->getDatastr("Select Hizb From Quran  Where Chapter = "+  line_Chapter->text()+" and Verse = "+line_Verse->text()+";").toInt());
      line_Verse->setValue(db->getDatastr("Select Verse From Quran   Where Chapter = "+  line_Chapter->text()+" and Verse = "+line_Verse->text()+";").toInt());
      if(!ok){
          if(db->getDatastr("Select Count(id) From Quran Where Chapter="+QString::number(Chapterint)+" and Verse="+QString::number(Verseint)+" ;")=="0"){
              Chapterint++;
              Verseint=1;
          }
          line_Chapter->setValue(Chapterint);
          ChapterChanged();
          line_Verse->setValue(Verseint);
          VerseChanged();
      }
    }
    void goToVerseFromSearch(QString str){

        line_Chapter->setValue(str.split(";").at(0).toInt());
          ChapterChanged();
        line_Verse->setValue(str.split(";").at(1).toInt());
         VerseChanged();

    }
    void AddToSubjects(){

        emit openAyeExplor(currentBlock,0);
    }
    void showDuplicatesWords(){

        emit openAyeExplor(currentBlock,3);


    }
    void AddToComment(){       
        emit openAyeExplor(currentBlock,1);


    }
    void ShowExplanations(){
        emit openAyeExplor(currentBlock,2);

    }
    void PlayQuran()
    {

        QStringList idListAdiuo;
        if(!mainQuranValue.isEmpty())
        idListAdiuo=mainQuranValue.at(0);


        QStringList ChapterNo,VerseNo;
        if (db->getDatastr("Select Verse From Quran Where id ="+mainQuranValue.at(0).at(currentId.toInt()))=="1" &&
           db->getDatastr("Select Chapter From Quran Where id ="+mainQuranValue.at(0).at(currentId.toInt()))!="9"
           && db->getDatastr("Select id  From Quran Where id ="+mainQuranValue.at(0).at(currentId.toInt()))!="1")
       {
          idListAdiuo.prepend("1");

       }

         for(int i=currentId.toInt();i<idListAdiuo.size();i++){
       ChapterNo.append(db->getDatastr("Select Chapter From Quran Where id ="+idListAdiuo.at(i)));
       VerseNo.append(db->getDatastr("Select Verse From Quran Where id ="+idListAdiuo.at(i)));
       };




         bool internal=db->ReadSettings("UsePhonon",true).toBool();

         if(internal){

             inPl->comb_ghary->setCurrentIndex(db->ReadSettings("Audio",0).toInt());
             inPl->comboBoxRep->setCurrentIndex(db->ReadSettings("Audio2",inPl->comboBoxRep->count()-1).toInt());
             inPl->checkBoxRep->setChecked(db->ReadSettings("PlayAudio2",false).toBool());

         inPl->idaudio(listAudio.at(2),
                     db->pathAudio,listAudio.at(1),
                     ChapterNo,VerseNo);

         inPl->setVisible(true);
         inPl->readyToPlay();
         }else{

             exPl->idaudio(listAudio.at(2),
                         db->pathAudio,listAudio.at(1),
                         ChapterNo,VerseNo);



             exPl->setVisible(true);
             exPl->readyToPlay();



         }





    }
    void similarVerses()
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action){
            QString sql = "Select Quran.id,Arabic,Quran.Chapter,Quran.Verse"+translationStr+" From Subjects Left Join quran On (Subjects.ID_VERSE=quran.id) ?translation left join ChapterProperty on (quran.Chapter=ChapterProperty.id)  Where Subjects.ID_SUBJECT ="+action->data().toString()+";";
            emit showRelated(action->text(),sql);
        }

    }
    void showFindReplase()
    {
          //findRep->setVisible(!findRep->isVisible());
         // findRep->lineEditFind->setFocus();
    }
    void zoomIn() {

      //  textChapter->setText(textChapter->document()->toHtml());
        //textChapter->zoomIn(1);

    }
    void zoomQut() {
       // textChapter->zoomOut(1);

    }
    bool finder(QString searchString)
    {
        QList<QListWidgetItem*> items =listWidget->findItems(searchString,Qt::MatchContains);
        if(items.size()>0){
            if(radio_both)
            {
                  listWidget->setCurrentItem(items.at(0));
                  int i= listWidget->currentIndex().row();
                  if(i>0)
                  listWidget->setCurrentItem(listWidget->item(i-1));

            }
            else
            listWidget->setCurrentItem(items.at(0));
            return true;
        }
        return false;
    }

    bool finder(QString chapter,QString verse)
    {
        QString str ="["+chapter+":"+verse+"]";
       return finder(str);
    }

signals:
 void showRelated(QString,QString);
 void openAyeExplor(QString,int);

public:
  DataBase * db;
  DBModelSubJectQuran *treeModel;
  bool quranValueBsm,isJuz,igEvent,radio_Quran,
  radio_tr,radio_both,check_subject,check_comment,
  check_SameWord,check_audio,check_explanation,check_Similer;
  QHash<int,QString> versePeroperti;
  int nextId;
  QString translationStr,strTranslation,quranValueSql;
  QList<QStringList> subjectList,translationList,listAudio,mainQuranValue,QuranRoot;
  QFont defultfont;

  ExternalPlayer *exPl;
  InternalPlayer *inPl;
  //FindReplace *findRep;
  QAction *acVerse,*acPlay,*acInter,*acSubject,*acComment,*acSimiler,*acfind,*acMail;
  QString currentBlock,currentId;
  enum { MaxRecent = 12 };
  QAction *acSub[MaxRecent];
  bool chengeVerse,stopHighlight;





};

#endif // QURANWIDGET_H
