#include "Armes.h"

Arme::Arme()
    : nom("Aucune"), atk(0), hit(0), dmg(0), portee(0), coutAP(1), regles("") {
}

Arme::Arme(const std::string& n, int a, int h, int d, int p, int c, const std::string& r)
    : nom(n), atk(a), hit(h), dmg(d), portee(p), coutAP(c), regles(r) {
}
