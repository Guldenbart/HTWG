#include "mainwindow.h"
#include "SleepStage.h"

#include <QApplication>
#include <qwt.h>

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
    // input signal parameters
    const std::size_t SIZE = 64;
    const Aquila::FrequencyType sampleFreq = 2000;
    const Aquila::FrequencyType f1 = 125, f2 = 700;

    Aquila::SineGenerator sineGenerator1 = Aquila::SineGenerator(sampleFreq);
    sineGenerator1.setAmplitude(32).setFrequency(f1).generate(SIZE);
    Aquila::SineGenerator sineGenerator2 = Aquila::SineGenerator(sampleFreq);
    sineGenerator2.setAmplitude(8).setFrequency(f2).setPhase(0.75).generate(SIZE);
    Aquila::SignalSource sum = sineGenerator1 + sineGenerator2;

    Aquila::TextPlot plt("Input signal");
    plt.plot(sum);

    // calculate the FFT
    auto fft = Aquila::FftFactory::getFft(SIZE);
    Aquila::SpectrumType spectrum = fft->fft(sum.toArray());

    plt.setTitle("Spectrum");
    plt.plotSpectrum(spectrum);
    */

    SleepStage *ss = new SleepStage();
    ss->readECGInput("C://HTWG//HTWG//Seminar//ECG.csv");
    ss->readHypnoInput("C:/HTWG/HTWG/Seminar/annotations.txt");


    return a.exec();
}
