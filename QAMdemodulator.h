// This class do simple QAM demodulation based on Euclidian distance.
//
// Created by Konstantin Terentev on 05.09.2024 for YADRO.

#include <complex>
#include <vector>

#ifndef GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_QAMDEMODULATOR_H
#define GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_QAMDEMODULATOR_H


class qamDemodulator: public qamModulator {
public:


    /*
     * TODO: normalization of input signal (in general its necessary, but in our task there is no need)
     */
    std::vector<std::complex<int> > normalize(const std::vector<std::complex<int> > input)
    {
        std::vector<std::complex<int> > signalNormalized;
        return signalNormalized;
    }


    /**
     * Demap input QAM modulated data.
     *
     * @param inputData is a complex vector of modulated data.
     * @param complexValuesConstellation is a vector of complex symbols.
     * @param GreyCodes is a vector of Grey codes in decimal format.
     * @return vector of binary demapped data.
     */
    std::vector<int> demapData(const std::vector<std::complex<double> >&    inputData,
                               const std::vector<std::complex<int> >&       complexValuesConstellation,
                               const std::vector<int>&                      GreyCodes)
                               {
        std::vector<int> outputData;
        // The simplest way is to use the Euclidean distance
        double distance = sqrt( pow(( complexValuesConstellation[1].real() - complexValuesConstellation[2].real() ),2 ) );
        int symbolIndex = -1;
        for (std::complex<double> i : inputData) {
            for (int j = 0; j < complexValuesConstellation.size(); j++)
                if (   i.real() >= complexValuesConstellation[j].real() - distance / 2
                    && i.real() <= complexValuesConstellation[j].real() + distance / 2
                    && i.imag() >= complexValuesConstellation[j].imag() - distance / 2
                    && i.imag() <= complexValuesConstellation[j].imag() + distance / 2) {
                    symbolIndex = j;
                    break;
                }
            outputData.push_back( GreyCodes[ symbolIndex ] );
        }
        std::vector<int> outputDataBinary = decimalToBinary( outputData, log2( getModulationOrder() ) );
        return outputDataBinary;
    }


    /**
     * QAM-Demodulate input data.
     *
     * @param inputData is a complex vector of modulated data.
     * @return vector of binary demapped data.
     */
    std::vector<int> demodulateData(const std::vector<std::complex<double> >& inputData,
                                    int modulationOrder)
    {
        std::vector<std::complex<int> >     complexValuesConstellation  = createComplexValuesConstellation( log2( modulationOrder ) );
        std::vector<int>                    GreyCodes                   = createGreyCodes( getModulationOrder() );
        std::vector<int>                    dataDemodulated             = demapData( inputData, complexValuesConstellation, GreyCodes );
        return dataDemodulated;
    }



private:


    /**
     * Convert decimal data vector to binary data vector.
     *
     * @param input is a decimal vector.
     * @param nDigits is a number bits to convert to binary.
     * @return vector of binary data.
     */
    std::vector<int> decimalToBinary(const std::vector<int>& input,
                                     int nDigits)
    {
        std::vector<int> output;
        int remain;
        int powerOf2;
        for (int i = 0; i < input.size(); i++) {
            remain = input[i];
            for (int j = nDigits - 1; j >= 0; j--) {
                powerOf2 = pow( 2,j );
                output.push_back( remain / powerOf2 );
                remain = remain % powerOf2;
            }
        }
        return output;
    }



};


#endif //GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_QAMDEMODULATOR_H
