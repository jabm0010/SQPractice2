/**
* @file main.cpp
* @brief main file with the calls to the different functions of the program
*
*/
#include <cstdlib>

#include "Parser.hpp"
#include "Restriction.hpp"
#include "Random.hpp"
#include "Especimen.hpp"
#include "Poblacion.hpp"
#include "AlgoritmosGeneticos.hpp"
using namespace std;

/**
 * @brief Main function of the program, in which attending to the given arguments, makes the necessary calls
 * to the functions and shows in screen the results obtained.
 *
 * @param [in]	argc	arguments needed for the program
 * @param [in]	argv	arguments needed for the program
 * @return 0
 */
int main(int argc, char** argv) {

    try{
        if (argc != 7)
            throw std::domain_error("Number of arguments is not correct");

        std::string path = argv[1];     std::string argv2 = argv[2]; 
        std::string argv4 = argv[4];    std::string argv5 = argv[5];
        std::string argv6 = argv[6];
        
        std::string dom_p, var_p, ctr_p;
        if(argv2 == "-uc")
        {
            dom_p = "/DOM.TXT";
            var_p = "/VAR.TXT";
            ctr_p = "/CTR.TXT";
        } else if(argv2 == "-lc")
        {
            dom_p = "/dom.txt";
            var_p = "/var.txt";
            ctr_p = "/ctr.txt";
        } else
            throw std::invalid_argument("El argumento no es correcto, los argumentos "
                "posibles son -uc para buscar archivos en mayúscula y -lc para buscar archivos"
                    " en minúscula");


        
        int tipoCruce;
        if (argv5 == "blx")
            tipoCruce = 0;
        else if (argv5 == "2puntos")
            tipoCruce = 1;
        else
            throw std::invalid_argument("El argumento no es correcto, los argumentos "
                    "posibles son blx para el cruce blx y 2puntos para cruce en 2 puntos");

        std::string seedString = argv[3];
        int seed = std::stoi(seedString);

        std::srand(seed);
        std::vector<Restriction> restrictions;
        std::vector<std::vector<int>> freqs;
        std::vector<Transistor> transistors;

        Parser::FRangeParse(path + dom_p, freqs);
        std::vector<int> equivalenceIndex;
        Parser::TParse(path + var_p, &freqs, transistors, equivalenceIndex, 1200);
        Parser::RTParse(path + ctr_p, restrictions, equivalenceIndex);
        std::vector<unsigned> indxTransRest;
        Parser::genIndexTransRestr(transistors, restrictions, indxTransRest);
        Poblacion poblacionActual(&transistors, &restrictions, &indxTransRest);
        poblacionActual.iniciarPoblacion(20);

        if (argv6 == "-t")
            std::cout << "Interferencia Inicial\tInterferencia Final " << std::endl;
        else if (argv6 != "-o")
            throw std::invalid_argument("El argumento no es correcto, los argumentos posibles son -t "
                    "para mostrar en terminal y -o para mostrar solucion optimizada para guardar en "
                    "un fichero de texto"
                   );

        std::cout << poblacionActual.getMejor().getInterference() << "\t";
        if (argv6 == "-t")
            std::cout << "\t\t";
        if (argv4 != "greedy")
        {
            int mejor;
            if(argv4 == "generacional")
                mejor = geneticoGeneracional(50, 20000, poblacionActual, 0.7, tipoCruce, 0.1);
            else if (argv4 == "estacionario")
                mejor = geneticoEstacionario(50,20000,poblacionActual,1,1,0.1);
            else if (argv4 == "am1010")
                mejor = am1010(20,20000,poblacionActual,0.7,tipoCruce,0.1);
            else if (argv4 == "am1001")
                mejor = am1001(20,20000,poblacionActual,0.7,tipoCruce,0.1);
            else if (argv4 == "am1001Mej")
                mejor = am1001Mej(20,20000,poblacionActual,0.7,tipoCruce,0.1);
            else
                throw std::invalid_argument("El argumento no es correcto, los argumentos posibles son "
                    "generacional para seguir un modelo de evolución generacional o estacionario para "
                    "seguir un modelo de evolución estacionario. Greedy para indicar solo solución greedy inicial"
                   );

            std::cout << mejor << "\t";
        }
        if (argv6 == "-t")
            std::cout << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    
//    Especimen greedy(&transistors, &restrictions, &indxTransRest);
//    
//    std::cout << greedy.getInterference();
    
//    for(int i = 0; i < restrictions.size(); ++i)
//    {
//        std::cout << i << " >> " << restrictions[i].trans1 << " " << restrictions[i].trans2
//                << " " << restrictions[i].bound << " " << restrictions[i].interference
//                << std::endl;
//        
//    }
//      for(int i = 0; i < indxTransRest.size(); ++i)
//    {
//        std::cout << i << " >> " << indxTransRest[i]
//                << std::endl;
//        
//    }
    
    
    return 0;
}

