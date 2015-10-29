/*
 * class with functions to detect sleep stages
 * Author: Daniel Fritz
 */
#ifndef SLEEPSTAGE_H
#define SLEEPSTAGE_H

#include <QList>
#include <QMap>

class SleepStage
{
	public:

        SleepStage();                       // default-Constructor
        ~SleepStage();                      // destructor

		QList<int> getLatestHypnogram();

        int readECGInput(QString path);     // read the ECG from file
		int detectRPeaks();					// detects all R peaks from ECG data
		int getBPM();						// calculate BPM for every .25 seconds
        int readHypnoInput(QString path);   // read the hypnogram from file
		int fftCalculation(double n);       // do FFT and calculate the required values
		double evaluateThresholds(double d1, double d2, double e, double f, double best);	// apply thresholds to the calculated parameters and evaluate the result

		int getECGInputForPlot(double*& x, double*& y);			// prepares two arrays for a QwtPlot of the ECG input data
		int getHypnoInputForPlot(double*& x, double*& y);		// prepares two arrays for a QwtPlot of the prescored hypnogram
		int getBPMForPlot(double*& x, double*& y);				// prepares two arrays for a QwtPlot of the BPM data
		int getLfHfRatioForPlot(double*& x, double*& y);		// prepares two arrays for a QwtPlot of the LF/HF ratio data
		int getRelativePowerHfForPlot(double*& x, double*& y);	// prepares two arrays for a QwtPlot of the relative power in the HF band data
		int getVariabilityHfForPlot(double*& x, double*& y);	// prepares two arrays for a QwtPlot of the variablility in the HF band data
		int getHypnogramForPlot(double*& x, double*& y);		// prepares two arrays for a QwtPlot of the self-scored hypnogram

	private:
	// functions

	// constants
		const int FFT_SIZE      = 4096;
		const double R_THRESH	= 0.35;
		/*
		 * sampling frquency is 4 Hz. => f_max is 2 Hz.
		 * Stepwidth is 4Hz/4096 samples = 1/1024
		 */
		const int LF_START      = 41;	// 41/1024 = 0.04... Hz
		const int LF_END        = 154;	// 155/1024 = 0.1504 Hz
		const int HF_START      = 154;
		const int HF_END        = 411;	// 411/1024 = 0.4014 Hz
	// vars
		QList<double> ECGInput;			// ECG data; one value each .004 sec
		QList<double> RPeaks;			// timestamps from 21:30 where R peaks are detected
		QList<double> BPMs;				// BPM value for every .25 seconds
		QList<int> hypnoInput;			// already prescored hypnogram
										/*
										 * 1: intermediate state (wake/REM)
										 * 0: wake
										 * -1: REM-sleep
										 * -2: NREM 1-2 => NREM1
										 * -3: NREM 3-4 => NREM2
										 */

		QList<double> LfHfRatio;        // calculated LF/HF ratio for every second
        QList<double> relativePowerHF;  // the calculated relative peak frequency power within the HF band
		QList<double> variabilityHF;	// the variablity of the peak frequency power within the HF band
		
		QList<int>	hypnogram;  		// self-calculated hypnogram
		QList<int>	latestHypnogram;	// latest self scored hypnogrma
};

#endif // SLEEPSTAGE_H
