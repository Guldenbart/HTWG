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

	private:
	// functions

	// vars
        const int ECGInputSize = 129596;

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
        QList<double> varHF;			// the variablity of the peak frequency power within the HF band
		
        QList<char>	hypnogram;  		// self-calculated hypnogram
};

#endif // SLEEPSTAGE_H
