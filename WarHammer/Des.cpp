#include "Des.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"

int Des::lancer(int nbDes, int seuil, const std::string& contexte) {
    int succes = 0;

    std::cout << CYAN << "\n Dice roll "
        << (contexte.empty() ? "" : "(" + contexte + ")")
        << " :\n" << RESET;

    for (int i = 0; i < nbDes; ++i) {
        int resultat = 1 + rand() % 6;

        std::cout << YELLOW << "  Dice " << (i + 1) << " -> " << resultat << RESET << " ";

        if (resultat >= seuil) {
            std::cout << GREEN << "HIT" << RESET;
            succes++;
        }
        else {
            std::cout << RED << "MISS" << RESET;
        }
        std::cout << "\n";
    }

    std::cout << CYAN << "-> " << succes << " success"
        << (succes > 1 ? "es" : "") << " total" << RESET << "\n";

    return succes;
}
