#include "qmlstandarditemmodel.h"

QmlStandardItemModel::QmlStandardItemModel(QObject *parent)
: QStandardItemModel(parent)
{
    int value1 = Qt::UserRole + 1;
    int value2 = Qt::UserRole + 2;
    int value3  =Qt::UserRole + 3;
    int value4  =Qt::UserRole + 4;
    int value5  =Qt::UserRole + 5;
    int value6  =Qt::UserRole + 6;
    QHash<int, QByteArray> roles;
    roles[value1] = "value1";
    roles[value2] = "value2";
    roles[value3] = "value3";
    roles[value4] = "value4";
    roles[value5] = "value5";
    roles[value6] = "value6";
    setRoleNames(roles);
}
QString QmlStandardItemModel::text(int r,int c)
{
   return data(index(r,0),Qt::UserRole + c+1).toString();
}
void QmlStandardItemModel::setText(int r,int c,QString text)
{
    setData(index(r,0),text,Qt::UserRole + c+1);
}

