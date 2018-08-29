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
#include "addons.h"

Addons::Addons(DataBase *database,QWidget *parent) :
    QDialog(parent)

{
    setupUi(this);
    db = database;
    mostclose=false;


    QFont defultfont = db->ReadSettings("ProgFont","Tahoma,8,-1,5,50,0,0,0,0,0").value<QFont>();
    #ifdef  Q_OS_LINUX
   defultfont = QFont();
  #endif
    QList<QWidget*> allWidget =findChildren<QWidget*>();
    foreach(QWidget* wid,allWidget)
            wid->setFont(defultfont);


    downlod=new Downloader(progressDownload,label,this);
    widget->setVisible(false);
    connect(downlod,SIGNAL(progressEvent(bool)),widget,SLOT(setVisible(bool)));


    treeChargeAddons(treeWidget);
}


void Addons::treeChargeAddons(QTreeWidget *view)
{
    QTreeWidgetItem *rootItem=new QTreeWidgetItem(view) ;
    QTreeWidgetItem *item=new QTreeWidgetItem(rootItem) ;
    QFile file(db->pathUser+"/addonse-0.5.0.xml");
    file.open(QIODevice::ReadOnly);
    view->clear();
    view->setColumnCount(9);
    view->setAutoExpandDelay(1000);
    view->setAlternatingRowColors(true);
   // view->setHeaderHidden(true);



    QXmlStreamReader xml;
    xml.setDevice(&file);
    while (!xml.atEnd()) {

        xml.readNext();
        if (xml.name() == "root"){
            QString name=xml.attributes().value("Name").toString();
            QString type=xml.attributes().value("type").toString();
            if(!name.isNull()){
                rootItem= new QTreeWidgetItem(view);
                rootItem->setText(0,db->trlang(name));
                rootItem->setText(1,type);
            }
        }else if (xml.name() == "Item"){
            QString name= xml.attributes().value("Name").toString();
            QString type=xml.attributes().value("type").toString();
            QString typep=xml.attributes().value("typep").toString();
            QString lang=xml.attributes().value("lang").toString();
            QString date= xml.attributes().value("date").toString();
            QString size=xml.attributes().value("size").toString();
            QString url=xml.attributes().value("url").toString();
            QString text= xml.attributes().value("text").toString();
            QString dir= xml.attributes().value("dir").toString();


            //name,lang,date,size,url


           // if(name!="")
           // makeTable <<QString("                <tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td><a href=\"%5\" target=\"_top\" >Download</a></td></tr>").arg(name).arg(lang).arg(date).arg(size).arg(url);

            if(!name.isNull()){
                item= new QTreeWidgetItem(rootItem);
                item->setIcon(0,QIcon(":images/ok.png"));

                if(typep=="tr"){
                    if(db->getDatastr("SELECT name FROM sqlite_master WHERE name='"+text+"' ;")!=text)
                    {
                        item->setCheckState(0,Qt::Unchecked);
                        item->setIcon(0,QIcon(":images/Download.png"));

                    }

                }else if(typep=="ex"){
                    if(db->getDatastr("SELECT count(id) FROM Explanation_Name where Name ='"+text+"' ;")=="0")
                        {
                            item->setCheckState(0,Qt::Unchecked);
                            item->setIcon(0,QIcon(":images/Download.png"));

                        }


                } else if(typep=="lang" or typep=="help"){
                    QFile file(db->pathUser+"/"+dir+"/"+text);
                    if(!file.exists())
                    {
                        item->setCheckState(0,Qt::Unchecked);
                        item->setIcon(0,QIcon(":images/Download.png"));

                    }

                }
                item->setText(0,db->trlang(name));
                item->setText(1,type);
                item->setText(2,typep);
                item->setText(3,db->trlang(lang));
                item->setText(4,date);
                item->setText(5,size);
                item->setText(6,url);
                item->setText(7,text);
                item->setText(8,dir);

            }
        }
    }
    xml.clear();

    //audio
    rootItem= new QTreeWidgetItem(view);
    rootItem->setIcon(0,QIcon(":images/audio.png"));

    rootItem->setText(0,db->trlang("Reciter"));
    rootItem->setText(1,"Reciter");
    QList<QStringList> audioList = db->getUpListData("Select Name,dir_Name,Url From Audio order by ord;");
    foreach (QStringList list,audioList)
    {
         item= new QTreeWidgetItem(rootItem);
         item->setText(0,db->trlang(list.at(0)));
         item->setText(1,"item");
         item->setText(2,"Reciter");
         item->setText(3,"");
         item->setText(4,"");
         item->setText(5,"");
         item->setText(6,list.at(2));
         item->setText(7,"");
         item->setText(8,list.at(1));
         item->setCheckState(0,Qt::Unchecked);
         item->setIcon(0,QIcon(":images/Download.png"));



    }
    view->setColumnHidden(1,true);
    view->setColumnHidden(2,true);
    view->setColumnHidden(6,true);
    view->setColumnHidden(7,true);
    view->setColumnHidden(8,true);
    QStringList labels;
    labels << db->trlang("Name") << "" << "" << db->trlang("Lang") << db->trlang("Date") << db->trlang("Size")
           << "" << "" << "";
    view->setHeaderLabels(labels);
    view->setColumnWidth(0,250);
    view->setColumnWidth(5,50);


    file.close();
}

void Addons::on_pushButtonDownload_clicked()
{
    mostclose = false;
   QList<QTreeWidgetItem *>items= treeWidget->findItems("item",Qt::MatchContains|Qt::MatchRecursive,1);
   int totalProgress=0;
   QStringList fileNameDownloaded;

   foreach(QTreeWidgetItem *item,items)
       if (item->checkState(0)==Qt::Checked)
           totalProgress++;

   if(totalProgress>1)
       progressTotal->setVisible(true);
   else
       progressTotal->setVisible(false);


   progressTotal->setMaximum(totalProgress);
   int progress=1;
   foreach(QTreeWidgetItem *item,items){
       if(mostclose)
           return;
       if (item->checkState(0)==Qt::Checked)
       {
           if(item->text(2)=="Reciter")
           {
              progressTotal->setVisible(true);
              progressTotal->setMaximum(6236);
              QStringList idus= db->getData("Select audio From Quran ;");
              int i =0;
              QString fileName;
              foreach (QString id,idus)
              {
                  if(mostclose)
                      return;
                  fileName = db->pathUser+"/Audio"+item->text(8)+"/"+id+".mp3";
                  if(!QFile::exists(fileName))
                      downlod->downloadFile(item->text(6)+id+".mp3",fileName);

                  i++;
                  progressTotal->setValue(i);

              }

           }else{
              QUrl url(item->text(6));
              QFileInfo fileInfo(url.path());
              QString fileName = fileInfo.fileName();
              fileName=db->pathUser+"/"+item->text(8)+"/"+fileName;
              downlod->downloadFile(item->text(6),fileName);
              if(downlod->downloded )
               {
                  if(item->text(2)=="tr" or  item->text(2)=="ex")
                              fileNameDownloaded << fileName;
               }
             progressTotal->setValue(progress);
             progress++;
           }
       }

   }
   emit downlodFinishd(fileNameDownloaded);
   treeChargeAddons(treeWidget);

}

/*
void Addons::on_pushButtonDownload_clicked()
{
    mostclose = false;
   QList<QTreeWidgetItem *>items= treeWidget->findItems("item",Qt::MatchContains|Qt::MatchRecursive,1);
   int totalProgress=0;
   QStringList fileNameDownloaded;

   foreach(QTreeWidgetItem *item,items)
       if (item->checkState(0)==Qt::Checked)
           totalProgress++;

   if(totalProgress>1)
       progressTotal->setVisible(true);
   else
       progressTotal->setVisible(false);


   progressTotal->setMaximum(totalProgress);
   int progress=1;
   foreach(QTreeWidgetItem *item,items)
   {
       if (item->checkState(0)==Qt::Checked && item->text(2)!="Reciter" )
       {
              QUrl url(item->text(6));
              QFileInfo fileInfo(url.path());
              QString fileName = fileInfo.fileName();
              fileName=db->pathUser+"/"+item->text(8)+"/"+fileName;
              downlod->downloadFile(item->text(6),fileName);
              if(downlod->downloded )
               {
                  if(item->text(2)=="tr" or  item->text(2)=="ex")
                              fileNameDownloaded << fileName;
               }
             progressTotal->setValue(progress);
             progress++;           
       }
   }
   emit downlodFinishd(fileNameDownloaded);
  // treeChargeAddons(treeWidget);
   foreach (QTreeWidgetItem *item,items)
   {
       if(mostclose)
             return;
       if(item->checkState(0)==Qt::Checked && item->text(2)=="Reciter")
       {
           qDebug()<<item->text(0);

          progressTotal->setVisible(true);
          progressTotal->setMaximum(6236);
          QStringList idus= db->getData("Select audio From Quran ;");
          int i =0;
          QString fileName;
          QFile recFiles;
          foreach (QString id,idus)
          {
              if(mostclose)
                  return;
              fileName = db->pathUser+"/Audio"+item->text(8)+"/"+id+".mp3";
              recFiles.setFileName(fileName);
              if(recFiles.exists())
                  if(recFiles.size()==0){
                      recFiles.remove();
                      downlod->downloadFile(item->text(6)+id+".mp3",fileName);
                  }
              else
                  downlod->downloadFile(item->text(6)+id+".mp3",fileName);
              i++;
              progressTotal->setValue(i);
          }
          //progressTotal->setVisible(false);
      }
   }
}

*/
void Addons::on_pushButtonUpdate_clicked()
{
    QString urlStr="http://al-anvar.sourceforge.net/addons/addonse-0.5.0.xml";
    QUrl url(urlStr);
    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
    fileName=db->pathUser+"/"+fileName+"-update";
    QFile updateFile(fileName);
    if(updateFile.exists())
         updateFile.remove();
     progressTotal->setVisible(false);
    downlod->downloadFile(urlStr,fileName);
    if(downlod->downloded)
    {
        QFile oldFile(db->pathUser+"/addonse-0.5.0.xml");
        if(oldFile.exists())
             oldFile.remove(db->pathUser+"/addonse-0.5.0.xml");
        if(updateFile.rename(db->pathUser+"/addonse-0.5.0.xml")){
            QMessageBox::information(this,db->trlang("Update Add-ons list"),db->trlang("Add-ons list was updated Successfully"), QMessageBox::Ok);
            treeChargeAddons(treeWidget);
        }else
            QMessageBox::warning(this,db->trlang("Update Add-ons list"),db->trlang("Add-ons list was not updated Successfully"), QMessageBox::Ok);


    }else
         QMessageBox::warning(this,db->trlang("Update Add-ons list"),db->trlang("Add-ons list was not updated Successfully"), QMessageBox::Ok);

}
void Addons::closeEvent(QCloseEvent *e)
{
  mostclose =true;
  e->accept();

 }
