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
        int readHypnoInput(QString path);   // read the hypnogram from file
		int fftCalculation();               // do FFT and calculate the required values
		int postFft();                      // some calculations that have to be done after FFT

	private:
	// functions

	// constants
		const int ECG_INPUT_SIZE= 129596;
		const int FFT_SIZE      = 4096;
		const int FFT_HALF_SIZE	= 2048;
		const int LF_START      = 41;
		const int LF_END        = 154;
		const int HF_START      = 155;
		const int HF_END        = 410;

	// vars
        QList<double> ECGInput;		// ECG data, already downsampled
        QList<char> hypnoInput;		// already prescored hypnogram
                                    /*
                                     * W: wake
                                     * R: REM-sleep
                                     * 1&2: NREM 1-2 => NREM1
                                     * 3: NREM 3-4 => NREM2
                                     */

        QList<double> FFTResult;		// result from calculating FFT from 'hypnoInput'
        QList<double> lfhfRatio;        // calculated LF/HF ratio for every second
        QList<double> relativePowerHF;  // the calculated relative peak frequency power within the HF band
		QList<double> variabilityHF;	// the variablity of the peak frequency power within the HF band
		
        QList<char>	hypnogram;  		// self-calculated hypnogram
};

#endif // SLEEPSTAGE_H
