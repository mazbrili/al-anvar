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
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "tools.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAbstractTextDocumentLayout>


class Downloader : public QObject
 {
    Q_OBJECT
    public:
    QString fileName_,urlStr_;
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    bool httpRequestAborted;
    QProgressBar *progressBar;
    QLabel *labelDownlod;
    tools tool;
    bool downloded;
    Downloader(QProgressBar *progress,QLabel *label,QObject *parent = 0);
    void downloadFile(QString urlStr,QString fileName);
   public slots:
    void startRequest(QUrl url);
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
signals:
    void progressEvent(bool);


};

#endif // DOWNLOADER_H
