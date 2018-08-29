#ifndef QMLSTANDARDITEMMODEL_H
#define QMLSTANDARDITEMMODEL_H
#include <QStandardItemModel>

class QmlStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    QmlStandardItemModel(QObject *parent = 0);
    QString text(int r,int c);
    void setText(int r,int c,QString text);

};

#endif // QMLSTANDARDITEMMODEL_H
