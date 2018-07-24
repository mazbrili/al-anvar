#ifndef CONTACTFORM_H
#define CONTACTFORM_H

#include <QDialog>
#include <QStatusBar>
#include "ui_contactform.h"
#include "tools.h"
namespace Ui {
class ContactForm;
}

class ContactForm : public QDialog ,public Ui::ContactForm
{
    Q_OBJECT
    
public:
    explicit ContactForm(QWidget *parent = 0);

    
private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonSend_clicked();

private:
    tools tool;
    QStatusBar *statusBar ;

};

#endif // CONTACTFORM_H
