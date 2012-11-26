#ifndef UTILITIES_H
#define UTILITIES_H
#include <QString>
#include <QList>
#include <QMap>

class Utilities
{
public:
    Utilities();
    void resetLetterDistributionVariables();
    double combinations(QString);
private:
    QList <double> fullChooseCombos;    // copied from Zyzzyva
    QList<QList<double> > subChooseCombos; // ditto
    QMap <unsigned char, int> letterDist;
};

#endif // UTILITIES_H
