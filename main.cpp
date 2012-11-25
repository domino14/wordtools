#include <QCoreApplication>
#include "dawg.h"
#include "reverseanagrammer.h"
#include <QTimer>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList arguments = QCoreApplication::arguments();
    if (arguments.length() < 3) {
        qDebug() << "usage:\n";
        qDebug() << "option letters [arguments..], where option can be: ";
        qDebug() << "anagram  -- find anagram for these letters (use ? for blanks)";
        qDebug() << "build    -- build all words with these letters (use ? for blanks)";
        qDebug() << "revanag  -- find 'reverse anagrams' for these letters. requires number of letters as an argument";
        qDebug() <<"             e.g. wordtools revanag KENJI 7 will find all 7-letter racks that go with KENJI to make an 8.";
        return 0;
    }

    ReverseAnagrammer *rev = new ReverseAnagrammer(&a);
    QObject::connect(rev, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, rev, SLOT(run()));
    return a.exec();
}
