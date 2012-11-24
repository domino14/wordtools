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
        qDebug() << "option letters, where option can be: ";
        qDebug() << "anagram  -- find anagram for these letters (use ? for blanks)";
        qDebug() << "build    -- build all words with these letters (use ? for blanks)";
        qDebug() << "revanag  -- find 7 & 8 letter 'reverse anagrams' for these letters";
        return 0;
    }

    ReverseAnagrammer *rev = new ReverseAnagrammer(&a);
    QObject::connect(rev, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, rev, SLOT(run()));
    return a.exec();
}
