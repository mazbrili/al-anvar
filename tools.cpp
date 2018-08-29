#include "tools.h"

tools::tools(QObject *parent)
        :QObject(parent)
    {

        bool cdMode=false;

      #ifdef  Q_OS_LINUX
        WriteSettings("ExternalPlayer",ReadSettings("ExternalPlayer","mplayer").toString());
      #endif
        defultPath  = QApplication::applicationDirPath();
      #ifdef  Q_OS_LINUX
        defultPath =QDir::homePath()+"/.al-anvar";
      #endif


        if(cdMode)
        {
            defultPath=QDir::homePath()+"/al-anvar";
      #ifdef  Q_OS_LINUX
            defultPath =QDir::homePath()+"/.al-anvar";
      #endif

        }
        pathUser= ReadSettings("UserDir",defultPath).toString();
        pathData= ReadSettings("DataBaseDir",defultPath).toString();

        QDir a_diruser = pathUser;
           if (! a_diruser.exists()){
              if(a_diruser.mkdir(pathUser))
                  WriteSettings("UserDir",pathUser);
           }else
               WriteSettings("UserDir",pathUser);


        QString pathAudio_;

            if(cdMode){
                   pathAudio_=QApplication::applicationDirPath()+"/Audio";
                   QFile databaseFileName(pathData+"/Data.db");
                     if(!databaseFileName.exists()){
                        QMessageBox::information(0,QString::fromUtf8("ایجاد بانک اطلاعات"),
                                                 QString::fromUtf8("برای انجام تغییرات در بانک اطلاعات مانند یادداشت گذاری وموضوع بندی شخصی لازم است بانک اطلاعات از لوح به رایانه شما منتقل شود چند لحظه شکیبا باشید تا این عملیات انجام گیرد . این کار تنها در اولین مرتبه ای که نرم افزار را اجرا میکنید  انجام خواهد گرفت"));
                           QFile::copy("Data.db",pathData+"/Data.db");
                     }
            }else
                   pathAudio_=ReadSettings("UserDir",pathUser).toString()+"/Audio";

            pathAudio=ReadSettings("AudioDir",pathAudio_).toString();


            QDir a_dir = pathAudio;
             WriteSettings("AudioDir",pathAudio);
         if (! a_dir.exists())
            a_dir.mkdir(pathAudio);

        bool internalavailable=false;
        QStringList availableMimeTypes= Phonon::BackendCapabilities::availableMimeTypes();
         if(availableMimeTypes.size()>0)
            internalavailable=true;
        WriteSettings("internalavailable",internalavailable);

       GetLangValue();

    }

QString tools::getDirection(QString txt)
{
    QString str="ltr";
    if(txt.isRightToLeft())
        str="rtl";
    return str;

}
QString tools::getHtmlTag_(QString sampleTxtToGetDirection,bool sql,QString feild,QString font,int size,QString more)
{

    QString str ;
    if(sql) str = QString("\"<p style='direction:%1;font-family:%2;font-size:%3pt;%4 '>\" || \"%5\" || \" </p>\"")
            .arg(getDirection(sampleTxtToGetDirection))
            .arg(font)
            .arg(size)
            .arg(more)
            .arg(feild);
    else str = QString("<p style='direction:%1;font-family:%2;font-size:%3pt;%4 '>%5</p>")
            .arg(getDirection(sampleTxtToGetDirection))
            .arg(font)
            .arg(size)
            .arg(more)
            .arg(feild);
    return str;
}
QString tools::getHtmlTagFeild(QString sampleTxtToGetDirection,QString feild,QString type,QString more)
{
   QFont font=ReadSettings(type, QFont()).value<QFont>();
   return getHtmlTag_(sampleTxtToGetDirection,
                     true,
                     feild,
                     font.family(),
                     font.pointSize());
}
QString tools::getHtmlTagString(QString str,QString type,QString more)
{

   QFont font=ReadSettings(type, QFont()).value<QFont>();
    return getHtmlTag_(str,
                     false,
                     str,
                     font.family(),
                     font.pointSize()
                      ,more);
}
QStringList tools::getHtmlTagStringList(QStringList list,QString type,QString more)
{
    QFont font=ReadSettings(type, QFont()).value<QFont>();
    QString sampleTxtToGetDirection = "";
    if(list.size()>0)
        sampleTxtToGetDirection = list.at(0);

    QStringList strList;
    foreach (QString str,list)
    strList << getHtmlTag_(sampleTxtToGetDirection,
                      false,
                      str,
                      font.family(),
                      font.pointSize()
                      ,more);
    return strList;
}
QString tools::getHtmlTagStringSampleTxt(QString sampleTxtToGetDirection,QString str,QString type,QString more )
{
   QFont font=ReadSettings(type, QFont()).value<QFont>();
  return getHtmlTag_(sampleTxtToGetDirection,
                     false,
                     str,
                     font.family(),
                     font.pointSize()
                      ,more);
}
QStringList tools::getHtmlTagStringListSampleTxt(QString sampleTxtToGetDirection,QStringList list,QString type,QString more)
{
    QFont font=ReadSettings(type, QFont()).value<QFont>();

    QStringList strList;
    foreach (QString str,list)
    strList << getHtmlTag_(sampleTxtToGetDirection,
                      false,
                      str,
                      font.family(),
                      font.pointSize()
                      ,more);
    return strList;
}

QString tools::setTextStyle(QString trTable,QString feild,QString type,QString moreField,QString more)
{
   return   QString("\"%1<p class=\"\"%2\"\" >\" || \"%3\" ||  %4 \" </p>%5\"")
           .arg(trTable)
           .arg(type)
           .arg(feild)
           .arg(moreField)
           .arg(more);
}
QString tools::clearErab(QString str)
{
    str.replace(QString::fromUtf8("ْ"),"");
    str.replace(QString::fromUtf8("ٌ"),"");
    str.replace(QString::fromUtf8("ٍ"),"");
    str.replace(QString::fromUtf8("ً"),"");
    str.replace(QString::fromUtf8("ُ"),"");
    str.replace(QString::fromUtf8("ِ"),"");
    str.replace(QString::fromUtf8("َ"),"");
    str.replace(QString::fromUtf8("ّ"),"");
    str.replace(QString::fromUtf8("‌َ"),"");
    str.replace(QString::fromUtf8("‌ٰ"),"");
    str.replace(QString::fromUtf8("‌ٓ"),"");
    str.replace(QString::fromUtf8("ٰ"),"");
    str.replace(QString::fromUtf8("ٓ"),"");

    return str;
}
QString tools::roolSearch(QString txt)
{
    QString pattern;
    for(int i =0;i<txt.count();i++)
    {
        QString t(txt[i]);
        t.replace(QString::fromUtf8("ک"),QString::fromUtf8("(ك"));
        t.replace(QString::fromUtf8("ى"),QString::fromUtf8("(ى|ي|ی)"));
        t.replace(QString::fromUtf8("ی"),QString::fromUtf8("(ى|ي|ی)"));
        pattern = pattern+"\\w*"+t;
    }
    return pattern;
}
QString tools::alifHamza(QString txt,bool agErab,bool agHamza){

    QString addtxt;
    txt = clearErab(txt);
    txt.replace(QString::fromUtf8("«"),"^");
    txt.replace(QString::fromUtf8("»"),"$");

    txt.replace(QString::fromUtf8("ی"),QString::fromUtf8("ي"));
    txt.replace(QString::fromUtf8("ک"),QString::fromUtf8("ك"));
   if(!agErab){
       QStringList arabics;
       arabics << QString::fromUtf8("ض") << QString::fromUtf8("ص") << QString::fromUtf8("ث") << QString::fromUtf8("ق") << QString::fromUtf8("ف")
               << QString::fromUtf8("غ") << QString::fromUtf8("ع") << QString::fromUtf8("ه") << QString::fromUtf8("ة") << QString::fromUtf8("خ")
               << QString::fromUtf8("ح") << QString::fromUtf8("ج") << QString::fromUtf8("چ") << QString::fromUtf8("ش") << QString::fromUtf8("س")
               << QString::fromUtf8("ی") << QString::fromUtf8("ي") << QString::fromUtf8("ب") << QString::fromUtf8("ل") << QString::fromUtf8("ا")
               << QString::fromUtf8("آ") << QString::fromUtf8("أ") << QString::fromUtf8("إ") << QString::fromUtf8("ئ") << QString::fromUtf8("ت")
               << QString::fromUtf8("ن") << QString::fromUtf8("م") << QString::fromUtf8("ک") << QString::fromUtf8("ك") << QString::fromUtf8("گ")
               << QString::fromUtf8("ظ") << QString::fromUtf8("ط") << QString::fromUtf8("ز") << QString::fromUtf8("ر") << QString::fromUtf8("ذ")
               << QString::fromUtf8("د") << QString::fromUtf8("پ") << QString::fromUtf8("و") << QString::fromUtf8("ؤ") << QString::fromUtf8("ژ");

       if(txt.contains("#")){
           txt.replace("#","");
           addtxt=roolSearch(txt);
       }else{
      for(int i =0;i<txt.count();i++)
      {
            QString set=txt[i]+"";
          if(arabics.contains(set))
        addtxt = addtxt+txt[i]+QString::fromUtf8("(ّ|َ|ِ|ُ|ً|ٍ|ٌ|ْ|ٰ)")+"*";
          else
        addtxt = addtxt+txt[i];
      }
    }

   }else
        addtxt =txt;

  if(!agHamza){

        addtxt.replace(QString::fromUtf8("و"),QString::fromUtf8("(و|ؤ)"));
        addtxt.replace(QString::fromUtf8("ا"),QString::fromUtf8("(آ|أ|إ|ا)"));
        addtxt.replace(QString::fromUtf8("ك"),QString::fromUtf8("(ك|ک)"));
        addtxt.replace(QString::fromUtf8("ي"),QString::fromUtf8("(ى|ي|ی)"));
        addtxt.replace(QString::fromUtf8("ه"),QString::fromUtf8("(ه|ة)"));
   }else{
       addtxt.replace(QString::fromUtf8("ك"),QString::fromUtf8("(ك|ک)"));
       addtxt.replace(QString::fromUtf8("ي"),QString::fromUtf8("(ى|ي|ی)"));

   }

 return addtxt;

   }
QString tools::getAnd(const QString &arg1,bool boundary,bool agErab,bool agHamza)
{
    QString str = arg1.trimmed();
    str.replace("+",",");
    str.replace("/",",");
    str.replace(";",",");
    QStringList list = str.split(",",QString::SkipEmptyParts);

    QStringList textList;
    foreach(QString rep,list){
        if(boundary)
        rep ="\\b"+rep.trimmed()+"\\b";
        textList.append(alifHamza(rep.trimmed(),agErab,agHamza));
        highlightPattern.append(alifHamza(rep.trimmed(),agErab,agHamza));


         }
    QString ret;
    if(textList.size()==1)
       ret= textList.at(0);
    if(textList.size()==2)
       ret= QString("^(?=.*%1)(?=.*%2)").arg(textList.at(0)).arg(textList.at(1));
    if(textList.size()==3)
        ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2));
    if(textList.size()==4)
         ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3));
     if(textList.size()==5)
         ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4));
     if(textList.size()==6)
         ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)(?=.*%6)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4)).arg(textList.at(5));
     if(textList.size()==7)
         ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)(?=.*%6)(?=.*%7)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4)).arg(textList.at(5)).arg(textList.at(6));
     if(textList.size()==8)
         ret= QString("^(?=.*%1)(?=.*%2)(?=.*%3)(?=.*%4)(?=.*%5)(?=.*%6)(?=.*%7)(?=.*%8)").arg(textList.at(0)).arg(textList.at(1)).arg(textList.at(2)).arg(textList.at(3)).arg(textList.at(4)).arg(textList.at(5)).arg(textList.at(6)).arg(textList.at(7));

    return ret;
}
QString tools::getAndAfter(QString arg1,bool boundary,bool agErab,bool agHamza)
{
    QString str = arg1.trimmed();
    str.replace("+",",");
    str.replace("/",",");
    str.replace(";",",");

    QString retStr;
    QStringList list = str.split(",",QString::SkipEmptyParts);
    QStringList listb;
    foreach(QString rep,list){
     if(boundary)
        rep ="\\b"+rep.trimmed()+"\\b";
        listb.append(alifHamza(rep.trimmed(),agErab,agHamza));
        highlightPattern.append(alifHamza(rep.trimmed(),agErab,agHamza));
     }
    if(listb.size()==1)
        retStr+=listb.at(0);
     else
        retStr+=listb.join(".+");
    return retStr;
}
QString tools::getUnWanted(QString arg1,bool boundary,bool agErab,bool agHamza)
 {
     QString str = arg1.trimmed();
     str.replace("+",",");
     str.replace("/",",");
     str.replace(";",",");
     str.replace(".",",");

     QString retStr;
     QStringList list = str.split(",",QString::SkipEmptyParts);
     QStringList unWanted;
     foreach(QString rep,list){
       if(boundary)
         rep ="\\b"+rep.trimmed()+"\\b";
         unWanted.append(alifHamza(rep.trimmed(),agErab,agHamza));
     }
     if(unWanted.size()>0)
        retStr = QString("^((?!%1).)*$").arg(unWanted.join("|"));
 return retStr;
 }
QString tools::getSearchPattern(QString unWanted,QString andall,QString andafter,bool boundary,bool agErab,bool agHamza){

    highlightPattern.clear();
    if(andall.isEmpty() or andafter.isEmpty())
         return getUnWanted(unWanted,boundary,agErab,agHamza)+getAnd(andall,boundary,agErab,agHamza)+getAndAfter(andafter,boundary,agErab,agHamza);

    return getUnWanted(unWanted,boundary,agErab,agHamza)+getAnd(andall,boundary,agErab,agHamza)+".+"+getAndAfter(andafter,boundary,agErab,agHamza);
}
QString tools::highlighSearch(QString value,int words,QString tag,QString tagEnd )
{
  foreach (QString pattern, highlightPattern)
  {
    QRegExp rx(pattern);
    QRegExp sp("\\s");
    int pos = 0;

    if(words==0)
    {
         while ((pos = rx.indexIn(value, pos)) != -1)
         {
             value.insert(pos,tag);
             value.insert(pos+rx.cap(0).length()+tag.length(),tagEnd);
             pos+=pos+rx.cap(0).length()+tag.length()+tagEnd.length();
         }

    }else {
     if(pos>=0)
    {
         while ((pos = rx.indexIn(value, pos)) != -1)
         {
             value.insert(pos,tag);
             value.insert(pos+rx.cap(0).length()+tag.length(),tagEnd);
             pos+=pos+rx.cap(0).length()+tag.length()+tagEnd.length();
         }
    int c = pos-words;
    if(c>0){
    int pos2 =sp.lastIndexIn(value,c);
     if(pos2>=0)
         value.remove(0,pos2);
    }
     int pos3 =sp.indexIn(value,pos+rx.cap(0).length()+tag.length()+tagEnd.length()+words);
     if(pos3>=0)
         value.remove(pos3,value.count()-pos3);
    }
    }
  }
    return value;
}

void tools::WriteSettings(QString key , QVariant Value)
    {
        QSettings settings(defultPath+"/al-anvar.ini",QSettings::IniFormat);
        settings.setValue(key,Value);
    }
QVariant tools::ReadSettings(QString key , QVariant DefultValue)
    {
       QVariant setting;
        QSettings settings(defultPath+"/al-anvar.ini",QSettings::IniFormat);
        setting = settings.value(key,DefultValue);
        return setting;
    }
QStringList tools::setRecent(QString name,const QString &text,int MaxRecent){

         QStringList files = ReadSettings(name,"").toStringList();
         files.removeAll(text.toUtf8());
         files.prepend(text.toUtf8());
         while (files.size() > MaxRecent)
             files.removeLast();
     WriteSettings(name, files);
     return files;

     }
QString tools::arabicNumToEnglish(QString num){

         num.replace("۱","1");
         num.replace("۲","2");
         num.replace("۳","3");
         num.replace("۴","4");
         num.replace("۵","5");
         num.replace("۶","6");
         num.replace("۷","7");
         num.replace("۸","8");
         num.replace("۹","9");
         num.replace("۰","0");
         return num;
     }
QStringList tools::ReadFromTextFile(QString name,QString character)
     {
        QFile file(name);
            QStringList list;
             if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
               QTextStream in(&file);
        in.setCodec(character.toLatin1());
        while(!in.atEnd())
         {
            QString line =in.readLine();
            list <<line;
         }
       }
             file.close();
      return list;
     }
void tools::WriteToText(QString name , QString str)
     {
         QFile file(name);

         if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
              {
                    QTextStream out(&file);
                   out.setCodec("UTF-8");
                    out << str << endl;
                }
                    file.close();
     }
void tools::WriteToText(QString name , QStringList list)
     {
         QFile file(name);

         if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
              {
                    QTextStream out(&file);
                   out.setCodec("UTF-8");
                   foreach (QString str,list)
                       out << str << endl;
                }
                    file.close();
     }
void tools::Zip(QString filename , QString zipfilename)
     {

         QFile infile(filename);
         QFile outfile(zipfilename);
         infile.open(QIODevice::ReadOnly);
         outfile.open(QIODevice::WriteOnly);
         QByteArray uncompressedData = infile.readAll();
         QByteArray compressedData = qCompress(uncompressedData,9);
         outfile.write(compressedData);
         infile.close();
         outfile.close();

     }
void tools::UnZip(QString zipfilename , QString filename)
     {
         QFile infile(zipfilename);
         QFile outfile(filename);
         infile.open(QIODevice::ReadOnly);
         outfile.open(QIODevice::WriteOnly);
         QByteArray uncompressedData = infile.readAll();
         QByteArray compressedData = qUncompress(uncompressedData);
         outfile.write(compressedData);
         infile.close();
         outfile.close();

     }
void tools::ZipOpen(QString OpenedData){
         if (OpenedData=="")
          OpenedData =  QFileDialog::getOpenFileName(0,"Open",QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

         if (OpenedData.isNull()) return;
         Zip(OpenedData,pathData+"/temp.zip");
         QFile file(OpenedData);
         QString BaseDir=Fitterdir(OpenedData);
         QFileInfo fileInfo(file.fileName());
         QString filename =BaseDir+fileInfo.fileName()+".nvr" ;
          Zip(OpenedData,filename);
          file.close();

     }
void tools::UnZipList(){

          QStringList filelist =  QFileDialog::getOpenFileNames(0,"Open",QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

         if (filelist.isEmpty()) return;
         foreach(QString OpenedData,filelist){
         QFile file(OpenedData);
         QString BaseDir=Fitterdir(OpenedData);
         QFileInfo fileInfo(file.fileName());
         QString filename =BaseDir+fileInfo.baseName()+".db" ;
          UnZip(OpenedData,filename);
         }
     }
void tools::ZipList(){

          QStringList filelist =  QFileDialog::getOpenFileNames(0,"Open",QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

         if (filelist.isEmpty()) return;
         foreach(QString OpenedData,filelist){
         QFile file(OpenedData);
         QString BaseDir=Fitterdir(OpenedData);
         QFileInfo fileInfo(file.fileName());
         QString filename =BaseDir+fileInfo.baseName()+".nvr" ;
          Zip(OpenedData,filename);
         }
     }
QString tools::Fitterdir (QString dir,int Type){
         QFileInfo info(dir);
         if (Type==1)
             return info.fileName();
         else if (Type==2)
             return info.completeSuffix();
         else
             return dir.replace(info.fileName(),"");
     }
int tools::randInt(int low, int high)
         {
                return qrand() % ((high + 1) - low) + low;

         }
QString tools::DataTime(QString format){
     QString datetime = QDateTime::currentDateTime().toString(format);
     return datetime;
     }
QStringList tools::GetLang(QString dir)
     {
         QDir lang(dir);
         QFileInfoList list = lang.entryInfoList(QDir::Files);
         QStringList langs;
          foreach (QFileInfo info ,list){
              if(info.completeSuffix()=="xml" or info.completeSuffix()=="XML"){
                  QFile filelang(dir+info.fileName());
                  filelang.open(QIODevice::ReadOnly);
                  QXmlStreamReader xmlLang;
                  xmlLang.setDevice(&filelang);
                  bool end=false;
                  while (!xmlLang.atEnd() && !end)
                  {
                    xmlLang.readNext();
                      if (xmlLang.name() == "locale"){
                        if(xmlLang.attributes().value("VERSION").toString()=="AL-ANVAR-0.6.0")
                          langs << xmlLang.attributes().value("NAME").toString()+";"+info.baseName();
                        else
                            QMessageBox::warning(0,"Language","Invalid language file.");
                          end=true;
                     }
                  }
               }
            }
        return langs;
     }
void tools::GetLangValue()
     {
         QString fileName = ":/language/"+ReadSettings("lang","English").toString()+".xml";
         if(!QFile::exists(fileName))
             fileName = pathUser+"/language/"+ReadSettings("lang","English").toString()+".xml";
         langValue.clear();
         QFile filelang(fileName);
         filelang.open(QIODevice::ReadOnly);
         QXmlStreamReader xmlLang;
         xmlLang.setDevice(&filelang);
         while (!xmlLang.atEnd())
         {
                xmlLang.readNext();

             if (xmlLang.name() == "text")
                 langValue.insert(xmlLang.attributes().value("id").toString(),xmlLang.attributes().value("value").toString());
         }
         filelang.close();
     }
QString tools::trlang(QString key)
     {
    if(langValue.value(key)=="" || langValue.value(key)=="PLEASE_COMPLETE_THIS")
             return key;
         else
             return langValue.value(key);
     }
QString tools::GetNodeIdCode()
     {
         QString datetime = QDateTime::currentDateTime().toString("MMddHHmmss");
         QTime time = QTime::currentTime();
         qsrand((uint)time.msec());
        return  datetime+QString::number(randInt(10,99));
     }
QString tools::GetNodeIdCodeCheachNotSame(QStringList list)
     {
         QString idCode = GetNodeIdCode();
         while (list.contains(idCode))
              idCode = GetNodeIdCode();
         return idCode;
     }
QString tools::GetNodeIdCodeCheachNotSame(QString list)
     {
         QString idCode = GetNodeIdCode();
         while (list==idCode)
              idCode = GetNodeIdCode();
         return idCode;
     }
QString tools::toJalali(QString gregorianDate)
     {
         int y, m, d;
         QString currentDate;
         gregorian_to_jalali(&y,&m,&d,gregorianDate.left(4).toInt(),gregorianDate.mid(5,2).toInt(),gregorianDate.right(2).toInt());
         currentDate=QString("%1-%2-%3").arg(y).arg(m).arg(d);
         if(m<10) currentDate.insert(5,"0");
         if(d<10) currentDate.insert(8,"0");
         return currentDate;
     }
bool tools::sendMail(QString user,QString pass,QString subject ,QString str)
     {
         SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
           smtp.setUser(user);
           smtp.setPassword(pass);
           MimeMessage message;
           message.setSender(new EmailAddress(pass, ""));
           message.addRecipient(new EmailAddress("etrate14@gmail.com", ""));
           message.setSubject(subject);
           MimeText text;
           text.setText(str);
           message.addPart(&text);
           if(!smtp.connectToHost())
           {
               QMessageBox::warning(0,trlang("SendMail"),trlang("Connection Failed."));
               return false;
           }
           else if(!smtp.login())
           {
               QMessageBox::warning(0,trlang("SendMail"),trlang("Authentification Failed."));
               return false;

           }else if(!smtp.sendMail(message))
           {
               QMessageBox::warning(0,trlang("SendMail"),trlang("Mail sending failed."));
               return false;
           }else
           smtp.quit();
           return true;

     }
QString tools::currentJalaliDate(){
      return  toJalali(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
     }
QString tools::toGregorian(QString jalaliDate)
     {
         int y, m, d;
         QString currentDate;
         jalali_to_gregorian(&y,&m,&d,jalaliDate.left(4).toInt(),jalaliDate.mid(5,2).toInt(),jalaliDate.right(2).toInt());
         currentDate=QString("%1-%2-%3").arg(y).arg(m).arg(d);
         if(m<10) currentDate.insert(5,"0");
         if(d<10) currentDate.insert(8,"0");
         return currentDate;
     }
QString tools::getJalaliDayName(QString jalaliDate){

          QString roz = QDateTime::fromString(toGregorian(jalaliDate),"yyyy-MM-dd").toString("ddd");

          roz.replace("Sun",QString::fromUtf8("یک شنبه"));
          roz.replace("Mon",QString::fromUtf8("دو شنبه"));
          roz.replace("Tue",QString::fromUtf8("سه شنبه"));
          roz.replace("Wed",QString::fromUtf8("چهار شنبه"));
          roz.replace("Thu",QString::fromUtf8("پنج شنبه"));
          roz.replace("Fri",QString::fromUtf8("جمعه"));
          roz.replace("Sat",QString::fromUtf8("شنبه"));

          return roz;

     }
QString tools::toJalaliDateTime(QString gregorianDate)
     {
         int y, m, d;
         QString currentDate;
         gregorian_to_jalali(&y,&m,&d,gregorianDate.left(4).toInt(),gregorianDate.mid(5,2).toInt(),gregorianDate.right(2).toInt());
         currentDate=QString("%1-%2-%3").arg(y).arg(m).arg(d);
         if(m<10) currentDate.insert(5,"0");
         if(d<10) currentDate.insert(8,"0");
         return currentDate.append(" , ").append(QTime::currentTime().toString("hh:mm"));
     }
void tools::gregorian_to_jalali(/* output */ int *j_y, int *j_m, int *j_d, int  g_y, int  g_m, int  g_d){

             int g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
          int j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

             int gy, gm, gd;
             int jy, jm, jd;
             long g_day_no, j_day_no;
             int j_np;

             int i;

             gy = g_y-1600;
             gm = g_m-1;
             gd = g_d-1;

             g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
             for (i=0;i<gm;++i)
                g_day_no += g_days_in_month[i];
             if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
                /* leap and after Feb */
                ++g_day_no;
             g_day_no += gd;

             j_day_no = g_day_no-79;

             j_np = j_day_no / 12053;
             j_day_no %= 12053;

             jy = 979+33*j_np+4*(j_day_no/1461);
             j_day_no %= 1461;

             if (j_day_no >= 366) {
                jy += (j_day_no-1)/365;
                j_day_no = (j_day_no-1)%365;
             }

             for (i = 0; (i < 11) && (j_day_no >= j_days_in_month[i]); ++i) {
                j_day_no -= j_days_in_month[i];
             }
             jm = i+1;
             jd = j_day_no+1;
             *j_y = jy;
             *j_m = jm;
             *j_d = jd;
      }
void  tools::jalali_to_gregorian(/* output */ int *g_y, int *g_m, int *g_d, int  j_y, int  j_m, int  j_d){
          int g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
          int j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

          int gy, gm, gd;
            int jy, jm, jd;
            long g_day_no, j_day_no;
            int leap;

            int i;

            jy = j_y-979;
            jm = j_m-1;
            jd = j_d-1;

            j_day_no = 365*jy + (jy/33)*8 + (jy%33+3)/4;
            for (i=0; i < jm; ++i)
               j_day_no += j_days_in_month[i];

            j_day_no += jd;

            g_day_no = j_day_no+79;

            gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
            g_day_no = g_day_no % 146097;

            leap = 1;
            if (g_day_no >= 36525) /* 36525 = 365*100 + 100/4 */
            {
               g_day_no--;
               gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
               g_day_no = g_day_no % 36524;

               if (g_day_no >= 365)
                  g_day_no++;
               else
                  leap = 0;
            }

            gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
            g_day_no %= 1461;

            if (g_day_no >= 366) {
               leap = 0;

               g_day_no--;
               gy += g_day_no/365;
               g_day_no = g_day_no % 365;
            }

            for (i = 0; g_day_no >= g_days_in_month[i] + (i == 1 && leap); i++)
               g_day_no -= g_days_in_month[i] + (i == 1 && leap);
            gm = i+1;
            gd = g_day_no+1;

            *g_y = gy;
            *g_m = gm;
            *g_d = gd;
      }

