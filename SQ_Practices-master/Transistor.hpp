/**
 * @file Transistor.hpp
 * @brief Headers declarations for class Transistor
 */

#ifndef TRANSISTOR_HPP
#define TRANSISTOR_HPP

#include <vector>
#include <stdexcept>
#include "Random.hpp"

/**
 * @brief The class Transistor encapsulates all the information related to transmisors,
 * and the frequencies it manages.
 *
 */
class Transistor{

	unsigned numID_;                                ///< Transistor id
    int NRange_;                                    ///< Range Number id in the frequencies matrix
    std::vector<std::vector<int>>* frecs_;          ///< Reference to Frequencies matrix
    
    public: 
    Transistor(unsigned numID, int NRange, std::vector<std::vector<int>>* frecs);
    int getNRange(); 
    int getRandFrec();
    int getFreqRange();
    int getNumID();

    void setFrecs(std::vector<std::vector<int>>* frecs);

    int operator[](unsigned indx);

    virtual ~Transistor();
};

#endif
