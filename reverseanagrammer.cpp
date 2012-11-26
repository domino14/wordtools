#include "reverseanagrammer.h"
#include <QtAlgorithms>
#include <QSet>

int pruned = 0;
int followed = 0;
QString letters = "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ??";
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

struct Rack {
    QString letters;
    double combinations;
    QString goesWith;
    Rack(QString _letters, double _combinations, QString _goesWith) : letters(_letters),
        combinations(_combinations),
        goesWith(_goesWith) {}
};

bool probLessThan(const Rack &a1, const Rack &a2)
{
    return a1.combinations > a2.combinations;
}


void generateUniqueRacks(bool blank) {
    if (!blank) {
        letters = letters.mid(0, 98);
    }
    for (int rackLength = 6; rackLength <= 7; rackLength++) {
        QSet <QString> combos;
        QString suffix = blank ? "_blank.txt" : ".txt";
        QString filename = "combos" + QString::number(rackLength) + suffix;
        genLetterCombinations(letters, rackLength, combos, "");
        qDebug() << "number of combos" << combos.size();
        QList <QString> combosList = combos.toList();
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        foreach (QString combo, combosList) {
            out << combo << "\n";
        }
        file.close();
    }
}

void writeToFile(QList<Rack> racks, QString filename) {
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    for (int i = 0; i < racks.length(); i++) {
        out << racks[i].letters << "\t" << racks[i].combinations << "\t" << racks[i].goesWith << "\n";
    }
    file.close();
}

void ReverseAnagrammer::generateLiveRacks(int rackLength, int withBlanks) {
    // needs a combos6_blank.txt or combos7_blank.txt file to exist
    // generates all non-dead racks (throws away racks that don't work with an extra blank)

    QString filename = "combos" + QString::number(rackLength) + (withBlanks ? "_blank.txt" : ".txt");
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QSet <QString> racks;
    while (!in.atEnd()) {
        QString line = in.readLine();
        racks.insert(line);
    }
    file.close();

    // racks contains all unique racks of this length

    QList <Rack> liveracks;
    QList <Rack> deadracks;
    foreach (QString rack, racks) {
        double combinations = utilities.combinations(rack);
        // check with all letters
        bool found = false;
        QString goesWith;
        for (char i = 'A'; i <= 'Z'; i++) {
            if (findAnagrams(rack + i).length() != 0) {
                found = true;
                goesWith += i;
            }
        }
        if (found) {
            liveracks.append(Rack(rack, combinations, goesWith));
            if (liveracks.size() % 5000 == 0) {
                qDebug() << "Found" << liveracks.size() << "live racks of this length so far";
            }
        }
        else {
            deadracks.append(Rack(rack, combinations, goesWith));   // of course goesWith is empty
        }
    }
    qSort(liveracks.begin(), liveracks.end(), probLessThan);
    qSort(deadracks.begin(), deadracks.end(), probLessThan);

    writeToFile(liveracks, "live" + QString::number(rackLength) + (withBlanks ? "_blank.txt" : ".txt"));
    writeToFile(deadracks, "dead" + QString::number(rackLength) + (withBlanks ? "_blank.txt" : ".txt"));
}

ReverseAnagrammer::ReverseAnagrammer(QObject* parent) : QObject(parent)
{
    utilities.resetLetterDistributionVariables();
}

void ReverseAnagrammer::test()
{
    qDebug() << utilities.combinations("AEINRST");
    qDebug() << utilities.combinations("MUUMUUS");
}

QStringList ReverseAnagrammer::findReverseAnagrams(QString str, int rackLength)
{
    qDebug() << "Finding reverse anagrams for " << str << rackLength;
    /* a reverse anagram finder
        revanag KENJI 7 will find all 7-letter racks that go with KENJI to make an 8.*/

    return QStringList();
}

QStringList ReverseAnagrammer::findAnagrams(QString str)
{
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
        int rackLength = arguments[3].toInt();
        if (rackLength != 6 && rackLength != 7) {
            qDebug() << "Rack length must be 6 or 7";
        } else {
            results = findReverseAnagrams(arguments[2], rackLength);
        }
    }
    if (arguments[1] == "test") {
        test();
    }
    if (arguments[1] == "liveracks") {
        generateLiveRacks(arguments[2].toInt(), arguments[3].toInt());
    }

    foreach (QString result, results) {
        qDebug(result.toAscii());
    }
    qDebug() << results.length() << "total words";
    emit finished();
}
