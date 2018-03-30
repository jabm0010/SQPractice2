/**
* @file Parser.hpp
* @brief file with the declarations of the functions meant to read the .txt files for class Parser
*
*/
#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector> 
#include <fstream>
#include <sstream>
#include <climits>

#include "Transistor.hpp"
#include "Restriction.hpp"

/**
 * @brief Class Parser, with the objective of translating the information stored in the different .txt files regarding
 * the problem to the different data structures used in the program.
 */
class Parser {
    public:
    static void FRangeParse(std::string path, std::vector<std::vector<int>>& results);

    static void RTParse(std::string path, std::vector<Restriction>& restrictions,
                        std::vector<int>& equivalenceIndex); 

    static int TParse(std::string path,
                      std::vector<std::vector<int>>* frecs, 
                      std::vector<Transistor>& result, std::vector<int>& equivalenceIndex,
                      int TNumber = INT_MAX);
    
    static void genIndexTransRestr(std::vector<Transistor>& transistors, std::vector<Restriction>& restrictions,
            std::vector<unsigned>& result);
};


#endif
