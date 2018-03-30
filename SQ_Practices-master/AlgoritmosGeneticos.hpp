/**
* @file AlgoritmosGeneticos.hpp
* @brief File with the declaration of some functions used by the class AlgoritmosGeneticos
*
*/

#ifndef ALGORITMOSGENETICOS_HPP
#define ALGORITMOSGENETICOS_HPP

#include "Poblacion.hpp"



int geneticoGeneracional(int nIndividuos, int evaluaciones,Poblacion& entorno, double cruce, int tipo,double mutacion);


int geneticoEstacionario(int nIndividuos, int evaluaciones,Poblacion& entorno, int parejas, int tipo, double mutacion);

#endif /* ALGORITMOSGENETICOS_HPP */

