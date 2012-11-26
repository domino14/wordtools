#ifndef REVERSEANAGRAMMER_H
#define REVERSEANAGRAMMER_H
#include "dawg.h"
#include <QObject>
#include "utilities.h"

class ReverseAnagrammer : public QObject
{
    Q_OBJECT
public:
    ReverseAnagrammer(QObject* parent);
private:
    Dawg *dawg;
    void findSubwords(QString str, QString substr, QSet<QString> &result);
    QStringList findReverseAnagrams(QString str, int rackLength);
    QStringList findAnagrams(QString str);
    QStringList build(QString str);
    void findSubwordsSubloop(QString toCheck, QString leftoverStr, QSet<QString> &result);
    Utilities utilities;
    void test();
    void generateLiveRacks(int rackLength, int withBlanks);
public slots:
    void run();
signals:
    void finished();
};

#endif // REVERSEANAGRAMMER_H
