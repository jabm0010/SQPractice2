/**
* @file parser.cpp
* @brief file with the code of the functions used to read .txt files  for class Parser
*
*/

#include "Parser.hpp"

/**
 * @brief Generates a matrix with the different levels of frequencies given by "dom.txt"
 *
 * @param [in]		path		path in which the file is stored
 * @param [in,out]	results	data structure in which the results will be stored
 */

void Parser::FRangeParse(std::string path,
                         std::vector<std::vector<int>>& results)
{
    std::ifstream input_file;
    input_file.open(path);
    if(!input_file.good())
        throw std::domain_error("File not found");

    std::string currentLine; 
    int indx_line = 0;
    while(std::getline(input_file, currentLine))
    {
        if(currentLine == " ")
            continue;
        std::string index, frec;
        std::stringstream iss(currentLine);
        iss >> index;
        
        results.push_back(*(new std::vector<int>()));
        do {
            iss >> frec;
            int frec_n = std::stoi(frec);
            results[indx_line].push_back(frec_n);

        }  while(iss);
        ++indx_line;
    }
}


/**
 * @brief Generates a vector with the transistors given by "var.txt"
 *
 * @param [in]		path				path in which the file is stored
 * @param [in]		frecs				data structure in which the frequencies range of each transistor will be stored
 * @param [in,out]	result				data structure in which the results will be stored
 * @param [in,out]	equivalenceIndex	refactoring index to avoid gaps between transistors
 * @param [in,out]	TNumber				the total number of transistors of the file
 * @return The total number of transistors generated
 */

int Parser::TParse(std::string path,
        std::vector<std::vector<int>>* frecs,
        std::vector<Transistor>& result, std::vector<int>& equivalenceIndex,
        int TNumber)
{
    std::ifstream input_file;
    input_file.open(path);
    if(!input_file.good())
        throw std::domain_error("File not found");

    std::string currentLine;
    int indx_trans = 0;
    while(std::getline(input_file, currentLine) && indx_trans < TNumber)
    {
        int  trans, nRange;
        std::stringstream iss(currentLine);

        if(currentLine == " ")
            continue;

        iss >> trans;
        iss >> nRange;

        indx_trans = trans;

        Transistor aux(trans, nRange, frecs);
        result.push_back(aux);

    }
    
    equivalenceIndex.reserve(indx_trans+1);
    for (unsigned i = 0; i < result.size(); ++i)
        equivalenceIndex[result[i].getNumID()] = i;
    
    if (indx_trans < TNumber)
        return indx_trans;
    return TNumber;
}


/**
 * @brief Generates a list of transistors given by the file "ctr.txt"
 *
 * @param [in]		path				path in which the file is stored
 * @param [in,out]	restrictions		data structure in which the restrictions will be stored
 * @param [in,out]	equivalenceIndex	refactoring index to avoid gaps between transistors
 * @return The number of transistors returned.
 */

void Parser::RTParse(std::string path, std::vector<Restriction>& restrictions,
        std::vector<int>& equivalenceIndex)
{
    std::ifstream input_file;
    input_file.open(path);
    if(!input_file.good())
        throw std::domain_error("File not found");
    
    std::string currentLine;
    while(std::getline(input_file, currentLine))
    {
        int  trans1, trans2, bound, interference;
        std::string letter, mock;
        std::stringstream iss(currentLine), mockstream(currentLine);
        mockstream >> mock;
        mockstream >> mock;
        if(currentLine == " " || !mockstream)
            continue;

        iss >> trans1;
        iss >> trans2;
        
        iss >> letter;
        
        if(letter == "D")
            continue;
        
        iss >> letter;
        
        iss >> bound;
        iss >> interference;
        
        Restriction aux; aux.trans1 = equivalenceIndex[trans1]; aux.trans2 = equivalenceIndex[trans2];
        aux.bound = bound; aux.interference = interference;

        restrictions.push_back(aux);
    }
}


/**
 * @brief Calculates indexes of the restrictions that we will have to be taken into account
 *
 * @param [in,out]	transistors_	 	vector with the transistors
 * @param [in,out]	restrictions_	 	vector with the restrictions
 * @param [in,out]	result		 		Indexes of the restrictions that we will have to be taken into account
 */

void Parser::genIndexTransRestr(std::vector<Transistor>& transistors_, 
                std::vector<Restriction>& restrictions_, std::vector<unsigned>& result)
{
    unsigned k = 0;
    for(unsigned i = 0; i < transistors_.size() && k <= restrictions_.size(); ++i)
    {
        result.push_back(k);
        while (restrictions_[k].trans1 == i)
            ++k;
        
    }
}

