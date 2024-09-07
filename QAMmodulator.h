// This class do Quadrature Amplitude Modulation (QAM) with variable
// modulation order. For example, this class can modulate signal by
// 16-QAM modulation.
//
// Created by Konstantin Terentev on 05.09.2024 for YADRO.

#include <iostream>
#include <complex>
#include <vector>

#ifndef GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_QAMMODULATOR_H
#define GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_QAMMODULATOR_H


class qamModulator {
public:


    /**
     * Get inputted modulation order and check
     * if it is correct.
     *
     * @param modulationOrderInput must be a power of two and
     * also must be square number, because we work
     * with rectangular constellations.
     */
    void setModulationOrder(int& modulationOrderInput)
    {
        if (modulationOrderInput < 2) {
            modulationOrder_ = 4;
            std::cerr
                    << "Modulation order cannot be lower than 2. The value of the modulation order is set to 4. \n"
                       "Порядок модуляции не может быть меньше 2. Установлено значение порядка модуляции равное 4.\n"
                    << std::endl;
        }
        if (modulationOrderInput % 2 == 1) {
            modulationOrder_ = modulationOrderInput / 2 * 2;
            std::cerr << "Modulation order cannot be odd. The value of the modulation order is set to "
                      << getModulationOrder()
                      << ". \nПорядок модуляции не может быть нечетным. Установлено значение порядка модуляции равное "
                      << getModulationOrder() << ".\n" << std::endl;
        }
        else if (fmod(sqrt(modulationOrderInput), 1) > 0) {
            modulationOrder_ = 16;
            std::cerr
                    << "Modulation orders different from 4, 16 or 64 are unsupported now. The value of the modulation order is set to 16. \n"
                       "Значения порядков модуляции, отличные от 4, 16 или 64 сейчас не поддерживается. Установлено значение порядка модуляции равное 16.\n"
                    << std::endl;
        }
        else {
            modulationOrder_ = modulationOrderInput;
        }
    }


    // Returns modulation order.
    int getModulationOrder()
    {
        return modulationOrder_;
    }


    /**
     * Calculates complex values in a signal constellation of a given size.
     *
     * @param bitsPerSymbol equals loq2 of modulation order.
     * @return vector of symbols that make up constellation.
     */
    std::vector<std::complex<int> > createComplexValuesConstellation(int bitsPerSymbol)
    {
        // Number of values in each axis.
        int nReImValues = pow(2, bitsPerSymbol / 2);
        // Fill axis values. Because of rectangular constellation
        // real and imaginary parts are equal.
        std::vector<int> ReImValues;
        for (int i = nReImValues - 1; i >= -nReImValues; i -= 2)
            ReImValues.push_back(i);
        // Makes vector of complex numbers.
        std::vector<std::complex<int> > complexValuesConstellation;
        for (int j: ReImValues)
            for (int i: ReImValues)
                complexValuesConstellation.push_back(std::complex<int>(i, j));
        return complexValuesConstellation;
    }


    /**
     * Calculates Grey codes of a given size via bitwise operations.
     *
     * @param nCodes is number of codes to generate from
     * decimal zero to decimal nCodes.
     * @return vector of codes in int type.
     */
    std::vector<int> createGreyCodes(int nCodes)
    {
        std::vector<int> GreyCodes;
        for (int i = 0; i <= nCodes-1; i++)
            GreyCodes.push_back( i ^ (i >> 1) );
        return GreyCodes;
    }


    /**
     * Map input data to QAM constellation aka do mapping.
     *
     * @param inputData is a vector of binary data.
     * @param complexValuesConstellation is a vector of complex symbols.
     * @param GreyCodes is a vector of Grey codes in decimal format.
     * @return complex vector of mapped data.
     */
    std::vector<std::complex<int> > mapData(const std::vector<int>& inputData, int bitsPerSymbol,
                                            const std::vector<std::complex<int> >& complexValuesConstellation,
                                            const std::vector<int>& GreyCodes)
                                            {
        std::vector<std::complex<int> > outputData;
        std::vector<int> inputDataDecimal = binaryToDecimal( inputData, bitsPerSymbol );
        for (int i : inputDataDecimal)
            outputData.push_back( complexValuesConstellation[ findIndex( GreyCodes, i ) ] );
        return outputData;
    }


    /**
     * QAM-Modulate input data aka do impulse modulation.
     *
     * @param inputData is a vector of binary data.
     * @return complex vector of modulated data.
     */
    std::vector<std::complex<int> > modulateData(const std::vector<int> inputData)
    {
        // Call method to create constellation of a given size.
        std::vector<std::complex<int> > complexValuesConstellation = createComplexValuesConstellation(log2(getModulationOrder()));
        // Call method to create Grey codes of a given number.
        std::vector<int> GreyCodes = createGreyCodes(getModulationOrder());
        // Map data.
        std::vector<std::complex<int> > dataModulated = mapData(inputData, log2(getModulationOrder()), complexValuesConstellation, GreyCodes);
        return dataModulated;
    }



private:


    /**
     * Finds index of an element in vector.
     *
     * @param vectorToExplore is a vector to explore, obviously.
     * @param elementToFind is an element to find, obviously...
     * @return index of an element in vector.
     */
    int findIndex(std::vector<int> vectorToExplore,
                  int elementToFind)
    {
        auto position = find( vectorToExplore.begin(), vectorToExplore.end(), elementToFind );
        int index;
        if ( position != vectorToExplore.end() )
            index = position - vectorToExplore.begin();
        else
            std::cerr << "The value was not found \nЗначение не найдено \n";
        return index;
    }


    /**
     * Convert binary data vector to decimal data vector.
     *
     * @param input is a binary vector.
     * @param nDigits is a number bits to convert to decimal.
     * @return vector of decimal data.
     */
    std::vector<int> binaryToDecimal(const std::vector<int>& input,
                                     int& nDigits)
    {
        std::vector<int> output;
        int power         =  nDigits-1;
        int decimalValue  =  0;
        for (int j = 0; j < input.size(); j++) {
            decimalValue += input[j] * pow( 2, power );
            power -= 1;
            if ((j+1) % nDigits == 0) {
                output.push_back( decimalValue );
                decimalValue = 0;
                power = nDigits - 1;
            }
        }
        return output;
    }



    int modulationOrder_;



};


#endif //GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_QAMMODULATOR_H
