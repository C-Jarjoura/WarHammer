#pragma once
#include "Personnage.h"

class Demon : public Personnage {
public:
    Demon(char s);

    // --- Types spécifiques de démons ---
    static Demon creerMineur(char s);
    static Demon creerMajeur(char s);
    static Demon creerDestructeur(char s);
};
