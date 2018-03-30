/**
 * @file   Poblacion.hpp
 * @brief  File with the headers and definition of attributes of the class Poblacion
 * @author Alberto Carrillo Ortega
 * @author Javier Martínez Cavanillas
 *
 * Created on 17 November 2017, 17:56
 */

#ifndef POBLACION_HPP
#define POBLACION_HPP

#include "Especimen.hpp"
#include "Random.hpp"
#include <vector>
#include <list>
#include <map>
#include <set>

/**
 * @brief Class Poblacion, managing a set of individuals
 *
 */

class Poblacion {
public:
    Poblacion(
            std::vector<Transistor>* transistors, 
            std::vector<Restriction>* restrictions, 
            std::vector<unsigned>* indxTransRestr
            );
    
    Poblacion(const Poblacion& orig);
    
    void iniciarPoblacion(int nIndividuos);
    void evolucionEstacionaria(int tipoCruce, double pMutacion = 0.1, int parejas = 1);
    void evolucionGeneracional(double probabilidad, int tipoCruce, double pMutacion = 0.1);
    
    friend int am1010(int nIndividuos, int evaluaciones,Poblacion& entorno, double cruce, int tipo, double mutacion);

    friend int am1001(int nIndividuos, int evaluaciones,Poblacion& entorno, double cruce, int tipo, double mutacion);

    friend int am1001Mej(int nIndividuos, int evaluaciones,Poblacion& entorno, double cruce, int tipo, double mutacion);
    
    bool comprobarRepetidos();
    
    void actualizarMejor();
    void reinicializar();
    
    Especimen getMejor();
    
    virtual ~Poblacion();
    
    
private:
    
    std::vector<Transistor>* 	transistors_;           ///< Vector with the transistors of the individual
    std::vector<Restriction>* 	restrictions_;          ///< Vector with the restrictions that the individual will have to consider
    std::vector<unsigned>* 		indxTransRestr_;        ///< Indexes of the restrictions that we will have to be taken into account

    std::vector<Especimen>* 	mundo_;					///< Vector with the individuals of the population

    int mejor_;											///< Index to keep track of the best individual
    
    
    
};

#endif /* POBLACION_HPP */

