#ifndef REVERSEANAGRAMMER_H
#define REVERSEANAGRAMMER_H
#include "dawg.h"
#include <QObject>

class ReverseAnagrammer : public QObject
{
    Q_OBJECT
public:
    ReverseAnagrammer(QObject* parent);
private:
    Dawg *dawg;
    void findSubwords(QString str, QString substr, QSet<QString> &result);
    QStringList findReverseAnagrams(QString str);
    QStringList findAnagrams(QString str);
    QStringList build(QString str);
public slots:
    void run();
signals:
    void finished();
};

#endif // REVERSEANAGRAMMER_H
