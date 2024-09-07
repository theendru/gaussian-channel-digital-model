#include "QAMmodulator.h"
#include "QAMdemodulator.h"
#include "GaussianChannel.h"
#include "Instruments.h"


int main() {
    // Initialize parameters to work with.
    std::vector<double> SNR = { -2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<int>    modulationOrders = { 4, 16, 64 };
    Instruments InstrumentsObj;

    // Write to file parameters (required to plot BER).
    std::vector<double> BER;
    for (double i : SNR)
        BER.push_back(i);
    BER.push_back(-1);
    for (double i : modulationOrders)
        BER.push_back(i);
    BER.push_back(-1);

    // Choose data to test system.
    std::string pathToFile = "./Data.txt";
    // Use line below and comment line above to work from terminal.
    // std::string pathToFile = getInputWithDefault( "Enter path to the file:", "/Users/theendru/Desktop/GaussianChannelDigitalModelConsoleApp/Data.txt");

    // Read file.
    std::string inputText = InstrumentsObj.readFile( pathToFile );

    // Make data binary.
    std::vector<int> inputTextBinary = InstrumentsObj.stringToBinary( inputText );

    // Initialize required objects of three developed class.
    qamModulator    QAMmodulatorObj;
    GaussianChannel GaussianChannelObj;
    qamDemodulator  QAMdemodulatorObj;

    // Start experiments.
    for (int k = 0; k < modulationOrders.size(); k++) {

        // Set modulation order.
        QAMmodulatorObj.setModulationOrder( modulationOrders[k] );
        GaussianChannelObj.setModulationOrder( modulationOrders[k] );
        QAMdemodulatorObj.setModulationOrder( modulationOrders[k] );

        // Modulate data (by Sklar - do Impulse Modulation, or do mapping).
        std::vector<std::complex<int> > dataModulated = QAMmodulatorObj.modulateData( inputTextBinary );

        for (double i : SNR) {

            // We will accumulate BER while...
            double tempBER = 0;
            // ...doing several experiments to provide ensemble averaging (assuming stationarity and ergodicity).
            int nExperiments = 100;
            for (int j = 0; j < nExperiments; j++) {

                // Add WGN to signal.
                std::vector<std::complex<double> > dataNoised = GaussianChannelObj.addGaussianNoise( dataModulated, i );

                // Demodulate data (do demapping).
                std::vector<int> dataDemodulated = QAMdemodulatorObj.demodulateData( dataNoised, modulationOrders[k] );

                tempBER += InstrumentsObj.computeBER( inputTextBinary, dataDemodulated );

            }

            // Averaging BER.
            BER.push_back( tempBER / nExperiments );

        }
    }

    // Write results in file.
    InstrumentsObj.writeFile( "./BERdata.csv", BER );

    return 1;
}