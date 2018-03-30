/**
 * @file Transistor.cpp
 * @brief Sources for the class Transistor
 *
 */

#include "Transistor.hpp"

/**
 * @brief Transistor default constructor
 *
 * @param [in]	numID		Transistor unique ID
 * @param [in]	NRange		Range of frequencies in the Frequencies matrix
 * @param [in]	frecs		Reference to Frequencies matrix
 */

Transistor::Transistor(unsigned numID, int NRange, std::vector<std::vector<int>>* frecs)
{
    this->numID_ = numID;
    this->NRange_ = NRange; 
    this->frecs_ = frecs;
}


/**
 * @brief Gets the range of transistor's frequencies in the frequencies matrix
 *
 * @return Range number
 */

int Transistor::getNRange()
{
    return NRange_;
}


/**
 * @brief Gets a random frequency inside his set of frequencies
 *
 * @return Random frequencies within his range of frequencies inside the frequencies matrix.
 */
int Transistor::getRandFrec()
{   
    int frecuencia=getRandomInt(0,getFreqRange()-1);
    return frecuencia;
}


/**
 * @brief Set the frequencies matrix
 *
 * @param frecs Reference to frequencies matrix
 */

void Transistor::setFrecs(std::vector<std::vector<int>>* frecs)
{
    this->frecs_ = frecs;
}


/**
 * @brief Access the frequence of a transistor
 *
 * @param [in]	indx	Position of the frequency
 * @return Frequency of the transistor that is in the given position
 */

int Transistor::operator[](unsigned indx)
{
    return (*frecs_)[NRange_][indx%getFreqRange()];
}


/**
 * @brief Gets the ID number of the transistor
 *
 * @return Transistor ID
 */

int Transistor::getNumID()
{
    return numID_;
}


/**
 * @brief Gets the number of different frequencies a transistor has
 *
 * @return The range of frequencies available for the transistor
 */

int Transistor::getFreqRange()
{
    return (*frecs_)[NRange_].size();
}


/**
 * @brief Destroyer
 *
 * Free allocated memory when the object is destroyed
 */

Transistor::~Transistor(){ }
