#include "player.h"

InternalPlayer::InternalPlayer(QWidget *parent) :
    QWidget(parent)
{

    setupUi(this);
    setAcceptDrops(true);
    slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(&m_MediaObject);
    volume = new Phonon::VolumeSlider(&m_AudioOutput);

    connect(&m_MediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(totalTimeChanged(qint64)));
    connect(&m_MediaObject, SIGNAL(finished()), this, SLOT(readyToPlay()));
    m_audioOutputPath = Phonon::createPath(&m_MediaObject, &m_AudioOutput);



    nowPlaySec=false;

    currentTotalTime=0;
    connect(toolButton,SIGNAL(clicked()),this,SLOT(closePlayer()));
    checkBoxRep->setText(tool.trlang("repeat with:"));
    verse=0;
    label_ghary->setText(tool.trlang("Reciter")+" : ");
        lableRepeat->setText(tool.trlang("Repeat"));
    progressBar->setVisible(false);
    dowloder = new Downloader(progressBar,label_Down,this);
    volumeLayout->addWidget(volume);
    seekTimeLayout->addWidget(slider);
   // videoLayout->addWidget(videoPlayer);
    connect(comb_ghary, SIGNAL(activated(int)), this, SLOT(cheangeGhary()));
    connect(comboBoxRep, SIGNAL(activated(int)), this, SLOT(cheangeGhary2()));
    connect(checkBoxRep, SIGNAL(clicked()), this, SLOT(savePlayAudio2()));

    connect(playPuse,SIGNAL(clicked()),this,SLOT(playPuseSlot()));


}
void InternalPlayer::idaudio(QStringList url,QString dir_,QStringList listAudio_,QStringList Chapter,QStringList Verse)
{
   url_=url;
   verse=0;
   repeat=0;
   chapterList=Chapter;
   veseList=Verse;
   dir=dir_;
   listAudio=listAudio_;
   label_ghary->setText(tool.trlang("Reciter")+" : ");

                       //      tool.trlang("RECITER",db->getData("Select Name From Audio ;").at(tool.ReadSettings("Audio","0").toInt()))
  QString result;
  QStringList ResultList;
  for(int i =0;i<Chapter.count();i++)
  {

    int ch = Chapter.at(i).toInt();
    if (ch<10)
        result= "00"+Chapter.at(i);
    else if(ch<100)
        result= "0"+Chapter.at(i);
    else
        result= Chapter.at(i);

    int ve = Verse.at(i).toInt();
    if (ve<10)
        result+= "00"+Verse.at(i);
    else if(ve<100)
        result+= "0"+Verse.at(i);
    else
        result+= Verse.at(i);

      ResultList << "%1/"+result+".mp3";
    }

 listAddress= ResultList;

}
void InternalPlayer::playPuseSlot()
{
    if(playPuse->text()=="Play")
        play();
    else
       pause();
}
void InternalPlayer::cheangeGhary()
{
    tool.WriteSettings("Audio",comb_ghary->currentIndex());
}
void InternalPlayer::cheangeGhary2()
{
    tool.WriteSettings("Audio2",comboBoxRep->currentIndex());

}
void InternalPlayer::savePlayAudio2()
{
    tool.WriteSettings("PlayAudio2",checkBoxRep->isChecked());

}
void InternalPlayer::readyToPlay()
{

    if(!nowPlaySec)
    {
    if(verse>=listAddress.size())
    {
        stop();
       setVisible(false);
        return;
    }


    int currentGhary=0;
    if(listAudio.size()>comb_ghary->currentIndex())
        currentGhary=comb_ghary->currentIndex();
    QString dirFull = dir+listAudio.at(tool.ReadSettings("Audio",currentGhary).toInt());
    QString adressFull= listAddress.at(verse).arg(dirFull);

    QFile  file(adressFull);
    if(file.size()==0)
         file.remove();

       if(!file.exists()){
           QFileInfo fileinfo(adressFull);
           dowloder->downloadFile(url_.at(tool.ReadSettings("Audio",comb_ghary->currentIndex()).toInt())+fileinfo.fileName(),adressFull);
           QFile downloaded(adressFull);
                 if(downloaded.size()==0){
                     downloaded.remove();
                     stop();
                    setVisible(false);
                     return;
                 }
       }
       setCurrentSource(adressFull);
       play();
       emit verseChanged(chapterList.at(verse),veseList.at(verse));
       lableChapteVerse->setText(tool.trlang("Chapter")+": "+chapterList.at(verse)+tool.trlang("Verse")+": "+veseList.at(verse));
        repeat++;
        if(checkBoxRep->isChecked())
        nowPlaySec=true;
    }else
    {
        if(verse>=listAddress.size())
        {
            stop();
           setVisible(false);
            return;
        }


        int currentGhary=comboBoxRep->count()-1;
        if(listAudio.size()>comboBoxRep->currentIndex())
            currentGhary=comboBoxRep->currentIndex();
        QString dirFull = dir+listAudio.at(tool.ReadSettings("Audio2",currentGhary).toInt());
        QString adressFull= listAddress.at(verse).arg(dirFull);

        QFile  file(adressFull);
        if(file.size()==0)
             file.remove();

           if(!file.exists()){
               QFileInfo fileinfo(adressFull);
               dowloder->downloadFile(url_.at(tool.ReadSettings("Audio2",comboBoxRep->currentIndex()).toInt())+fileinfo.fileName(),adressFull);
               QFile downloaded(adressFull);
                     if(downloaded.size()==0){
                         downloaded.remove();
                         stop();
                         setVisible(false);
                         return;
                     }
           }
           setCurrentSource(adressFull);
           play();
           emit verseChanged(chapterList.at(verse),veseList.at(verse));
           lableChapteVerse->setText(tool.trlang("Chapter")+": "+chapterList.at(verse)+tool.trlang("Verse")+": "+veseList.at(verse));
            nowPlaySec=false;
    }
       if(repeat>spinBox->value()-1 && !nowPlaySec)
       {
         verse++;
         repeat=0;

       }
}
void InternalPlayer::setCurrentSource(QString source)
{
    m_MediaObject.setCurrentSource(source);

}
void InternalPlayer::play()
{
      if (m_MediaObject.state() == Phonon::PlayingState)
        stop();
    m_MediaObject.play();
    playPuse->setText("Pause");
     playPuse->setIcon(QIcon(":images/pause.png"));

}
void InternalPlayer::stop()
{
    m_MediaObject.stop();
      playPuse->setText("Play");
       playPuse->setIcon(QIcon(":images/playback.png"));
}
void InternalPlayer::pause()
{
    m_MediaObject.pause();
      playPuse->setText("Play");
      playPuse->setIcon(QIcon(":images/playback.png"));
}
void InternalPlayer::videoPlayerTick(qint64 time)
{
    QTime displayTime(time / (60000*60), (time / 60000) % 60, (time / 1000) % 60);
    labelCurrentTime->setText(displayTime.toString("hh:mm:ss"));
}
void InternalPlayer::totalTimeChanged(qint64 time)
{
    QTime displayTime(time / (60000*60), (time / 60000) % 60, (time / 1000) % 60);
    labelTotalTime->setText(displayTime.toString("hh:mm:ss"));
   emit totalTimeC (time/ 1000);

}
void InternalPlayer::closePlayer()
{
      setVisible(false);
      stop();
      listAddress.clear();

}
void InternalPlayer::hideEvent(QHideEvent *event)
  {
   // stop();
    listAddress.clear();

  }
void InternalPlayer::closeEvent(QCloseEvent *event){
      stop();
      listAddress.clear();

   }


ExternalPlayer::ExternalPlayer(QWidget *parent) :
    QWidget(parent)
{

    setupUi(this);
    verse=0;
    connect(&player,SIGNAL(finished(int)),SLOT(readyToPlay()));
    label_ghary->setText(tool.trlang("Reciter")+" : ");
    spinBox->setVisible(false);
    lableRepeat->setVisible(false);
    comboBoxRep->setVisible(false);
    checkBoxRep->setVisible(false);
    playPuse->setVisible(false);
    labelTotalTime->setVisible(false);
    labelCurrentTime->setVisible(false);
    dowloder = new Downloader(progressBar,label_Down,this);
    connect(comb_ghary, SIGNAL(activated(int)), this, SLOT(cheangeGhary()));
    connect(toolButton,SIGNAL(clicked()),this,SLOT(closePlayer()));


}
void ExternalPlayer::idaudio(QStringList url,QString dir_,QStringList listAudio_,QStringList Chapter,QStringList Verse)
{

   url_=url;
   verse=0;
   repeat=0;
   chapterList=Chapter;
   veseList=Verse;
   label_ghary->setText(tool.trlang("Reciter")+" : ");
   dir=dir_;
   listAudio=listAudio_;

  QString result;
  QStringList ResultList;
  for(int i =0;i<Chapter.count();i++)
  {

    int ch = Chapter.at(i).toInt();
    if (ch<10)
        result= "00"+Chapter.at(i);
    else if(ch<100)
        result= "0"+Chapter.at(i);
    else
        result= Chapter.at(i);

    int ve = Verse.at(i).toInt();
    if (ve<10)
        result+= "00"+Verse.at(i);
    else if(ve<100)
        result+= "0"+Verse.at(i);
    else
        result+= Verse.at(i);

      ResultList << "%1/"+result+".mp3";
    }

 listAddress= ResultList;

}
void ExternalPlayer::closePlayer()
{
      setVisible(false);
      listAddress.clear();

}
void ExternalPlayer::cheangeGhary()
{
    tool.WriteSettings("Audio",comb_ghary->currentIndex());
}
void ExternalPlayer::readyToPlay()
{


    QString  ExternalPlayerName =tool.ReadSettings("ExternalPlayer","mplayer").toString();

    if(verse>=listAddress.size())
    {
        player.close();
        listAddress.clear();
        setVisible(false);
        return;
    }
    if(ExternalPlayerName!="mplayer" && verse>0)
    {
        player.close();
        listAddress.clear();
       setVisible(false);
        return;
    }

    int currentGhary=0;
    if(listAudio.size()>comb_ghary->currentIndex())
        currentGhary=comb_ghary->currentIndex();
    QString dirFull = dir+listAudio.at(tool.ReadSettings("Audio",currentGhary).toInt());
    QString adressFull= listAddress.at(verse).arg(dirFull);

       QFile  file(adressFull);
       if(file.size()==0)
            file.remove();

          if(!file.exists()){
              QFileInfo fileinfo(adressFull);
              dowloder->downloadFile(url_.at(tool.ReadSettings("Audio",currentGhary).toInt())+fileinfo.fileName(),adressFull);
              QFile downloaded(adressFull);
                    if(downloaded.size()==0){
                        downloaded.remove();
                        player.close();
                        setVisible(false);
                        return;
                    }
          }

       player.start(ExternalPlayerName, QStringList(adressFull), QIODevice::NotOpen );
       emit verseChanged(chapterList.at(verse),veseList.at(verse));
       lableChapteVerse->setText(tool.trlang("Chapter")+": "+chapterList.at(verse)+tool.trlang("Verse")+": "+veseList.at(verse));
        repeat++;
       if(repeat>spinBox->value()-1){
         verse++;
         repeat=0;
       }
}
void ExternalPlayer::hideEvent(QHideEvent *event)
  {
     player.close();
     listAddress.clear();
     setVisible(false);

  }
void ExternalPlayer::closeEvent(QCloseEvent *event){
      player.close();
      listAddress.clear();
      setVisible(false);
   }
