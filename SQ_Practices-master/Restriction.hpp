/**
 * @file Restriction.hpp
 * @author Javier
 * @brief header files for Restriction struct declaration
 *
 * Created on 10 November 2017, 10:20
 */

#ifndef RESTRICTION_HPP
#define RESTRICTION_HPP

/**
 * @brief Data structure to store the information related to a Restriction
 *
 */

struct Restriction
{
    unsigned trans1;	///< First transistor
    unsigned trans2;	///< Second transistor
    int bound;			///< Bound of frequency difference when a interference starts to appear
    int interference;	///< Interference produced
};


#endif /* RESTRICTION_HPP */

