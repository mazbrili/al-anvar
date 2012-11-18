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
#ifndef PLAYER_H
#define PLAYER_H


#include "database.h"
#include "Downloader.h"
#include "ui_player.h"
#include <SeekSlider>
#include <VolumeSlider>
#include <BackendCapabilities>
#include <MediaSource>
#include <MediaObject>
#include <AudioOutput>
#include <VolumeSlider>
#include <VideoPlayer>
#include <VideoWidget>
#include <SeekSlider>



namespace Ui {
    class Player;
}

class InternalPlayer : public QWidget, public Ui::Player
{
    Q_OBJECT
public:
    QStringList listAddress,chapterList,veseList,listAudio,url_;

    tools tool;
    QString dir;
    int verse,repeat;  
    bool nowPlaySec;
    qint64 currentTotalTime;
    Downloader *dowloder;

    Phonon::MediaObject m_MediaObject;
    Phonon::AudioOutput m_AudioOutput;
    Phonon::Path m_audioOutputPath;
    Phonon::SeekSlider *slider;
    Phonon::VolumeSlider *volume;

    Phonon::MediaSource mSources;



    explicit InternalPlayer(QWidget *parent = 0) :
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


    void idaudio(QStringList url,QString dir_,QStringList listAudio_,QStringList Chapter,QStringList Verse)
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

public slots:
    void playPuseSlot()
    {
        if(playPuse->text()=="Play")
            play();
        else
           pause();
    }
    void cheangeGhary()
    {
        tool.WriteSettings("Audio",comb_ghary->currentIndex());
    }
    void cheangeGhary2()
    {
        tool.WriteSettings("Audio2",comboBoxRep->currentIndex());

    }
    void savePlayAudio2()
    {
        tool.WriteSettings("PlayAudio2",checkBoxRep->isChecked());

    }

    void readyToPlay()
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
    void setCurrentSource(QString source)
    {
        m_MediaObject.setCurrentSource(source);

    }
    void play()
    {
          if (m_MediaObject.state() == Phonon::PlayingState)
            stop();
        m_MediaObject.play();
        playPuse->setText("Pause");
         playPuse->setIcon(QIcon(":images/pause.png"));

    }
    void stop()
    {
        m_MediaObject.stop();
          playPuse->setText("Play");
           playPuse->setIcon(QIcon(":images/playback.png"));
    }
    void pause()
    {
        m_MediaObject.pause();
          playPuse->setText("Play");
          playPuse->setIcon(QIcon(":images/playback.png"));
    }
    void videoPlayerTick(qint64 time)
    {
        QTime displayTime(time / (60000*60), (time / 60000) % 60, (time / 1000) % 60);
        labelCurrentTime->setText(displayTime.toString("hh:mm:ss"));
    }
    void totalTimeChanged(qint64 time)
    {
        QTime displayTime(time / (60000*60), (time / 60000) % 60, (time / 1000) % 60);
        labelTotalTime->setText(displayTime.toString("hh:mm:ss"));
       emit totalTimeC (time/ 1000);

    }
    void closePlayer()
    {
          setVisible(false);
          stop();
          listAddress.clear();

    }

signals:
    void totalTimeC(qint64 time);
    void verseChanged(QString chapter,QString verse_);


protected:
    virtual void hideEvent(QHideEvent *event)
      {
       // stop();
        listAddress.clear();

      }
    virtual void closeEvent(QCloseEvent *event){
          stop();
          listAddress.clear();

       }


};

class ExternalPlayer : public QWidget,public Ui::Player
{
    Q_OBJECT
public:
    QProcess player;
    QStringList listAddress,chapterList,veseList,listAudio,url_;
    tools tool;
    QString dir;
    int verse,repeat;
    Downloader *dowloder;


    explicit     ExternalPlayer(QWidget *parent=0) :
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

    void idaudio(QStringList url,QString dir_,QStringList listAudio_,QStringList Chapter,QStringList Verse)
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

public slots:
    void closePlayer()
    {
          setVisible(false);
          listAddress.clear();

    }
    void cheangeGhary()
    {
        tool.WriteSettings("Audio",comb_ghary->currentIndex());
    }
    void readyToPlay()
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

protected:
    virtual void hideEvent(QHideEvent *event)
      {
         player.close();
         listAddress.clear();
         setVisible(false);

      }
    virtual void closeEvent(QCloseEvent *event){
          player.close();
          listAddress.clear();
          setVisible(false);
       }
signals:
      void verseChanged(QString chapter,QString verse_);


};

#endif // PLAYER_H
