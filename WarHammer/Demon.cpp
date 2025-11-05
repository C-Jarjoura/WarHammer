#include "Demon.h"
#include "Armes.h"

Demon::Demon(char s)
    : Personnage(4, 3, 3, 5, 12, 6, s) {
    ajouterArme(Arme("Griffe", 4, 3, 5, 1, 1, "Base"));
}
Demon Demon::creerMineur(char s) {
    Demon d(s);
    d.atk = 3; d.hit = 4; d.def = 3; d.dgt = 4; d.pv = 10; d.mvt = 7;
    d.armes.clear();
    d.ajouterArme(Arme("Griffes rapides", 3, 4, 4, 1, 1, "Rapide"));
    d.armeActive = 0; // <-- important
    return d;
}

Demon Demon::creerMajeur(char s) {
    Demon d(s);
    d.atk = 5; d.hit = 3; d.def = 4; d.dgt = 8; d.pv = 20; d.mvt = 5;
    d.armes.clear();
    d.ajouterArme(Arme("Hache du Warp", 6, 3, 9, 1, 2, "Brutal"));
    d.armeActive = 0; // <-- important
    return d;
}

Demon Demon::creerDestructeur(char s) {
    Demon d(s);
    d.atk = 7; d.hit = 2; d.def = 4; d.dgt = 10; d.pv = 25; d.mvt = 6;
    d.armes.clear();
    d.ajouterArme(Arme("Lame du Chaos", 7, 2, 10, 1, 3, "Mortelle"));
    d.armeActive = 0; // <-- important
    return d;
}
