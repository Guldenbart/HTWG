/*
 * class with functions to detect sleep stages
 * Author: Daniel Fritz
 */
#ifndef SLEEPSTAGE_H
#define SLEEPSTAGE_H

#include <QList>

class SleepStage
{
	public:

        SleepStage();                       // default-Constructor
        ~SleepStage();                      // destructor

        int readECGInput(QString path);     // read the ECG from file
		int detectRPeaks();					// detects all R peaks from ECG data
		int getBPM();						// calculate BPM for every .25 seconds
        int readHypnoInput(QString path);   // read the hypnogram from file
		int fftCalculation();               // do FFT and calculate the required values
		int postFft();                      // some calculations that have to be done after FFT
		double evaluateThresholds(double d, double e, double f, double best);

		int getECGInputForPlot(double*& x, double*& y);
		int getHypnoInputForPlot(double*& x, double*& y);
		int getBPMForPlot(double*& x, double*& y);
		int getLfHfRatioForPlot(double*& x, double*& y);
		int getRelativePowerHfForPlot(double*& x, double*& y);
		int getVariabilityHfForPlot(double*& x, double*& y);
		int getHypnogramForPlot(double*& x, double*& y);

	private:
	// functions

	// constants
		const int ECG_INPUT_SIZE= 129596;
		const int FFT_SIZE      = 4096;
		const int FFT_HALF_SIZE	= 2048;
		const double R_THRESH	= 0.47;
		const int LF_START      = 41;
		const int LF_END        = 154;
		const int HF_START      = 155;
		const int HF_END        = 410;

	// vars
		QList<double> ECGInput;		// ECG data; one value each .004 sec
		QList<double> RPeaks;		// timestamps from 21:30 where R peaks are detected
		QList<double> BPMs;			// BPM value for every .25 seconds
		QList<int> hypnoInput;		// already prescored hypnogram
                                    /*
									 * 2: undefined
									 * 1: intermediate state (wake/REM)
									 * 0: wake
									 * -1: REM-sleep
									 * -2: NREM 1-2 => NREM1
									 * -3: NREM 3-4 => NREM2
                                     */

		QList<double> LfHfRatio;        // calculated LF/HF ratio for every second
        QList<double> relativePowerHF;  // the calculated relative peak frequency power within the HF band
		QList<double> variabilityHF;	// the variablity of the peak frequency power within the HF band
		double LfHfRatio_min;
		double LfHfRatio_90;
		double relativePowerHF_min;
		double relativePowerHF_90;
		
		QList<int>	hypnogram;  		// self-calculated hypnogram
};

#endif // SLEEPSTAGE_H
