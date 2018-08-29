#include "dbmodel.h"

DBModel::DBModel(DataBase *database,QWidget *parent ,bool istrush,QString nameTable,bool leyzing,bool Checkable,bool updatable,QString fildOrder,bool sortAsc)
        :QStandardItemModel(parent)
    {
        db = database;

        Checkablebool=Checkable;
        tableName = nameTable ;
         isleyzing = leyzing;
         isTrush =istrush;
         if(sortAsc)
         ascending= " ASC";//"DESC"
         else
         ascending= " DESC";
         Ordering=fildOrder+ascending;
         SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
         SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);

         connect(this,SIGNAL(childFinded(QString)),this,SLOT(getChildsId(QString)));
         TitleIcon= QIcon(":/images/file.png");

         if(updatable)
            updateTree();


    }
void DBModel::updateAsTreeWidget(QTreeWidget *treeWidget)
    {
        QStringList names =   db->getData("Select Name From "+tableName+" Where Parent_ID ='0' ;");
        QStringList ids =   db->getData("Select id From "+tableName+" Where Parent_ID ='0' ;");
        QStringList types =   db->getData("Select Type From "+tableName+" Where Parent_ID ='0' ;");
        QTreeWidgetItem *treeItem = new QTreeWidgetItem;
        treeItem->setText(0,"");
        for (int i = 0; i < names.size(); ++i)
            getTreeWidgetItem(ids.at(i),types.at(i),names.at(i),treeItem);
        treeWidget->insertTopLevelItems(0,treeItem->takeChildren());

    }
void DBModel::updateAsTreeWidget(QTreeWidget *treeWidget,QStringList idList)
    {
        foreach (QString idT,idList)
        {
        QStringList names =   db->getData("Select Name From "+tableName+" Where id ='"+idT+"' ;");
        QStringList ids =   db->getData("Select id From "+tableName+" Where id ='"+idT+"' ;");
        QStringList types =   db->getData("Select Type From "+tableName+" Where id ='"+idT+"' ;");
        QTreeWidgetItem *treeItem = new QTreeWidgetItem;
        treeItem->setText(0,"");
        for (int i = 0; i < names.size(); ++i)
            getTreeWidgetItem(ids.at(i),types.at(i),names.at(i),treeItem);
        treeWidget->insertTopLevelItems(0,treeItem->takeChildren());
        }

    }
void DBModel::getTreeWidgetItem(QString id,QString type,QString name ,QTreeWidgetItem *treeItem)
    {

        QTreeWidgetItem *childItem = new QTreeWidgetItem(treeItem);
        childItem->setText(0,name);
        childItem->setText(1,id);

        childItem->setIcon(0,TitleIcon);
        childItem->setData(0, Qt::UserRole,"bookmark");


       if (type=="0")
       {
           childItem->setIcon(0,SubjectIcon);
           childItem->setData(0, Qt::UserRole,"folder");

           QStringList names =   db->getData("Select Name From "+tableName+" Where Parent_ID ='"+id+"' ;");
           QStringList types =   db->getData("Select Type From "+tableName+" Where Parent_ID ='"+id+"' ;");
           QStringList ids =   db->getData("Select id From "+tableName+" Where Parent_ID ='"+id+"' ;");
           for (int i = 0; i < names.size(); ++i)
               getTreeWidgetItem(ids.at(i),types.at(i),names.at(i),childItem);
       }

    }
void DBModel::getXMl()
    {
        QString fileName =QFileDialog::getSaveFileName(0, "Save XML File","","XML Files (*.xml)");

        if (fileName.isEmpty())
            return;
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(0, "QXmlStream Tree",QString("Cannot write file %1: %2.").arg(fileName).arg(file.errorString()));
            return;
        }
        QTreeWidget *treeWidget = new QTreeWidget;
        updateAsTreeWidget(treeWidget);

          xml.setAutoFormatting(true);
        if (writeTreeToXml(&file,treeWidget))
        {
            QMessageBox::about(0,db->trlang("export xml"),db->trlang("done."));
            delete treeWidget ;
        }

    }
void DBModel::getXMl(QStringList idList)
    {
        QString fileName =QFileDialog::getSaveFileName(0, "Save XML File","","XML Files (*.xml)");

        if (fileName.isEmpty())
            return;
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(0, "QXmlStream Tree",QString("Cannot write file %1: %2.").arg(fileName).arg(file.errorString()));
            return;
        }
        QTreeWidget *treeWidget = new QTreeWidget;
        updateAsTreeWidget(treeWidget,idList);

          xml.setAutoFormatting(true);
        if (writeTreeToXml(&file,treeWidget))
        {
            QMessageBox::about(0,db->trlang("export xml"),db->trlang("done."));
            delete treeWidget ;
        }

    }
bool DBModel::writeTreeToXml(QIODevice *device,QTreeWidget *treeWidget)
    {
        xml.setDevice(device);

        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE XML>");
        xml.writeStartElement("xml");
        xml.writeAttribute("version", "1.0");
        for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
            writeItem(treeWidget->topLevelItem(i));

        xml.writeEndDocument();
        return true;
    }
void DBModel::writeItem(QTreeWidgetItem *item)
    {
        QString tagName = item->data(0, Qt::UserRole).toString();
        if (tagName == "folder") {
            xml.writeStartElement(tagName);
            xml.writeTextElement("title", item->text(0));
            for (int i = 0; i < item->childCount(); ++i)
                writeItem(item->child(i));
            xml.writeEndElement();
        } else if (tagName == "bookmark") {
            xml.writeStartElement(tagName);
            xml.writeTextElement("title", item->text(0));
            if (!item->text(1).isEmpty())
            {
            QString id =item->text(1);
            xml.writeTextElement("text",db->getDatastr(" Select text From "+tableName+"Value Where IdText ="+id));
            QStringList idkeyword =db->getData("Select ParentId From keywordsValue Where  TextId='"+id+"' And  Type ='keyword';");
            QStringList idalam =db->getData("Select ParentId From keywordsValue Where  TextId='"+id+"' And  Type ='alam';");
            QStringList idsource =db->getData("Select ParentId From keywordsValue Where  TextId='"+id+"' And  Type ='source';");
            QStringList keywords,alams,sources;
            foreach (QString id,idkeyword)
              keywords << db->getDatastr("Select Name From keywords Where id="+id);
            foreach (QString id,idalam)
              alams << db->getDatastr("Select Name From keywords Where id="+id);
            foreach (QString id,idsource)
              sources << db->getDatastr("Select Name From keywords Where id="+id);

           if(keywords.size()>0)
            xml.writeTextElement("keywords", keywords.join("/"));

           if(alams.size()>0)
            xml.writeTextElement("alams", alams.join("/"));

           if(sources.size()>0)
            xml.writeTextElement("sources", sources.join("/"));
            }
            xml.writeEndElement();
        }
    }
void DBModel::updateTree()
      {
          clear();
          QString rootParent;
          if(isTrush)
              rootParent="0101010101";
          else
              rootParent="0";

          QList<QStringList> subject_List =   db->getListData("SELECT Name,id,Parent_ID,Type,BookID,GROUP_CONCAT(peroperty,'|') AS peroperty  From "+tableName+" Where Parent_ID ='"+rootParent+"' GROUP BY name,type ORDER BY "+Ordering+";");
        //  QList<QStringList> subject_List =   db->getListData("Select Name,id,Parent_ID,Type,BookID,peroperty From "+tableName+" Where Parent_ID ='"+rootParent+"' ORDER BY "+Ordering+";");

          for(int i =0;i<subject_List.at(0).count();i++){
                  QList<QStandardItem*> items;
                  QString titleText =subject_List.at(0).at(i).mid(0,100);
                  titleText.replace("\n"," ");
                  titleText.replace("\t"," ");
                  titleText.replace("{","");
                  titleText.replace("}","");
                  titleText.replace("+","");
                  titleText.replace("/","");
                  QStandardItem *itemName = new  QStandardItem(TitleIcon,titleText.trimmed());
                  QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                  QStandardItem *itemParentId = new  QStandardItem(subject_List.at(2).at(i));
                  QStandardItem *SubjectRoot = new  QStandardItem("1");
                  QStandardItem *SubjectType = new  QStandardItem(subject_List.at(3).at(i));
                  QStandardItem *SubjectBookId = new  QStandardItem(subject_List.at(4).at(i));
                  QStandardItem *SubjectPerpperty = new  QStandardItem(subject_List.at(5).at(i));
                    if (subject_List.at(3).at(i)=="0"){
                      itemName->setIcon(SubjectIcon);
                      itemName->setData("folder");
                     }else
                    itemName->setData("item");

//          itemName->setFlags( Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
//          if(Checkablebool && subject_List.at(3).at(i)=="1" )
//            itemName->setCheckState(Qt::Unchecked);
         items.append(itemName);
          items.append(itemId);
          items.append(itemParentId);
          items.append(SubjectRoot);
          items.append(SubjectType);
          items.append(SubjectBookId);
          items.append(SubjectPerpperty);

          appendRow(items);
          QApplication::processEvents();

          if (subject_List.at(3).at(i)=="0")
                getChild(itemName,itemId);


      }

      emit  updated();
    }
QString DBModel::getCurrentText(int col,QModelIndex parent){

        return data(index(parent.row(),col,parent.parent())).toString();

    }
void DBModel::getParents(QString findTxt)
    {
        QList<QStringList> subject_List =   db->getListData("Select Name,id,Parent_ID,Type,BookID From "+tableName+" Where Parent_ID ='"+findTxt+"' ORDER BY "+Ordering+";");
        for(int i =0;i<subject_List.at(0).count();i++){

                QList<QStandardItem*> items;
                QString titleText =subject_List.at(0).at(i);
                titleText.replace("\n"," ");
                titleText.replace("{","");
                titleText.replace("}","");
                titleText.replace("+","");
                titleText.replace("/","");
                QStandardItem *itemName = new  QStandardItem(TitleIcon,titleText);
                QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                QStandardItem *itemParentId = new  QStandardItem(subject_List.at(2).at(i));
                QStandardItem *SubjectRoot = new  QStandardItem("1");
                QStandardItem *SubjectType = new  QStandardItem(subject_List.at(3).at(i));
                QStandardItem *SubjectBookId = new  QStandardItem(subject_List.at(4).at(i));
                  if (subject_List.at(3).at(i)=="0")
                    itemName->setIcon(SubjectIcon);
//        itemName->setFlags( Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
//        if(Checkablebool && subject_List.at(3).at(i)=="1" )
//          itemName->setCheckState(Qt::Unchecked);
        items.append(itemName);
        items.append(itemId);
        items.append(itemParentId);
        items.append(SubjectRoot);
        items.append(SubjectType);
        items.append(SubjectBookId);
        this->appendRow(items);
    }

    }
void DBModel::getChild(QStandardItem *iname,QStandardItem *id)
       {
           QList<QStringList> subject_List =   db->getListData("SELECT Name,id,Parent_ID,Type,BookID,GROUP_CONCAT(peroperty,'|') AS peroperty  From "+tableName+" Where Parent_ID ='"+id->text()+"' GROUP BY name,type ORDER BY "+Ordering+";");
          // QList<QStringList> subject_List =  db->getListData("Select Name,id,Parent_ID,Type,BookID,peroperty From "+tableName+" Where Parent_ID ='"+id->text()+"' ORDER BY "+Ordering+";");

           for (int i=0;i<subject_List.at(0).count();i++){
                           QList<QStandardItem*> items;
                           QString titleText =subject_List.at(0).at(i).mid(0,100);
                           titleText.replace("\n"," ");
                           titleText.replace("{","");
                           titleText.replace("}","");
                           titleText.replace("+","");
                           titleText.replace("/","");
                           titleText.replace("\t"," ");
                           QStandardItem *itemName = new  QStandardItem(TitleIcon,titleText.trimmed());
                           QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                           QStandardItem *itemParentId = new  QStandardItem(subject_List.at(2).at(i));
                           QStandardItem *SubjectRoot = new  QStandardItem("1");
                           QStandardItem *SubjectType = new  QStandardItem(subject_List.at(3).at(i));
                           QStandardItem *SubjectBookId = new  QStandardItem(subject_List.at(4).at(i));
                           QStandardItem *SubjectPerpperty = new  QStandardItem(subject_List.at(5).at(i));

                             if (subject_List.at(3).at(i)=="0")
                             {
                              itemName->setIcon(SubjectIcon);
                              itemName->setData("folder");
                              }else
                               itemName->setData("item");

//                               itemName->setFlags(  Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
//                            if(Checkablebool && subject_List.at(3).at(i)=="1" )
//                               itemName->setCheckState(Qt::Unchecked);

                            items.append(itemName);
                            items.append(itemId);
                            items.append(itemParentId);
                            items.append(SubjectRoot);
                            items.append(SubjectType);
                            items.append(SubjectBookId);
                            items.append(SubjectPerpperty);

                            iname->appendRow(items);
                              QApplication::processEvents();

                         if (subject_List.at(3).at(i)=="0" && !isleyzing )
                             getChild(itemName,itemId);

         }
    }
void DBModel::getChildsId(QString id)
    {
        QList<QStringList> subjectList = db->getListData("Select Name,id,Type From "+tableName+" Where Parent_Id ='"+id+"' ORDER BY "+Ordering+";");
        for (int i=0;i<subjectList.at(0).count();i++)
        {
         listChildIds.append(subjectList.at(1).at(i));
         if (subjectList.at(2).at(i)=="0" )
           emit childFinded(subjectList.at(1).at(i));
        }
    }


DBModelSubJectQuran::DBModelSubJectQuran(DataBase *database,QWidget *parent ,bool istrush,QString nameTable,bool leyzing,bool Checkable,bool updatable,QString fildOrder,bool sortAsc)
        :QStandardItemModel(parent)
    {
        db = database;

        Checkablebool=Checkable;
        tableName = nameTable ;
         isleyzing = leyzing;
         isTrush =istrush;
         if(sortAsc)
         ascending= " ASC";//"DESC"
         else
         ascending= " DESC";
         Ordering=fildOrder+ascending;
         SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
         SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);

         connect(this,SIGNAL(childFinded(QString)),this,SLOT(getChildsId(QString)));
         TitleIcon= QIcon(":/images/file.png");

         if(updatable)
            updateTree();


    }
 void DBModelSubJectQuran::updateAsTreeWidget(QTreeWidget *treeWidget)
    {
        QStringList names =   db->getData("Select Name From "+tableName+" Where Parent_ID ='0' ;");
        QStringList ids =   db->getData("Select id From "+tableName+" Where Parent_ID ='0' ;");
        QStringList types =   db->getData("Select Type From "+tableName+" Where Parent_ID ='0' ;");
        QTreeWidgetItem *treeItem = new QTreeWidgetItem;
        treeItem->setText(0,"");
        for (int i = 0; i < names.size(); ++i)
            getTreeWidgetItem(ids.at(i),types.at(i),names.at(i),treeItem);
        treeWidget->insertTopLevelItems(0,treeItem->takeChildren());

    }
 void DBModelSubJectQuran::updateAsTreeWidget(QTreeWidget *treeWidget,QStringList idList)
    {
        foreach (QString idT,idList)
        {
        QStringList names =   db->getData("Select Name From "+tableName+" Where id ='"+idT+"' ;");
        QStringList ids =   db->getData("Select id From "+tableName+" Where id ='"+idT+"' ;");
        QStringList types =   db->getData("Select Type From "+tableName+" Where id ='"+idT+"' ;");
        QTreeWidgetItem *treeItem = new QTreeWidgetItem;
        treeItem->setText(0,"");
        for (int i = 0; i < names.size(); ++i)
            getTreeWidgetItem(ids.at(i),types.at(i),names.at(i),treeItem);
        treeWidget->insertTopLevelItems(0,treeItem->takeChildren());
        }

    }
 void DBModelSubJectQuran::getTreeWidgetItem(QString id,QString type,QString name ,QTreeWidgetItem *treeItem)
    {

        QTreeWidgetItem *childItem = new QTreeWidgetItem(treeItem);
        childItem->setText(0,name);
        childItem->setText(1,id);

        childItem->setIcon(0,TitleIcon);
        childItem->setData(0, Qt::UserRole,"bookmark");


       if (type=="0")
       {
           childItem->setIcon(0,SubjectIcon);
           childItem->setData(0, Qt::UserRole,"folder");

           QStringList names =   db->getData("Select Name From "+tableName+" Where Parent_ID ='"+id+"' ;");
           QStringList types =   db->getData("Select Type From "+tableName+" Where Parent_ID ='"+id+"' ;");
           QStringList ids =   db->getData("Select id From "+tableName+" Where Parent_ID ='"+id+"' ;");
           for (int i = 0; i < names.size(); ++i)
               getTreeWidgetItem(ids.at(i),types.at(i),names.at(i),childItem);
       }

    }
 void DBModelSubJectQuran::getXMl()
    {
        QString fileName =QFileDialog::getSaveFileName(0, "Save XML File","","XML Files (*.xml)");

        if (fileName.isEmpty())
            return;
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(0, "QXmlStream Tree",QString("Cannot write file %1: %2.").arg(fileName).arg(file.errorString()));
            return;
        }
        QTreeWidget *treeWidget = new QTreeWidget;
        updateAsTreeWidget(treeWidget);

          xml.setAutoFormatting(true);
        if (writeTreeToXml(&file,treeWidget))
        {
            QMessageBox::about(0,db->trlang("export xml"),db->trlang("done."));
            delete treeWidget ;
        }

    }
 void DBModelSubJectQuran::getXMl(QStringList idList)
    {
        QString fileName =QFileDialog::getSaveFileName(0, "Save XML File","","XML Files (*.xml)");

        if (fileName.isEmpty())
            return;
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(0, "QXmlStream Tree",QString("Cannot write file %1: %2.").arg(fileName).arg(file.errorString()));
            return;
        }
        QTreeWidget *treeWidget = new QTreeWidget;
        updateAsTreeWidget(treeWidget,idList);

          xml.setAutoFormatting(true);
        if (writeTreeToXml(&file,treeWidget))
        {
            QMessageBox::about(0,db->trlang("export xml"),db->trlang("done."));
            delete treeWidget ;
        }

    }
 bool DBModelSubJectQuran::writeTreeToXml(QIODevice *device,QTreeWidget *treeWidget)
    {
        xml.setDevice(device);

        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE XML>");
        xml.writeStartElement("xml");
        xml.writeAttribute("version", "1.0");
        for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
            writeItem(treeWidget->topLevelItem(i));

        xml.writeEndDocument();
        return true;
    }
 void DBModelSubJectQuran::writeItem(QTreeWidgetItem *item)
    {
        QString tagName = item->data(0, Qt::UserRole).toString();
        if (tagName == "folder") {
            xml.writeStartElement(tagName);
            xml.writeTextElement("title", item->text(0));
            for (int i = 0; i < item->childCount(); ++i)
                writeItem(item->child(i));
            xml.writeEndElement();
        } else if (tagName == "bookmark") {
            xml.writeStartElement(tagName);
            xml.writeTextElement("title", item->text(0));
            if (!item->text(1).isEmpty())
            {
            QString id =item->text(1);
            xml.writeTextElement("text",db->getDatastr(" Select text From "+tableName+"Value Where IdText ="+id));
            QStringList idkeyword =db->getData("Select ParentId From keywordsValue Where  TextId='"+id+"' And  Type ='keyword';");
            QStringList idalam =db->getData("Select ParentId From keywordsValue Where  TextId='"+id+"' And  Type ='alam';");
            QStringList idsource =db->getData("Select ParentId From keywordsValue Where  TextId='"+id+"' And  Type ='source';");
            QStringList keywords,alams,sources;
            foreach (QString id,idkeyword)
              keywords << db->getDatastr("Select Name From keywords Where id="+id);
            foreach (QString id,idalam)
              alams << db->getDatastr("Select Name From keywords Where id="+id);
            foreach (QString id,idsource)
              sources << db->getDatastr("Select Name From keywords Where id="+id);

           if(keywords.size()>0)
            xml.writeTextElement("keywords", keywords.join("/"));

           if(alams.size()>0)
            xml.writeTextElement("alams", alams.join("/"));

           if(sources.size()>0)
            xml.writeTextElement("sources", sources.join("/"));
            }
            xml.writeEndElement();
        }
    }
 void DBModelSubJectQuran::getChild(QStandardItem *iname,QStandardItem *id)
    {
        QList<QStringList> subject_List =  db->getListData("Select Name,id,Parent_ID,Type,BookID From "+tableName+" Where Parent_ID ='"+id->text()+"' ORDER BY "+Ordering+";");

                    for (int i=0;i<subject_List.at(0).count();i++){

                        QList<QStandardItem*> items;
                        QString titleText =db->trlang(subject_List.at(0).at(i));
                        titleText.replace("\n"," ");
                        titleText.replace("{","");
                        titleText.replace("}","");
                        titleText.replace("+","");
                        titleText.replace("/","");
                        titleText.replace("\t"," ");


                        QStandardItem *itemName = new  QStandardItem(TitleIcon,titleText.trimmed());
                        if(db->getDatastr("Select count(id) From Subjects Where ID_SUBJECT  = "+subject_List.at(1).at(i)).toInt()>0)
                                                    itemName->setIcon(QIcon(":/images/ok.png"));

                        QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                        QStandardItem *itemParentId = new  QStandardItem(subject_List.at(2).at(i));
                        QStandardItem *SubjectRoot = new  QStandardItem("1");
                        QStandardItem *SubjectType = new  QStandardItem(subject_List.at(3).at(i));
                        QStandardItem *SubjectBookId = new  QStandardItem(subject_List.at(4).at(i));
                          if (subject_List.at(3).at(i)=="0")
                          {
                           itemName->setIcon(SubjectIcon);
                           itemName->setData("folder");
                           }else
                            itemName->setData("item");

//                               itemName->setFlags(  Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
//                            if(Checkablebool && subject_List.at(3).at(i)=="1" )
//                               itemName->setCheckState(Qt::Unchecked);

                         items.append(itemName);
                         items.append(itemId);
                         items.append(itemParentId);
                         items.append(SubjectRoot);
                         items.append(SubjectType);
                         items.append(SubjectBookId);
                           iname->appendRow(items);
                           QApplication::processEvents();

                      if (subject_List.at(3).at(i)=="0" && !isleyzing )
                          getChild(itemName,itemId);

      }
 }
 void DBModelSubJectQuran::getChildsId(QString id)
 {
     QList<QStringList> subjectList = db->getListData("Select Name,id,Type From "+tableName+" Where Parent_Id ='"+id+"' ORDER BY "+Ordering+";");
     for (int i=0;i<subjectList.at(0).count();i++)
     {
      listChildIds.append(subjectList.at(1).at(i));
      if (subjectList.at(2).at(i)=="0" )
        emit childFinded(subjectList.at(1).at(i));
     }
 }

 void DBModelSubJectQuran::updateTree()
      {
          clear();
          QString rootParent;
          if(isTrush)
              rootParent="0101010101";
          else
              rootParent="0";

          QList<QStringList> subject_List =   db->getListData("Select Name,id,Parent_ID,Type,BookID From "+tableName+" Where Parent_ID ='"+rootParent+"' ORDER BY "+Ordering+";");
          for(int i =0;i<subject_List.at(0).count();i++){
                  QList<QStandardItem*> items;
                  QString titleText =db->trlang(subject_List.at(0).at(i));
                  titleText.replace("\n"," ");
                  titleText.replace("\t"," ");
                  titleText.replace("{","");
                  titleText.replace("}","");
                  titleText.replace("+","");
                  titleText.replace("/","");
                  QStandardItem *itemName = new  QStandardItem(TitleIcon,titleText.trimmed());

                  if(db->getDatastr("Select count(id) From Subjects Where ID_SUBJECT  = "+subject_List.at(1).at(i)).toInt()>0)
                                      itemName->setIcon(QIcon(":/images/ok.png"));

                  QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                  QStandardItem *itemParentId = new  QStandardItem(subject_List.at(2).at(i));
                  QStandardItem *SubjectRoot = new  QStandardItem("1");
                  QStandardItem *SubjectType = new  QStandardItem(subject_List.at(3).at(i));
                  QStandardItem *SubjectBookId = new  QStandardItem(subject_List.at(4).at(i));
                    if (subject_List.at(3).at(i)=="0"){
                      itemName->setIcon(SubjectIcon);
                      itemName->setData("folder");
                     }else
                    itemName->setData("item");

    //      itemName->setFlags( Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
     //     if(Checkablebool && subject_List.at(3).at(i)=="1" )
      //      itemName->setCheckState(Qt::Unchecked);
          items.append(itemName);
          items.append(itemId);
          items.append(itemParentId);
          items.append(SubjectRoot);
          items.append(SubjectType);
          items.append(SubjectBookId);
          appendRow(items);
          QApplication::processEvents();

          if (subject_List.at(3).at(i)=="0")
                getChild(itemName,itemId);


      }

      emit  updated();
    }
 QString DBModelSubJectQuran::getCurrentText(int col,QModelIndex parent){

        return data(index(parent.row(),col,parent.parent())).toString();

    }
 void DBModelSubJectQuran::getParents(QString findTxt)
    {
        QList<QStringList> subject_List =   db->getListData("Select Name,id,Parent_ID,Type,BookID From "+tableName+" Where Parent_ID ='"+findTxt+"' ORDER BY "+Ordering+";");
        for(int i =0;i<subject_List.at(0).count();i++){

                QList<QStandardItem*> items;
                QString titleText =db->trlang(subject_List.at(0).at(i));
                titleText.replace("\n"," ");
                titleText.replace("{","");
                titleText.replace("}","");
                titleText.replace("+","");
                titleText.replace("/","");
                QStandardItem *itemName = new  QStandardItem(TitleIcon,titleText);
                if(db->getDatastr("Select count(id) From Subjects Where ID_SUBJECT  = "+subject_List.at(1).at(i)).toInt()>0)
                                itemName->setIcon(QIcon(":/images/ok.png"));

                QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                QStandardItem *itemParentId = new  QStandardItem(subject_List.at(2).at(i));
                QStandardItem *SubjectRoot = new  QStandardItem("1");
                QStandardItem *SubjectType = new  QStandardItem(subject_List.at(3).at(i));
                QStandardItem *SubjectBookId = new  QStandardItem(subject_List.at(4).at(i));
                  if (subject_List.at(3).at(i)=="0")
                    itemName->setIcon(SubjectIcon);
//        itemName->setFlags( Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled);
//        if(Checkablebool && subject_List.at(3).at(i)=="1" )
//          itemName->setCheckState(Qt::Unchecked);
        items.append(itemName);
        items.append(itemId);
        items.append(itemParentId);
        items.append(SubjectRoot);
        items.append(SubjectType);
        items.append(SubjectBookId);
        this->appendRow(items);
    }

    }




QuranModel::QuranModel(DataBase *database,QWidget *parent)
             :QStandardItemModel(parent)
     {
     db = database;
     SubjectIcon.addPixmap(parent->style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
     TitleIcon= QIcon(":/images/file.png");


     QList<QStringList> subject_List =   db->getListData("Select descent,id From ChapterProperty ;");
     for(int i =0;i<subject_List.at(0).count();i++){
             QList<QStandardItem*> items;
             QString titleText =db->trlang(subject_List.at(0).at(i));
             QStandardItem *itemName = new  QStandardItem(SubjectIcon,db->trlang(subject_List.at(1).at(i))+"-"+titleText);

             QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
             QStandardItem *itemType = new  QStandardItem("chapter");
 //            itemName->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);


     items.append(itemName);
     items.append(itemId);
     items.append(itemType);
     getChild(itemName,itemId);

     this->appendRow(items);
     }
     }

void QuranModel::getChild(QStandardItem *iname,QStandardItem *id)
        {
         QList<QStringList> subject_List =   db->getListData("Select verse,id From Quran Where Chapter="+id->text()+" ;");
         for(int i =0;i<subject_List.at(0).count();i++){
                 QList<QStandardItem*> items;
                 QStandardItem *itemName = new  QStandardItem(TitleIcon,db->trlang("Verse")+": "+subject_List.at(0).at(i));
                 if(db->getDatastr("Select count(id) From Comment Where id = "+subject_List.at(1).at(i)).toInt()>0)
                 {
                     if(db->getDatastr("Select Value From Comment Where id = "+subject_List.at(1).at(i)+";").trimmed()!="")
                        itemName->setIcon(QIcon(":/images/ok.png"));
                 }

                 QStandardItem *itemId = new  QStandardItem(subject_List.at(1).at(i));
                 QStandardItem *itemType = new  QStandardItem("verse");
        //         itemName->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);



         items.append(itemName);
         items.append(itemId);
         items.append(itemType);
         iname->appendRow(items);

        }


 }
