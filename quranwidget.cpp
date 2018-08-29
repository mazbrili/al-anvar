#include "quranwidget.h"



QuranWidget::QuranWidget(DataBase *database,QWidget *parent):
    QWidget(parent)
{
   setupUi(this);
   db = database;
  igChange=false;
   acVerse= new QAction(0);
   acPlay= new QAction(QIcon(":images/audio.png"),db->trlang("Play"),0);
   acInter= new QAction(QIcon(":images/Explanations.png"),db->trlang("Interpretation"),0);
   acSubject= new QAction(QIcon(":images/tree.png"),db->trlang("Subject of the Verses"),0);
   acComment= new QAction(QIcon(":images/comment_show.png"),db->trlang("Add a Point"),0);
   acMail = new QAction(QIcon(":images/newsletter.png"),db->trlang("need Explanations"),0);
   acZoomIn= new QAction(QIcon(":images/zoom-in.png"),db->trlang("zoom in"),0);
   acZoomOut= new QAction(QIcon(":images/zoom-out.png"),db->trlang("zoom out"),0);
   acSimiler= new QAction(db->trlang("Similar Words"),0);
   acBookmark =  new QAction(QIcon(":images/bookmark.png"),db->trlang("‌Bookmark"),0);
   acPlay->setShortcut(Qt::CTRL + Qt::Key_Q);
   acInter->setShortcut(Qt::CTRL + Qt::Key_I);
   acSubject->setShortcut(Qt::CTRL + Qt::Key_T);
   acComment->setShortcut(Qt::CTRL + Qt::Key_N);
   acZoomIn->setShortcut(Qt::Key_Plus);
   acZoomOut->setShortcut(Qt::Key_Minus);
   checkBoxNoShowQuran->setText(db->trlang("Do Not Show Quran Text"));


   for (int i = 0; i < MaxRecent; ++i)
   {
      acSub[i] = new QAction(0);
      acSub[i]->setIcon(QIcon(":images/tag.png"));
      acSub[i]->setVisible(false);
      connect(acSub[i], SIGNAL(triggered()),this, SLOT(similarVerses()));
  }

   connect(acPlay,SIGNAL(triggered()),this,SLOT(PlayQuran()));
   connect(acInter,SIGNAL(triggered()),this,SLOT(ShowExplanations()));
   connect(acSubject,SIGNAL(triggered()),this,SLOT(AddToSubjects()));
   connect(acComment,SIGNAL(triggered()),this,SLOT(AddToComment()));
   connect(acSimiler,SIGNAL(triggered()),this,SLOT(showDuplicatesWords()));
   connect(acMail, SIGNAL(triggered()), this, SLOT(needExplanations()));
   connect(acZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
   connect(acZoomOut, SIGNAL(triggered()), this, SLOT(zoomQut()));
   connect(acBookmark, SIGNAL(triggered()), this, SLOT(addBookmark()));



   webView->addAction(acVerse);
   QAction *separ = new QAction(0);
   separ->setSeparator(true);


   webView->addAction(separ);
   webView->addAction(acBookmark);
   webView->addAction(acPlay);
   webView->addAction(acInter);
   webView->addAction(acSubject);
   webView->addAction(acComment);
   webView->addAction(acMail);
   webView->addAction(acSimiler);
   webView->addAction(acZoomIn);
   webView->addAction(acZoomOut);
    toolBar = new QToolBar;
    toolBar->setIconSize(QSize(24,24));
    toolBar->addAction(acPlay);
    toolBar->addAction(acInter);
    toolBar->addAction(acSubject);
    toolBar->addAction(acComment);
    toolBar->addAction(acZoomIn);
    toolBar->addAction(acZoomOut);
   QAction *separ2 = new QAction(0);
   separ2->setSeparator(true);
   webView->addAction(separ2);

   QAction *sub = new QAction(0);
   sub->setText(db->trlang("Similar Verses")+":");   
   webView->addAction(sub);
   for (int i = 0; i < MaxRecent; ++i)
     webView->addAction(acSub[i]);

    exPl=new ExternalPlayer(0);
    inPl=new InternalPlayer(0);
    exPl->setVisible(false);
    inPl->setVisible(false);
    playLayout->addWidget(inPl);
    playLayout->addWidget(exPl);
    if(db->trlang("Align")=="Right")
    {
      exPl->setLayoutDirection(Qt::RightToLeft);
      inPl->setLayoutDirection(Qt::RightToLeft);
    }
    connect(exPl,SIGNAL(verseChanged(QString,QString)),this,SLOT(finder(QString,QString)));
    connect(inPl,SIGNAL(verseChanged(QString,QString)),this,SLOT(finder(QString,QString)));


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
     line_Verse->setInputMethodHints(Qt::ImhDigitsOnly);


     lableChapter->setText(db->trlang("Chapter"));
     lableVerse->setText(db->trlang("Verse"));
     lablePage->setText(db->trlang("Page"));
     lableJuz->setText(db->trlang("Section"));
     lableHizb->setText(db->trlang("Hizb"));



    actionsLayout->addWidget(toolBar);
    listAudio = db->getListData("Select Name,dir_Name,Url From Audio order by ord;");

        foreach (QString str,listAudio.at(0))
        {
          exPl->comb_ghary->addItem(db->trlang(str));
          inPl->comb_ghary->addItem(db->trlang(str));
          inPl->comboBoxRep->addItem(db->trlang(str));
        }
        trTable = db->getData("Select TableName From Tr_Translation;");
        trName = db->getData("Select Name From Tr_Translation;");
        trTable.prepend("Note");
        trTable.prepend("None");
        trName.prepend(db->trlang("Note"));
        trName.prepend(db->trlang("None"));
        comboBoxTr->addItems(trName);
        if(comboBoxTr->count()>0)
        comboBoxTr->setCurrentIndex(db->ReadSettings("translation","0").toInt());
        setTranslation();
        if(comboBoxTr->currentIndex()==0)
            checkBoxNoShowQuran->setEnabled(false);
        else
            checkBoxNoShowQuran->setEnabled(true);

        line_Chapter->setValue(1);
        label_Juz->setKeyboardTracking(false);
        label_Hizb->setKeyboardTracking(false);
        line_Page->setKeyboardTracking(false);
        line_Chapter->setKeyboardTracking(false);

          connect(line_Chapter,SIGNAL(valueChanged(QString)),this,SLOT(ChapterChanged()));
          connect(line_Verse, SIGNAL(valueChanged(QString)), this, SLOT(VerseChanged()));
          connect(label_Juz, SIGNAL(valueChanged(QString)), this, SLOT(showJuz(QString)));
          connect(label_Hizb,SIGNAL(valueChanged(QString)),this,SLOT(showHizb(QString)));
          connect(line_Page,SIGNAL(valueChanged(QString)),this,SLOT(PageChanged()));


          // v6

          sampleQObject = new JsObject(this);
          connect(sampleQObject,SIGNAL(sendValue(QVariant)),this,SLOT(getValue(QVariant)));
          connect(webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),
                  this,SLOT(addJavaScriptObject()));
}

void QuranWidget::addJavaScriptObject()
{
   webView->page()->mainFrame()
                     ->addToJavaScriptWindowObject("sampleQObject", sampleQObject);
}


void QuranWidget::getValue(QVariant v)
{

    igChange =true;
    currentVerseId = v.toString();
    QString name =db->trlang(db->getDatastr("Select descent From Quran Left Join ChapterProperty on (Quran.Chapter=ChapterProperty.id)  Where Quran.id= "+currentVerseId));
    QString chapter = db->getDatastr("Select Chapter From Quran Where id= "+currentVerseId);
    QString verse = db->getDatastr("Select Verse From Quran Where id= "+currentVerseId);

    line_Chapter->setValue(chapter.toInt());
    line_Verse->setValue(verse.toInt());

    line_Page->setValue(db->getDatastr("Select Page From Quran Where id= "+currentVerseId).toInt());
    label_Hizb->setValue(db->getDatastr("Select Hizb From Quran Where id= "+currentVerseId).toInt());
    label_Juz->setValue(db->getDatastr("Select Juz From Quran Where id= "+currentVerseId).toInt());
    acVerse->setText(QString("%1: %2(%3) - %4: %5").arg(db->trlang("Chapter")).arg(name).arg(chapter).arg(db->trlang("Verse")).arg(verse));

   // lableMakki->setText(db->trlang(db->getDatastr("Select Nozol From ChapterProperty Where id="+line_Chapter->text()));
    if(db->getDatastr("select  count(id) from comment where id="+currentVerseId)=="0")
          acComment->setIcon(QIcon(":images/comment_add.png"));
    else
          acComment->setIcon(QIcon(":images/comment_show.png"));

    if(db->getDatastr("Select Count(id) From BookMarks Where id="+currentVerseId)=="0")
       acBookmark->setText(db->trlang("Bookmark"));
    else
       acBookmark->setText(db->trlang("Unbookmark"));


    QList<QStringList> subnamelist=db->getListData("Select SubjectName.id,Name From Subjects left Join SubjectName on (Subjects.ID_SUBject=SubjectName.id) Where ID_VERSE="+currentVerseId);

    for (int i = 0; i < MaxRecent; ++i)
      acSub[i]->setVisible(false);

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
    igChange =false;

}
void QuranWidget::showVerse()
{
    QString  query,startQuery,sampleText;
    sampleText ="ff";

    if(trTableName=="None")
        startQuery= QString(" Select (%1) AS verseValue From Quran ")
                .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" onclick=\"\"selectRow(this.id)\"\" >","Arabic","arabictext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"));

   else if(trTableName=="Note" && !checkBoxNoShowQuran->isChecked()){
       sampleText = db->getDatastr("select Value from Comment;");
      startQuery= QString(" Select ( %1  || \n  %2 ) AS verseValue From Quran Left Join Comment on Quran.id= Comment.id ")
              .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" onclick=\"\"selectRow(this.id)\"\" >"))
              .arg(db->setTextStyle("","Value","trtext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"));

    }else if(trTableName=="Note" && checkBoxNoShowQuran->isChecked())
    {
         sampleText = db->getDatastr("select Value from Comment;");
        startQuery= QString(" Select ( %1) AS verseValue From Quran Left Join Comment on Quran.id= Comment.id ")
                .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" onclick=\"\"selectRow(this.id)\"\" >","Value","trtext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"));


    }else if( checkBoxNoShowQuran->isChecked())
    {
         sampleText = db->getDatastr("select tr from "+trTableName);
       startQuery= QString(" Select ( %1 ) AS verseValue From Quran Left Join %2 on Quran.id= %2.id ")
               .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" onclick=\"\"selectRow(this.id)\"\" >","tr","trtext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"))
               .arg(trTableName);

    }else{
         sampleText = db->getDatastr("select tr from "+trTableName);
       startQuery= QString(" Select ( %1  || \n  %2 ) AS verseValue From Quran Left Join %3 on Quran.id= %3.id ")
               .arg(db->setTextStyle("<div id=\"\"\"||quran.id||\"\"\" onclick=\"\"selectRow(this.id)\"\" >"))
               .arg(db->setTextStyle("","tr","trtext"," \"(\"|| \"chapter\" ||\":\"|| \"verse\"  || \")\"|| ","</div>"))
               .arg(trTableName);
    }

    QString endQuery;
    if(showType=="chapter")
        endQuery = QString(" Where Chapter =%1;").arg(mainTndex);

    else if (showType=="juz")
         endQuery = QString(" Where juz =%1;").arg(mainTndex);

    else if (showType=="hizb")
         endQuery = QString(" Where hizb =%1;").arg(mainTndex);
    else if (showType=="page")
         endQuery = QString(" Where Page =%1;").arg(mainTndex);
    else if (showType=="subject")
                 endQuery =  QString(" Left Join Subjects on Quran.id=Subjects.ID_VERSE Where ID_SUBJECT =%1;").arg(mainTndex);

    query = startQuery+endQuery;

    listId = db->getData("Select Quran.id From Quran "+endQuery);
    QString html;
    if(QFile::exists("quranview.html"))
        html =db->ReadFromTextFile("quranview.html").join("\n    ");
    else
        html = db->ReadFromTextFile(":quranview.html").join("\n    ");
    html.replace("_Quran_Value_",db->getData(query).join("\n"));
    QFont fontQuran=db->ReadSettings("QuranFont", QFont()).value<QFont>();
    QFont fontTr=db->ReadSettings("TrFont", QFont()).value<QFont>();

    html.replace("_arabictext_family_",fontQuran.family());
    html.replace("_arabictext_size_",QString::number(fontQuran.pointSize()));
    html.replace("_trtext_direction_",db->getDirection(sampleText));
    html.replace("_trtext_family_",fontTr.family());
    html.replace("_trtext_size_",QString::number(fontTr.pointSize()));
    /*
    if(QFile::exists("sample.html"))
         QFile::remove("sample.html");
     db->WriteToText("sample.html",html);
     qDebug()<< query;
     */

     webView->setHtml(html,QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html")));
    if(listId.size()>0)
      finder(listId.at(0));

}

void QuranWidget::setQuranPeroperty(QString id)
{
    line_Chapter->setValue(db->getDatastr("Select Chapter From Quran Where id="+id).toInt());
    line_Page->setValue(db->getDatastr("Select Page From Quran Where id="+id).toInt());
    label_Juz->setValue(db->getDatastr("Select Juz From Quran Where id="+id).toInt());
    label_Hizb->setValue(db->getDatastr("Select Hizb From Quran Where id="+id).toInt());

}
void QuranWidget::setTranslation(){
    trTableName="None";
    int idtranslation = db->ReadSettings("translation",0).toInt();
       if(idtranslation>trTable.size()-1)
           idtranslation=0;
       trTableName = trTable.at(idtranslation);
}

void QuranWidget::on_comboBoxTr_activated(int index)
{
    if(index==0)
        checkBoxNoShowQuran->setEnabled(false);
    else
        checkBoxNoShowQuran->setEnabled(true);

    db->WriteSettings("translation",index);
    trTableName = trTable.at(index);
    showVerse();
}

void QuranWidget::needExplanations()
{

    ContactForm cotact;
    cotact.setWindowTitle(db->trlang(""));
    cotact.plainTextEdit->setPlainText(db->trlang("Tel Me More about this verse:")+"\n"+db->getDatastr("Select tr From "+trTableName+" Where id ="+currentVerseId)+
                                       "["+db->getDatastr("Select Chapter From Quran Where id ="+currentVerseId)+
                                       ";"+db->getDatastr("Select Verse From Quran Where id ="+currentVerseId)+"]");
    cotact.exec();
}

void QuranWidget::PageChanged()
{
    if(igChange)
             return;
    mainTndex=line_Page->text();
    showType="page";
    showVerse();
}
void QuranWidget::showJuz(QString text){
    if(igChange)
        return;    
    line_Page->setFocus();
    mainTndex=text;
    showType="juz";
    showVerse();
    label_Juz->setFocus();
}
void QuranWidget::showHizb(QString text){
    if(igChange)
        return;  
    line_Page->setFocus();
    mainTndex=text;
    showType="hizb";
    showVerse();
    label_Hizb->setFocus();
}
void QuranWidget::ChapterChanged()
{
    if(igChange)
        return;
    db->WriteSettings("lastChapter",line_Chapter->text());
    line_Page->setFocus();
    mainTndex=line_Chapter->text();
    showType="chapter";
    showVerse();
    line_Chapter->setFocus();
}
void QuranWidget::VerseChanged()
{
    if(igChange)
        return;
   if(!finder(line_Chapter->text(),line_Verse->text()))
      {
      int chapterint =line_Chapter->value();
      int verseint=line_Verse->value();
      if(db->getDatastr("Select Count(id) From Quran Where Chapter="+line_Chapter->text()+" and Verse="+line_Verse->text()+" ;")=="0")
         {
          chapterint++;
          verseint=1;
         }
         igChange=true;
         line_Chapter->setValue(chapterint);
         igChange=false;
         ChapterChanged();
         line_Verse->setValue(verseint);
         finder(line_Chapter->text(),line_Verse->text());

      }
}
void QuranWidget::goToVerseFromSearch(QString str){

    line_Chapter->setValue(str.split(";").at(0).toInt());
      ChapterChanged();
    line_Verse->setValue(str.split(";").at(1).toInt());
     VerseChanged();

}
void QuranWidget::AddToSubjects(){

    emit openAyeExplor(currentVerseId,0);
}
void QuranWidget::showDuplicatesWords(){

    emit openAyeExplor(currentVerseId,3);
}
void QuranWidget::AddToComment(){
    emit openAyeExplor(currentVerseId,1);
}
void QuranWidget::ShowExplanations(){
    emit openAyeExplor(currentVerseId,2);
}
void QuranWidget::PlayQuran()
{
    QStringList idListAdiuo;
    idListAdiuo=listId;
    QStringList ChapterNo,VerseNo;
    if (db->getDatastr("Select Verse From Quran Where id ="+currentVerseId)=="1" &&
       db->getDatastr("Select Chapter From Quran Where id ="+currentVerseId)!="9"
       && db->getDatastr("Select id  From Quran Where id ="+currentVerseId)!="1")
   {
      idListAdiuo.prepend("1");
   }

    currentRow = listId.indexOf(currentVerseId);
    if(currentRow==-1)
        return;
    else
        currentRow++;
     for(int i=currentRow;i<idListAdiuo.size();i++)
     {
      ChapterNo.append(db->getDatastr("Select Chapter From Quran Where id ="+idListAdiuo.at(i)));
      VerseNo.append(db->getDatastr("Select Verse From Quran Where id ="+idListAdiuo.at(i)));
     }

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
void QuranWidget::addBookmark()
{
   if(db->getDatastr("Select Count(id) From BookMarks Where id="+currentVerseId)=="0")
   {
    QStringList f,v;
    f << "id" << "title";
    bool ok;
    QString text = QInputDialog::getText(this, db->trlang("Bookmark"),db->trlang("Title")+":", QLineEdit::Normal,"", &ok);
     if (ok)
       v <<  currentVerseId << text;
     else
        v <<  currentVerseId << "";
      db->insert("BookMarks",f,v);
   }else
      db->deleterow("BookMarks","id",currentVerseId);

    getValue(QVariant(currentVerseId));
}
void QuranWidget::similarVerses()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        emit showRelated(action->text(),action->data().toString());

}

void QuranWidget::zoomIn()
{

    int percent = static_cast<int>(webView->zoomFactor() * 100);
    if (percent < 400) {
        percent += 25;
        percent = 25 * (int(percent / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        webView->setZoomFactor(factor);
        acZoomIn->setEnabled(percent < 400);
        acZoomOut->setEnabled(true);

    }

}
void QuranWidget::zoomQut()
{
    int percent = static_cast<int>(webView->zoomFactor() * 100);
    if (percent > 25)
    {
        percent -= 25;
        percent = 25 * (int((percent + 25 - 1) / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        webView->setZoomFactor(factor);
        acZoomOut->setEnabled(percent > 25);
        acZoomIn->setEnabled(true);
    }
}

bool QuranWidget::finder(QString chapter,QString verse)
{
    QString id = db->getDatastr("Select Id From Quran Where Chapter ="+chapter+" And Verse = "+verse+" ;");
   return finder(id);

}
bool QuranWidget::finder(QString id)
{
    //send to js  
  return webView->page()->mainFrame()->evaluateJavaScript("selectRowF('"+id+"')").toBool();
}



void QuranWidget::on_checkBoxNoShowQuran_clicked()
{
    showVerse();
}
void QuranWidget::keyPressEvent (QKeyEvent *qkeyevent)
{
    switch (qkeyevent->key()){
        case Qt::Key_Up:
        currentRow = listId.indexOf(currentVerseId);
        if(currentRow!=-1)
        {
            currentRow--;
            if(currentRow>=0)
                finder(listId.at(currentRow));
        }
        break;
        case Qt::Key_Down:
        currentRow = listId.indexOf(currentVerseId);
        if(currentRow!=-1)
        {
            currentRow++;
            if(currentRow<listId.size())
                finder(listId.at(currentRow));
        }
        break;
        default:
            qkeyevent->ignore();
    }
}
