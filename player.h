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
    explicit InternalPlayer(QWidget *parent = 0);
    void idaudio(QStringList url,QString dir_,QStringList listAudio_,QStringList Chapter,QStringList Verse);

public slots:
    void playPuseSlot();
 void cheangeGhary();
 void cheangeGhary2();
 void savePlayAudio2();
 void readyToPlay();
 void setCurrentSource(QString source);
 void play();
 void stop();
 void pause();
 void videoPlayerTick(qint64 time);
 void totalTimeChanged(qint64 time);
 void closePlayer();

signals:
    void totalTimeC(qint64 time);
    void verseChanged(QString chapter,QString verse_);

protected:
    virtual void hideEvent(QHideEvent *event);
    virtual void closeEvent(QCloseEvent *event);
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


    explicit     ExternalPlayer(QWidget *parent=0);
    void idaudio(QStringList url,QString dir_,QStringList listAudio_,QStringList Chapter,QStringList Verse);

public slots:
    void closePlayer();
    void cheangeGhary();
    void readyToPlay();

protected:
    virtual void hideEvent(QHideEvent *event);
    virtual void closeEvent(QCloseEvent *event);
signals:
      void verseChanged(QString chapter,QString verse_);


};

#endif // PLAYER_H
