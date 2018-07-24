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

#include "anvar.h"
#include "ui_anvar.h"
#include "moshafwidget.h"
#include <QStandardPaths>
#include <QPrintPreviewWidget>

anvar::anvar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::anvar)
{
    ui->setupUi(this);

    fristSearchQ=true;
    fristSearchRoot=true;

    ui->toolBoxQuran->setCurrentIndex(0);
    ui->tabWidgetQuran->removeTab(1);
    ui->tabWidgetQuran->removeTab(0);
    ui->actionLayoutChapter->setIcon(QIcon(":/images/layoutdelete.png"));
    ui->actionExit->setShortcut(QKeySequence::Close);
    ui->actionSave->setShortcut(QKeySequence::SaveAs);
    ui->actionPrint->setShortcut(QKeySequence::Print);
    ui->actionHelp->setShortcut(Qt::Key_F1);
    ui->actionOptions->setShortcut(Qt::CTRL + Qt::Key_O);
    this->setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/images/app.png"));
    QApplication::setApplicationName("Al-Anavr");
    QWidget *titleWidget = new QWidget(this);
    ui->dockWidget->setTitleBarWidget(titleWidget);
 nextId=0;
 stp = new Setting(this);
  stp->radioButtonExterna->setChecked(tool.ReadSettings("UseExterna",false).toBool());
  stp->radioButtonPhonon->setChecked(tool.ReadSettings("UsePhonon",true).toBool());
#ifdef  Q_OS_LINUX
   stp->lineEditExternalPlayer->setText(tool.ReadSettings("ExternalPlayer","mplayer").toString());
  #endif
#ifdef Q_OS_WIN32
   stp->lineEditExternalPlayer->setText(tool.ReadSettings("ExternalPlayer","C:\\Program Files\\Windows Media Player\\wmplayer.exe").toString());
#endif
   stp->radioButtonPhonon->setEnabled(tool.ReadSettings("internalavailable",true).toBool());
   if(!stp->radioButtonPhonon->isEnabled())
         stp->radioButtonExterna->setChecked(true);
    stp->comboBoxStyleSheet->addItem(tool.trlang("default"));
    stp->comboBoxStyleSheet->addItem(tool.trlang("simple"));
    stp->comboBoxStyleSheet->addItem(tool.trlang("Pro"));


    QStringList styeList ;
    styeList << "default" <<"simple" <<"Pro";
    stp->comboBoxStyleSheet->setCurrentIndex(tool.ReadSettings("StyleSheet",0).toInt());
    QString StyleProg;
    foreach(QString str,tool.ReadFromTextFile(":Styles/"+styeList.at(tool.ReadSettings("StyleSheet",0).toInt())+".css"))
        StyleProg +=str;
    setStyleSheet(StyleProg);

    stp->comboBoxStyle->addItems(QStyleFactory::keys());
    QString styleStr = tool.ReadSettings("Style",QApplication::style()->objectName()).toString();
    if(styleStr!=""){
    stp->comboBoxStyle->setCurrentIndex(stp->comboBoxStyle->findText(styleStr,Qt::MatchFixedString));
    QApplication::setStyle(QStyleFactory::create(styleStr));
    }
    stp->radioButtonProgFC->setText(tool.trlang("Font of the Software"));
    stp->radioButtonQuranFC->setText(tool.trlang("Font of the Quran"));
    stp->radioButtonTrFC->setText(tool.trlang("Font of the Translation"));
    /*stp->check_subject->setChecked(tool.ReadSettings("iconsubject",true).toBool());
    stp->check_Similer->setChecked(tool.ReadSettings("similiarVerse",false).toBool());
    stp->check_audio->setChecked(tool.ReadSettings("iconaudio",true).toBool());
    stp->check_comment->setChecked(tool.ReadSettings("iconcomment",true).toBool());
    stp->check_explanation->setChecked(tool.ReadSettings("iconexplanation",true).toBool());
    stp->check_SameWord->setChecked(tool.ReadSettings("SameWord",false).toBool());
    */
    stp->radio_both->setChecked(tool.ReadSettings("showboth",true).toBool());
    stp->radio_Quran->setChecked(tool.ReadSettings("showquran",false).toBool());
    stp->radio_tr->setChecked(tool.ReadSettings("showtr",false).toBool());
    stp->check_Border->setChecked(tool.ReadSettings("QuranBorder",false).toBool());
    stp->lineEditPathDatabase->setText(tool.ReadSettings("DataBaseDir",tool.pathData).toString());
    stp->lineEditPathAudio->setText(tool.ReadSettings("AudioDir",tool.pathAudio).toString());


    QFont font = tool.ReadSettings("ProgFont",stp->fontprog).value<QFont>();
     QApplication::setFont(font);
     this->setFont(font);
     QList<QWidget*> allWidget =findChildren<QWidget*>();
     foreach(QWidget* wid,allWidget)
         wid->setFont(font);

     connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
     connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(showOptions()));
     connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(AcAbout()));
     connect(ui->actionHelp,SIGNAL(triggered()),this,SLOT(AcHelp()));
     connect(ui->actionLayoutChapter,SIGNAL(triggered()),this,SLOT(showHideLayoutChapter()));

     connectToDataBase();

     QStringList com_LangItems =tool.GetLang(":/language");

     foreach(QString str,com_LangItems)
       stp->comb_lang->addItem(str.split(";").at(0),str.split(";").at(1));


     stp->comb_lang->setCurrentIndex(
                 stp->comb_lang->findData(tool.ReadSettings("lang","English").toString())
                 );


     SetLanguage();

}

void anvar::closeEvent(QCloseEvent *e)
{

    db->insertsql("DELETE FROM Comment WHERE Value='';");
    if (ayeExp->maybeSave())
        e->accept();
    else


        e->ignore();

 }

void anvar::connectToDataBase()
{
    db = new DataBase(tool.pathData+"/Data.db");

    if(db->getDatastr("SELECT count(name) FROM sqlite_master WHERE type='table' ;").toInt()<10)
    {
        QStringList listSql =tool.ReadFromTextFile(":Data.sql");
            db->insertsql(listSql.join("\n"));
    }

    if(db->isOpen()){
            addonsForm=new Addons(db,this);
            addonsForm->setWindowFlags(Qt::Tool);
            if(tool.trlang("Align")=="Right")
                addonsForm->setLayoutDirection(Qt::RightToLeft);
            else
                addonsForm->setLayoutDirection(Qt::LeftToRight);

            addonsForm->Btn_Install_Addons->setText(tool.trlang("Install Add-ons"));
            addonsForm->pushButtonDownload->setText(tool.trlang("Download And Install"));
            addonsForm->setWindowTitle(tool.trlang("Add-ons Page"));
            addonsForm->pushButtonUpdate->setText(tool.trlang("Update Add-ons list"));

           treeModel = new DBModelSubJectQuran(db,this,false,"SubjectName");

            dtree_ = new DbTreeViewSubJectQuran(db,this,false,"SubjectName",treeModel);
            dtree_->tb_find->setVisible(true);
            ui->layout_Subject->addWidget(dtree_);
            ui->layout_Subject->addLayout(dtree_->find);



            quranModel =new QuranModel(db,this);
            quranWidget = new QuranWidget(db);

            QuranTreeView *quranTree =new  QuranTreeView(db,quranModel,"1",this);
            ayeExp = new AyeExplor(db,treeModel,quranTree,this);
            ayeExp->setVisible(false);
            connect(quranWidget,SIGNAL(showRelated(QString,QString)),this,SLOT(showRelated(QString,QString)));
            connect(quranWidget,SIGNAL(openAyeExplor(QString,int)),this,SLOT(openAyeExplor(QString,int)));

            quranWidget->igEvent=false;
            //Quran Translations
            quranWidget->translationList = db->getListData("Select id,Name,TableName From Tr_Translation;");
            stp->comb_Translation->addItems(quranWidget->translationList.at(1));
            if(stp->comb_Translation->count()>0)
            stp->comb_Translation->setCurrentIndex(tool.ReadSettings("translation","0").toInt());
            quranWidget->setTranslation();
            quranWidget->isJuz=false;
             //subjects
            quranWidget->subjectList = db->getListData("Select id,Name From SubjectName;");
             //Program Translations
             quranWidget->QuranRoot = db->getListData("SELECT *FROM quran_root ORDER BY id;");

            //get list audio..
            quranWidget->listAudio = db->getListData("Select Name,dir_Name,Url From Audio ;");
            foreach (QString str,quranWidget->listAudio.at(0))
            stp->combo_Audio->addItem(tool.trlang(str));
            stp->combo_Audio->setCurrentIndex(tool.ReadSettings("Audio",0).toInt());
            SetTree("");
            ui->labelSort->setText(tool.trlang("Sort chapters by:"));
            ui->comboBoxSortBy->addItem(tool.trlang("moshaf"),"moshaf");
            ui->comboBoxSortBy->addItem(tool.trlang("Orginal"),"orginal");
            ui->comboBoxSortBy->addItem(tool.trlang("Name"),"name");
            ui->comboBoxSortBy->addItem(tool.trlang("Maximum verse"),"max");
            ui->comboBoxSortBy->addItem(tool.trlang("Least verse"),"min");
            ui->comboBoxSortBy->setCurrentIndex(0);

            foreach(QString dirstr,quranWidget->listAudio.at(1)){
                QDir dir(tool.pathAudio+dirstr);
                if(!dir.exists())
                    dir.mkdir(tool.pathAudio+dirstr);

            }




            connect(ui->actionAddonsPage,SIGNAL(triggered()),this,SLOT(showAddonsePage()));
            connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(AcSave()));
            connect(ui->actionPrint,SIGNAL(triggered()),this,SLOT(AcPrint()));
            connect(ui->actionSavepdf,SIGNAL(triggered()),this,SLOT(AcPdf()));
            connect(ui->actionBackup, SIGNAL(triggered()), this, SLOT(BackupOfSubjectAndComment()));


            connect(ui->quranTree, SIGNAL(clicked(QModelIndex)), this, SLOT(TreeEventQuran()));
            connect(ui->comboBoxJuz, SIGNAL(activated(int)), this, SLOT(TreeEventJuz()));
            connect(ui->comboBoxHizb, SIGNAL(activated(int)), this, SLOT(TreeEventHizb()));
            connect(dtree_, SIGNAL(activated(QModelIndex)), this, SLOT(SubjectEvent()));
            connect(dtree_, SIGNAL(clicked(QModelIndex)), this, SLOT(SubjectEvent()));

            connect(addonsForm->Btn_Install_Addons, SIGNAL(clicked()), this, SLOT(installAddonsFormFile()));
            connect(addonsForm, SIGNAL(downlodFinishd(QStringList)), this, SLOT(installAddons(QStringList)));


           QString sql ="Select  Quran.id,Arabic,Chapter,Verse"+quranWidget->translationStr+" From Quran ?translation Where Quran.Page = "+tool.ReadSettings("lastChapter","1").toString()+" ;";
           QString chapterId=db->getDatastr("Select Chapter From Quran Where Page ="+tool.ReadSettings("lastChapter","1").toString());
           addTab(
                  tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+chapterId+" ; "))
                 ,sql);



           goToQuranTree(chapterId.toInt());

           addonsForm->label_getaddons->setText("<a href='https://sourceforge.net/projects/al-anvar/files/addons/'>"+tool.trlang("Get Add-ons")+"</a>");
           addonsForm->label_getaddons->setOpenExternalLinks(true);

           connect(this,SIGNAL(findVerse(QString)),this,SLOT(goToVerseFromSearch(QString)));


          // searchWin->setWindowTitle(tool.trlang("Search"));
           ui->radioInQuran->setText(tool.trlang("Search in the Qur'an"));
           ui->radioInTr->setText(tool.trlang("Search in Translation"));
           ui->labelResultQ->setText(tool.trlang("Results")+"  ");
           QuranRoot = db->getListData("SELECT *FROM quran_root ORDER BY id;");
           ui->list_Quran_Root->addItems(QuranRoot.at(1));
           QCompleter *rootCompleter = new QCompleter(QuranRoot.at(1),this);
           ui->line_SearchRoot->setCompleter(rootCompleter);
           ui->labelQuranRootResult->setText(tool.trlang("Results")+"  ");
           connect(ui->tableResultRoot,SIGNAL(clicked(QModelIndex)),this,SLOT(showResultRoot(QModelIndex)));
           connect(ui->list_Quran_Root,SIGNAL(clicked(QModelIndex)), this, SLOT(findRoot()));
           connect(ui->line_SearchRoot,SIGNAL(returnPressed()), this, SLOT(findRoot()));

           connect(ui->line_SearchRoot,SIGNAL(textEdited(QString)), this, SLOT(findRootInList(QString)));

           connect(ui->pushSearchQ,SIGNAL(clicked()),this,SLOT(doSearchQ()));
           connect(ui->lineAndQ,SIGNAL(returnPressed()),this,SLOT(doSearchQ()));
           connect(ui->lineAndAfterQ,SIGNAL(returnPressed()),this,SLOT(doSearchQ()));
           connect(ui->lineNotQ,SIGNAL(returnPressed()),this,SLOT(doSearchQ()));
           connect(ui->tableResultQ,SIGNAL(clicked(QModelIndex)),this,SLOT(showResultQ(QModelIndex)));

          if(tool.trlang("Align")=="Right"){
          ayeExp->setLayoutDirection(Qt::RightToLeft);
         }else{
          ayeExp->setLayoutDirection(Qt::LeftToRight);

           }
          ayeExp->setFont(tool.ReadSettings("ProgFont",stp->fontprog).value<QFont>());
          ayeExp->textComment->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
          ayeExp->textExp->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
          ayeExp->textTr->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
          ayeExp->tabWidget->setTabText(0,tool.trlang("Subject of the Verses"));
          ayeExp->tabWidget->setTabText(1,tool.trlang("Show and Edit the Notes"));
          ayeExp->tabWidget->setTabText(2,tool.trlang("Interpretation"));
          ayeExp->tabWidget->setTabText(3,tool.trlang("Similar Words"));
          ui->textEditShowQ->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
          ui->textEditShowRoot->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());


           ayeExp->exLable->setText(tool.trlang("Select the Interpretation"));
           ayeExp->trLable->setText(tool.trlang("Select the Translation"));
           ayeExp->okButtonSimilar->setText( tool.trlang("Add selected words to unwanted list"));
           ayeExp->label_5->setText( tool.trlang("Subject List:"));
           ayeExp->label_6->setText( tool.trlang("Double Clicked to")+tool.trlang("add subject to Verse"));
           ayeExp->label_3->setText( tool.trlang("Subject of this Verse:"));
           ayeExp->label_4->setText( tool.trlang("Double Clicked to")+"\n"+tool.trlang("remove from Verse"));
           ayeExp->okButtonComment->setText(tool.trlang("Save"));
           ayeExp->toolButtoNext->setText(tool.trlang("Next verse"));
           ayeExp->toolButtonPrevious->setText(tool.trlang("Previous verse"));

}

}
void anvar::SetLanguage()
{


  if(tool.trlang("Align")=="Right"){
 setLayoutDirection(Qt::RightToLeft);
 ui->dockWidget->setLayoutDirection(Qt::RightToLeft);
 stp->setLayoutDirection(Qt::RightToLeft);
 addDockWidget(Qt::RightDockWidgetArea, ui->dockWidget);

}

  stp->setWindowTitle(tool.trlang("Preferences"));
  ui->toolBoxQuran->setItemIcon(0,QIcon(":/images/quran.png"));
  ui->toolBoxQuran->setItemIcon(1,QIcon(":/images/tree.png"));
  //ui->toolBoxQuran->setItemIcon(2,QIcon(":/images/books.png"));
  ui->toolBoxQuran->setItemIcon(2,QIcon(":/images/search.png"));

  ui->actionMoshafView->setText(db->trlang("MoshafView"));

  //ui->tabWidgetList->setTabIcon(0,QIcon(":/images/quran.png"));
 // ui->tabWidgetList->setTabIcon(1,QIcon(":/images/juz.png"));
 // ui->tabWidgetList->setTabText(0,tool.trlang("list of the Chapters"));
  ui->labelJuz->setText(tool.trlang("list of the Section"));
  ui->labelHizb->setText(tool.trlang("list of the Hizb"));

  ui->toolBoxQuran->setItemText(0,tool.trlang("Quran"));
  ui->toolBoxQuran->setItemText(1,tool.trlang("Subjects"));
//  ui->toolBoxQuran->setItemText(2,tool.trlang("Books"));
  ui->toolBoxQuran->setItemText(2,tool.trlang("Search"));



  ui->actionUpdates->setText(tool.trlang("Check for updates"));
  ui->actionHomePage->setText(tool.trlang("Site"));
  ui->actionNewsLetter->setText(tool.trlang("For Subscribe to our Newsletters and Email Updates"));
  stp->label_audio->setText(tool.trlang("Reciter"));
  stp->label_lang->setText(tool.trlang("Language of the Program"));
  stp->label_tr->setText(tool.trlang("Qur'an Translations"));
  /*
  stp->check_audio->setText(tool.trlang("Playing Icon"));
  stp->check_comment->setText(tool.trlang("Remarking Icon"));
  stp->check_explanation->setText(tool.trlang("Interpretation"));
  stp->check_subject->setText(tool.trlang("Subjects"));
  stp->check_Similer->setText(tool.trlang("Similar Verses"));
  stp->check_SameWord->setText(tool.trlang("Similar Words"));
  stp->label_icons->setText(tool.trlang("Tools"));


  */
  stp->label_value->setText(tool.trlang("Show the Verses"));
  stp->radio_Quran->setText(tool.trlang("Qur'an Only"));
  stp->radio_tr->setText(tool.trlang("Translation Only"));
  stp->pushButtonSet->setText(tool.trlang("Ok"));
  stp->pushButtonCancel->setText(tool.trlang("Cancel"));
  stp->radio_both->setText(tool.trlang("Both"));
  stp->Btn_ChangeFont->setText(tool.trlang("Change Fonts"));
  stp->Btn_ChangeColor->setText(tool.trlang("Change Color"));
  ui->menuFile->setTitle(tool.trlang("File"));
  ui->menuHelp->setTitle(tool.trlang("Help"));
  ui->menuTools->setTitle(tool.trlang("Tools"));
  ui->actionAbout->setText(tool.trlang("About"));
  ui->actionAddonsPage->setText(tool.trlang("Add-ons Page"));
  ui->menuAdonse->setTitle(tool.trlang("Add-ons"));
  ui->actionExit->setText(tool.trlang("Exit"));
  ui->actionHelp->setText(tool.trlang("Help"));
  ui->actionOptions->setText(tool.trlang("Options.."));
  ui->actionPrint->setText(tool.trlang("Print"));
  ui->actionSave->setText(tool.trlang("Save as.."));
  ui->actionSavepdf->setText(tool.trlang("Save as pdf"));
  ui->actionBackup->setText(tool.trlang("Backup Subjects and Notes"));
  ui->actionLayoutChapter->setText(tool.trlang("Hide chapter layout"));
  stp->check_Border->setText(tool.trlang("Quran Border"));
  stp->labelStyle->setText(tool.trlang("Style"));
  stp->labelStyleSheet->setText(tool.trlang("Theme and color"));
  stp->labelPathDatabase->setText(tool.trlang("DataBase Dir"));
  stp->labelPathAudio->setText(tool.trlang("Audio Dir"));

  stp->radioButtonExterna->setText(tool.trlang("Use external program:"));
  stp->radioButtonPhonon->setText(tool.trlang("Play via Phonon"));
  stp->labelPlayer->setText(tool.trlang("Playback"));
  stp->tabWidget->setTabText(0,tool.trlang("interface"));
  stp->tabWidget->setTabText(1,tool.trlang("audio"));
  stp->tabWidget->setTabText(2,tool.trlang("Quran setting"));

  ui->groupBoxAdQ->setTitle(tool.trlang("Advanced Search"));
  //ui->checkBoxErabQ->setText(tool.trlang("Match the e'rab"));
  ui->checkBoxHamziQ->setText(tool.trlang("Match Alifhamza"));
  ui->checkBoxWholeQ->setText(tool.trlang("Match Case"));
  ui->pushSearchQ->setText(tool.trlang("Search"));
  ui->labelAnd->setText(tool.trlang("all these words"));
  ui->labelAndAfter->setText(tool.trlang("all these words(sequence)"));
  ui->labelNot->setText(tool.trlang("none of these words"));
  ui->labelCom->setText(tool.trlang("for split words use / or ,"));
  ui->tabWidgetSearch->setTabText(0,tool.trlang("Advanced Search"));
  ui->tabWidgetSearch->setTabText(1,tool.trlang("Quran Root"));

  //ui->radioButtonBooks->setText(tool.trlang("Books Search"));


  //ui->dockWidget->setGeometry(ui->dockWidget->x(),ui->dockWidget->y(),350,ui->dockWidget->height());


}
void anvar::doSearchQ()
{
//expr1,tr,Quran.Id
    QString searchQuery=" SELECT (  Chapter ||  \";\" || Verse ) AS expr1,Arabic,Id From Quran WHERE  Arabic REGEXP '%1' ;";
    serPat.highlightPattern.clear();
    serPat.setOptions(false,ui->checkBoxHamziQ->isChecked(),ui->checkBoxWholeQ->isChecked());


    if(ui->radioInTr->isChecked())
       {
        searchQuery = "SELECT (  Chapter ||  \";\" || Verse ) AS expr1,tr,Quran.Id  From Quran Left Join "+quranWidget->strTranslation+" On ("+quranWidget->strTranslation+".id=quran.id) WHERE tr REGEXP '%1'  ;";
        ui->tableResultQ->setFont(tool.ReadSettings("trfont",QFont()).value<QFont>());
         ui->textEditShowQ->setFont(tool.ReadSettings("trfont",QFont()).value<QFont>());
    }else
    {
         ui->tableResultQ->setFont(tool.ReadSettings("ProgFont",QFont()).value<QFont>());
         ui->textEditShowQ->setFont(tool.ReadSettings("ProgFont",QFont()).value<QFont>());
      }

      addTab(tool.trlang("Advanced Search"),"",0,"adSearch");
      QString searchPattern =serPat.getSearchPattern("",""
                                                            ,serPat.getAnd(ui->lineAndQ->text())
                                                            ,serPat.getAndAfter(ui->lineAndAfterQ->text()));

      QStandardItemModel *mainItem =db->getMode(searchQuery.arg(searchPattern));

      //serPat.getUnWanted(ui->lineNotQ->text())
      QStringList title;
      qDebug()<<searchPattern;
     title << tool.trlang("Address") << tool.trlang("text") ;
     mainItem->setHorizontalHeaderLabels(title);
     QSortFilterProxyModel *mainItemF = new QSortFilterProxyModel;
     mainItemF->setSourceModel(mainItem);
     mainItemF->setFilterKeyColumn(1);
     mainItemF->setFilterRegExp(serPat.getUnWanted(ui->lineNotQ->text()));
     ui->labelResultQ->setText(tool.trlang("Results")+" "+QString::number(mainItemF->rowCount()));

     ui->tableResultQ->setModel(mainItemF);


     ui->tableResultQ->setColumnWidth(0,100);
     ui->tableResultQ->setColumnHidden(2,true);

     int wi = ui->tableResultQ->columnWidth(0);
     if(ui->tableResultQ->verticalScrollBar()->isVisible())
         wi += ui->tableResultQ->verticalScrollBar()->width();
     wi +=ui->tableResultQ->verticalHeader()->width();

      wi+=6;
        if(fristSearchQ)
           wi+=17;
     fristSearchQ=false;

     wi =ui->tableResultQ->width()-wi;
     ui->tableResultQ->setColumnWidth(1,wi);
     if(ui->tableResultQ->model()->rowCount()>0)
     {
     ui->tableResultQ->setCurrentIndex(ui->tableResultQ->model()->index(0,0));
     showResultQ(ui->tableResultQ->model()->index(0,0));
     }


}

void anvar::showResultQ(const QModelIndex &index)
{


    ui->textEditShowQ->setText(ui->tableResultQ->model()->data(ui->tableResultQ->model()->index(index.row(),1)).toString());

    if(serPat.highlightPattern.size()>0)
    new Highlighter(serPat.highlightPattern,ui->textEditShowQ->document());

}

void anvar::showResultRoot(const QModelIndex &index)
{


   QString chapter =ui->tableResultRoot->model()->data(ui->tableResultRoot->model()->index(index.row(),0)).toString();
   QString verse = ui->tableResultRoot->model()->data(ui->tableResultRoot->model()->index(index.row(),1)).toString();

    QString text = db->getDatastr("Select Arabic From Quran Where Chapter ="+chapter+" And Verse ="+verse+";");
    ui->textEditShowRoot->setText(text);
    QStringList highlightPattern,wordList;
    text.remove(QString::fromUtf8("ۛ"));
        text.remove(QString::fromUtf8("ۖ"));
        text.remove(QString::fromUtf8("ۗ"));
        text.remove(QString::fromUtf8("ۚ"));
        text.remove(QString::fromUtf8("ۙ"));
        text.remove(QString::fromUtf8("ۘ"));
        text.remove(QString::fromUtf8("۩"));

    wordList=text.split(" ",QString::SkipEmptyParts);
    QStringList counttList= ui->tableResultRoot->model()->data(ui->tableResultRoot->model()->index(index.row(),3)).toString().split(",");

    foreach(QString countStr,counttList)
    {
      int highlighInt =countStr.toInt()-1;
      if(wordList.size()-1>=highlighInt)
          highlightPattern.append(wordList.at(highlighInt));
    }

    if(highlightPattern.size()>0)
       new Highlighter(highlightPattern,ui->textEditShowRoot->document());
}

void anvar::findRootInList( QString str)
{
    str.replace(QString::fromUtf8("ی"),QString::fromUtf8("ي"));
    str.replace( QString::fromUtf8("ک"),QString::fromUtf8("ك"));
  QList<QListWidgetItem *>items= ui->list_Quran_Root->findItems(str,Qt::MatchContains | Qt::MatchRecursive);
  if(items.count()>0)
  ui->list_Quran_Root->setCurrentItem(items.at(0));

}
void anvar::findRoot()
{
    addTab(tool.trlang("Quran Root"),"",0,"rootSerach");
    ui->tableResultRoot->setFont(tool.ReadSettings("Progfont",QFont()).value<QFont>());
    ui->textEditShowRoot->setFont(tool.ReadSettings("Progfont",QFont()).value<QFont>());
    int currentRow= ui->list_Quran_Root->currentRow();
    QString root =QuranRoot.at(3).at(currentRow);
    QStringList list = root.split(";");
    QStandardItem *tmpa ,*tmpc,*tmpv, *tmph;
    QStandardItemModel *mainItem = new QStandardItemModel;
       foreach(QString str,list)
       {
            QString id = str.split(":").at(0) ;
             tmpa = new  QStandardItem(db->getDatastr("Select  Arabic From Quran   Where Quran.id ="+QString::number(id.toInt()+1)));
             tmpc = new  QStandardItem(db->getDatastr("Select  Chapter From Quran  Where Quran.id ="+QString::number(id.toInt()+1)));
             tmpv = new  QStandardItem(db->getDatastr("Select  Verse From Quran  Where Quran.id ="+QString::number(id.toInt()+1)));
             tmph = new  QStandardItem(str.split(":").at(1));
             QList<QStandardItem*> items;
            items.append(tmpc);
            items.append(tmpv);
            items.append(tmpa);
            items.append(tmph);
            mainItem->appendRow(items);
       }
          QStringList title;
          title << db->trlang("Chapter") <<  db->trlang("Verse") << db->trlang("text")  << db->trlang("") ;
          ui->labelQuranRootResult->setText(db->trlang("Results")+QString::number(mainItem->rowCount()));

          mainItem->setHorizontalHeaderLabels(title);

                  ui->tableResultRoot->setModel(mainItem);
                  ui->tableResultRoot->setColumnWidth(0,35);
                  ui->tableResultRoot->setColumnWidth(1,35);
                  int wi =ui->tableResultRoot->columnWidth(0)+ui->tableResultRoot->columnWidth(1);
                  wi+=10;
                    if(fristSearchRoot)
                       wi+=17;
                 fristSearchRoot=false;
                  if(ui->tableResultRoot->verticalScrollBar()->isVisible())
                      wi +=ui->tableResultRoot->verticalScrollBar()->width();
                  wi += ui->tableResultRoot->verticalScrollBar()->width();
                  wi =ui->tableResultRoot->width()-wi;
                 ui->tableResultRoot->setColumnWidth(2,wi);
             //    ui->tableResultRoot->setColumnHidden(3,true);

                 if(ui->tableResultRoot->model()->rowCount()>0)
                 {
                 ui->tableResultRoot->setCurrentIndex(ui->tableResultRoot->model()->index(0,0));
                 showResultRoot(ui->tableResultRoot->model()->index(0,0));
                 }

}

void anvar::on_tableResultQ_doubleClicked(const QModelIndex &index)
{


    emit findVerse(ui->tableResultQ->model()->data(ui->tableResultQ->model()->index(index.row(),0)).toString());
}


void anvar::SetTree(QString sortBy)
{
    ui->quranTree->clear();

    ui->comboBoxJuz->clear();
    ui->comboBoxHizb->clear();
    ui->quranTree->setColumnCount(5);
    QString sqlStute="Select descent,id,Verse,Nozol From ChapterProperty %1 ";
    if(sortBy=="max")
        sqlStute=sqlStute.arg("ORDER BY Verse DESC;");
    else if(sortBy=="min")
        sqlStute=sqlStute.arg("ORDER BY Verse ASC;");
    else if(sortBy=="orginal")
        sqlStute=sqlStute.arg("ORDER BY revel ASC;");
    else
        sqlStute=sqlStute.arg(";");

    QList<QStringList> chapter_List = db->getListData(sqlStute);
    QList<QStringList> JuzList = db->getListData("Select Name,id From Juz;");
    QList<QStringList> HizbList = db->getListData("Select Name,id From Hizb;");

   for (int i=0;i<chapter_List.at(0).count();i++)
    {
       QTreeWidgetItem *Node = new  QTreeWidgetItem(ui->quranTree);

       Node->setText(0,chapter_List.at(1).at(i)+"- "+tool.trlang(chapter_List.at(0).at(i)));
       Node->setIcon(0,QIcon(":/images/sorah.png"));
       Node->setText(1,chapter_List.at(1).at(i));   
       Node->setText(2,tool.trlang(chapter_List.at(0).at(i)));
       Node->setText(3,chapter_List.at(2).at(i));
       Node->setText(4,tool.trlang(chapter_List.at(3).at(i)));

   }

    for (int i=0;i<30;i++)
    {
       QString str =JuzList.at(0).at(i);
       str.replace("Section",tool.trlang("Section"));
       ui->comboBoxJuz->addItem(QIcon(":/images/juz.png"),str,JuzList.at(1).at(i));

     }

   for (int i=0;i<120;i++)
   {
               QString str =HizbList.at(0).at(i);
               str.replace("Hizb",tool.trlang("Hizb"));
               ui->comboBoxHizb->addItem(QIcon(":/images/hizb.png"),str,HizbList.at(1).at(i));
   }


   if(sortBy=="name")
   ui->quranTree->sortItems(2,Qt::AscendingOrder);

   ui->quranTree->setHeaderHidden(false);
   ui->quranTree->setColumnHidden(1,true);
   ui->quranTree->setColumnHidden(2,true);
   ui->quranTree->setColumnWidth(0,120);
   ui->quranTree->setColumnWidth(3,50);
   ui->quranTree->setColumnWidth(4,50);
   QStringList labels ;
   labels  <<tool.trlang("Name") << ""<< ""<< tool.trlang("Verses")<<tool.trlang("Descent");
   ui->quranTree->setHeaderLabels(labels);

}
void anvar::showRelated(QString name, QString sql)
{
    addTab(tool.trlang("Subject :")+" "+name,sql,0,"subject");

}
void anvar::openAyeExplor(QString currentBlock,int i)
{
           ayeExp->Id=currentBlock;
           ayeExp->tabWidget->setCurrentIndex(i);
           ayeExp->treeAye->setCurrent(currentBlock);
           ayeExp->showAll();
           ayeExp->setVisible(true);
           addTab(tool.trlang("Research tools"),"",0,"exp");
}

void anvar::on_tabWidgetQuran_tabCloseRequested(int index)
{
    if(ui->tabWidgetQuran->count()==1)
        return;
    if(ui->tabWidgetQuran->tabText(index)==tool.trlang("Research tools"))
    {
      if(!ayeExp->maybeSave())
          return;
    }
        ui->tabWidgetQuran->removeTab(index);
        tabListTitle.removeAt(index);

      if(ui->tabWidgetQuran->count()==1)
        ui->tabWidgetQuran->setTabsClosable(false);


}
void anvar::addTab(QString title,QString sql,int verse,QString type)
{
    QIcon tabIcon=QIcon(":/images/search.png");

    if(type=="subject")
      tabIcon=QIcon(":/images/tree.png");
    else if(type=="chapter")
        tabIcon=QIcon(":/images/quran.png");

    title.replace("\n"," ");
    title.replace("\t"," ");
    title =title.mid(0,70);

    if(!tabListTitle.contains(title)){
        tabListTitle.append(title);

        if(type=="moshaf")
        {
            tabIcon=QIcon(":/images/quran.png");
            MoshafWidget *mos = new MoshafWidget(db,0);
            connect(mos,SIGNAL(showRelated(QString,QString)),this,SLOT(showRelated(QString,QString)));
            connect(mos,SIGNAL(openAyeExplor(QString,int)),this,SLOT(openAyeExplor(QString,int)));

            ui->tabWidgetQuran->addTab(mos,tabIcon,title);
            if(ui->tabWidgetQuran->count()>1)
            ui->tabWidgetQuran->setTabsClosable(true);
            ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);

        }/* else if(type=="book")
          {
             tabIcon=QIcon(":/images/Explanations.png");
             Library *lib = new Library(db,0);
             ui->tabWidgetQuran->addTab(lib,tabIcon,title);
             if(ui->tabWidgetQuran->count()>1)
               ui->tabWidgetQuran->setTabsClosable(true);
             ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);

             lib->bookId =  sql.split("<Page>").at(0);
             QStringList pages =sql.split("<Page>").at(1).split("|");
             lib->getTextFromQuerty(pages);


        }*/ else if(type=="exp")
        {
            tabIcon=QIcon(":/images/Explanations.png");
            ui->tabWidgetQuran->addTab(ayeExp,tabIcon,title);
            if(ui->tabWidgetQuran->count()>1)
            ui->tabWidgetQuran->setTabsClosable(true);
            ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);

        }else if(type=="rootSerach"){
             ui->tabWidgetQuran->addTab(ui->tabSearchRoot,tabIcon,title);
             if(ui->tabWidgetQuran->count()>1)
             ui->tabWidgetQuran->setTabsClosable(true);
             ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);


        }else if(type=="adSearch"){
          ui->tabWidgetQuran->addTab(ui->tabAdSearch,tabIcon,title);
        if(ui->tabWidgetQuran->count()>1)
        ui->tabWidgetQuran->setTabsClosable(true);
        ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);
        }else{


    quranWidget = new QuranWidget(db);
    connect(quranWidget,SIGNAL(showRelated(QString,QString)),this,SLOT(showRelated(QString,QString)));
    connect(quranWidget,SIGNAL(openAyeExplor(QString,int)),this,SLOT(openAyeExplor(QString,int)));


   ui->tabWidgetQuran->addTab(quranWidget,tabIcon,title);
   if(ui->tabWidgetQuran->count()>1)
   ui->tabWidgetQuran->setTabsClosable(true);
   ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);
   bool bsm =true;
   bool check_Border=tool.ReadSettings("QuranBorder",false).toBool();
            if(check_Border && quranWidget->listWidget->width()>500){
                tool.makeBorder(quranWidget->listWidget,quranWidget->vLayout);
            }

   if(type=="subject")
   {
   //quranWidget->scrollArea->setVisible(false);
       quranWidget->line_Chapter->setVisible(false);
       quranWidget->line_Verse->setVisible(false);
       quranWidget->line_Page->setVisible(false);
       quranWidget->label_Hizb->setVisible(false);
       quranWidget->label_Juz->setVisible(false);
       quranWidget->lableChapter->setVisible(false);
       quranWidget->lableVerse->setVisible(false);
       quranWidget->lablePage->setVisible(false);
       quranWidget->lableHizb->setVisible(false);
       quranWidget->lableJuz->setVisible(false);

   bsm=false;
  }


   if(type=="chapter")
         connect(quranWidget->line_Chapter, SIGNAL(valueChanged(int)), this, SLOT(goToQuranTree(int)));

        quranWidget->getQuranValue(sql,bsm);
      if(verse!=0){
          quranWidget->line_Verse->setValue(verse);
          quranWidget->VerseChanged();
      }
       }}else{
        /*
        if(type=="book")
        {
            ui->tabWidgetQuran->setCurrentIndex(tabListTitle.indexOf(title));

            Library *lib = qobject_cast<Library*>(ui->tabWidgetQuran->widget(tabListTitle.indexOf(title)));
            lib->bookId =  sql.split("<Page>").at(0);
            QStringList pages =sql.split("<Page>").at(1).split("|");
            lib->getTextFromQuerty(pages);

        }else*/
           ui->tabWidgetQuran->setCurrentIndex(tabListTitle.indexOf(title));
    }


}
void anvar::goToVerseFromSearch(QString str)
{

     QString title =str;
     title=tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+str.split(";").at(0)))+title.replace(";",":");
     QString sql ="Select  Quran.id,Arabic,Chapter,Verse"+quranWidget->translationStr+" From Quran ?translation Where Quran.Chapter = "+str.split(";").at(0)+" ;";
     addTab(tool.trlang("Search Result :")+title,sql,str.split(";").at(1).toUInt(),"search");
     quranWidget->finder(str.split(";").at(0),str.split(";").at(1));
}
void anvar::TreeEventQuran()
{

    if(ui->quranTree->currentItem())
    {
        QString sql ="Select  Quran.id,Arabic,Chapter,Verse"+quranWidget->translationStr+" From Quran ?translation Where Quran.Chapter = "+ui->quranTree->currentItem()->text(1)+" ;";
        addTab(tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+ui->quranTree->currentItem()->text(1)+" ; ")),sql);

    }
}
void anvar::TreeEventJuz()
{
     if(ui->comboBoxJuz->currentIndex())
     {
        addTab("","Select  Quran.id,Arabic,Chapter,Verse"+quranWidget->translationStr+" From Quran ?translation Where Quran.Chapter = 1 ;");
        quranWidget->showJuz(ui->comboBoxJuz->itemData(ui->comboBoxJuz->currentIndex()).toString());

     }

}

void anvar::TreeEventHizb()
{
    if(ui->comboBoxHizb->currentIndex())
    {

        addTab("","Select  Quran.id,Arabic,Chapter,Verse"+quranWidget->translationStr+" From Quran ?translation Where Quran.Chapter = 1 ;");
        quranWidget->showHizb(ui->comboBoxHizb->itemData(ui->comboBoxHizb->currentIndex()).toString());

    }

}



void anvar::SubjectEvent(){
    if(dtree_->getCurrentText(4,dtree_->currentIndex())=="0")
        return;

    if(dtree_->getCurrentText(4,dtree_->currentIndex())=="1"){
      if(db->getDatastr("Select Count(id) From Subjects Where ID_SUBJECT ="+dtree_->getCurrentText(1,dtree_->currentIndex())+"; ")=="0")
          return;
       QString sql = "Select Quran.id,Arabic,Quran.Chapter,Quran.Verse"+quranWidget->translationStr+" From Subjects Left Join quran On (Subjects.ID_VERSE=quran.id) ?translation left join ChapterProperty on (quran.Chapter=ChapterProperty.Id)  Where Subjects.ID_SUBJECT ="+dtree_->getCurrentText(1,dtree_->currentIndex())+";";


    addTab(tool.trlang("Subject :")+" "+dtree_->getCurrentText(0,dtree_->currentIndex()),sql,0,"subject");
    }
}
void anvar::finder(QString searchString)
{
    QString str ="["+db->getDatastr("select chapter from quran where id ="+searchString)+":"+db->getDatastr("select Verse from quran where id ="+searchString)+"]";
    quranWidget->finder(str);

}

void anvar::showHideLayoutChapter()
{
  ui->dockWidget->setVisible(!ui->dockWidget->isVisible());
    if(ui->dockWidget->isVisible()){
        ui->actionLayoutChapter->setIcon(QIcon(":/images/layoutdelete.png"));
        ui->actionLayoutChapter->setText(tool.trlang("Hide chapter layout"));
   }else{
        ui->actionLayoutChapter->setIcon(QIcon(":/images/layoutadd.png"));
        ui->actionLayoutChapter->setText(tool.trlang("Show chapter layout"));

    }

}

void anvar::showOptions(){

    stp->mostRestart=false;
   if(stp->exec())
   {
       tool.WriteSettings("translation",stp->comb_Translation->currentIndex());
       /*
       tool.WriteSettings("iconsubject",stp->check_subject->isChecked());
       tool.WriteSettings("similiarVerse",stp->check_Similer->isChecked());
       tool.WriteSettings("iconaudio",stp->check_audio->isChecked());
       tool.WriteSettings("iconcomment",stp->check_comment->isChecked());
       tool.WriteSettings("iconexplanation",stp->check_explanation->isChecked());
       tool.WriteSettings("SameWord",stp->check_SameWord->isChecked());

       */
       tool.WriteSettings("showboth",stp->radio_both->isChecked());
       tool.WriteSettings("showquran",stp->radio_Quran->isChecked());
       tool.WriteSettings("showtr",stp->radio_tr->isChecked());
       tool.WriteSettings("QuranBorder",stp->check_Border->isChecked());

       tool.WriteSettings("Quranfont",stp->fontchapter.toString());
       tool.WriteSettings("QuranColor",stp->colorQ.currentColor().name());
       tool.WriteSettings("bookfont",stp->fontBook.toString());
       tool.WriteSettings("bookColor",stp->colorBook.currentColor().name());
       tool.WriteSettings("trColor",stp->colorTr.currentColor().name());
       tool.WriteSettings("trfont",stp->fontchapterTr.toString());
       tool.WriteSettings("ProgFont",stp->fontprog.toString());
       tool.WriteSettings("Style",stp->comboBoxStyle->currentText());
       tool.WriteSettings("StyleSheet",stp->comboBoxStyleSheet->currentIndex());
       tool.WriteSettings("Audio",stp->combo_Audio->currentIndex());
       tool.WriteSettings("lang",stp->comb_lang->itemData(stp->comb_lang->currentIndex()).toString());
       tool.WriteSettings("UseExterna",stp->radioButtonExterna->isChecked());
       tool.WriteSettings("UsePhonon",stp->radioButtonPhonon->isChecked());
       tool.WriteSettings("ExternalPlayer",stp->lineEditExternalPlayer->text());


       QFile databaseFileName(stp->lineEditPathDatabase->text()+"/Data.db");
         if(databaseFileName.exists())
            tool.WriteSettings("DataBaseDir",stp->lineEditPathDatabase->text());
         else if(QFile::copy(tool.pathData+"/Data.db",stp->lineEditPathDatabase->text()+"/Data.db"))
            tool.WriteSettings("DataBaseDir",stp->lineEditPathDatabase->text());
         else
            tool.WriteSettings("DataBaseDir",tool.pathData);

         tool.WriteSettings("AudioDir",stp->lineEditPathAudio->text());



         stp->comb_Translation->setCurrentIndex(tool.ReadSettings("translation","0").toInt());
             quranWidget->setTranslation();
             QStringList styeList;

             styeList << "default" <<"simple" <<"Pro";
           QString StyleProg;
             foreach(QString str,tool.ReadFromTextFile(":Styles/"+styeList.at(tool.ReadSettings("StyleSheet",0).toInt())+".css"))
               StyleProg +=str;
           setStyleSheet(StyleProg);
           QString styleStr = tool.ReadSettings("Style",QApplication::style()->objectName()).toString();
             if(styleStr!="")
             QApplication::setStyle(QStyleFactory::create(styleStr));
             QFont font = tool.ReadSettings("ProgFont",stp->fontprog).value<QFont>();
              QApplication::setFont(font);
              this->setFont(font);
              QList<QWidget*> allWidget =findChildren<QWidget*>();
              foreach(QWidget* wid,allWidget)
                  wid->setFont(font);
        stp->combo_Audio->setCurrentIndex(tool.ReadSettings("Audio",0).toInt());

       // tool.WriteSettings("frist",false);
       if(stp->mostRestart)
             restartApp();
   }
}
void anvar::AcAbout(){

   QString text = "<b>"+tool.trlang("Version")+" 0.5.0</b><br>"
           +checkLatestVersion()+"<br>"
           +tool.trlang("Abouttxt")+
           "<br>"+tool.trlang("Email")+": <a href='mailto:etrate14@gmail.com'>etrate14@gmail.com</a>";
     QMessageBox::about(this, tool.trlang("About"),text);


}
QString anvar::checkLatestVersion()
{
    QString updateReply="";
    QFile file(tool.pathUser+"/latestversion.txt");
    if(file.exists())
        file.remove();

    QNetworkAccessManager *m_NetworkMngr = new QNetworkAccessManager;
    QNetworkReply *reply= m_NetworkMngr->get(QNetworkRequest(QUrl("http://al-anvar.sourceforge.net/addons/latestversion.txt")));
    QEventLoop loop;
    connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    if(reply->error())
        file.remove();

    delete reply;
    file.close();
 if(file.exists()){
    QString latestversion = tool.ReadFromTextFile(tool.pathUser+"/latestversion.txt").join("\n").trimmed();
     file.remove();
   if ( QApplication::applicationVersion().toFloat() <  latestversion.toFloat())
        updateReply =  tool.trlang("Latest version is avalible")+" "+
                                "<a href='https://sourceforge.net/projects/al-anvar/files/latest/download'>"
                                +tool.trlang("click here to download Latest version")+"</a>";
   else
        updateReply = tool.trlang("Al-Anvar is up to date ;");
}else
     updateReply = tool.trlang("check update failed");


return  updateReply ;


}
void anvar::on_actionHomePage_triggered()
{
    QDesktopServices::openUrl( QUrl( "http://al-anvar.com" ) );

}

void anvar::on_actionNewsLetter_triggered()
{
    QDesktopServices::openUrl( QUrl( "http://feedburner.google.com/fb/a/mailverify?uri=sourceforge/eozG&loc=en_US/" ) );

}

void anvar::AcHelp(){
    /*
    QString docfile = tool.pathUser+"/language/help/al-anvar-manual.English.html";
    QFile file(tool.pathUser+"/language/help/al-anvar-manual."+tool.ReadSettings("lang","English").toString()+".html");
    if(file.exists())
        docfile =tool.pathUser+"/language/help/al-anvar-manual."+tool.ReadSettings("lang","English").toString()+".html";

              helpWindow =new HelpWindow(docfile, 0);
              helpWindow->setWindowFlags(Qt::Tool);
              helpWindow->raise();
              helpWindow->show();
              */
    ContactForm coFor;
    coFor.exec();



}
void anvar::AcSave(){


    QString fn = QFileDialog::getSaveFileName(this, tool.trlang("Save as.."),
    //                                          QDesktopServices::storageLocation(QDesktopServices::HomeLocation), tr("HTML-Files (*.htm *.html);;ODF files (*.odt);;All Files (*)"));
                                                QStandardPaths::writableLocation(QStandardPaths::HomeLocation), tr("HTML-Files (*.htm *.html);;ODF files (*.odt);;All Files (*)"));   
    if (fn.isEmpty())
        return ;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
        fn += ".html"; // default

    QTextDocumentWriter writer(fn);
    QTextEdit *Save = new QTextEdit;
    QString SaveText;

    for(int i=0;i<listWidgetPrint->count();i++)
        SaveText+=listWidgetPrint->item(i)->text()+"\n";

    Save->setText(SaveText);
    writer.write(Save->document());



}
void anvar::AcPrint(){

    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    dlg->setWindowTitle(tool.trlang("Print"));
    if (dlg->exec() == QDialog::Accepted) {
        QTextEdit *Save = new QTextEdit;
        //*Save = new QTextEdit(this);
        QTextEdit *textEdit = new QTextEdit(this);
        QString SaveText;
        for(int i=0;i<listWidgetPrint->count();i++)
            SaveText+=listWidgetPrint->item(i)->text()+"\n";

         Save->setText(SaveText);
         Save->print(&printer);

    }
    delete dlg;


}
void anvar::AcPdf(){

    QString fileName = QFileDialog::getSaveFileName(this, tool.trlang("Save as pdf"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        QTextEdit *Save = new QTextEdit;
        QString SaveText;
        for(int i=0;i<listWidgetPrint->count();i++)
            SaveText+=listWidgetPrint->item(i)->text()+"\n";

         Save->setText(SaveText);
        Save->document()->print(&printer);
    }


}
void anvar::installAddons(QStringList fileNames)
{

 if (fileNames.isEmpty()) return;
 QFileInfo fi(fileNames.at(0));
 tool.WriteSettings("lastDir",fi.absolutePath());

 bool isBook_=false;
 foreach(QString OpenedData,fileNames){

     QFileInfo fi(OpenedData);

     QString fileName ="\n"+tool.trlang("File Name")+" : "+fi.fileName();
 QFile file(tool.pathData+"/temp.db");
 if(file.exists())
     file.remove();

 tool.UnZip(OpenedData,tool.pathData+"/temp.db");


 db = new DataBase(tool.pathData+"/temp.db");
  bool istranslation=false;
  bool isExplanation=false;
  bool isBackup=false;
  bool isBook=false;
  bool isanr = true;
  if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='Tr_Translation' ;")=="Tr_Translation")
      istranslation =true;
  else   if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='Explanation_Name' ;")=="Explanation_Name")
      isExplanation =true;
  else   if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='Comment' ;")=="Comment")
      isBackup =true;
  else   if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='makebook' ;")=="makebook"){
      isBook =true;
      isBook_=true;
  }


  else
      isanr = false;

  if (!isanr){
      QMessageBox::warning(0,tool.trlang("Install Add-ons"), tool.trlang("This File is not Recognized as an add-on for the program")+fileName, QMessageBox::Ok);
      db = new DataBase( tool.pathData+"/Data.db");
  }

  //install translation
  else if(istranslation)
    {
        QString tableName,tabllSql;
        QList<QStringList> tr_Translation,translation;
          tr_Translation = db->getListData("SELECT  *FROM Tr_Translation");
          tableName = tr_Translation.at(2).at(0);
            tabllSql  = db->getDatastr("SELECT sql FROM sqlite_master where name='"+tableName+"' ;")+";";
            translation = db->getListData("SELECT  * FROM "+tableName+" ;");
            db = new DataBase(tool.pathData+"/Data.db");
              if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='"+tableName+"' ;")==tableName)
                      QMessageBox::warning(0,tool.trlang("Install Add-ons"), tool.trlang("This Translaton has already been Installed")+fileName, QMessageBox::Ok);
              else{
                   db->insertsql("BEGIN TRANSACTION ;");
                   db->insertsql("INSERT INTO Tr_Translation VALUES("+tr_Translation.at(0).at(0)+",'"+tr_Translation.at(1).at(0)+"','"+tr_Translation.at(2).at(0)+"');");
                   db->insertsql(tabllSql);
                     for (int i=0;i<translation.at(0).count();i++){
                          QString str = translation.at(1).at(i);
                          str.replace("'","''");
                          db->insertsql("INSERT INTO "+tableName+" VALUES("+translation.at(0).at(i)+",'"+str+"');");
                       }
                    db->insertsql("COMMIT;");
                    stp->comb_Translation->clear();
                    quranWidget->translationList = db->getListData("Select id,Name,TableName From Tr_Translation;");
                    stp->comb_Translation->addItems(quranWidget->translationList.at(1));
                    stp->comb_Translation->setCurrentIndex(quranWidget->translationList.at(0).count()-1);

                    QMessageBox::information(0,tool.trlang("Install Add-ons"), tool.trlang("Qur'an Translation was Installed Successfully")+fileName, QMessageBox::Ok);
                }
//install Explanation
    }else if(isExplanation){
      QList<QStringList> Explanation_Name,Explanation;
      Explanation_Name = db->getListData("SELECT  *FROM Explanation_Name");
      Explanation = db->getListData("SELECT  * FROM Explanations ;");
      db = new DataBase(tool.pathData+"/Data.db");
    if (db->getDatastr("SELECT count(id) FROM Explanation_Name where id ="+Explanation_Name.at(0).at(0)+" ;")!="0")
        QMessageBox::warning(0,tool.trlang("Install Add-ons"), tool.trlang("This Interpretation has already been Installed")+fileName, QMessageBox::Ok);
    else{
        db->insertsql("BEGIN TRANSACTION ;");
        db->insertsql("INSERT INTO Explanation_Name VALUES("+Explanation_Name.at(0).at(0)+",'"+Explanation_Name.at(1).at(0)+"');");
          for (int i=0;i<Explanation.at(0).count();i++)
             db->insertsql("INSERT INTO Explanations VALUES("+Explanation.at(0).at(i)+",'"+Explanation.at(1).at(i)+"','"+Explanation.at(2).at(i)+"');");
        db->insertsql("COMMIT;");
        QMessageBox::information(0,tool.trlang("Install Add-ons"), tool.trlang("This Interpretation was Installed Successfully")+fileName, QMessageBox::Ok);
    }
  }
  else if(isBackup){
      QStringList tables;
      tables <<"Comment" <<"SubjectName" <<"Subjects" <<"unwantedWords" ;
      QStringList sqlDrop,sqlInsert;
      sqlInsert = db->backup(tables);
      foreach(QString table,tables)
            sqlDrop <<"DROP TABLE "+table;

    db = new DataBase(tool.pathData+"/Data.db");
    QMessageBox::StandardButton sb;
    sb = QMessageBox::warning(this, tool.trlang("Warning"),tool.trlang("Are you sure to Restore  Backup ?")+"\n"+tool.trlang("all Current Subject and Comment will be lost..")+fileName,
                               QMessageBox::Yes | QMessageBox::No);
    if (sb == QMessageBox::No)
        return;

    db->insertsqls(sqlDrop);
    db->insertsqls(sqlInsert,false);
    QMessageBox::information(0,tool.trlang("Install Add-ons"), tool.trlang("This Backup was Installed Successfully")+fileName, QMessageBox::Ok);
    dtree_->treeModel_->updateTree();

  }


  if(file.exists())
      file.remove();
  QFile file2(tool.pathData+"/temp.zip");
  if(file2.exists())
      file2.remove();
}


}

void anvar::installAddonsFormFile()
{
    QStringList fileNames =  QFileDialog::getOpenFileNames(0,"Open",tool.ReadSettings("lastDir","/home/").toString(),tr(" Al-Anvar Add-ons (*.nvr)"));

    installAddons(fileNames);
}
void anvar::BackupOfSubjectAndComment(){

    QString BackupName =   QFileDialog::getSaveFileName(this, tool.trlang("Backup"),QStandardPaths::writableLocation(QStandardPaths::HomeLocation)  );
    if(BackupName.isNull())
        return;
    QStringList table;
    table <<"Comment" <<"SubjectName" <<"Subjects" <<"unwantedWords" ;
    QStringList listSql = db->backup(table);
    db = new DataBase(BackupName);
    db->insertsqls(listSql,false);
    db = new DataBase(tool.pathData+"/Data.db");
    tool.ZipOpen(BackupName);
     QFile file(BackupName+".nvr");

     QFile fileDel(BackupName);
     if(fileDel.exists())
         fileDel.remove();

    if(file.exists())
        QMessageBox::information(this,tool.trlang("Backup Subjects and Notes"),tool.trlang("Backup was create Successfully"));
    else
        QMessageBox::warning(this, tool.trlang("Backup Subjects and Notes"),tool.trlang("Backup was not create Successfully"));


}
void anvar::goToQuranTree(int value)
{
    QList<QTreeWidgetItem*> findlist= ui->quranTree->findItems(QString::number(value),Qt::MatchExactly | Qt::MatchRecursive,1);
      if(findlist.size()>0)
      if(findlist.at(0)->text(2)=="chapters")
        ui->quranTree->setCurrentItem(findlist.at(0));
      QString newTitle = tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+QString::number(value)));
      tabListTitle.replace(tabListTitle.indexOf(ui->tabWidgetQuran->tabText(ui->tabWidgetQuran->currentIndex())),newTitle);
      ui->tabWidgetQuran->setTabText(ui->tabWidgetQuran->currentIndex(),newTitle);

}
void anvar::on_tabWidgetQuran_currentChanged(int)
{
   // listWidgetPrint =   ui->tabWidgetQuran->currentWidget()->findChild<QListWidget *>();

}
void anvar::on_dockWidget_visibilityChanged(bool visible)
{
    if(visible){
        ui->actionLayoutChapter->setIcon(QIcon(":/images/layoutdelete.png"));
        ui->actionLayoutChapter->setText(tool.trlang("Hide chapter layout"));
   }else{
        ui->actionLayoutChapter->setIcon(QIcon(":/images/layoutadd.png"));
        ui->actionLayoutChapter->setText(tool.trlang("Show chapter layout"));

    }

}
void anvar::restartApp()
{
    QMessageBox::StandardButton sb;

        sb = QMessageBox::warning(0, tool.trlang("Need Restart"),
                            tool.trlang("Some Preferences need for restart Al-Anvar to apply."),
                               QMessageBox::Reset |QMessageBox::Cancel);
    if (sb ==QMessageBox::Cancel)
        return;

    QProcess::startDetached(QApplication::applicationFilePath());
    exit(2);
}
anvar::~anvar()
{

    delete ui;
}


void anvar::showAddonsePage()
{
    addonsForm->treeChargeAddons(addonsForm->treeWidget);
    QRect qRect(QApplication::desktop()->screenGeometry());
    int iXpos=qRect.width()/2-addonsForm->width()/2;
    int iYpos=qRect.height()/2-addonsForm->height()/2;
    addonsForm->move(iXpos,iYpos);
    addonsForm->show();
    addonsForm->activateWindow();

}





void anvar::on_comboBoxSortBy_activated(int index)
{
    SetTree(ui->comboBoxSortBy->itemData(index).toString());
    goToQuranTree(quranWidget->line_Chapter->value());
}

void anvar::on_actionUpdates_triggered()
{
    QMessageBox::information(0,tool.trlang("Check for updates"),checkLatestVersion());
}

void anvar::on_actionSearch_triggered()
{
  ui->toolBoxQuran->setCurrentIndex(2);
  ui->lineAndQ->setFocus();
}

void anvar::on_tableResultRoot_doubleClicked(const QModelIndex &index)
{
    emit findVerse(ui->tableResultRoot->model()->data(ui->tableResultRoot->model()->index(index.row(),0)).toString()+";"
                   +ui->tableResultRoot->model()->data(ui->tableResultRoot->model()->index(index.row(),1)).toString());
}

void anvar::on_actionMoshafView_triggered()
{
    addTab(db->trlang("MoshafView"),"",0,"moshaf");

}

void anvar::bookTreeEvent()
{

  /*  addTab(db->getDatastr("Select Name From Books Where BookId ='"+bookTree->getCurrentText(5,bookTree->currentIndex())+"' ; ")
           ,bookTree->getCurrentText(5,bookTree->currentIndex())+"<Page>"+bookTree->getCurrentText(6,bookTree->currentIndex())
           ,0,"book");

*/
}


