#include "contactform.h"
#include "./sendermail/SmtpMime"
ContactForm::ContactForm(QWidget *parent) :
    QDialog(parent)

{
    setupUi(this);
    statusBar = new QStatusBar;
    statusBarLayout->addWidget(statusBar);


    labelName->setText(tool.trlang("your name"));
    labelMail->setText(tool.trlang("your mail"));
    labelSubject->setText(tool.trlang("subjct"));
    labelText->setText(tool.trlang("bodytext"));
    pushButtonSend->setText(tool.trlang("send"));
    pushButtonCancel->setText(tool.trlang("Cancel"));
    setWindowTitle(tool.trlang("Contact Form"));
}


void ContactForm::on_pushButtonCancel_clicked()
{
    reject();
}

void ContactForm::on_pushButtonSend_clicked()
{

    QRegExp regexEmail(
       "^[^_\.][a-z0-9_]+[\.]?[a-z0-9_]+[^_\.]@{1}[a-z0-9]+[\.]{1}(([a-z]{2,4})|([a-z]   {2,4}[\.]{1}[a-z]{2,3}))$" );
       QRegExpValidator validator(regexEmail,0);
       QString qsEmail = lineEditMail->text(); //return a qstring value

    int pos=0;
    if ( validator.validate(qsEmail, pos) != QValidator::Acceptable ||lineEditMail->text().isEmpty())
    {
        lineEditMail->setFocus();
        lineEditMail->setStyleSheet("background-color: rgb(255, 255, 0);");
        return;
    }
    lineEditMail->setStyleSheet("background-color: rgb(255, 255, 255);");
      pushButtonSend->setEnabled(false);
      QString user;
      QString pass;
        if(user.isNull() || pass.isNull())
        {
            QMessageBox::warning(0,tool.trlang("SendMail"),tool.trlang("USER_PASS_IS_NULL"));
            return;
        }
        QString subject =lineEditSubject->text()+" Al-Anvar Mail";
        QString str=plainTextEdit->toPlainText()
                +"\nName\t"+lineEditName->text()
                +"\nMail\t"+lineEditMail->text();

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
              statusBar->showMessage(tool.trlang("Connection Failed."));
              pushButtonSend->setEnabled(true);
              return;
          }else
          statusBar->showMessage(tool.trlang("Connected"));
          if(!smtp.login())
          {
              statusBar->showMessage(tool.trlang("Authentification Failed."));
              pushButtonSend->setEnabled(true);
              return ;
          }else
          statusBar->showMessage(tool.trlang("Authentificated"));
           if(!smtp.sendMail(message))
          {
              statusBar->showMessage(tool.trlang("Mail sending failed."));
              pushButtonSend->setEnabled(true);
              return ;
          }else {
          statusBar->showMessage(tool.trlang("SendMail ok."));
          smtp.quit();
          QMessageBox::about(this,tool.trlang("SendMail"),tool.trlang("SendMail ok."));
           }


    accept();
}
