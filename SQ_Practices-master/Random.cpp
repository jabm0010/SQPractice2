/**
 * @file Random.cpp
 * @brief Sources for random function
 */

#include "Random.hpp"

/**
 * @brief Generates an integer within a range between two numbers(int) being both included
 *
 * @param [in]	inferior  Lower bound (closed interval)
 * @param [in]	superior  Upper bound (closed interval)
 * @return Random integer within the interval
 */

int getRandomInt(int inferior, int superior){
    int aleatorio= inferior + rand()%(superior-inferior);
    return aleatorio;
};

