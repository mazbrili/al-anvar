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
anvar::anvar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::anvar)
{
    ui->setupUi(this);
    firstAddons = true;
    firstAyeExp = true;
    ui->toolBoxQuran->setCurrentIndex(0);
    ui->tabWidgetQuran->removeTab(1);
    ui->tabWidgetQuran->removeTab(0);
    ui->tabWidgetQuran->setVisible(false);
    ui->dockWidget->setVisible(false);
    ui->tabWidgetQuran->setTabsClosable(true);

    ui->introLayout->addWidget(&intro);

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

     QStringList com_LangItems =tool.GetLang(":/language/");

     foreach(QString str,com_LangItems)
       stp->comb_lang->addItem(str.split(";").at(0),str.split(";").at(1));

      com_LangItems =tool.GetLang(db->pathUser+"/language/");

      foreach(QString str,com_LangItems){
          if(stp->comb_lang->findData(str.split(";").at(1))==-1)
               stp->comb_lang->addItem(str.split(";").at(0),str.split(";").at(1));
      }




     stp->comb_lang->setCurrentIndex(
                 stp->comb_lang->findData(tool.ReadSettings("lang","English").toString())
                 );


     SetLanguage();


    QList<QStringList> bookmarks = db->getUpListData("select ChapterProperty.descent,quran.verse,BookMarks.title,quran.chapter from BookMarks left join quran on  BookMarks.id=quran.id left join ChapterProperty on ChapterProperty.id=quran.chapter ;");

    intro.listWidget->clear();
    connect(intro.listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(bookmarkEvent(QListWidgetItem*)));
    int row=0;
    foreach (QStringList list, bookmarks)
    {
        QListWidgetItem *item = new QListWidgetItem(db->trlang(list.at(0))+"("+list.at(1)+") "+list.at(2));
        item->setData(Qt::UserRole,list.at(3));
        item->setData(Qt::UserRole+1,list.at(1));
        intro.listWidget->insertItem(row,item);
        row++;
    }


           rootSearchModelSF = new QSortFilterProxyModel;
           rootSearchModelSF = db->getProxyMode("Select text As RootWords,id From quran_root ;");
           ui->listViewRootList->setModel(rootSearchModelSF);

       QStringList listC = db->getData("Select descent From ChapterProperty ;");
       foreach (QString str, listC)
           chapterList << db->trlang(str);




       connect(ui->listViewRootList,SIGNAL(clicked(QModelIndex)),this,SLOT(rootSearch(QModelIndex)));
       connect(ui->line_SearchRoot,SIGNAL(textEdited(QString)),this,SLOT(filterRoot(QString)));


    trTableSearch = db->getData("Select TableName From Tr_Translation;");
    trNameSearch = db->getData("Select Name From Tr_Translation;");
    trTableSearch.prepend("Note");
    trTableSearch.prepend("None");
    trNameSearch.prepend(db->trlang("Note"));
    trNameSearch.prepend(db->trlang("Quran"));
    ui->comboBoxSelectSearch->addItems(trNameSearch);
    searchHistory = db->ReadSettings("searchHistory","").toStringList();
    ui->lineAnd->setCompleter(new QCompleter(searchHistory, this));
    limitSearch=50;
    sampleQObject = new JsObject(this);
    sampleQObjectRoot = new JsObject(this);

    connect(sampleQObject,SIGNAL(sendValue(QVariant)),this,SLOT(getValue(QVariant)));
    connect(ui->webViewQuranSearch->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),
            this,SLOT(addJavaScriptObject()));


    connect(sampleQObjectRoot,SIGNAL(sendValue(QVariant)),this,SLOT(getValueRoot(QVariant)));
    connect(ui->webViewRootSearch->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),
            this,SLOT(addJavaScriptObjectRoot()));
    ui->webViewQuranSearch->setContextMenuPolicy(Qt::NoContextMenu);

    if(db->getDatastr("Select Count(id) From rootindex ;").toInt()==0)
    {
     QList<QStringList> rootmaker = db->getUpListData("Select id,id_verse From quran_root order by id ;");
     db->begin();
     foreach (QStringList ro,rootmaker)
     {
       QString address = ro.at(1);
       QStringList list = address.split(";");
         foreach (QString str,list)
           {
             QStringList v,f;
             f << "idroot" << "idverse" << "matched";
             v << ro.at(0) << QString::number(str.split(":").at(0).toInt()+1)<<QString::number(str.split(":").at(1).toInt()-1);
             db->insert("rootindex",f,v);
           }
     }
    db->commit();
    }

}

void anvar::quranSearch()
{
    addTab(tool.trlang("Advanced Search"),"","adSearch");
    if(!searchHistory.contains(ui->lineAnd->text()))
    {
         searchHistory<< ui->lineAnd->text();
         db->WriteSettings("searchHistory",searchHistory);
         ui->lineAnd->setCompleter(new QCompleter(searchHistory, this));
    }

    int type = ui->comboBoxSelectSearch->currentIndex();
    translationTable = trTableSearch.at(type);


    searchText =  db->getSearchPattern(
                ui->lineNot->text(),
                ui->lineAnd->text(),
                ui->lineAndAfter->text(),
                ui->checkBoxWhole->isChecked(),
                false,
                ui->checkBoxHamza->isChecked());

    QString getFindedRecordQuery;

    if(translationTable=="None"){
        startQuery= QString(" Select (%1) AS verseValue From Quran  Where Arabic REGEXP '%2' ")
                .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" ondblclick=\"\"selectRow(this.id)\"\" >","Arabic","arabictext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"))
                .arg(searchText);
        getFindedRecordQuery = QString(" Select count(Quran.id) From Quran  Where Arabic REGEXP '%1' ").arg(searchText);

   }else if(translationTable=="Note"){
      sampleText = db->getDatastr("select Value from Comment;");
      startQuery= QString(" Select ( %1  || \n  %2 ) AS verseValue From Quran Left Join Comment on Quran.id= Comment.id Where Value REGEXP '%3' ")
              .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" ondblclick=\"\"selectRow(this.id)\"\" >"))
              .arg(db->setTextStyle("","Value","trtext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"))
              .arg(searchText);
      getFindedRecordQuery = QString(" Select count(Quran.id) From Quran Left Join Comment on Quran.id= Comment.id Where Value REGEXP '%1' ").arg(searchText);



    }else{
       sampleText = db->getDatastr("select tr from "+translationTable);
       startQuery= QString(" Select ( %1 ) AS verseValue From Quran Left Join %2 on Quran.id= %2.id Where tr REGEXP '%3' ")
               .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" ondblclick=\"\"selectRow(this.id)\"\" >","tr","trtext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"))
               .arg(translationTable)
               .arg(searchText);
       getFindedRecordQuery = QString("Select count(Quran.id) From Quran Left Join %1 on Quran.id= %1.id Where tr REGEXP '%2' ").arg(translationTable).arg(searchText);


    }
    allrecordSearch = db->getDatastr(getFindedRecordQuery).toInt();
    ui->labelResult->setText(tool.trlang("Results")+" "+QString::number(allrecordSearch));
    int startLimitSearch =0;

    ui->comboBoxLimit->clear();

      for(int i=0;i<=allrecordSearch/limitSearch;i++)
    {
      ui->comboBoxLimit->addItem(QString::number(i+1),startLimitSearch);
      startLimitSearch+=limitSearch;
    }
    if(ui->comboBoxLimit->count()<1)
        ui->comboBoxLimit->addItem("1");
    ui->labelResult->setText(tool.trlang("Results")+" "+QString::number(allrecordSearch)+tool.trlang("on")+" "+QString::number(ui->comboBoxLimit->count())+" "+tool.trlang("Page"));
    on_comboBoxLimit_activated();
}

void anvar::on_comboBoxLimit_activated()
{
    int startLimitSearch =ui->comboBoxLimit->itemData(ui->comboBoxLimit->currentIndex()).toInt();
    QString html,html_;
    if(QFile::exists("quranview.html"))
        html =db->ReadFromTextFile("quranview.html").join("\n    ");
    else
        html = db->ReadFromTextFile(":quranview.html").join("\n    ");
    QStringList listResult =db->getData(startQuery+QString(" LIMIT %1 ,%2 ;").arg(startLimitSearch).arg(limitSearch));
    foreach(QString value,listResult)
        html_ += db->highlighSearch(value,0)+"\n";


    html.replace("_Quran_Value_",html_);
    QFont fontQuran=db->ReadSettings("QuranFont", QFont()).value<QFont>();
    QFont fontTr=db->ReadSettings("TrFont", QFont()).value<QFont>();

    html.replace("_arabictext_family_",fontQuran.family());
    html.replace("_arabictext_size_",QString::number(fontQuran.pointSize()));
    html.replace("_trtext_direction_",db->getDirection(sampleText));
    html.replace("_trtext_family_",fontTr.family());
    html.replace("_trtext_size_",QString::number(fontTr.pointSize()));
    ui->webViewQuranSearch->setHtml(html,QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html")));
}
void anvar::getValue(QVariant v)
{
    ui->webViewQuranSearch->findText("") ;
    showQuranFromSearch(db->getData("Select verse,chapter From Quran Where id="+v.toString()).join(";"));
}
void anvar::showQuranFromSearch(QString str)
{
    addTab(tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+str.split(";").at(1)+" ; ")),
           str.split(";").at(1),"chapter",str.split(";").at(0).toInt());
}
void anvar::addJavaScriptObject()
{
   ui->webViewQuranSearch->page()->mainFrame()
                     ->addToJavaScriptWindowObject("sampleQObject", sampleQObject);
}

void anvar::filterRoot(QString str)
{
    rootSearchModelSF->setFilterRegExp(db->roolSearch(str));
}
void anvar::rootSearch(QModelIndex index)
{
    if (!index.isValid())
        return;
    addTab(tool.trlang("Quran Root"),"","rootSerach");
    QString getFindedRecordQueryRoot;

   rootstartQuery= QString("Select Arabic,matched,quran.id,chapter,verse From Quran Left Join RootIndex on Quran.id=RootIndex.idverse Where RootIndex.idroot=%1  ")
           .arg(rootSearchModelSF->index(index.row(),1).data().toString());
   getFindedRecordQueryRoot = QString("Select Count(Quran.id) From Quran Left Join RootIndex on Quran.id=RootIndex.idverse Where RootIndex.idroot=%1 ;")
           .arg(rootSearchModelSF->index(index.row(),1).data().toString());

   allrecordSearchRoot = db->getDatastr(getFindedRecordQueryRoot).toInt();
   ui->labelQuranRootResult->setText(tool.trlang("Results")+" "+QString::number(allrecordSearchRoot));
   int startLimitSearchRoot =0;

   ui->comboBoxLimitRoot->clear();

     for(int i=0;i<=allrecordSearchRoot/limitSearch;i++)
   {
     ui->comboBoxLimitRoot->addItem(QString::number(i+1),startLimitSearchRoot);
     startLimitSearchRoot+=limitSearch;
   }
   if(ui->comboBoxLimitRoot->count()<1)
       ui->comboBoxLimitRoot->addItem("1");
   ui->labelQuranRootResult->setText(tool.trlang("Results")+" "+QString::number(allrecordSearchRoot)+tool.trlang("on")+" "+QString::number(ui->comboBoxLimitRoot->count())+" "+tool.trlang("Page"));
   on_comboBoxLimitRoot_activated();

}

void anvar::on_comboBoxLimitRoot_activated()
{
    QString html,html_;
    int  startLimitSearch= ui->comboBoxLimit->itemData(ui->comboBoxLimit->currentIndex()).toInt();
    if(QFile::exists("quranview.html"))
        html =db->ReadFromTextFile("quranview.html").join("\n    ");
    else
        html = db->ReadFromTextFile(":quranview.html").join("\n    ");
    QList<QStringList> listResult =db->getUpListData(rootstartQuery+QString(" LIMIT %1 ,%2 ;").arg(startLimitSearch).arg(limitSearch));
    QString tag="<b><font color='red'>";
    QString tagEnd = "</font></b>";
    foreach(QStringList val,listResult)
    {
        QString value = val.at(0);
        int mached = val.at(1).toInt();
            value.remove(QString::fromUtf8("ۛ"));
            value.remove(QString::fromUtf8("ۖ"));
            value.remove(QString::fromUtf8("ۗ"));
            value.remove(QString::fromUtf8("ۚ"));
            value.remove(QString::fromUtf8("ۙ"));
            value.remove(QString::fromUtf8("ۘ"));
            value.remove(QString::fromUtf8("۩"));
            QStringList  wordList=value.split(" ",QString::SkipEmptyParts);
            if(wordList.size()>mached && mached>=0)
            {
            QString machedText = wordList.at(mached);
             int pos = value.indexOf(machedText);
             value.insert(pos,tag);
             value.insert(pos+tag.count()+machedText.count(),tagEnd);
            }

         value += QString("(%1:%2)").arg(val.at(3)).arg(val.at(4));
        html_ +=  QString("<div id=\"%1\" ondblclick=\"selectRow(this.id)\"><p class=\"arabictext\">%2</p></div>").arg(val.at(2)).arg(value)+"\n";
    }
    html.replace("_Quran_Value_",html_);
    QFont fontQuran=db->ReadSettings("QuranFont", QFont()).value<QFont>();
    QFont fontTr=db->ReadSettings("TrFont", QFont()).value<QFont>();
    html.replace("_arabictext_family_",fontQuran.family());
    html.replace("_arabictext_size_",QString::number(fontQuran.pointSize()));
    html.replace("_trtext_direction_",db->getDirection(sampleText));
    html.replace("_trtext_family_",fontTr.family());
    html.replace("_trtext_size_",QString::number(fontTr.pointSize()));
    ui->webViewRootSearch->setHtml(html,QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html")));

}
void anvar::getValueRoot(QVariant v)
{
    ui->webViewRootSearch->findText("") ;
    showQuranFromSearchRoot(db->getData("Select chapter,verse From Quran Where id="+v.toString()).join(";"));
}
void anvar::addJavaScriptObjectRoot()
{
   ui->webViewRootSearch->page()->mainFrame()
                     ->addToJavaScriptWindowObject("sampleQObject", sampleQObjectRoot);
}
void anvar::showQuranFromSearchRoot(QString str)
{
    addTab(tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+str.split(";").at(0)+" ; ")),
           str.split(";").at(0),"chapter",str.split(";").at(1).toInt());
}


void anvar::bookmarkEvent(QListWidgetItem* item)
{
    if(!item)
        return;
    addTab(tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+item->data(Qt::UserRole).toString()+" ; ")),
           item->data(Qt::UserRole).toString(),"chapter",item->data(Qt::UserRole+1).toInt());

}

void anvar::closeEvent(QCloseEvent *e)
{

    if(firstAyeExp)
        return;
    db->insertsql("DELETE FROM Comment WHERE Value='';");
    if (ayeExp->maybeSave())
        e->accept();
    else
        e->ignore();

 }
void anvar::openLastPage()
{
    QString chapterId=db->getDatastr("Select Chapter From Quran Where Page ="+tool.ReadSettings("lastChapter","1").toString());
     addTab(
           tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+chapterId+" ; "))
                 ,tool.ReadSettings("lastChapter","1").toString(),"chapter");
     goToQuranTree(chapterId.toInt());
}
void anvar::OpenReseachTools()
{
  openAyeExplor("1",1);
}
void anvar::connectToDataBase()
{

    QApplication::processEvents();
    db = new DataBase(tool.pathData+"/Data.db");
    QStringList table =db->getData("select tableName From Tr_Translation;");

    if(db->getDatastr("SELECT count(name) FROM sqlite_master WHERE type='table' ;").toInt()<10)
    {
        QStringList listSql =tool.ReadFromTextFile(":Data.sql");
            db->insertsql(listSql.join("\n"));            
            db->insertsql("DELETE From Subjects WHERE  ID_SUBJECT = '823192522974'  AND ID_VERSE=1;");

    }else if (db->getDatastr("Select Version From Version")!="060" /*QApplication::applicationVersion()*/)
    {
    // QMessageBox::information(0, db->trlang("Upadate database"),db->trlang("database most to update . please waite..."));

                             table <<"Comment" <<"SubjectName" <<"Subjects" <<"unwantedWords" <<"BookMarks" <<"Explanation_Name" << "Explanations" << "Tr_Translation";
                             QString fileName = tool.pathData+"/fullbackup"+QDateTime::currentDateTime().toString("yyMMddhhmmss");
                             BackupOfSubjectAndComment(table,fileName);


        QStringList tables = db->getData("SELECT name FROM sqlite_master WHERE type='table';");
        db->begin();
        foreach (QString table, tables)
            db->insertsql("DROP  TABLE "+table+" ;");
        db->commit();
        QStringList listSql =tool.ReadFromTextFile(":Data.sql");
        db->insertsql(listSql.join("\n"));
        db->insertsql("DELETE From Subjects WHERE  ID_SUBJECT = '823192522974'  AND ID_VERSE=1;");
        installAddons(QStringList(fileName+".nvr"),false);
        qDebug()<<"Doneeeeeeeee";


    }

    if(db->isOpen()){


           treeModel = new DBModelSubJectQuran(db,this,false,"SubjectName");

            dtree_ = new DbTreeViewSubJectQuran(db,this,false,"SubjectName",treeModel);
            dtree_->tb_find->setVisible(true);
            ui->layout_Subject->addWidget(dtree_);
            ui->layout_Subject->addLayout(dtree_->find);


            QStringList audioList = db->getData("Select Name From Audio order by ord ;");
            foreach (QString str,audioList)
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




            quranWidget = new QuranWidget(db);
            connect(quranWidget,SIGNAL(showRelated(QString,QString)),this,SLOT(showRelated(QString,QString)));
            connect(quranWidget,SIGNAL(openAyeExplor(QString,int)),this,SLOT(openAyeExplor(QString,int)));

            //get list audio..
            quranWidget->listAudio = db->getListData("Select Name,dir_Name,Url From Audio order by ord;");

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
            connect(dtree_, SIGNAL(clicked(QModelIndex)), this, SLOT(SubjectEvent()));


          // searchWin->setWindowTitle(tool.trlang("Search"));
           ui->labelResult->setText(tool.trlang("Results")+"  ");
           QuranRoot = db->getListData("SELECT *FROM quran_root ORDER BY id;");
           ui->labelQuranRootResult->setText(tool.trlang("Results")+"  ");
           connect(ui->quranSearch,SIGNAL(clicked()),this,SLOT(quranSearch()));
           connect(ui->lineAnd,SIGNAL(returnPressed()),this,SLOT(quranSearch()));
           connect(ui->lineAndAfter,SIGNAL(returnPressed()),this,SLOT(quranSearch()));
           connect(ui->lineNot,SIGNAL(returnPressed()),this,SLOT(quranSearch()));
           connect(intro.toolButtonSearch, SIGNAL(clicked()), this, SLOT(on_actionSearch_triggered()));
           connect(intro.toolButtonSetting, SIGNAL(clicked()), this, SLOT(showOptions()));
           connect(intro.toolButtonAddons, SIGNAL(clicked()), this, SLOT(showAddonsePage()));
           connect(intro.toolButtonResearch, SIGNAL(clicked()), this, SLOT(OpenReseachTools()));
           connect(intro.toolButtonLast, SIGNAL(clicked()), this, SLOT(openLastPage()));
           connect(intro.toolButtonTopic, SIGNAL(clicked()), this, SLOT(gotoTopics()));

}

}
void anvar::SetLanguage()
{

  if(tool.trlang("Align")=="Right"){
 setLayoutDirection(Qt::RightToLeft);
 ui->dockWidget->setLayoutDirection(Qt::RightToLeft);
 stp->setLayoutDirection(Qt::RightToLeft);
 addDockWidget(Qt::RightDockWidgetArea, ui->dockWidget);
 intro.setLayoutDirection(Qt::RightToLeft);
 intro.horizontalLayoutMain->addWidget(intro.widget_2);
 intro.spacerLayout->addStretch();
}

  stp->setWindowTitle(tool.trlang("Preferences"));
  ui->toolBoxQuran->setItemIcon(0,QIcon(":/images/quran.png"));
  ui->toolBoxQuran->setItemIcon(1,QIcon(":/images/tree.png"));
  //ui->toolBoxQuran->setItemIcon(2,QIcon(":/images/books.png"));
  ui->toolBoxQuran->setItemIcon(2,QIcon(":/images/search.png"));
  ui->labelSelectSearch->setText(tool.trlang("Search in:"));
  ui->actionMoshafView->setText(db->trlang("MoshafView"));
  ui->labelPage->setText(tool.trlang("Page"));
  ui->labelPageRoot->setText(tool.trlang("Page"));


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
  /*
  stp->check_audio->setText(tool.trlang("Playing Icon"));
  stp->check_comment->setText(tool.trlang("Remarking Icon"));
  stp->check_explanation->setText(tool.trlang("Interpretation"));
  stp->check_subject->setText(tool.trlang("Subjects"));
  stp->check_Similer->setText(tool.trlang("Similar Verses"));
  stp->check_SameWord->setText(tool.trlang("Similar Words"));
  stp->label_icons->setText(tool.trlang("Tools"));


  */
  stp->pushButtonSet->setText(tool.trlang("Ok"));
  stp->pushButtonCancel->setText(tool.trlang("Cancel"));
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
  stp->labelStyle->setText(tool.trlang("Style"));
  stp->labelStyleSheet->setText(tool.trlang("Theme and color"));
  stp->pushButtonPathDatabase->setText(tool.trlang("DataBase Dir"));
  stp->pushButtonPathAudio->setText(tool.trlang("Audio Dir"));

  stp->radioButtonExterna->setText(tool.trlang("Use external program:"));
  stp->radioButtonPhonon->setText(tool.trlang("Play via Phonon"));
  stp->labelPlayer->setText(tool.trlang("Playback"));

  ui->groupBoxAdQ->setTitle(tool.trlang("Advanced Search"));
  ui->checkBoxHamza->setText(tool.trlang("Match Alifhamza"));
  ui->checkBoxWhole->setText(tool.trlang("Match Case"));
  ui->quranSearch->setText(tool.trlang("Search"));
  ui->labelAnd->setText(tool.trlang("all these words"));
  ui->labelAndAfter->setText(tool.trlang("all these words sequence"));
  ui->labelNot->setText(tool.trlang("none of these words"));
  ui->labelCom->setText(tool.trlang("for split words use / or ,"));
  ui->tabWidgetSearch->setTabText(0,tool.trlang("Advanced Search"));
  ui->tabWidgetSearch->setTabText(1,tool.trlang("Quran Root"));

  //ui->radioButtonBooks->setText(tool.trlang("Books Search"));


  //ui->dockWidget->setGeometry(ui->dockWidget->x(),ui->dockWidget->y(),350,ui->dockWidget->height());


  intro.toolButtonLast->setText(tool.trlang("Last Page Visited"));
  intro.toolButtonSetting->setText(tool.trlang("Options.."));
  intro.toolButtonSearch->setText(tool.trlang("Advanced Search"));
  intro.toolButtonTopic->setText(tool.trlang("QuickTopic").replace(":inter:","\n"));
  intro.toolButtonAddons->setText(tool.trlang("QuickAddons").replace(":inter:","\n"));
  intro.toolButtonResearch->setText(tool.trlang("QuickResearch").replace(":inter:","\n"));
  intro.labelBookMarks->setText(db->trlang("Bookmarks"));
  //
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
    addTab(tool.trlang("Subject :")+" "+name,sql,"subject");

}
void anvar::openAyeExplor(QString currentBlock,int i)
{
    if(firstAyeExp)
    {
        quranModel =new QuranModel(db,this);
        QuranTreeView *quranTree =new  QuranTreeView(db,quranModel,"1",this);
        ayeExp = new AyeExplor(db,treeModel,quranTree,this);
        ayeExp->setVisible(false);

       if(tool.trlang("Align")=="Right"){
       ayeExp->setLayoutDirection(Qt::RightToLeft);
      }else
       ayeExp->setLayoutDirection(Qt::LeftToRight);

        ayeExp->setFont(tool.ReadSettings("ProgFont",stp->fontprog).value<QFont>());
        ayeExp->textComment->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
        ayeExp->textExp->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
        ayeExp->textTr->setFont(tool.ReadSettings("trfont",stp->fontprog).value<QFont>());
        ayeExp->tabWidget->setTabText(0,tool.trlang("Subject of the Verses"));
        ayeExp->tabWidget->setTabText(1,tool.trlang("Show and Edit the Notes"));
        ayeExp->tabWidget->setTabText(2,tool.trlang("Interpretation"));
        ayeExp->tabWidget->setTabText(3,tool.trlang("Similar Words"));
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
        firstAyeExp=false;

    }
           ayeExp->Id=currentBlock;
           ayeExp->tabWidget->setCurrentIndex(i);
           ayeExp->treeAye->setCurrent(currentBlock);
           ayeExp->showAll();
           ayeExp->setVisible(true);
           addTab(tool.trlang("Research tools"),"","exp");


}

void anvar::on_tabWidgetQuran_tabCloseRequested(int index)
{

    if(ui->tabWidgetQuran->tabText(index)==tool.trlang("Research tools"))
    {
      if(!ayeExp->maybeSave())
          return;
    }
        ui->tabWidgetQuran->removeTab(index);
        tabListTitle.removeAt(index);

      if(ui->tabWidgetQuran->count()<1){
        ui->tabWidgetQuran->setVisible(false);
        ui->dockWidget->setVisible(false);
        intro.setVisible(true);
      }


}
void anvar::addTab(QString title,QString mainId,QString type,int verse)
{
    if( ui->tabWidgetQuran->isHidden()){
        ui->tabWidgetQuran->setVisible(true);
        ui->dockWidget->setVisible(true);
        intro.setVisible(false);
    }
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

    if(type=="exp")
        {
            tabIcon=QIcon(":/images/Explanations.png");
            ui->tabWidgetQuran->addTab(ayeExp,tabIcon,title);
            if(ui->tabWidgetQuran->count()>1)
            ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);
        }else if(type=="rootSerach"){
             ui->tabWidgetQuran->addTab(ui->tabSearchRoot,tabIcon,title);
             if(ui->tabWidgetQuran->count()>1)
             ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);

        }else if(type=="adSearch"){
          ui->tabWidgetQuran->addTab(ui->tabAdSearch,tabIcon,title);
        if(ui->tabWidgetQuran->count()>1)
        ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);

    }else{


    quranWidget = new QuranWidget(db);
    ui->tabWidgetQuran->addTab(quranWidget,tabIcon,title);

    connect(quranWidget,SIGNAL(showRelated(QString,QString)),this,SLOT(showRelated(QString,QString)));
    connect(quranWidget,SIGNAL(openAyeExplor(QString,int)),this,SLOT(openAyeExplor(QString,int)));

   if(ui->tabWidgetQuran->count()>1)
   ui->tabWidgetQuran->setCurrentIndex(ui->tabWidgetQuran->count()-1);


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
       quranWidget->mainTndex=mainId;
       quranWidget->showType=type;
       quranWidget->showVerse();

  }
  else if(type=="chapter"){
   quranWidget->mainTndex=mainId;
   quranWidget->showType=type;
   quranWidget->showVerse();
      if(verse!=0){
          quranWidget->line_Verse->setValue(verse);
          quranWidget->VerseChanged();
      }
    }else{
       quranWidget->mainTndex=mainId;
       quranWidget->showType=type;
       quranWidget->showVerse();
   }
  }
 } else
    {
        ui->tabWidgetQuran->setCurrentIndex(tabListTitle.indexOf(title));
        if(type=="chapter" )
        {
            if(verse!=0){
                quranWidget->line_Verse->setValue(verse);
                quranWidget->VerseChanged();
            }
        }

    }

}

void anvar::TreeEventQuran()
{
 if(ui->quranTree->currentItem())

        addTab(tool.trlang(db->getDatastr("Select descent  From ChapterProperty Where Id = "+ui->quranTree->currentItem()->text(1)+" ; ")),
               ui->quranTree->currentItem()->text(1),"chapter");
}
void anvar::TreeEventJuz()
{
     if(ui->comboBoxJuz->currentIndex())     
         addTab(db->trlang("Section")+QString::number(ui->comboBoxJuz->currentIndex()+1),QString::number(ui->comboBoxJuz->currentIndex()+1),"juz");
}
void anvar::TreeEventHizb()
{
    if(ui->comboBoxHizb->currentIndex())
        addTab(db->trlang("Hizb")+QString::number(ui->comboBoxHizb->currentIndex()+1),QString::number(ui->comboBoxHizb->currentIndex()+1),"hizb");
}

void anvar::SubjectEvent(){
    if(dtree_->getCurrentText(4,dtree_->currentIndex())=="0")
        return;

    if(dtree_->getCurrentText(4,dtree_->currentIndex())=="1"){
      if(db->getDatastr("Select Count(id) From Subjects Where ID_SUBJECT ="+dtree_->getCurrentText(1,dtree_->currentIndex())+"; ")=="0")
          return;
    addTab(tool.trlang("Subject :")+" "+dtree_->getCurrentText(0,dtree_->currentIndex())
           ,dtree_->getCurrentText(1,dtree_->currentIndex()),"subject");
    }
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
    //stp->radioButtonQuranFC->setChecked(true);
    //stp->radioButtonPhonon->setChecked(true);
   if(stp->exec())
   {
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

   QString text = "<b>"+tool.trlang("Version")+" 0.6.0</b><br>"
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
    ContactForm coFor;
    coFor.exec();
}
void anvar::AcSave(){

   if (ui->tabWidgetQuran->currentWidget()->findChild<QWebView *>()==0)
         return;

    QString fn = QFileDialog::getSaveFileName(this, tool.trlang("Save as.."),
                                              QDesktopServices::storageLocation(QDesktopServices::HomeLocation), "HTML-Files (*.htm *.html);;ODF files (*.odt);;All Files (*)");
    if (fn.isEmpty())
        return ;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
        fn += ".html"; // default

    QWebView *save = ui->tabWidgetQuran->currentWidget()->findChild<QWebView *>();
    db->WriteToText(fn,save->page()->mainFrame()->toHtml());

}
void anvar::AcPrint()
{
    if (ui->tabWidgetQuran->currentWidget()->findChild<QWebView *>()==0)
          return;

    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    dlg->setWindowTitle(tool.trlang("Print"));
    if (dlg->exec() == QDialog::Accepted)
    {
      QWebView *save = ui->tabWidgetQuran->currentWidget()->findChild<QWebView *>();
         save->print(&printer);
    }
    delete dlg;



}
void anvar::AcPdf()
{
    if (ui->tabWidgetQuran->currentWidget()->findChild<QWebView *>()==0)
          return;


    QString fileName = QFileDialog::getSaveFileName(this, tool.trlang("Save as pdf"),
                                                    QDesktopServices::storageLocation(QDesktopServices::HomeLocation), "*.pdf");
    if (!fileName.isEmpty())
    {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        QWebView *save =  ui->tabWidgetQuran->currentWidget()->findChild<QWebView *>();
        save->print(&printer);
    }


}
void anvar::installAddons(QStringList fileNames,bool report)
{
    QStringList invalidFiles,addedFiles,successFiles;

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
       if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='Comment' ;")=="Comment")
        isBackup =true;
      else if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='Tr_Translation' ;")=="Tr_Translation")
          istranslation =true;
      else   if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='Explanation_Name' ;")=="Explanation_Name")
          isExplanation =true;

      else   if (db->getDatastr("SELECT name FROM sqlite_master WHERE name='makebook' ;")=="makebook")
      {
         isBook =true;
         isBook_=true;
      }
      else
        isanr = false;
     if (!isanr)
     {

      invalidFiles << fileName;
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
                     addedFiles << fileName;
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
                    quranWidget->trTable = db->getData("Select TableName From Tr_Translation;");
                    quranWidget->trName = db->getData("Select Name From Tr_Translation;");
                    quranWidget->trTable.prepend("Note");
                    quranWidget->trTable.prepend("None");
                    quranWidget->trName.prepend(db->trlang("Note"));
                    quranWidget->trName.prepend(db->trlang("None"));
                    quranWidget->comboBoxTr->clear();
                    ui->comboBoxSelectSearch->clear();
                    quranWidget->comboBoxTr->addItems(quranWidget->trName);
                    ui->comboBoxSelectSearch->addItems(quranWidget->trName);
                   successFiles <<  fileName;
                }
//install Explanation
    }else if(isExplanation){
      QList<QStringList> Explanation_Name,Explanation;
      Explanation_Name = db->getListData("SELECT  *FROM Explanation_Name");
      Explanation = db->getListData("SELECT  * FROM Explanations ;");
      db = new DataBase(tool.pathData+"/Data.db");
    if (db->getDatastr("SELECT count(id) FROM Explanation_Name where id ="+Explanation_Name.at(0).at(0)+" ;")!="0")
        addedFiles << fileName;
    else{
        db->insertsql("BEGIN TRANSACTION ;");
        db->insertsql("INSERT INTO Explanation_Name VALUES("+Explanation_Name.at(0).at(0)+",'"+Explanation_Name.at(1).at(0)+"');");
          for (int i=0;i<Explanation.at(0).count();i++)
             db->insertsql("INSERT INTO Explanations VALUES("+Explanation.at(0).at(i)+",'"+Explanation.at(1).at(i)+"','"+Explanation.at(2).at(i)+"');");
        db->insertsql("COMMIT;");
        successFiles << fileName;
    }
  }
  else if(isBackup){
      QStringList tables = db->getTables();
      QStringList sqlDrop,sqlInsert;
      sqlInsert = db->backup(tables);
      foreach(QString table,tables)
            sqlDrop <<"DROP TABLE "+table;

    db = new DataBase(tool.pathData+"/Data.db");
    if(report)
    {
    QMessageBox::StandardButton sb;
    sb = QMessageBox::warning(this, tool.trlang("Warning"),tool.trlang("Are you sure to Restore  Backup ?")+"\n"+tool.trlang("all Current Subject and Comment will be lost..")+fileName,
                               QMessageBox::Yes | QMessageBox::No);
    if (sb == QMessageBox::No)
        return;
     }

    db->insertsqls(sqlDrop);
    db->insertsqls(sqlInsert);
    QStringList added = db->getData("Select ID From Comment ;");
    QStringList sqls;
    for (int i=1;i<6237;i++)
    {
        if(!added.contains(QString::number(i)))
            sqls << QString("INSERT INTO Comment (id,Value) VALUES(%1,' ');").arg(i);
    }
    sqls << "CREATE INDEX IF NOT EXISTS INDEXCommentid ON Comment (id  ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS INDEXQuranid ON Quran (id  ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS INDEXSubjectsID_SUBJECT ON Subjects (ID_SUBJECT  ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS INDEXSubjectsID_VERSE ON Subjects (ID_VERSE  ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS INDEXChapterPropertyId ON ChapterProperty (Id  ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS SubjectName_Date ON SubjectName(Date ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS SubjectName_Id ON SubjectName(id ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS SubjectName_Name ON SubjectName(Name ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS SubjectName_Ordering ON SubjectName(ordering ASC);";
    sqls << "CREATE INDEX IF NOT EXISTS SubjectName_Parent_Id ON SubjectName(Parent_ID ASC);";
    db->insertsqls(sqls);
    successFiles <<fileName;
    if(report)
    dtree_->treeModel_->updateTree();
  }


  if(file.exists())
      file.remove();
  QFile file2(tool.pathData+"/temp.zip");
  if(file2.exists())
      file2.remove();
  }
 //
 if(!report)
     return;
   QString reportMsg;
   if(invalidFiles.size()>0)
       reportMsg +=tool.trlang("These files are invalid")+":\n"+invalidFiles.join(",")+"\n";
   if(addedFiles.size()>0)
       reportMsg +=tool.trlang("These files had already been Installed")+":\n"+addedFiles.join(",")+"\n";
   if(successFiles.size()>0)
       reportMsg +=tool.trlang("These files were Installed Successfully")+":\n"+successFiles.join(",");
   QMessageBox::information(0,tool.trlang("Install Add-ons"),reportMsg);

}

void anvar::installAddonsFormFile()
{
    QStringList fileNames =  QFileDialog::getOpenFileNames(0,"Open",tool.ReadSettings("lastDir","/home/").toString()," Al-Anvar Add-ons (*.nvr)");

    installAddons(fileNames,true);
}
void anvar::BackupOfSubjectAndComment()
{
    QString BackupName =   QFileDialog::getSaveFileName(this, tool.trlang("Backup"),QDesktopServices::storageLocation(QDesktopServices::HomeLocation)  );
    if(BackupName.isNull())
        return;
    QStringList table;
    table <<"Comment" <<"SubjectName" <<"Subjects" <<"unwantedWords" <<"BookMarks" ;
    if(BackupOfSubjectAndComment(table,BackupName))
        QMessageBox::information(this,tool.trlang("Backup Subjects and Notes"),tool.trlang("Backup was create Successfully"));
    else
        QMessageBox::warning(this, tool.trlang("Backup Subjects and Notes"),tool.trlang("Backup was not create Successfully"));

}
bool anvar::BackupOfSubjectAndComment(QStringList table,QString BackupName)
{
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
        return true;
    return false;
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
    close();
}
anvar::~anvar()
{

    delete ui;
}


void anvar::showAddonsePage()
{

    if(firstAddons)
    {
        addonsForm = new Addons(db,this);
        addonsForm->setWindowFlags(Qt::Tool);
        if(tool.trlang("Align")=="Right")
            addonsForm->setLayoutDirection(Qt::RightToLeft);
        else
            addonsForm->setLayoutDirection(Qt::LeftToRight);

        addonsForm->Btn_Install_Addons->setText(tool.trlang("Install Add-ons"));
        addonsForm->pushButtonDownload->setText(tool.trlang("Download And Install"));
        addonsForm->setWindowTitle(tool.trlang("Add-ons Page"));
        addonsForm->pushButtonUpdate->setText(tool.trlang("Update Add-ons list"));
        connect(addonsForm->Btn_Install_Addons, SIGNAL(clicked()), this, SLOT(installAddonsFormFile()));
        connect(addonsForm, SIGNAL(downlodFinishd(QStringList)), this, SLOT(installAddons(QStringList)));

        addonsForm->label_getaddons->setText("<a href='https://sourceforge.net/projects/al-anvar/files/addons/'>"+tool.trlang("Get Add-ons")+"</a>");
        addonsForm->label_getaddons->setOpenExternalLinks(true);
        firstAddons=false;


    }
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
   if(ui->dockWidget->isHidden())
        ui->dockWidget->setVisible(true);
  ui->toolBoxQuran->setCurrentIndex(2);
  ui->lineAnd->setFocus();

}
void anvar::gotoTopics()
{
 if(ui->dockWidget->isHidden())
        ui->dockWidget->setVisible(true);
  ui->toolBoxQuran->setCurrentIndex(1);
}



