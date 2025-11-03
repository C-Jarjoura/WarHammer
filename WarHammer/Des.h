#pragma once
#include <string>

class Des {
public:
    // Lance un certain nombre de dés (nbDes), chaque dé réussit sur un résultat >= seuil.
    // Retourne le nombre de réussites obtenues.
    static int lancer(int nbDes, int seuil, const std::string& contexte = "");
};
