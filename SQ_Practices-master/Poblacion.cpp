/**
 * @file   Poblacion.cpp
 * @brief  File with the source code of Poblacion
 * @author 	Alberto Carrillo Ortega
 * @author  Javier Martínez Cavanillas
 *
 * Created on 17 November 2017, 17:56
 */

#include "Poblacion.hpp"

/**
 * @brief Default Constructor
 *
 * Generates a new population
 * @param [in]	transistors	 		vector with the transistors that conform the population
 * @param [in]	restrictions	 	vector with the restrictions that rules the ppopulation
 * @param [in] 	indxTransRestr   	Indice que indica la posición de cada transistor en el conjunto de restricciones
 */

Poblacion::Poblacion(std::vector<Transistor>* transistors, 
        std::vector<Restriction>* restrictions, 
        std::vector<unsigned>* indxTransRestr)    : transistors_(transistors),
        restrictions_(restrictions), indxTransRestr_(indxTransRestr)
{
    mejor_ = 0;
    mundo_ = new std::vector<Especimen>;
}


/**
 * @brief Copy Constructor
 *
 * Generates a new copy of a given population
 * @param [in]	orig	Original population to copy
 */

Poblacion::Poblacion(const Poblacion& orig)     :
		transistors_(orig.transistors_), restrictions_(orig.restrictions_),
		indxTransRestr_(orig.indxTransRestr_),  mejor_(orig.mejor_)
{
    mundo_ = new std::vector<Especimen>;
    *mundo_ = *orig.mundo_;
}


/**
 * @brief Destroyer
 *
 * Free allocated memory when the object is destroyed
 */

Poblacion::~Poblacion() 
{
    delete mundo_;
}


/**
 * @brief Generates and establish the individuals of a population by a greedy algorithm
 *
 * @param [in]	nIndividuos		number of individuals at the population
 */

void Poblacion::iniciarPoblacion(int nIndividuos)
{
//    int vMejor = INT_MAX;
    for(int i = 0; i < nIndividuos; ++i)
    {
        Especimen nuevoEsp( transistors_ , restrictions_, indxTransRestr_);
        mundo_->push_back(nuevoEsp);
    }
    actualizarMejor();
}


/**
 * @brief Evolves population using a generational model
 *
 * @param [in]	probabilidad  Cross probability
 * @param [in]	tipoCruce     Cross type to use, 0 = BLX 1 = 2 Points
 * @param [in]	pMutacion     Mutation probability
 */

void Poblacion::evolucionGeneracional(double probabilidad, int tipoCruce, double pMutacion)
{
    // Inicializamos hijos y lista de candidatos
    std::vector<Especimen>* hijos = new std::vector<Especimen>;
    *hijos = *mundo_;
    
    std::vector<int> candidatos;
    
    // Calculamos esperanza Matematica
    int numCandidatos = (int)(mundo_->size()*probabilidad);
    int mutados = (int)(mundo_->size()*pMutacion);
    
    if(!numCandidatos)
        return; 
    
    if(numCandidatos%2 != 0) ++numCandidatos;
    
    // Seleccionamos por Torneo Binario
    candidatos.reserve(numCandidatos);
    for(int i = 0; i < numCandidatos; ++i)
    {
        int candidato1 = getRandomInt(0, mundo_->size()-1);
        int candidato2 = getRandomInt(0, mundo_->size()-1);
        
        while (candidato1 == candidato2)
            candidato2 = getRandomInt(0, mundo_->size()-1);
        
        if( (*mundo_)[candidato1].getInterference() < (*mundo_)[candidato2].getInterference() )
            candidatos.push_back( candidato1 );
        else
            candidatos.push_back( candidato2 );
    }
    
    // Cruzamos los candidatos
    for (int i = 0; i < numCandidatos; i +=2 )
        if(tipoCruce == 0)
            cruceBlx( (*hijos) [ candidatos[i] ] , (*hijos) [ candidatos[i+1] ] );
        else if (tipoCruce == 1)
            cruce2Puntos( (*hijos) [ candidatos[i] ] , (*hijos) [ candidatos[i+1] ] );
    
    // Evaluamos los que Hayamos cruzado
    for(int i = 0; i < numCandidatos; ++i)
    {
        // Comprobamos que no lo hayamos evaluado antes ya que puede haberse cruzado 2 veces
        bool evaluado = false;
        for (int j = i-1; j >= 0; --j)
            if(candidatos[i] == candidatos[j])
            {
                evaluado = true;
                break;
            }
        
        if(!evaluado)
            (*hijos)[ candidatos[i] ].evaluate();
        
    }
    
    // Mutacion: Mutamos y evaluamos los mutados
    for (int i = 0; i < mutados; ++i)
    {
        int mutado = getRandomInt(0, hijos->size());
        mutar( (*hijos)[mutado] );
        (*hijos)[mutado].evaluate();
    }
    
    // Elitismo: Si el mejor ha sido reemplazado la sustituimos por el peor de los hijos
    if( (*mundo_)[mejor_] != (*hijos)[mejor_] ) 
    {
        int peor = 0;
        int vPeor = (*hijos)[0].getInterference();
        for(unsigned i = 1; i < hijos->size(); ++i)
            if( vPeor < (*hijos)[i].getInterference() )
            {
                vPeor = (*hijos)[i].getInterference();
                peor = i;
            }

        (*hijos)[peor] = (*mundo_)[mejor_];
    }
    
    // Reemplazamos
    delete mundo_;
    mundo_ = hijos;
    
    // Calculamos y actualizamos el mejor
    int mejor = 0;
    int vMejor = (*mundo_)[0].getInterference();
    for(unsigned i = 1; i < mundo_->size(); ++i)
        if( vMejor > (*mundo_)[i].getInterference() )
        {
            vMejor = (*mundo_)[i].getInterference();
            mejor = i;
        }
    
    mejor_ = mejor;
//    (*mundo_)[mejor].evaluate();
}


/**
 * @brief Evolves population using a steady one model
 *
 * @param [in]	tipoCruce Cross type to use, 0 = BLX 1 = 2 Points
 * @param [in]	pMutacion Mutation probability
 * @param [in]	parejas  	Numbers of pairs to cross
 */

void Poblacion::evolucionEstacionaria(int tipoCruce, double pMutacion, int parejas) 
{
    // Inicializamos hijos
    std::vector<Especimen> hijos;
    
    // Seleccionamos hijos al azar
    for(int i = 0; i < parejas*2; ++i)
    {
        int candidato = getRandomInt(0, mundo_->size()-1);
        hijos.push_back( (*mundo_)[candidato] );
    }
    
    // Cruzamos los candidatos
    for (unsigned i = 0; i < hijos.size(); i +=2 )
        if(tipoCruce == 0)
            cruceBlx( hijos [i] , hijos [i+1] );
        else if (tipoCruce == 1)
            cruce2Puntos( hijos [i] , hijos [i+1] );
    
    // Mutamos los hijos
    for(unsigned i = 0; i < hijos.size(); ++i)
        if( (double)( rand() / RAND_MAX ) <= pMutacion) 
            mutar(hijos[i]);

    
    // Evaluamos
    for (unsigned i = 0; i < hijos.size(); ++i)
        hijos[i].evaluate();
    
    // Buscamos los N peores y reemplazamos
    std::set<int> vistos;
    for(unsigned i = 0; i < hijos.size(); ++i)
    { 
        int peor = 0;
        int vPeor = -1;
        for(unsigned j = 0; j < mundo_->size(); ++j)
            if( vPeor < hijos[j].getInterference() && vistos.find(j) == vistos.end())
            {
                vPeor = hijos[j].getInterference();
                peor = j;
            }
        
        vistos.insert(peor);
        
        // Comprobamos si es el mejor hasta el momento y si lo es lo colocamos en
        // el sitio que este piensa ocupar. El del que antes era el peor
        if ( (*mundo_)[mejor_].getInterference() > hijos[i].getInterference())
            mejor_ = peor; 
        
        if((*mundo_)[peor].getInterference() > hijos[i].getInterference())
            (*mundo_)[peor] = hijos[i];
    }
    
}


/**
 * @brief Check if the population has at least a 20% of different individuals
 *
 * @return boolean
 */

bool Poblacion::comprobarRepetidos(){
    std::map<Especimen,int> arbol;
    for(Especimen caso:*mundo_){
        std::pair<Especimen,int> nodo(caso,0);
        auto iterador=arbol.insert(nodo);
        ++(iterador.first->second);
        if(iterador.first->second >= mundo_->size()*0.8)           
            return true;
    }
    return false;
}


/**
 * @brief Reinit population preserving the best previous individual
 *
 */

void Poblacion::reinicializar(){
    Especimen ejemplar=(*mundo_)[mejor_];
    
    int nIndividuos=mundo_->size();
    mundo_->clear();
    
    mejor_=0;
    (*mundo_).push_back(ejemplar);
    
    int vMejor=ejemplar.getInterference();
    for(int i = 1; i < nIndividuos; ++i)
    {
        Especimen nuevoEsp( transistors_ , restrictions_, indxTransRestr_);
        mundo_->push_back(nuevoEsp);
        if(vMejor > nuevoEsp.getInterference())
        {   
            mejor_ = i;
            vMejor = nuevoEsp.getInterference();
        }
    }
}


/**
 * @brief Gets the best individual of the population
 *
 * @return Especimen
 */

Especimen Poblacion::getMejor()
{
    return (*mundo_)[mejor_];
}


/**
 * @brief Updates the best candidate of the population
 *
 */

void Poblacion::actualizarMejor()
{
    // Calculamos y actualizamos el mejor
    int mejor = 0;
    int vMejor = (*mundo_)[0].getInterference();
    for(unsigned i = 1; i < mundo_->size(); ++i)
        if( vMejor > (*mundo_)[i].getInterference() )
        {
            vMejor = (*mundo_)[i].getInterference();
            mejor = i;
        }
    
    mejor_ = mejor;
}

