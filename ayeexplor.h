#ifndef AYEEXPLOR_H
#define AYEEXPLOR_H

#include <QDialog>
#include "ui_ayeexplor.h"
#include "dbtreeview.h"
#include "findreplace.h"


namespace Ui {
    class AyeExplor;
}

class AyeExplor : public QWidget,public Ui::AyeExplor
{
    Q_OBJECT

public:
    QString Id;
    QuranTreeView *treeAye;

    explicit AyeExplor(DataBase *database,DBModelSubJectQuran *treeModel,QuranTreeView *quranTree,QWidget *parent = 0);

private:

    DBModelSubJectQuran *treeModel_;
    QStandardItemModel *model;
    DbTreeViewSubJectQuran *SubjectTree;
    VerseTree *verseSubjects;
    DataBase *db;
    FindReplace *findExp,*findComment;
    bool mostAdd;
    QList<QStringList> listTr,listEx;
    QString verseText;
    QFont defultfont;

public slots:

    void showAye();
    void showAll();
    bool saveComment();
    void showComment();
    bool maybeSave();
    void delitemsSimilar();
    void chengeItemExp();
    void showValue(int i);
    void showtr(int i);
    void updateVerseTree();
    void addToSubjects();
    void addToSubjectsDragDrop();
    void deleteSubject();
    void showSimilar();
    void filterWord();
    void on_toolButtonPrevious_clicked();
    void on_toolButtoNext_clicked();
    void showSimilarSelected();
private slots:
    void on_actionFindSelected_triggered();
};

#endif // AYEEXPLOR_H
