#pragma once
#include "Personnage.h"

class Astartes : public Personnage {
public:
    Astartes(char s);

    // --- Types spécifiques d'Astartes ---
    static Astartes creerTactique(char s);
    static Astartes creerAssaut(char s);
    static Astartes creerDevastator(char s);
};
