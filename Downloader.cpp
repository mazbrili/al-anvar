#include "Downloader.h"

Downloader::Downloader(QProgressBar *progress,QLabel *label,QObject *parent ):
    QObject(parent)
{

 progressBar=progress;
 labelDownlod=label;
 downloded=false;
 connect(this,SIGNAL(progressEvent(bool)),progressBar,SLOT(setVisible(bool)));



}
void Downloader::downloadFile(QString urlStr,QString fileName)
{
    downloded=false;
    urlStr_=urlStr;
    url = urlStr_;
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(0, tr("HTTP"),
                                 tool.trlang("There already exists a file called %1 in the current directory. Overwrite?").arg(fileName),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return;
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, tr("HTTP"),
                                 tool.trlang("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    // schedule the request

    httpRequestAborted = false;
    startRequest(url);
}
void Downloader::startRequest(QUrl url)
{
    QEventLoop loop;
    emit progressEvent(true);

    labelDownlod->setText(tool.trlang("Downloding"));

   reply = qnam.get(QNetworkRequest(url));
   connect(reply, SIGNAL(finished()),
           this, SLOT(httpFinished()));
   connect(reply, SIGNAL(readyRead()),
           this, SLOT(httpReadyRead()));
   connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
           this, SLOT(updateDataReadProgress(qint64,qint64)));
   connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
   loop.exec();
}
void Downloader::httpFinished()
{
    if (httpRequestAborted)
    {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
    }
        reply->deleteLater();
        emit progressEvent(false);
        labelDownlod->setText("");
        downloded=false;


        return;
    }

    emit progressEvent(false);

    labelDownlod->setText("");

    file->flush();
    file->close();


    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(0, tr("HTTP"),
                                 tool.trlang("Download failed: %1.")
                                 .arg(reply->errorString()));
        downloded=false;

    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(0, tr("HTTP"),
                                  tool.trlang("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            downloded=false;

            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
    }
     if (!reply->error())
           downloded=true;

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
}
void Downloader::httpReadyRead()
{

    if (file)
        file->write(reply->readAll());
}
void Downloader::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    progressBar->setMaximum(totalBytes);
    progressBar->setValue(bytesRead);
    double bytesReadkb =bytesRead/1024;
    double totalByteskb =totalBytes/1024;

    double  bytesReadmb = static_cast<double> (bytesReadkb)/1024;
    double  totalBytesmb = static_cast<double> (totalByteskb)/1024;


    QString tbyte=QString::number(totalBytesmb);
    if(tbyte.size()>5)
    tbyte.remove(5,tbyte.size()-5);

    QString byte=QString::number(bytesReadmb);
    if(byte.size()>5)
    byte.remove(5,byte.size()-5);
    labelDownlod->setText(byte+" MB of "+tbyte+" MB");
}


