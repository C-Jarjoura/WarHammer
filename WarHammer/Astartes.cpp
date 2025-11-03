#include "Astartes.h"
#include "Armes.h"

Astartes::Astartes(char s)
    : Personnage(4, 3, 3, 5, 15, 6, s) {
    ajouterArme(Arme("Bolter", 4, 3, 5, 3, 1, "Standard"));
}

// --- ASTARTES TACTIQUE ---
Astartes Astartes::creerTactique(char s) {
    Astartes a(s);
    a.atk = 4;
    a.hit = 3;
    a.def = 3;
    a.dgt = 5;
    a.pv = 15;
    a.mvt = 6;
    a.armes.clear();
    a.ajouterArme(Arme("Bolter", 4, 3, 5, 3, 1, "Standard"));
    a.ajouterArme(Arme("Chainsword", 5, 3, 6, 1, 1, "Melee"));
    return a;
}

// --- ASTARTES D'ASSAUT ---
Astartes Astartes::creerAssaut(char s) {
    Astartes a(s);
    a.atk = 5;
    a.hit = 3;
    a.def = 3;
    a.dgt = 6;
    a.pv = 14;
    a.mvt = 8;
    a.armes.clear();
    a.ajouterArme(Arme("Plasma pistol", 4, 3, 5, 1, 2, "Hot, Piercing 1"));
    a.ajouterArme(Arme("Power sword", 5, 3, 7, 1, 2, "Brutal"));
    return a;
}

// --- ASTARTES DEVASTATOR ---
Astartes Astartes::creerDevastator(char s) {
    Astartes a(s);
    a.atk = 4;
    a.hit = 3;
    a.def = 3;
    a.dgt = 8;
    a.pv = 16;
    a.mvt = 5;
    a.armes.clear();
    a.ajouterArme(Arme("Heavy bolter", 5, 3, 8, 6, 2, "Heavy, Piercing 1"));
    a.ajouterArme(Arme("Missile launcher", 4, 4, 10, 8, 3, "Explosive"));
    return a;
}
