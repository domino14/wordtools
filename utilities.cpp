#include "utilities.h"



QMap <unsigned char, int> getEnglishDist()
{
    QMap <unsigned char, int> dist;


    dist.insert('A', 9); dist.insert('B', 2); dist.insert('C', 2);
    dist.insert('D', 4); dist.insert('E', 12); dist.insert('F', 2);
    dist.insert('G', 3); dist.insert('H', 2); dist.insert('I', 9);
    dist.insert('J', 1); dist.insert('K', 1); dist.insert('L', 4);
    dist.insert('M', 2); dist.insert('N', 6); dist.insert('O', 8);
    dist.insert('P', 2); dist.insert('Q', 1); dist.insert('R', 6);
    dist.insert('S', 4); dist.insert('T', 6); dist.insert('U', 4);
    dist.insert('V', 2); dist.insert('W', 2); dist.insert('X', 1);
    dist.insert('Y', 2); dist.insert('Z', 1); dist.insert('?', 2);
    return dist;
}

Utilities::Utilities()
{
}

void Utilities::resetLetterDistributionVariables()
{
    /* this function is basically taken from Michael Thelen's CreateDatabaseThread.cpp, part of Zyzzyva, which is
       GPLed software, source code available at http://www.zyzzyva.net, copyright Michael Thelen. */
    int maxFrequency = 15;

    int totalLetters = 0;
    letterDist = getEnglishDist();

    foreach (unsigned char c, letterDist.keys())
    {
        int frequency = letterDist.value(c);
        totalLetters += frequency;
        if (frequency > maxFrequency)
            maxFrequency = frequency;
    }

    // Precalculate M choose N combinations - use doubles because the numbers
    // get very large
    double a = 1;
    double r = 1;
    for (int i = 0; i <= maxFrequency; ++i, ++r)
    {
        fullChooseCombos.append(a);
        a *= (totalLetters + 1.0 - r) / r;

        QList<double> subList;
        for (int j = 0; j <= maxFrequency; ++j)
        {
            if ((i == j) || (j == 0))
                subList.append(1.0);
            else if (i == 0)
                subList.append(0.0);
            else {
                // XXX: For some reason this crashes on Linux when referencing
                // the first value as subChooseCombos[i-1][j-1], so value() is
                // used instead.  Weeeeird.
                subList.append(subChooseCombos.value(i-1).value(j-1) +
                               subChooseCombos.value(i-1).value(j));
            }
        }
        subChooseCombos.append(subList);
    }

}

double Utilities::combinations(QString alphagram)
{
    /* this function is basically taken from Michael Thelen's CreateDatabaseThread.cpp, part of Zyzzyva, which is
       GPLed software, source code available at http://www.zyzzyva.net, copyright Michael Thelen. */
    // Build parallel arrays of letters with their counts, and the
    // precalculated combinations based on the letter frequency
    QList<unsigned char> letters;
    QList<int> counts;
    QList<const QList<double>*> combos;
    for (int i = 0; i < alphagram.length(); ++i) {
        unsigned char c = alphagram.at(i).toAscii();

        bool foundLetter = false;
        for (int j = 0; j < letters.size(); ++j) {
            if (letters[j] == c) {
                ++counts[j];
                foundLetter = true;
                break;
            }
        }

        if (!foundLetter) {
            letters.append(c);
            counts.append(1);
            combos.append(&subChooseCombos[ letterDist[c] ]);
        }
    }

    // XXX: Generalize the following code to handle arbitrary number of blanks
    double totalCombos = 0.0;
    int numLetters = letters.size();

    // Calculate the combinations with no blanks
    double thisCombo = 1.0;
    for (int i = 0; i < numLetters; ++i) {
        thisCombo *= (*combos[i])[ counts[i] ];
    }
    totalCombos += thisCombo;

    // Calculate the combinations with one blank
    for (int i = 0; i < numLetters; ++i) {
        --counts[i];
        thisCombo = subChooseCombos[ letterDist['?'] ][1];
        for (int j = 0; j < numLetters; ++j) {
            thisCombo *= (*combos[j])[ counts[j] ];
        }
        totalCombos += thisCombo;
        ++counts[i];
    }

    // Calculate the combinations with two blanks
    for (int i = 0; i < numLetters; ++i) {
        --counts[i];
        for (int j = i; j < numLetters; ++j) {
            if (!counts[j])
                continue;
            --counts[j];
            thisCombo = subChooseCombos[ letterDist['?'] ][2];

            for (int k = 0; k < numLetters; ++k) {
                thisCombo *= (*combos[k])[ counts[k] ];
            }
            totalCombos += thisCombo;
            ++counts[j];
        }
        ++counts[i];
    }

    return totalCombos;

}


