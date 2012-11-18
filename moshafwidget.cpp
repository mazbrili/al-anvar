#include "moshafwidget.h"

MoshafWidget::MoshafWidget(DataBase *database,QWidget *parent) :
    QWidget(parent)

{
    setupUi(this);
    db = database;
    textEditQuran->setContextMenuPolicy(Qt::ActionsContextMenu);
    chengeVerse=true;
    stopHighlight=false;

    acVerse= new QAction(0);
    acPlay= new QAction(QIcon(":images/audio.png"),db->trlang("Play"),0);
    acInter= new QAction(QIcon(":images/Explanations.png"),db->trlang("Interpretation"),0);
    acSubject= new QAction(QIcon(":images/tree.png"),db->trlang("Subject of the Verses"),0);
    acComment= new QAction(QIcon(":images/comment_show.png"),db->trlang("Add a Point"),0);
    acMail = new QAction(QIcon(":images/newsletter.png"),db->trlang("need Explanations"),0);
    acSimiler= new QAction(db->trlang("Similar Words"),0);
    acPlay->setShortcut(Qt::CTRL + Qt::Key_Q);
    acInter->setShortcut(Qt::CTRL + Qt::Key_I);
    acSubject->setShortcut(Qt::CTRL + Qt::Key_T);
    acComment->setShortcut(Qt::CTRL + Qt::Key_N);


    labelChapter->setText(db->trlang("Chapter"));
    labelAye->setText(db->trlang("Verse"));
    labelPage->setText(db->trlang("Page"));
    labelJuz->setText(db->trlang("Section"));
    labelHizb->setText(db->trlang("Hizb"));
    //pushButtonStekhare->setText(db->trlang("Stekhare"));

    toolButtonBack->setText(db->trlang("Previous"));
    toolButtonNext->setText(db->trlang("Next"));

    connect(acPlay,SIGNAL(triggered()),this,SLOT(play()));
    connect(acInter,SIGNAL(triggered()),this,SLOT(ShowExplanations()));
    connect(acSubject,SIGNAL(triggered()),this,SLOT(AddToSubjects()));
    connect(acComment,SIGNAL(triggered()),this,SLOT(AddToComment()));
    connect(acSimiler,SIGNAL(triggered()),this,SLOT(showDuplicatesWords()));
    connect(acMail, SIGNAL(triggered()), this, SLOT(needExplanations()));

    textEditQuran->addAction(acPlay);
    textEditQuran->addAction(acInter);
    textEditQuran->addAction(acSubject);
    textEditQuran->addAction(acComment);
    textEditQuran->addAction(acMail);
    textEditQuran->addAction(acSimiler);

    for (int i = 0; i < MaxRecent; ++i) {
       acSub[i] = new QAction(textEditQuran);
       acSub[i]->setIcon(QIcon(":images/tag.png"));
       acSub[i]->setVisible(false);
       connect(acSub[i], SIGNAL(triggered()),
               this, SLOT(similarVerses()));
   }
    for (int i = 0; i < MaxRecent; ++i)
      textEditQuran->addAction(acSub[i]);

    translationList = db->getListData("Select id,Name,TableName From Tr_Translation;");

    setTranslation();
   comboBoxChapter->addItems(db->getData("SELECT  ( Chapter || \"   \" ||  Name ) AS expr1 FROM Quran Left Join ChapterProperty on (Quran.Chapter=ChapterProperty.id) GROUP BY Name ORDER BY Quran.Chapter ; "));
   comboBoxJuz->addItems(db->getData("Select  ( \" "+db->trlang("Section")+" \" || id ) AS expr1 From Juz;"));
   comboBoxHizb->addItems(db->getData("Select  ( \" "+db->trlang("Hizb")+" \" || id ) AS expr1 From Hizb;"));

   spinBoxPage->setValue(1);
   on_spinBoxPage_editingFinished();
   connect(textEditQuran, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
   textEditQuran->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   inPl = new InternalPlayer(0);
   verticalLayoutPlayer->addWidget(inPl);
   if(db->trlang("Align")=="Right")
   {
  inPl->setLayoutDirection(Qt::RightToLeft);
   }
   connect(inPl,SIGNAL(verseChanged(QString,QString)),this,SLOT(finder(QString,QString)));
   listAudio = db->getListData("Select Name,dir_Name,Url From Audio ;");
   foreach (QString str,listAudio.at(0))
   {
     inPl->comb_ghary->addItem(db->trlang(str));
     inPl->comboBoxRep->addItem(db->trlang(str));
   }
   inPl->toolButton->setVisible(false);
   inPl->setVisible(false);

}
void MoshafWidget::setTranslation(){
    translationStr="";
    strTranslation="";
    int idtranslation =      db->ReadSettings("translation",0).toInt();
       translationStr =",tr";
       if(idtranslation>translationList.at(2).size()-1)
           idtranslation=0;
       strTranslation = translationList.at(2).at(idtranslation);


}

void MoshafWidget::getQuranValue(QString sql)
{

    stopHighlight=true;
    textEditQuran->clear();
   versePeropertys.clear();

   mainQuranValue = db->getListData(sql);
            QTextDocument* quranDoc = new QTextDocument(textEditQuran);
            textEditQuran->setDocument(quranDoc);
            QTextCursor* quranCursor = new QTextCursor(quranDoc);
            QTextCharFormat formatAr;
            formatAr.setFont(db->ReadSettings("Quranfont",font()).value<QFont>());
            formatAr.setForeground(db->ReadSettings("QuranColor",QColor()).value<QColor>());

    if (!mainQuranValue.isEmpty()){
    for(int i=0;i<mainQuranValue.at(0).size();i++){
       VersePeroperty vP;
       vP.posStartPeroperty=quranCursor->position();
       vP.idPeroperty=mainQuranValue.at(0).at(i);
       vP.chapterPeroperty=mainQuranValue.at(2).at(i);
       vP.versePeroperty=mainQuranValue.at(3).at(i);
       vP.order = QString::number(i);

       quranCursor->setCharFormat(formatAr);
       QString ve =mainQuranValue.at(3).at(i);

       ve.replace("1",QString::fromUtf8("۱"));
       ve.replace("2",QString::fromUtf8("۲"));
       ve.replace("3",QString::fromUtf8("۳"));
       ve.replace("4",QString::fromUtf8("۴"));
       ve.replace("5",QString::fromUtf8("۵"));
       ve.replace("6",QString::fromUtf8("۶"));
       ve.replace("7",QString::fromUtf8("۷"));
       ve.replace("8",QString::fromUtf8("۸"));
       ve.replace("9",QString::fromUtf8("۹"));
       ve.replace("0",QString::fromUtf8("۰"));

       quranCursor->insertText(mainQuranValue.at(1).at(i)+"("+ve+")");
       vP.posEndPeroperty=quranCursor->position()-1;
       versePeropertys.append(vP);
 }
}

      int pos = textEditQuran->document()->firstBlock().position();
      QTextCursor cursor = textEditQuran->textCursor();
      cursor.setPosition(pos);
      textEditQuran->setTextCursor(cursor);
      textEditQuran->setFocus();
      stopHighlight=false;
      textEditQuran->setAlignment(Qt::AlignJustify);



}
void MoshafWidget::highlightCurrentLine()
{


   int pos = textEditQuran->textCursor().position();
            foreach ( VersePeroperty vp, versePeropertys)
            {
                if(pos>=vp.posStartPeroperty && pos<=vp.posEndPeroperty)
                {
                QTextCursor c = textEditQuran->textCursor();
                c.setPosition(vp.posStartPeroperty);
                c.setPosition(vp.posEndPeroperty, QTextCursor::KeepAnchor);
                textEditQuran->setTextCursor(c);
                comboBoxChapter->setCurrentIndex(db->getDatastr(tr("Select Chapter From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt()-1);
                spinBoxVerse->setValue(db->getDatastr(tr("Select Verse From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt());
                comboBoxJuz->setCurrentIndex(db->getDatastr(tr("Select Juz From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt()-1);
                comboBoxHizb->setCurrentIndex(db->getDatastr(tr("Select Hizb From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt()-1);
                spinBoxPage->setValue(db->getDatastr(tr("Select Page From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt());


                QString trStr = db->getDatastr(tr("Select tr From %1 Where id=%2 ;").arg(strTranslation).arg(vp.idPeroperty));
                textEditTr->setText(trStr);
                currentVersePeroperty =vp;
                }
            }

}

void MoshafWidget::finder(QString ch,QString vr)
{
    bool finded = false;
        foreach ( VersePeroperty vp, versePeropertys)
             {
                 if(ch==vp.chapterPeroperty && vr==vp.versePeroperty)
                 {
                 QTextCursor c = textEditQuran->textCursor();
                 c.setPosition(vp.posStartPeroperty);
                 c.setPosition(vp.posEndPeroperty, QTextCursor::KeepAnchor);
                 textEditQuran->setTextCursor(c);
                 comboBoxChapter->setCurrentIndex(db->getDatastr(tr("Select Chapter From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt()-1);
                 comboBoxJuz->setCurrentIndex(db->getDatastr(tr("Select Juz From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt()-1);
                 comboBoxHizb->setCurrentIndex(db->getDatastr(tr("Select Hizb From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt()-1);
                 spinBoxPage->setValue(db->getDatastr(tr("Select Page From Quran Where id=%1 ;").arg(vp.idPeroperty)).toInt());
                 QString trStr = db->getDatastr(tr("Select tr From %1 Where id=%2 ;").arg(strTranslation).arg(vp.idPeroperty));
                 textEditTr->setText(trStr);
                 currentVersePeroperty =vp;
                 currentVersePeroperty =vp;
                 finded=true;
                 }
             }
        if(!finded)
        {
            spinBoxPage->setValue(spinBoxPage->value()+1);
            on_spinBoxPage_editingFinished();
        }

}
void MoshafWidget::on_comboBoxChapter_activated(int arg1)
{

   // spinBoxVerse->setMaximum(db->getData(QString("Select Verse From Quran Where Chapter =%1 ").arg(comboBoxChapter->currentIndex()+1)).size()+1);
    QString page = db->getDatastr(tr("Select Page From Quran Where Chapter =%1").arg(arg1+1));
    spinBoxPage->setValue(page.toInt());
    on_spinBoxPage_editingFinished();
}



void MoshafWidget::on_spinBoxPage_editingFinished()
{
    getQuranValue(tr("Select id,Arabic,Chapter,Verse From Quran  Where Page = %1 ;").arg(spinBoxPage->text()));
    db->WriteSettings("lastChapterM",spinBoxPage->value());
}

void MoshafWidget::on_comboBoxJuz_activated(int arg1)
{
    QString page = db->getDatastr(tr("Select Page From Quran Where Juz =%1").arg(arg1+1));
    spinBoxPage->setValue(page.toInt());
    on_spinBoxPage_editingFinished();
}

void MoshafWidget::on_comboBoxHizb_activated(int arg1)
{
    QString page = db->getDatastr(tr("Select Page From Quran Where Hizb =%1").arg(arg1+1));
    spinBoxPage->setValue(page.toInt());
    on_spinBoxPage_editingFinished();
}

void MoshafWidget::on_toolButtonNext_clicked()
{
   spinBoxPage->setValue(spinBoxPage->value()+1);
   on_spinBoxPage_editingFinished();
}

void MoshafWidget::on_toolButtonBack_clicked()
{
    spinBoxPage->setValue(spinBoxPage->value()-1);
    on_spinBoxPage_editingFinished();
}
void MoshafWidget::AddToSubjects(){

    emit openAyeExplor(currentVersePeroperty.idPeroperty,0);
}
void MoshafWidget::showDuplicatesWords(){

    emit openAyeExplor(currentVersePeroperty.idPeroperty,3);


}
void MoshafWidget::AddToComment(){
    emit openAyeExplor(currentVersePeroperty.idPeroperty,1);

}
void MoshafWidget::ShowExplanations(){
    emit openAyeExplor(currentVersePeroperty.idPeroperty,2);
}
void MoshafWidget::play()
{
    inPl->setVisible(true);

    QStringList idListAdiuo;
    if(!mainQuranValue.isEmpty())
    idListAdiuo=mainQuranValue.at(0);


    QStringList ChapterNo,VerseNo;
    if (db->getDatastr("Select Verse From Quran Where id ="+mainQuranValue.at(0).at(currentVersePeroperty.order.toInt()))=="1" &&
       db->getDatastr("Select Chapter From Quran Where id ="+mainQuranValue.at(0).at(currentVersePeroperty.order.toInt()))!="9"
       && db->getDatastr("Select id  From Quran Where id ="+mainQuranValue.at(0).at(currentVersePeroperty.order.toInt()))!="1")
   {
      idListAdiuo.prepend("1");

   }

     for(int i=currentVersePeroperty.order.toInt();i<idListAdiuo.size();i++){
   ChapterNo.append(db->getDatastr("Select Chapter From Quran Where id ="+idListAdiuo.at(i)));
   VerseNo.append(db->getDatastr("Select Verse From Quran Where id ="+idListAdiuo.at(i)));
   };
    inPl->comb_ghary->setCurrentIndex(db->ReadSettings("Audio",0).toInt());
    inPl->comboBoxRep->setCurrentIndex(db->ReadSettings("Audio2",inPl->comboBoxRep->count()-1).toInt());
    inPl->checkBoxRep->setChecked(db->ReadSettings("PlayAudio2",false).toBool());
    inPl->idaudio(listAudio.at(2),
                db->pathAudio,listAudio.at(1),
                ChapterNo,VerseNo);

    inPl->readyToPlay();

}
void MoshafWidget::similarVerses()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        QString sql = "Select Quran.id,Arabic,Quran.Chapter,Quran.Verse"+translationStr+" From Subjects Left Join quran On (Subjects.ID_VERSE=quran.id) ?translation left join ChapterProperty on (quran.Chapter=ChapterProperty.id)  Where Subjects.ID_SUBJECT ="+action->data().toString()+";";
        emit showRelated(action->text(),sql);
    }

}
void MoshafWidget::needExplanations()
{

    ContactForm cotact;
    cotact.setWindowTitle(db->trlang(""));
    cotact.plainTextEdit->setPlainText(db->trlang("Tel Me More about this verse:")+"\n"+db->getDatastr("Select tr From "+strTranslation+" Where id ="+currentVersePeroperty.idPeroperty)+
                                       "["+db->getDatastr("Select Chapter From Quran Where id ="+currentVersePeroperty.idPeroperty)+
                                       ";"+db->getDatastr("Select Verse From Quran Where id ="+currentVersePeroperty.idPeroperty)+"]");
    cotact.exec();
}


void MoshafWidget::on_spinBoxVerse_valueChanged(const QString &arg1)
{
    finder(QString::number(comboBoxChapter->currentIndex()+1),arg1);
}
