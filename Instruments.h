// This class is helpful... Believe me.
//
// Created by Konstantin Terentev on 05.09.2024 for YADRO.

#include <iostream>
#include <fstream>

#ifndef GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_INSTRUMENTS_H
#define GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_INSTRUMENTS_H


class Instruments {
public:



    /**
     * Read data from file contains only one line.
     *
     * @param pathToFile is a path to the file in text (string) format.
     * @param nDigits is a number bits to convert to decimal.
     * @return data contains in file.
     */
    std::string readFile(std::string& pathToFile) {
        std::string line;
        std::ifstream in( pathToFile );
        if (in.is_open())
            std::getline( in, line );
        else
            std::cerr << "Error while opening file to read" << std::endl;
        return line;
    }



    /**
     * Write (overwrite) data to file.
     *
     * @param pathToFile is a path to the file in text (string) format.
     * @param data is a Numerical vector which has to be written in file.
     * Every new value writes in new line.
     */
    void writeFile(std::string pathToFile, std::vector<double>& data) {
        std::ofstream out( pathToFile );
        if (out.is_open())
            for (double i : data)
                out << i << std::endl;
        else
            std::cerr << "Error while opening file to write" << std::endl;
    }



    /**
     * Convert string data to binary.
     *
     * @param input is a text information.
     * @return binary data in int type. From MSB to LSB.
     */
    std::vector<int> stringToBinary(const std::string& input) {
        std::vector<int> output;
        for (char c : input) {
            for (int i = 7; i >= 0; i--) {
                bool bit = (c >> i) & 1;
                output.push_back(bit ? 1 : 0);
            }
        }
        return output;
    }



    /**
     * Convert binary data to string. Written to output data in terminal.
     *
     * @param input is a vector of binary data in int type. From MSB to LSB.
     * @return text data.
     */
    std::string binaryToString(const std::vector<int>& input) {
        std::string output;
        int decimalValue  =  0;
        int power         =  7;
        for (int i = 0; i < input.size(); i++) {
            decimalValue += input[i] * pow(2, power);
            power--;
            if (i % 8 == 7) {
                output += static_cast<char>( decimalValue );
                decimalValue  =  0;
                power         =  7;
            }
        }
        return output;
    }



    /**
     * Calculate Bit Error Rate (BER). If input is vector of
     * decimal numbers it is calculating Symbol Error Rate (SER).
     *
     * @param input1 first vector of data in int type to be
     * compared.
     * @param input2 second vector of data in int type to be
     * compared.
     * @return value of BER.
     */
    double computeBER(const std::vector<int>& input1, const std::vector<int>& input2) {
        double BER = 0;
        for (int i = 0; i < input1.size(); i++)
            (input1[i] == input2[i]) ? (BER = BER) : (BER++);
        return BER / (double)input1.size();
    }



    /**
     * Get input but with default output which can be
     * chosen to input. Written to work from terminal.
     *
     * @param textBefore is a text to show before input.
     * @param defaultValue is a default (or example) input.
     * @return inputted text.
     */
    std::string getInputWithDefault(const std::string& textBefore, const std::string& defaultValue) {
        std::string input;
        std::cout << textBefore << std::endl << defaultValue << std::endl;
        std::getline(std::cin, input);
        if (input.empty())
            return defaultValue;
        return input;
    }



};


#endif //GAUSSIANCHANNELDIGITALMODELCONSOLEAPP_INSTRUMENTS_H
