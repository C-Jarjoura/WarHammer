#pragma once
#include "Astartes.h"
#include "Demon.h"
#include <vector>
#include <utility>

class Grille {
private:
    int taille;
    std::vector<std::vector<char>> cases;

    void renderFrame(const std::vector<Astartes>& escouade,
        const std::vector<Demon>& demons,
        const std::vector<std::pair<int, int>>& trace,
        int idActif);

public:
    Grille(const std::vector<Astartes>& escouade,
        const std::vector<Demon>& demons);

    void majPositions(const std::vector<Astartes>& escouade,
        const std::vector<Demon>& demons);

    void afficher(const std::vector<Astartes>& escouade,
        const std::vector<Demon>& demons,
        int idActif = -1);

    void afficherPortee(const Astartes& p,
        const std::vector<Astartes>& escouade,
        const std::vector<Demon>& demons);

    bool deplacer(Astartes& p,
        const std::vector<Demon>& demons,
        std::vector<Astartes>& escouade,
        int idActif);

    // --- Animations ---
    void clignoterSelection(const Astartes& perso,
        const std::vector<Astartes>& escouade,
        const std::vector<Demon>& demons);

    void animationTir(const Astartes& tireur,
        const Personnage& cible,
        std::vector<Astartes>& escouade,
        std::vector<Demon>& demons);

    void animationCac(const Astartes& attaquant,
        const Personnage& cible,
        std::vector<Astartes>& escouade,
        std::vector<Demon>& demons);

    int getTaille() const { return taille; }
};
