/**
* @file Especimen.hpp
* @brief File of the class Especimen
*
*/

#ifndef ESPECIMEN_HPP
#define ESPECIMEN_HPP

#include "Restriction.hpp"
#include "Transistor.hpp"
#include "Random.hpp"
#include <vector>
#include <climits>

/**
 * @brief Class Especimen, representing each member of the population
 *
 */

class Especimen {
public:
    Especimen( std::vector<Transistor>* transistors, 
            std::vector<Restriction>* restrictions,
            std::vector<unsigned>* indxTransRestr );
    Especimen(const Especimen& orig);
    virtual ~Especimen();
    
    int getSize();
    int getFreqRange(int trans);
    
    int evaluate();
    
    int scanVal(unsigned trans, int pos);
    void sigVal(unsigned trans, int pos);
    
    bool operator== (const Especimen& otro);
    bool operator!= (const Especimen& otro);
    bool operator< (const Especimen& otro) const;
    
    int getInterference() const;
    
    friend void cruce2Puntos(Especimen &padreA, Especimen &padreB,int minimo = 1,int maximo = 0);
    friend void cruceBlx(Especimen &padreA, Especimen &padreB,float alpha = 0.1);
    friend void mutar(Especimen &individuo,double probabilidad = 0.1);
private:
    std::vector<Transistor>* transistors_;		///< Vector with the transistors of the individual
    std::vector<Restriction>* restrictions_;	///< Vector with the restrictions that the individual will have to consider
    
   
    std::vector<int> freqs_;					///< Vector with the frequencies of the individual
    std::vector<int> indexes_;					///< Index which represents the valid frequencies this individual will be able to take
    std::vector<unsigned>* indxTransRestr_;		///< Indexes of the restrictions that we will have to be taken into account
    
    void greedInit();
    void randInit();
    void fullGreedInit();
    
    int bestFreq(unsigned trans);
    int calcCost(unsigned trans, int freq);
    
    int fullBestFreq(unsigned trans, unsigned cabecera, unsigned cola);
    int fullCalcCost(unsigned trans, int freq, unsigned cabecera, unsigned cola);
    
    int totalInterference_;						///< The sum of the interference obtained for this member of the population
};

#endif /* ESPECIMEN_HPP */

