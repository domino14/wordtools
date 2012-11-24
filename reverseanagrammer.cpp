#include "reverseanagrammer.h"
#include <QtAlgorithms>
#include <QSet>

bool lengthLessThan(const QString &s1, const QString &s2)
{
    return s1.length() < s2.length();
}


ReverseAnagrammer::ReverseAnagrammer(QObject* parent) : QObject(parent)
{
}

QStringList ReverseAnagrammer::findReverseAnagrams(QString str)
{
    qDebug() << "Finding reverse anagrams for " << str;

}

QStringList ReverseAnagrammer::findAnagrams(QString str)
{
    QStringList ret;
    qDebug() << "Finding anagrams for " << str;
    QSet<QString> result;
    QStringList resultList;
    findSubwords(str, "", result);

    foreach (QString item, result) {
        if (item.length() == str.length()) {
            resultList.append(item);
        }
    }
    return resultList;
}

QStringList ReverseAnagrammer::build(QString str)
{
    qDebug() << "Building" << str;
    QSet<QString> result;
    QStringList resultList;
    findSubwords(str, "", result);
    resultList = result.toList();
    qSort(resultList.begin(), resultList.end(), lengthLessThan);
    return resultList;
}

void ReverseAnagrammer::findSubwords(QString str, QString substr, QSet<QString> &result)
{
    if (this->dawg->findWord(substr)) {
        result.insert(substr);
    } else {
    }
    QString toCheck;
    QString leftoverStr;
    for (int i = 0; i < str.length(); i++) {
        toCheck = substr + str[i];
        leftoverStr = str.mid(0, i) + str.mid(i+1);
        findSubwords(leftoverStr, toCheck, result);
    }
}

void ReverseAnagrammer::run() {
    Dawg dawg;
    dawg.readDawg("OWL2.trie");
    this->dawg = &dawg;
    QStringList arguments = QCoreApplication::arguments();
    QStringList results;
    if (arguments[1] == "anagram") {
        results = findAnagrams(arguments[2]);
    }

    if (arguments[1] == "build") {
        results = build(arguments[2]);
    }

    if (arguments[1] == "revanag") {
        results = findReverseAnagrams(arguments[2]);
    }
    foreach (QString result, results) {
        qDebug(result.toAscii());
    }

    emit finished();
}
