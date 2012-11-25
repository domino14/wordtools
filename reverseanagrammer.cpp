#include "reverseanagrammer.h"
#include <QtAlgorithms>
#include <QSet>

int pruned = 0;
int followed = 0;

QString letters = "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ";
//QString letters = "AAB";
// 2484184 distinct racks without the blank
// 3199724 with the blank
// http://math.stackexchange.com/questions/243685/how-many-possible-scrabble-racks-are-there-at-the-beginning-of-the-game
void genLetterCombinations(QString n, int k, QSet<QString> &combinations, QString soFar) {
   // qDebug() << "Called with" << n << k << soFar;
    // choose k from n letters
    if (soFar.length() == k) {
        combinations.insert(soFar);
        if (combinations.size() % 10000 == 0) {
            qDebug() << "Combinations so far:" << combinations.size();
        }
        return;
    }
    for (int i = 0; i < n.length(); i++) {
        QString modified = n;
        modified = modified.mid(i+1);
       // qDebug() << i;
        genLetterCombinations(modified, k, combinations, soFar + n[i]);
    }
}

bool lengthLessThan(const QString &s1, const QString &s2)
{
    return s1.length() < s2.length();
}


ReverseAnagrammer::ReverseAnagrammer(QObject* parent) : QObject(parent)
{
}

QStringList ReverseAnagrammer::findReverseAnagrams(QString str, int rackLength)
{
    qDebug() << "Finding reverse anagrams for " << str << rackLength;
    /* a reverse anagram finder
        revanag KENJI 7 will find all 7-letter racks that go with KENJI to make an 8.*/
    // first, must generate all possible racks of length rackLength
    QSet <QString> combos;
    genLetterCombinations(letters, rackLength, combos, "");
    qDebug() << "number of combos" << combos.size();
    QList <QString> combosList = combos.toList();
    QFile file("combos_.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    foreach (QString combo, combosList) {
        out << combo << "\n";
    }
    file.close();
    return QStringList();
}

QStringList ReverseAnagrammer::findAnagrams(QString str)
{
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

void ReverseAnagrammer::findSubwordsSubloop(QString toCheck, QString leftoverStr, QSet<QString> &result)
{
    bool shouldCheck = true;
    if (this->dawg->findPartialWord(toCheck) == NULL_NODE) {
        pruned++;
        shouldCheck = false;
    }
    followed++;
    if (shouldCheck) {
        findSubwords(leftoverStr, toCheck, result);
    }
}

void ReverseAnagrammer::findSubwords(QString str, QString substr, QSet<QString> &result)
{
    if (this->dawg->findWord(substr)) {
        result.insert(substr);
    }
    QString toCheck;
    QString leftoverStr;
    for (int i = 0; i < str.length(); i++) {
        leftoverStr = str.mid(0, i) + str.mid(i+1);
        if (str[i] == '?') {
            for (int j = 0; j < 26; j++) {
                toCheck = substr + ('a' + j);
                findSubwordsSubloop(toCheck, leftoverStr, result);
            }
        }
        else {
            toCheck = substr + str[i];
            findSubwordsSubloop(toCheck, leftoverStr, result);
        }
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
        results = findReverseAnagrams(arguments[2], arguments[3].toInt());
    }
    foreach (QString result, results) {
        qDebug(result.toAscii());
    }
    qDebug() << results.length() << "total words";
    emit finished();
}
