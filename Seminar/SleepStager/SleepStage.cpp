#include "SleepStage.h"

#include <QString>
#include <QDebug>
#include <QFile>

#include <math.h>


/*
 * default-constructor
 */
SleepStage::SleepStage()
{

}

/*
 * destructor
 */
SleepStage::~SleepStage()
{

}


/*
 * readECGInput
 *
 * reads ECG data from 'file', downsamples it and writes it to 'ECGInput'
 *
 * param:
 * - path: path to the file with the ECG data
 *
 * return: error value
 */
int SleepStage::readECGInput(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return -1;
    }

    // we have to skip the first two lines because they are legend/caption
    file.readLine();
    file.readLine();

    char buf[32];
    int counter = 0;
    int progressCounter = 12956; // (number of values to calculate: 129596)

    while (!file.atEnd()) {
        double sum = 0.0;

        /*
         * There are 250 samples per second and we only need 4. 250/4 = 62,5.
         * So we take 63 samples for the first value, 62 for the 2nd, 63 for th 3rd and so on...
         */
        for (int i=0+(counter%2); i<63; i++) {
            file.readLine(buf, sizeof(buf));
            QString line(buf);
            double curVal = (line.section(',',1,1)).toDouble();
            sum += curVal;

            memset(buf, 0, sizeof(buf)); // reset buffer
        }
        this->ECGInput.append(sum/static_cast<double>(63-(counter%2)));

        counter++;
        if (counter > progressCounter) {
            qDebug() << "Progress: " << static_cast<int>(ceil((static_cast<double>(progressCounter)/static_cast<double>(ECGInputSize))*100)) << "%";
            progressCounter += 12956;
        }
    }

    file.close();
    return 0;
}


/*
 * readHypnoInput
 *
 * read the hypnogram from file, extend it (it's only once every 30 sec) and write it to 'hypnoInput'
 *
 * param:
 * - path: path to the file with the hypnogram
 *
 * return: error value
 */
int SleepStage::readHypnoInput(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return -1;
    }

    // We have to skip the first line because it is legend/caption
    file.readLine();

    char buf[64];

    while (!file.atEnd()) {

        qint64 lineSize = file.readLine(buf, sizeof(buf));
        QString line(buf);
        char annotation = line.at(lineSize-3).toLatin1(); // we have to skip the last 3 characters ('\r', '\n', '\0')

        // We have one annotation for every 30 sec. But we need 4 per second
        for (int i=0; i<120; i++) {
            this->hypnoInput.append(annotation);
        }
    }

    file.close();

    /*
     * we propably have more items now in 'hypnoInput' than in 'ECGInput', at best, they are the same.
     * But we have to be sure of it, so:
     */

    if (!this->ECGInput.empty()) {
        for (int i=(this->hypnoInput.size()-this->ECGInput.size()); i>0; i--) {
            this->hypnoInput.removeLast();
        }
    }

    return 0;
}
