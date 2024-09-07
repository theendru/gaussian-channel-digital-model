// This class appends to signal Additive White Gaussian Noise (AWGN)
// with variable Signal-To-Noise Ratio (SNR) (or Eb/N0 - parameter
// widely used in telecommunications).
//
// Created by Konstantin Terentev on 05.09.2024 for YADRO.

#include <random>

#ifndef GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_GAUSSIANCHANNEL_H
#define GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_GAUSSIANCHANNEL_H


class GaussianChannel : public qamModulator {
public:


    /**
     * Add white Gaussian noise.
     *
     * @param inputSignal is a complex vector of double modulated data.
     * @param SNR is a signal-to-noise ratio value (or the ratio of bit energy to noise power).
     * @return complex vector of modulated noised data.
     */
    std::vector<std::complex<double> > addGaussianNoise(const std::vector<std::complex<double> >& inputSignal,
                                                        double SNR)
    {
        std::vector<std::complex<double> >  outputSignal;
        std::vector<std::complex<double> >  AWGN;
        std::random_device                  generateNumber;
        std::vector<std::complex<int> >     complexValuesConstellation = qamModulator::createComplexValuesConstellation(log2(getModulationOrder()));
//        // Calculating constellation energy
//        double E = 0;
//        for (std::complex<int> i : complexValuesConstellation) {
//            E += pow(abs(i), 2);
//        }
//        // Calculating symbol energy
//        double Es =  E/getModulationOrder();
//        // Calculating bit energy
//        double Eb = Es/log2(getModulationOrder());
        // Calculating average bit energy via special formula
        double Eb = (getModulationOrder()-1)/(3*log2(getModulationOrder()));
        // Calculating noise density
        double No = Eb/pow(10,(SNR)/10);
        // Adding noise
        std::normal_distribution<double> N(0, sqrt(No/2));
        for (std::complex<double> i : inputSignal) {
            outputSignal.push_back(i + std::complex<double>(N(generateNumber), N(generateNumber)));
        }
        return outputSignal;
    }


    /**
     * Add white Gaussian noise.
     *
     * @param inputSignal is a complex vector of integer modulated data.
     * @param SNR is a signal-to-noise ratio value (or the ratio of bit energy to noise power).
     * @return complex vector of modulated noised data.
     */
    std::vector<std::complex<double> > addGaussianNoise(const std::vector<std::complex<int> >& inputSignal,
                                                        double SNR)
    {
        std::vector<std::complex<double> > inputSignalDouble = cast<double, int>(inputSignal);
        std::vector<std::complex<double> > outputSignal = addGaussianNoise(inputSignalDouble, SNR);
        return outputSignal;
    }



private:


    // Changes type of elements in vector.
    template <typename O, typename I> std::vector<std::complex<O> > cast(const std::vector<std::complex<I> > inputData)
    {
        std::vector<std::complex<O> > outputData;
        for (int i = 0; i < inputData.size(); i++)
            outputData.push_back( std::complex<O>( inputData[i].real(), inputData[i].imag() ) );
        return outputData;
    }



};


#endif //GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_GAUSSIANCHANNEL_H
