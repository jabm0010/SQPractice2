/**
* @file Especimen.cpp
* @brief File of the class Especimen with the code of the functions
*
*/

#include "Especimen.hpp"
#include "sec_op.hpp"

/**
 * @brief Main constructor
 *
 * Generates an "Especimen" starting the frequences with a greedy algorithm
 *
 * @param [in] 	transistors 		Transistor vector in which the population will be stored
 * @param [in]  restrictions		Restrictions of the population
 * @param [in]  indxTransRestr 		Index with the information of each transistor in the restriction group
 */

Especimen::Especimen(std::vector<Transistor>* transistors, std::vector<Restriction>* restrictions,
        std::vector<unsigned>* indxTransRestr) :
                    transistors_(transistors), restrictions_(restrictions), 
                    indxTransRestr_(indxTransRestr)
{
    randInit();
    evaluate();
}


/**
 * @brief Copy constructor
 *
 * Generates a copy of the "Especimen"
 * @param [in] 	orig	instance of the class Especimen
 */

Especimen::Especimen(const Especimen& orig) :
                    transistors_(orig.transistors_), restrictions_(orig.restrictions_),
					freqs_(orig.freqs_), indexes_(orig.indexes_), indxTransRestr_(orig.indxTransRestr_),
                    totalInterference_(orig.totalInterference_)
{
}


/**
 * @brief Calculates the total sum of the interferences of the frequencies
 * @return sum of the interferences
 */

int Especimen::evaluate()
{
    totalInterference_ = 0;
    for (unsigned i = 0; i < restrictions_->size(); ++i)
        if ( (*restrictions_)[i].bound < 
                std::abs(freqs_[ (*restrictions_)[i].trans1] - freqs_[ (*restrictions_)[i].trans2]) )
            totalInterference_ = sec_sum(totalInterference_,(*restrictions_)[i].interference);
    return totalInterference_;
}


/**
 * @brief Calculates the interference that would result as a change in a frequency
 * @param [in] 	trans	transistor in which the frequency will change
 * @param [in]	pos 	position of the frequency
 */

int Especimen::scanVal(unsigned trans, int pos)
{
    int prevision = 0;
    int rfreq = (*transistors_)[trans].getFreqRange();
    prevision = sec_diff(totalInterference_,fullCalcCost(trans, indexes_[trans], 0, transistors_->size() ));
    
    indexes_[trans] = (indexes_[trans] + pos)%rfreq;
    freqs_[trans] = (*transistors_)[trans][indexes_[trans]];
    
    prevision = sec_sum(prevision,fullCalcCost(trans, indexes_[trans], 0, transistors_->size() ));
    
    indexes_[trans] = (rfreq + (indexes_[trans] - pos)%rfreq) % rfreq;
    freqs_[trans] = (*transistors_)[trans][indexes_[trans]];
    return prevision;
}


/**
 * @brief Increases the values of the frequency of the selected transistor
 * @param [in]	trans	transistor in which the frequency will change
 * @param [in]	pos		position of the frequency
 */
void Especimen::sigVal(unsigned trans, int pos)
{
    totalInterference_ = sec_diff(totalInterference_,fullCalcCost(trans, indexes_[trans], 0, transistors_->size()-1 ));
    indexes_[trans] = (indexes_[trans] + pos)%(*transistors_)[trans].getFreqRange();
    freqs_[trans] = (*transistors_)[trans][indexes_[trans]];
    totalInterference_ = sec_sum(totalInterference_,fullCalcCost(trans, indexes_[trans], 0, transistors_->size()-1 ));
}


/**
 *  @brief Random initializer algorithm
 *
 */

void Especimen::randInit()
{
    indexes_.resize(transistors_->size());
    freqs_.resize(transistors_->size());
    for (unsigned i = 0; i < transistors_->size(); ++i)
    {
        indexes_[i] = (*transistors_)[i].getRandFrec();
        freqs_[i] = (*transistors_)[i][indexes_[i]];
    }

}


/**
 * @brief Greedy initializer algorithm
 *
 */
void Especimen::greedInit()
{
    int k = 0;
    int limit = getRandomInt(transistors_->size()/3, transistors_->size()-1);
    indexes_.resize(transistors_->size());
    freqs_.resize(transistors_->size());
    for(int i = transistors_->size()-1; i >= 0; --i )
    {
        if(k < limit)
        {
            indexes_[i] = (*transistors_)[i].getRandFrec();
            freqs_[i] = (*transistors_)[i][indexes_[i]];
        }
        else
        {
            indexes_[i] = bestFreq(i);
            freqs_[i] = (*transistors_)[i][indexes_[i]];
        }
            
        ++k;
    }
    
}


/**
 * @brief Improved version of the greedy initializer algorithm
 *
 */

void Especimen::fullGreedInit() 
{
    int start = getRandomInt(0, transistors_->size()-1);
    indexes_.resize(transistors_->size());
    freqs_.resize(transistors_->size());
    
    indexes_[start] = (*transistors_)[start].getRandFrec();
    freqs_[start] = (*transistors_)[start][indexes_[start]];
    
    int cabecera = (start + 1)%transistors_->size();
    int cola = start;
    
    while ( cabecera != cola )
    {
        indexes_[cabecera] = fullBestFreq(cabecera, cabecera, cola);
        freqs_[cabecera] = (*transistors_)[cabecera][indexes_[cabecera]];
        
        cabecera = (cabecera + 1)%transistors_->size();
    }
}


/**
 * @brief Calculates the best frequency of a transistor
 *
 * @param [in] 	trans		transistor for which the best frequency will be calculated
 * @param [in] 	cabecera	beginning of the data structure in which the frequencies are stored
 * @param [in] 	cola		end of the data structure in which the frequencies are stored
 * @return the frequency that adds the minimum value(best value) to the total sum of the interferences
 */

int Especimen::fullBestFreq(unsigned trans, unsigned cabecera, unsigned cola)
{
    int minimo=INT_MAX;
    int frecMin=-1;
    int rango = (*transistors_)[trans].getFreqRange();
    for(int i = 0; i < rango ; ++i)
    {
        int coste =fullCalcCost(trans,i, cabecera, cola);
        if(minimo > coste)
        {
            minimo = coste;
            frecMin=i;
        }
        if(!minimo)
            break;
    }
    return frecMin;
}


/**
 * @brief Calculates the cost that a transistor adds to the total
 *
 * @param [in]	trans		current transistor index
 * @param [in]	freq 		current frequency index
 * @param [in]	cabecera	beginning of the data structure in which the frequencies are stored
 * @param [in]	cola 		end of the data structure in which the frequencies are stored
 * @return the cost the transistor adds
 */
int Especimen::fullCalcCost(unsigned trans, int freq, unsigned cabecera, unsigned cola)
{
    //    Calcula el coste de uno
    int k = (*indxTransRestr_)[trans];
    int cost = 0;
    if ( trans+1 < (*indxTransRestr_).size())
    {
        if(cola < cabecera)
        {
            while (k != (int)(*indxTransRestr_)[trans+1])
            {
                if( (*restrictions_)[k].trans2 > cola &&  (*restrictions_)[k].trans2 < cabecera)
                    if( (*restrictions_)[k].bound < std::abs((*transistors_)[trans][freq] -
                        freqs_[(*restrictions_)[k].trans2]) )
                    cost = sec_sum(cost,(*restrictions_)[k].interference);

                ++k;
            }  
            
            k = (*indxTransRestr_)[trans];
            while(k >= 0 && k >= (int)cola)
            {
                if ((*restrictions_)[k].trans2 == trans)
                    if( (*restrictions_)[k].bound < std::abs((*transistors_)[trans][freq] -
                        freqs_[(*restrictions_)[k].trans1]) )
                    cost = sec_sum(cost,(*restrictions_)[k].interference);
                --k;
            }
        }
        else
        {
            while (k != (int)(*indxTransRestr_)[trans+1])
            {
                if( !((*restrictions_)[k].trans2 > cola &&  (*restrictions_)[k].trans2 < cabecera) )
                    if( (*restrictions_)[k].bound < std::abs((*transistors_)[trans][freq] -
                        freqs_[(*restrictions_)[k].trans2]) )
                    cost = sec_sum(cost,(*restrictions_)[k].interference);

                ++k;
            }
            
            k = (*indxTransRestr_)[trans];
            while(k >= 0)
            {
                if ((*restrictions_)[k].trans2 == trans)
                    if( (*restrictions_)[k].bound < std::abs((*transistors_)[trans][freq] -
                        freqs_[(*restrictions_)[k].trans1]) )
                    cost = sec_sum(cost,(*restrictions_)[k].interference);
                --k;
            }
        }
    }

    return cost;
}


/**
 * @brief Searches the best frequency taking into account the restrictions of the transistors placed
 * on his right in the data structure.
 *
 * @param [in]	trans	current transistor index
 * @return the minimum frequency found
 */
int Especimen::bestFreq(unsigned trans)
{
    int minimo=INT_MAX;
    int frecMin=-1;
    int rango = (*transistors_)[trans].getFreqRange();
    for(int i = 0; i < rango ; ++i)
    {
        int coste =calcCost(trans,i);
        if(minimo > coste)
        {
            minimo = coste;
            frecMin=i;
        }
        if(!minimo)
            break;
    }
    return frecMin;
}


/**
 * @brief Calculates the individual cost of a transistor taking into account the restrictions related to the transistors placed
 * on his right in the data structure.
 *
 * @param [in]	trans	 current transistor index
 * @param [in] 	freq	current frequency index
 * @return the cost of the transistor
 */
int Especimen::calcCost(unsigned trans, int freq)
{
    //    Calcula el coste de uno
    unsigned k = (*indxTransRestr_)[trans];
    int cost = 0;
    if ( trans+1 < (*indxTransRestr_).size())
    {
        while (k != (*indxTransRestr_)[trans+1]) 
        {
            if( (*restrictions_)[k].bound < std::abs((*transistors_)[trans][freq] -
                    freqs_[(*restrictions_)[k].trans2]) )
            	cost = sec_sum(cost,(*restrictions_)[k].interference);
            ++k;
        }
    }
    return cost;
}


/**
 * @brief Compares two instances of "Especimen. First the total sum of interference is compared, in case it is equal, the frequence
 * values of the transistors are compared.
 *
 * @param [in]	otro	 instance of the class Especimen
 * @return false if they are not equal, true if they are.
 */

bool Especimen::operator==(const Especimen& otro) {
    
    if(this->totalInterference_==otro.totalInterference_){
        for(unsigned i = 0; i< this->freqs_.size();++i)
            if(this->freqs_[i]!=otro.freqs_[i])
                return false;
    }else
        return false;
    
    return true;
}


/**
 * @brief Compares two instances of "Especimen. First the total sum of interference is compared, in case it is equal, the frequence
 * values of the transistors are compared.
 *
 * @param [in]	otro	 instance of the class Especimen
 * @return true if they are not equal, false if they are.
 */

bool Especimen::operator!=(const Especimen& otro)
{
    if(this->totalInterference_==otro.totalInterference_){
    for(unsigned i = 0; i< this->freqs_.size();++i)
        if(this->freqs_[i]!=otro.freqs_[i])
            return true;
    }else
        return true;
    
    return false;
}


/**
 * @brief Compares two instances of "Especimen. First the total sum of interference is compared, in case it is equal, the frequence
 * values of the transistors are compared.
 *
 * @param [in]	otro	instance of the class Especimen
 * @return false if otro has a smaller totalinterference value, true in the opposite case.
 *
 */

bool Especimen::operator<(const Especimen& otro) const{
    if(totalInterference_ < otro.totalInterference_)
        return true;
    
    if(totalInterference_ > otro.totalInterference_)
        return false;
    
    if (freqs_ < otro.freqs_)
        return true;
    
    return false;
}


/**
 * @brief Getter of the totalInterference attribute
 *
 * @return the total interference
 */
int Especimen::getInterference() const
{
    return totalInterference_;
}


/**
 * @brief Destroyer
 *
 * Free allocated memory when the object is destroyed
 */
Especimen::~Especimen() 
{
}


/**
 * @brief Cross in two points algorithm, exchanges the fragments of the two vectors delimited by the points
 * minimo and maximo
 *
 * @param [in,out]	padreA		first vector that will be used for the cross
 * @param [in,out]	padreB		second vector that will be used for the cross
 * @param [in]		minimo		first point of the cross
 * @param [in]		maximo		second point of the cross
 */

void cruce2Puntos(Especimen &padreA, Especimen &padreB,int minimo,int maximo){
    if(maximo<=0)
        maximo=padreA.freqs_.size()/3;
    int puntoA=getRandomInt(0,padreA.freqs_.size()-1);  	//posicion de inicio para los intercambios
    int volumen=getRandomInt(minimo,maximo);                //numero de elementos a intercambiar
    
    int fAux,iAux,pos;
    for(int i = 0; i < volumen; i++){
        pos=(puntoA+i) % padreA.freqs_.size();
        fAux=padreA.freqs_[pos];
        iAux=padreA.indexes_[pos];
        
        padreA.freqs_[pos]=padreB.freqs_[pos];
        padreA.indexes_[pos]=padreB.indexes_[pos];
        
        padreB.freqs_[pos]=fAux;
        padreB.indexes_[pos]=iAux;
    }
}


/**
 * @brief BLX cross algorithm, changes the frequency of the individuals by an intermediate value obtained
 *  by their current values and a randomness factor
 *
 * @param [in,out]	padreA		first vector that will be used for the cross
 * @param [in,out]	padreB		second vector that will be used for the cross
 * @param [in] 		alpha		percentage of randomness, 0.5 is the standard
 */

void cruceBlx(Especimen &padreA, Especimen &padreB,float alpha){
    
    for (unsigned transistor = 0; transistor < padreA.freqs_.size(); ++transistor)
    {
        int max,min;
        
        float intervalo;
    
        intervalo=abs(padreA.indexes_[transistor]-padreB.indexes_[transistor])*alpha;
        max=std::max<int>( padreA.indexes_[transistor], padreB.indexes_[transistor]);
        min=std::min<int>( padreA.indexes_[transistor], padreB.indexes_[transistor]);
        max += intervalo;
        min -= intervalo;
        max=std::min<int>(max, padreA.indexes_.size()-1);
        min=std::max<int>(min,0);
        if(intervalo > 0)
        {
           
                padreA.indexes_[transistor]=getRandomInt(min,max);
                padreB.indexes_[transistor]=getRandomInt(min,max);
                padreA.freqs_[transistor]=(*padreA.transistors_) [transistor][padreA.indexes_[transistor]];
                padreB.freqs_[transistor]=(*padreB.transistors_) [transistor][padreB.indexes_[transistor]];
        }



    }
}


/**
 * @brief Mutates the frequencies of an individual randomly
 *
 * @param	[in,out] individuo			individual that will be muted
 * @param	[in] 	 probabilidad		mutation probablility, the standard is 0.1
 */

void mutar(Especimen &individuo, double probabilidad){
    
    // Calculamos esperanza matematica
    int espMat = (int)(individuo.freqs_.size()-1)*probabilidad;
    
    // Si la esperanza matematica es 0, se opta por el metodo tradicional
    if (espMat == 0)
        for(unsigned i=0; i< individuo.freqs_.size();++i){
            if( (double)( rand() / RAND_MAX) <= probabilidad){
                individuo.indexes_[i] = getRandomInt(0,individuo.indexes_.size()-1);
                individuo.freqs_[i] = (*individuo.transistors_) [i][individuo.indexes_[i]];
            }
        }
    else {
        for (int i = 0; i < espMat; ++i) {
            int mutado = getRandomInt(0, individuo.freqs_.size()-1);
            individuo.indexes_[mutado] = getRandomInt(0,individuo.indexes_.size()-1);
            individuo.freqs_[mutado] = (*individuo.transistors_) [i][individuo.indexes_[i]];
        }
    }
}


/**
 * @brief Getter of the frequencies range
 *
 * @param [in] trans 	transistor from which its frequencies range will be obtained
 * @return	the frequency range
 */

int Especimen::getFreqRange(int trans)
{
    return (*transistors_)[trans].getFreqRange();
}


/**
 * @brief Getter of the size of the data structure in which the transistors of the individual are stored
 *
 * @return	the size of said data structure
 */

int Especimen::getSize()
{
    return transistors_->size();
}

