#include "mainwindow.h"
#include "SleepStage.h"

#include <QApplication>
#include <QDateTime>
#include <QProgressDialog>

#include <fstream>
#include <string>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include "aquila/global.h"
#include "aquila/source/generator/SineGenerator.h"
#include "aquila/transform/FftFactory.h"
#include "aquila/tools/TextPlot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

	int retVal = 0;
	SleepStage *ss = new SleepStage();

	retVal = ss->readECGInput("C://HTWG//HTWG//Seminar//ECG.csv");
	if (retVal < 0) {
		qDebug() << "Couldn't read ECG input file!";
		delete ss;
		return retVal;
	}

	retVal = ss->readHypnoInput("C:/HTWG/HTWG/Seminar/annotations.txt");
	if (retVal < 0) {
		qDebug() << "Couldn't read hypnogram input file!";
		delete ss;
		return retVal;
	}

	retVal = ss->detectRPeaks();
	if (retVal < 0) {
		qDebug() << "R peak detection failed!";
		delete ss;
		return retVal;
	}

	retVal = ss->getBPM();
	if (retVal < 0) {
		qDebug() << "heart rate calculation failed!";
		delete ss;
		return retVal;
	}

	ss->fftCalculation(0.5);

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

	int size_BPM = ss->getBPMForPlot(x_BPM, y_BPM);

	QwtPlot plot_BPM(QwtText(QString("BPM data")));
	QwtPlotCurve *curve_BPM = new QwtPlotCurve();

	curve_BPM->setRawSamples(x_BPM, y_BPM, size_BPM);
	curve_BPM->setPen(QPen(Qt::black));
	curve_BPM->attach(&plot_BPM);
	plot_BPM.setAxisScale(2, 0.0, 33000.0);
	//w.addQwtPlot(&plot_BPM);
	plot_BPM.show();


	// plot LF/HF ratio data
	int size_LfHfRatio = ss->getLfHfRatioForPlot(x_LFHF, y_LFHF);

	QwtPlot plot_LfHfRatio(QwtText(QString("LF/HF ratio")));
	QwtPlotCurve *curve_LfHfRatio = new QwtPlotCurve();

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


	QList<double> d1, d2, e, f;
	d1 << 1.00 << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << 1.30 << 1.35 << 1.40 << 1.45 << 1.50;
	d2 << 0.1 << 0.15 << 0.2 << 0.25 << 0.3 << 0.35 << 0.4 << 0.45 << 0.5 << 0.55 << 0.6 << 0.65 << 0.7 << 0.75 << 0.8 << 0.85 << 0.9 << 0.95 << 1.0;
	e << 0.010 << 0.011 << 0.012 << 0.013 << 0.014 << 0.015 << 0.016 << 0.017 << 0.018 << 0.019 << 0.020;
	f << 50.0 << 65.0 << 80.0 << 90.0 << 100.0 << 120.0 << 150.0 << 200.0 << 350.0 << 450.0 << 500.0 << 600.0;
	double best = 0.0;
	QMap<double, QList<double> >results;
	QMap<double, QList<int> >hypnoMap;
	QDateTime now = QDateTime::currentDateTime();
	std::string filename = "C://HTWG//HTWG//Seminar//results_" + ((now.date().toString("dd.MM.yyyy"))).toStdString() +".txt";
	std::ofstream fout(filename);
	fout << "value for the threshold: " << 50 << " percent\n";
	fout << "LF/HF ratio" << '\t' << "upper limit" << '\t' << "rel.power HF" << '\t' << "var HF" << "\t\t" << "result:\n";

	QProgressDialog progress("evaluating thresholds", "Cancel [don't!]", 0, (d1.size()*d2.size()*e.size()*f.size()));
	progress.setWindowTitle("Evaluating...");
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setWindowFlags(Qt::Tool);
	int progressCounter = 0;

	for (int i=0; i<d1.size(); i++) {
		for (int j=0; j<d2.size(); j++) {
			for (int k=0; k<e.size(); k++) {
				for (int l=0; l<f.size(); l++) {
					double result = ss->evaluateThresholds(d1.at(i), d2.at(j), e.at(k), f.at(l), best);
					qDebug() << i<< " | " << j << " | " << k << " | " << l;
					fout << d1.at(i) << "\t\t" << d2.at(j) << "\t\t" << e.at(k) << "\t\t" << f.at(l) << "\t\t" << result << '\n';

					hypnoMap.insert(result, ss->getLatestHypnogram());
					QList<double> indices = {d1.at(i), d2.at(j), e.at(k), f.at(l)};
					results.insert(result, indices);
					best = std::max(result, best);

					progress.setValue(++progressCounter);
				}
			}
		}
	}

	QMapIterator<double, QList<double> > it_results(results);
	it_results.toBack();
	it_results.previous();

	fout << "Best result:\n" << it_results.value().at(0) << "\t\t" << it_results.value().at(1) << "\t\t" << it_results.value().at(2) << "\t\t" << it_results.value().at(3) << "\t\t" << it_results.key();

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


	/*
	 * plot threshold values in the three diagrams
	 * structure:	x for LH/HF ratio Nr1
	 *				y for "	"	"	"
	 *				x for rel power HF Nr1
	 *				....
	 */
	double** thresholds = new double*[18];
	for (int i=0; i<18; i+=2) {
		thresholds[i] = new double[2];
		thresholds[i][0] = 0.0;
		thresholds[i][1] = 32000.0;
	}
	for (int i=0; i<9; i++) {
		thresholds[i*2+1] = new double[2];
		thresholds[i*2+1][0] = thresholds[i*2+1][1] = (it_results.value()).at(i%3);

		if (i%3 == 2) {
			it_results.previous();
		}
	}

	/*
	for (int i=0; i<3; i++) {
		QwtPlotCurve *line_thresh = new QwtPlotCurve();
		line_thresh->setRawSamples(thresholds[i*6], thresholds[i*6+1], 2);
		line_thresh->setPen(QPen(QColor(7+i)));
		line_thresh->attach(&plot_LfHfRatio);
	}

	for (int i=0; i<3; i++) {
		QwtPlotCurve *line_thresh = new QwtPlotCurve();
		line_thresh->setRawSamples(thresholds[i*6+2], thresholds[i*6+3], 2);
		line_thresh->setPen(QPen(QColor(7+i)));
		line_thresh->attach(&plot_relativePowerHF);
	}

	for (int i=0; i<3; i++) {
		QwtPlotCurve *line_thresh = new QwtPlotCurve();
		line_thresh->setRawSamples(thresholds[i*6+4], thresholds[i*6+5], 2);
		line_thresh->setPen(QPen(QColor(7+i)));
		line_thresh->attach(&plot_varHF);
	}
	*/


	// start the program
    return a.exec();
}
