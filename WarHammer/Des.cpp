#include "Des.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int Des::lancer(int nbDes, int seuil, const std::string& contexte) {
    int succes = 0;

    std::cout << "\n🎲 Jet de des " << (contexte.empty() ? "" : "(" + contexte + ")") << " :\n";
    for (int i = 0; i < nbDes; ++i) {
        int resultat = 1 + rand() % 6;
        std::cout << "  De " << (i + 1) << " → " << resultat;
        if (resultat >= seuil) {
            std::cout << " ✅";
            succes++;
        }
        else {
            std::cout << " ❌";
        }
        std::cout << "\n";
    }

    std::cout << "→ " << succes << " reussite" << (succes > 1 ? "s" : "") << " obtenue(s).\n";
    return succes;
}
