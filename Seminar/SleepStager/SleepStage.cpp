#include "SleepStage.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QProgressDialog>

#include <math.h>
#include <complex>

#include "aquila/global.h"
#include "aquila/transform/FftFactory.h"
//#include "aquila/source/generator/SineGenerator.h"

/*
 * default constructor
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
 * getLatestHypnogram
 *
 * return: the last calculated Hypnogram
 */
QList<int> SleepStage::getLatestHypnogram()
{
	return this->latestHypnogram;
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
	// open the file
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

	QProgressDialog progress("Read ECG input", "Cancel [don't!]", 0, 8099750);
	progress.setWindowTitle("Reading...");
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowFlags(Qt::Tool);
	progress.show();

    while (!file.atEnd()) {

		// There are 250 samples per second
		file.readLine(buf, sizeof(buf));
		QString line(buf);
		double curVal = (line.section(',',1,1)).toDouble();

		memset(buf, 0, sizeof(buf)); // reset buffer
		progress.setValue(++counter);

		this->ECGInput.append(curVal);
    }

    file.close();
    return 0;
}


/*
 * detectRPeaks
 *
 * detects all R peaks from ECG data and saves their timestamp in RPeaks
 *
 * return: error value
 */
int SleepStage::detectRPeaks()
{
	if (this->ECGInput.empty()) {
		qDebug("ERROR: No ECGInput to work with!");
		return -1;
	}

	//bool carry = false;

	for (int i=1; i<(this->ECGInput.size()-1); i++) {

		/*
		if (carry) { // ECGInput[i-1] == ECGInput[i]; we still have to find out, if it's a peak or a saddle point
			if (this->ECGInput.at(i) > this->ECGInput.at(i+1)) {
				// found a peak

				this->RPeaks.append(static_cast<double>(i)*0.004); // we multiply with 0.004 only here to have a more precise time
				i++; // might as well skip 1 entry, because it can't be a peak again ;)
				carry = false;

			} else if (this->ECGInput.at(i) < this->ECGInput.at(i+1)) {

				// it wasn't a peak, just a saddle point
				carry = false;

			} else {

				// still the same value, "carry" on...
			}
		} else { // carry == false
			if (this->ECGInput.at(i) <= this->R_THRESH) {
				continue;
			}

			if ((this->ECGInput.at(i) > this->ECGInput.at(i-1)) && (this->ECGInput.at(i) > this->ECGInput.at(i+1))) {
				// found a peak

				this->RPeaks.append(static_cast<double>(i)*0.004); // we multiply with 0.004 only here to have a more precise time
				i++; // might as well skip 1 entry, because it can't be a peak again ;)
			}
			if ((this->ECGInput.at(i) > this->ECGInput.at(i-1)) && (this->ECGInput.at(i) == this->ECGInput.at(i+1))) {
				carry = true;
			}
		}
		*/

		if (this->ECGInput.at(i) > this->R_THRESH) {
			if ((this->ECGInput.at(i) >= this->ECGInput.at(i-1)) && (this->ECGInput.at(i) > this->ECGInput.at(i+1))) {
				this->RPeaks.append(static_cast<double>(i)*0.004); // we multiply with 0.004 only here to have a more precise time
				i++; // might as well skip 1 entry, because it can't be a peak again ;)
			}
		}
	}

	return 0;
}


/*
 * getBPM
 *
 * function to calculate the heart rate in [bpm] for every minute of the recording
 *
 * return: error value
 */
int SleepStage::getBPM()
{
	if (this->RPeaks.empty()) {
		qDebug("No RPeaks to work with!");
		return -1;
	}

	int numIntervals = 0;		// interval count of one minute
	double intervalTime = 0.0;	// complete interval time of one minute
	int minutes = 1;			// minute count
	for (int i=1; i<this->RPeaks.size(); i++) {

		if (this->RPeaks[i] < static_cast<double>(minutes*60)) {
			// we have another peak of the current minute

			intervalTime += (this->RPeaks[i] - this->RPeaks[i-1]);
			numIntervals++;
		} else {
			// we collected all intervals of this minute; time to calcluate the BPM value

			double bpmValue = (1.0/(intervalTime/static_cast<double>(numIntervals)))*60.0;

			// add a BPM value vor every 0.25 seconds for the FFT
			for (int k=0; k<240; k++) {
				this->BPMs.append(bpmValue);
			}

			numIntervals = 0;  // reset this counter for the next minute.
			intervalTime = 0.0;// same here.
			minutes++;
		}
	} // end for

	if (numIntervals != 0) {
		// we haven't taken care of the very last minute yet, so:

		double bpmValue = (1/(numIntervals/static_cast<double>(numIntervals)))*60.0;
		for (int k=0; k<240; k++) {
			this->BPMs.append(bpmValue);
		}
	}

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
	int sleepStage;

    while (!file.atEnd()) {

        qint64 lineSize = file.readLine(buf, sizeof(buf));
        QString line(buf);
        char annotation = line.at(lineSize-3).toLatin1(); // we have to skip the last 3 characters ('\r', '\n', '\0')

		if (annotation == 'W') {
			sleepStage = 0;
		} else if (annotation == 'R') {
			sleepStage = -1;
		} else if (annotation == '1' || annotation == '2') {
			sleepStage = -2;
		} else if (annotation == '3') {
			sleepStage = -3;
		} else {
			sleepStage = 2; // which would be a problem...
		}

		this->hypnoInput.append(sleepStage);

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


/*
 * fftCalculation
 *
 * calculates FFT for every 4096 samples and calculates all values necessary for sleep stage classification
 *
 * return: error value
 */
int SleepStage::fftCalculation(double n)
{
	QProgressDialog progress("calculating FFT", "Cancel [don't!]", 0, (this->BPMs.size()-this->FFT_SIZE));
	progress.setWindowTitle("Calculating...");
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setWindowFlags(Qt::Tool);

	for (int i=0; i<(this->BPMs.size()-this->FFT_SIZE); i++) {
		progress.setValue(i);

		double sumLF = 0.0;
		double sumHF = 0.0;
		double normSumLF = 0.0;
		double normSumHF = 0.0;
		double maxHF = -1000.0;
		double minHF = 1000.0;

		// set up FFT
		auto fft = Aquila::FftFactory::getFft(this->FFT_SIZE);

		// fill the array for FFT execution
		double* sampleArray = new double[this->FFT_SIZE];
		for (int j=0; j<this->FFT_SIZE; j++) {
			sampleArray[j] = this->BPMs[i+j];
		}

		// FFT
		const Aquila::SampleType* st = sampleArray;
		Aquila::SpectrumType fftResult = fft->fft(st);

		// get the abs value of every complex value
		double* spectrum = new double[fftResult.size()];
		for (unsigned int j=0; j<fftResult.size(); j++) {
			std::complex<double> complexTmp = fftResult.at(j);
			spectrum[j] = std::abs(complexTmp);
			// irgendeine Form der Korrektur?
		}

		// get sumLF and sumHF. first round: find out the mean value
		for (int j=this->LF_START; j<this->LF_END; j++) {
			sumLF += spectrum[j];
		}
		for (int j=this->HF_START; j<this->HF_END; j++) {
			sumHF += spectrum[j];
			if (spectrum[j] > maxHF) {
				maxHF = spectrum[j];
			}
			if (spectrum[j] < minHF) {
				minHF = spectrum[j];
			}
		}

		double meanLF = sumLF/static_cast<double>(this->LF_END-this->LF_START);
		double meanHF = sumHF/static_cast<double>(this->HF_END-this->HF_START);


		// second round:
		for (int j=this->LF_START; j<this->LF_END; j++) {
			if (spectrum[j]>(meanLF*(1.0+n)) || spectrum[j]<(meanLF*(1.0-n))) {
				continue;
			}
			normSumLF += spectrum[j];
		}
		for (int j=this->HF_START; j<this->HF_END; j++) {
			if (spectrum[j]>(meanHF*(1.0+n)) || spectrum[j]<(meanHF*(1.0-n))) {
				continue;
			}
			normSumHF += spectrum[j];
		}

		// add the values to their respective QList
		this->LfHfRatio.append(normSumLF/normSumHF);
		this->relativePowerHF.append(maxHF/sumHF);
		this->variabilityHF.append(maxHF-minHF);

		delete sampleArray;
		delete spectrum;
	}

	return 0;
}


/*
 * evaluateThresholds
 *
 * determines the sleep stages for three given threshold values
 * and compares the result with the prescored hypnogram
 *
 * param:
 * - d1: lower threshold value for the LF/HF ratio
 * - d2: parameter for the upper threshold value for the LF/HF ratio, being d1+d2
 * - e: threshold value for the relative power in te HF band
 * - f: threshold value for the variability of the HF band
 * - best: value to compare the degree of accordance with the prescored hypnogram with.
 *
 * return: degree of accordance with the prescored hypnogram [%] if the values calculated this round were better than 'best'.
 *		otherwise 'best'.
 */
double SleepStage::evaluateThresholds(double d1, double d2, double e, double f, double best)
{
	QList<int> ownHypno; // our own sleep stage classification for the three given parameters

	// first we assume NREM1/2 for everything.
	for (int i=0; i< this->LfHfRatio.size(); i++) {
		ownHypno.append(-2);
	}

	// first parameter: LF/HF ratio
	for (int i=0; i < this->LfHfRatio.size(); i++) {
		if (this->LfHfRatio.at(i) >= (d1+d2)) {
			ownHypno[i] = 1;
		} else if(this->LfHfRatio.at(i) < d1){
			ownHypno[i] = -3;
		}
	}

	// second parameter: relative power HF
	for (int i=0; i<ownHypno.size(); i++) {
		if (ownHypno.at(i) != -1) {
			// we only care about epochs that are marked as 'NREM12'
			continue;
		}

		if (this->relativePowerHF.at(i) >= e) {
			ownHypno[i] = 1;
		}
	}

	// third parameter: variability HF
	for (int i=0; i<ownHypno.size(); i++) {
		if (ownHypno.at(i) != 1) {
			// we only care about epochs that were previously marked as 'wake or REM'
			continue;
		}

		if (this->variabilityHF.at(i) >= f) {
			ownHypno[i] = -1;
		} else {
			ownHypno[i] = 0;
		}
	}

	// go back to 30s epochs
	QList<int> ownHypnoEpochs;
	int epochCounter = 0;
	int epochEnd;
	int wakeCounter, remCounter, nrem12Counter, nrem34Counter;

	// 30s * 4 values/s = 120
	while ((epochCounter*120) < ownHypno.size()) {
		wakeCounter = remCounter = nrem12Counter = nrem34Counter = 0;

		if (((epochCounter+1)*120) < ownHypno.size()) {
			epochEnd = 120;
		} else {
			epochEnd = ownHypno.size()-(epochCounter*120);
		}

		for (int i=0; i<epochEnd; i++) {
			switch (ownHypno.at((epochCounter*120)+i)) {
				case 0:
					wakeCounter++;
					break;
				case -1:
					remCounter++;
					break;
				case -2:
					nrem12Counter++;
					break;
				default:
					// should be -3
					nrem34Counter++;
					break;
			}
		}

		// use a QMap to sort the values
		QMap<int, int> m;
		m.insert(wakeCounter, 0);
		m.insert(remCounter, -1);
		m.insert(nrem12Counter, -2);
		m.insert(nrem34Counter, -3);
		ownHypnoEpochs.append(m.value(m.lastKey()));

		epochCounter++;
	}

	// now that we did all the classifying, let's see how good it was:
	int accordanceCounter = 0;
	for (int i=0; i<ownHypnoEpochs.size(); i++) {
		if (ownHypnoEpochs.at(i) == this->hypnoInput.at(i)) {
			accordanceCounter++;
		}
	}


	/* without 30s epochs
	for (int i=0; i<ownHypno.size(); i++) {
		if (ownHypno.at(i) == this->hypnoInput.at( (i-(i%120))/120 )) {
			accordanceCounter++;
		}
	}
	*/

	this->latestHypnogram = ownHypnoEpochs;
	// check if we have a new 'best result'
	double accordance = static_cast<double>(accordanceCounter)/static_cast<double>(ownHypnoEpochs.size());
	if (accordance > best) {
		this->hypnogram = ownHypnoEpochs;
	}

	return accordance;
}


/*
 * getECGInputForPlot
 *
 * prepares two arrays for a QwtPlot of the ECG input data
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getECGInputForPlot(double *&x, double *&y)
{
	x = new double[this->ECGInput.size()];
	y = new double[this->ECGInput.size()];

	for (int i=0; i<this->ECGInput.size(); i++) {
		x[i] = static_cast<double>(i)*0.25;
		y[i] = this->ECGInput[i];
	}

	return this->ECGInput.size();
}


/*
 * getHypnoInputForPlot
 *
 * prepares two arrays for a QwtPlot of the prescored hypnogram
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getHypnoInputForPlot(double *&x, double *&y)
{
	x = new double[this->hypnoInput.size()];
	y = new double[this->hypnoInput.size()];

	for (int i=0; i<this->hypnoInput.size(); i++) {
		x[i] = static_cast<double>(i)*30.0;
		y[i] = this->hypnoInput[i];
	}

	return this->hypnoInput.size();
}


/*
 * getBPMForPlot
 *
 * prepares two arrays for a QwtPlot of the BPM data
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getBPMForPlot(double *&x, double *&y)
{
	x = new double[this->BPMs.size()];
	y = new double[this->BPMs.size()];

	for (int i=0; i<this->BPMs.size(); i++) {
		x[i] = static_cast<double>(i)*0.25;
		y[i] = this->BPMs[i];
	}

	return this->BPMs.size();
}


/*
 * getLfHfRatioForPlot
 *
 * prepares two arrays for a QwtPlot of the LF/HF ratio data
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getLfHfRatioForPlot(double*& x, double*& y)
{
	x = new double[this->LfHfRatio.size()];
	y = new double[this->LfHfRatio.size()];

	for (int i=0; i<this->LfHfRatio.size(); i++) {
	//for (int i=0; i<20000; i++) {
		x[i] = static_cast<double>(i)*0.25;
		y[i] = this->LfHfRatio[i];
	}

	return this->LfHfRatio.size();
	//return 20000;
}


/*
 * getRelativePowerHfForPlot
 *
 * prepares two arrays for a QwtPlot of the relative power in the HF band data
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getRelativePowerHfForPlot(double *&x, double *&y)
{
	x = new double[this->relativePowerHF.size()];
	y = new double[this->relativePowerHF.size()];

	for (int i=0; i<this->relativePowerHF.size(); i++) {
		x[i] = static_cast<double>(i)*0.25;
		y[i] = this->relativePowerHF[i];
	}

	return this->relativePowerHF.size();
}


/*
 * getVariabilityHfForPlot
 *
 * prepares two arrays for a QwtPlot of the variablility in the HF band data
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getVariabilityHfForPlot(double *&x, double *&y)
{
	x = new double[this->variabilityHF.size()];
	y = new double[this->variabilityHF.size()];

	for (int i=0; i<this->variabilityHF.size(); i++) {
		x[i] = static_cast<double>(i)*0.25;
		y[i] = this->variabilityHF[i];
	}

	return this->variabilityHF.size();
}


/*
 * getHypnogramForPlot
 *
 * prepares two arrays for a QwtPlot of the self-scored hypnogram
 *
 * param:
 * -x: array for the x-axis values
 * -y: array for the y-axis values
 *
 * return: number of values in the arrays
 */
int SleepStage::getHypnogramForPlot(double *&x, double *&y)
{
	x = new double[this->hypnogram.size()];
	y = new double[this->hypnogram.size()];

	for (int i=0; i<this->hypnogram.size(); i++) {
		//x[i] = static_cast<double>(i)*0.25;
		x[i] = static_cast<double>(i)*30.0;
		y[i] = this->hypnogram[i];
	}

	return this->hypnogram.size();
}

