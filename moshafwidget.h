#ifndef MOSHAFWIDGET_H
#define MOSHAFWIDGET_H

#include <QWidget>
#include "database.h"
#include "ui_moshafwidget.h"
#include "player.h"
#include "contactform.h"

namespace Ui {
class MoshafWidget;
}

class MoshafWidget : public QWidget ,public Ui::MoshafWidget
{
    Q_OBJECT
    
public:
    explicit MoshafWidget(DataBase *database,QWidget *parent = 0);
    
private slots:
    void on_comboBoxChapter_activated(int arg1);
    void on_spinBoxPage_editingFinished();
    void on_comboBoxJuz_activated(int arg1);
    void on_comboBoxHizb_activated(int arg1);
    void on_toolButtonNext_clicked();
    void on_toolButtonBack_clicked();
    void highlightCurrentLine();
    void play();
    void AddToSubjects();
    void showDuplicatesWords();
    void AddToComment();
    void ShowExplanations();
    void similarVerses();
    void needExplanations();
    void setTranslation();
    void finder(QString ch,QString vr);

    void on_spinBoxVerse_valueChanged(const QString &arg1);

private:
    void getQuranValue(QString sql);
    bool stopHighlight,chengeVerse;
    QAction *acVerse,*acPlay,*acInter,*acSubject,*acComment,*acSimiler,*acMail;


     enum { MaxRecent = 12 };
    QAction *acSub[MaxRecent];
    DataBase *db;
    QString translationStr,strTranslation;

    struct VersePeroperty
    {
        int posStartPeroperty;
        int posEndPeroperty;
        QString order;
        QString idPeroperty;
        QString chapterPeroperty;
        QString versePeroperty;

    };
    QVector<VersePeroperty> versePeropertys;
    VersePeroperty currentVersePeroperty;
    QList<QStringList> mainQuranValue,listAudio,translationList;
    InternalPlayer *inPl;

signals:
 void showRelated(QString,QString);
 void openAyeExplor(QString,int);


};

#endif // MOSHAFWIDGET_H
