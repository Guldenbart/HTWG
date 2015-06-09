#include "mainwindow.h"
#include "SleepStage.h"

#include <QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <fstream>

#include "aquila/global.h"
#include "aquila/source/generator/SineGenerator.h"
#include "aquila/transform/FftFactory.h"
#include "aquila/tools/TextPlot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


	/*
    const std::size_t SIZE = 16;
	const Aquila::FrequencyType sampleFreq = 160;
    const Aquila::FrequencyType f1 = 30;

    Aquila::SineGenerator sineGenerator1 = Aquila::SineGenerator(sampleFreq);
    sineGenerator1.setAmplitude(5).setFrequency(f1).generate(SIZE);
    Aquila::SignalSource sum = sineGenerator1;


	Aquila::TextPlot aPlt("Input signal");
	aPlt.plot(sum);

    // calculate the FFT
    auto fft = Aquila::FftFactory::getFft(SIZE);
	//const Aquila::SampleType* st = sum.toArray();
    Aquila::SpectrumType spectrum = fft->fft(sum.toArray());

	aPlt.setTitle("Spectrum");
	aPlt.plotSpectrum(spectrum);
	*/


    SleepStage *ss = new SleepStage();
	ss->readECGInput("C://HTWG//HTWG//Seminar//ECG.csv");
	ss->readHypnoInput("C:/HTWG/HTWG/Seminar/annotations.txt");
	ss->detectRPeaks();
	ss->getBPM();
	ss->fftCalculation();

	double* x_ECG, *y_ECG, *x_BPM, *y_BPM, *x_LFHF, *y_LFHF, *x_relHF, *y_relHF, *x_varHF, *y_varHF, *x_hypIn, *y_hypIn, *x_hyp, *y_hyp;

	// plot ECG data
	/*
	int size_ECG = ss->getECGInputForPlot(x_ECG, y_ECG);
	QwtPlot plot_ECG(QwtText(QString("ECG data")));
	QwtPlotCurve *curve_ECG = new QwtPlotCurve();

	curve_ECG->setRawSamples(x_ECG, y_ECG, size_ECG);
	curve_ECG->setPen(QPen(Qt::red));
	curve_ECG->attach(&plot_ECG);
	plot_ECG.setAxisScale(2, 0.0, 33000.0);
	w.addQwtPlot(&plot_ECG);
	plot_ECG.show();
	*/


	// plot BPM data
	/*
	int size_BPM = ss->getBPMForPlot(x_BPM, y_BPM);

	QwtPlot plot_BPM(QwtText(QString("BPM data")));
	QwtPlotCurve *curve_BPM = new QwtPlotCurve();

	curve_BPM->setRawSamples(x_BPM, y_BPM, size_BPM);
	curve_BPM->setPen(QPen(Qt::black));
	curve_BPM->attach(&plot_BPM);
	plot_BPM.setAxisScale(2, 0.0, 33000.0);
	w.addQwtPlot(&plot_BPM);
	plot_BPM.show();
	*/

	// plot LF/HF ratio data
	int size_LfHfRatio = ss->getLfHfRatioForPlot(x_LFHF, y_LFHF);

	QwtPlot plot_LfHfRatio(QwtText(QString("LF/HF ratio")));
	QwtPlotCurve *curve_LfHfRatio = new QwtPlotCurve();

	double *x, *y;
	x = new double[2];
	y = new double[2];
	x[0] = 0.0; x[1] = 32000.0;
	y[0] = y[1] = 1.3;
	QwtPlotCurve *line_thresh = new QwtPlotCurve();
	line_thresh->setRawSamples(x, y, 2);
	line_thresh->setPen(QPen(Qt::red));
	line_thresh->attach(&plot_LfHfRatio);

	curve_LfHfRatio->setRawSamples(x_LFHF, y_LFHF, size_LfHfRatio);
	curve_LfHfRatio->setPen(QPen(Qt::gray));
	curve_LfHfRatio->attach(&plot_LfHfRatio);
	plot_LfHfRatio.setAxisScale(2, 0.0, 33000.0);
	plot_LfHfRatio.setAxisScale(0, 0.4, 2.2);
	w.addQwtPlot(&plot_LfHfRatio);
	plot_LfHfRatio.show();

	// plot relative power of HF data
	int size_relativePowerHF = ss->getRelativePowerHfForPlot(x_relHF, y_relHF);

	QwtPlot plot_relativePowerHF(QwtText(QString("relative power of the HF band")));
	QwtPlotCurve *curve_relativePowerHF = new QwtPlotCurve();

	/*
	y[0] = y[1] = 0.02;
	line_thresh = new QwtPlotCurve();
	line_thresh->setRawSamples(x, y, 2);
	line_thresh->setPen(QPen(Qt::red));
	line_thresh->attach(&plot_relativePowerHF);
	*/

	curve_relativePowerHF->setRawSamples(x_relHF, y_relHF, size_relativePowerHF);
	curve_relativePowerHF->setPen(QPen(Qt::green));
	curve_relativePowerHF->attach(&plot_relativePowerHF);
	plot_relativePowerHF.setAxisScale(2, 0.0, 33000.0);
	w.addQwtPlot(&plot_relativePowerHF);
	plot_relativePowerHF.show();


	// plot variability in the HF band data
	int size_varHF = ss->getVariabilityHfForPlot(x_varHF, y_varHF);

	QwtPlot plot_varHF(QwtText(QString("variability of the HF band")));
	QwtPlotCurve *curve_varHF = new QwtPlotCurve();

	curve_varHF->setRawSamples(x_varHF, y_varHF, size_varHF);
	curve_varHF->setPen(QPen(Qt::darkGreen));
	curve_varHF->attach(&plot_varHF);
	plot_varHF.setAxisScale(2, 0.0, 33000.0);
	w.addQwtPlot(&plot_varHF);
	plot_varHF.show();


	// plot prescored hypnogram
	int size_hypIn = ss->getHypnoInputForPlot(x_hypIn, y_hypIn);

	QwtPlot plot_hypIn(QwtText(QString("prescored hypnogram")));
	QwtPlotCurve *curve_hypIn = new QwtPlotCurve();

	curve_hypIn->setRawSamples(x_hypIn, y_hypIn, size_hypIn);
	curve_hypIn->setPen(QPen(Qt::darkBlue));
	curve_hypIn->attach(&plot_hypIn);
	plot_hypIn.setAxisScale(2, 0.0, 33000.0);
	plot_hypIn.setAxisScale(0, -3.0, 0.0, 1.0);
	w.addQwtPlot(&plot_hypIn);
	plot_hypIn.show();

	double d[11] = {1.30, 1.32, 1.34, 1.36, 1.38, 1.40, 1.42, 1.44, 1.46, 1.48, 1.50};
	double e[11] = {0.010, 0.011, 0.012, 0.013, 0.014, 0.015, 0.016, 0.017, 0.018, 0.019, 0.020};
	double f[11] = {50.0, 65.0, 80.0, 90.0, 100.0, 150.0, 200.0, 350.0, 450.0, 500.0, 600.0};
	double best = 0.0;
	std::ofstream fout("C://HTWG//HTWG//Seminar//results.txt");
	fout << "LF/HF ratio" << '\t' << "rel.power HF" << '\t' << "var HF" << "\t\t" << "result:\n";

	for (int i=0; i<11; i++) {
		for (int j=0; j<11; j++) {
			for (int k=0; k<11; k++) {
				double result = ss->evaluateThresholds(d[i], e[j], f[k], best);
				qDebug() << i<< " | " << j << " | " << k;
				fout << d[i] << "\t\t" << e[j] << "\t\t" << f[k] << "\t\t" << result << '\n';
				best = std::max(result, best);
			}
		}
	}

	fout.close();

	// plot self scored hypnogram
	int size_hyp = ss->getHypnogramForPlot(x_hyp, y_hyp);

	QwtPlot plot_hyp(QwtText(QString("self-scored hypnogram")));
	QwtPlotCurve *curve_hyp = new QwtPlotCurve();

	curve_hyp->setRawSamples(x_hyp, y_hyp, size_hyp);
	curve_hyp->setPen(QPen(Qt::darkBlue));
	curve_hyp->attach(&plot_hyp);
	plot_hyp.setAxisScale(2, 0.0, 33000.0);
	plot_hyp.setAxisScale(0, -3.0, 0.0, 1.0);
	w.addQwtPlot(&plot_hyp);
	plot_hyp.show();

	// start the program
    return a.exec();
}
