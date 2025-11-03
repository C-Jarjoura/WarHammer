#pragma once
#include <string>

class Arme {
private:
    std::string nom;
    int atk;       // nombre de dés lancés
    int hit;       // seuil pour toucher (ex: 3+)
    int dmg;       // dégâts infligés par touche
    int portee;    // portée de l’arme
    int coutAP;    // coût en points d’action pour l’utiliser
    std::string regles;

public:
    Arme();
    Arme(const std::string& n, int a, int h, int d, int p, int c, const std::string& r);

    std::string getNom() const { return nom; }
    int getAtk() const { return atk; }
    int getHit() const { return hit; }
    int getDmg() const { return dmg; }
    int getPortee() const { return portee; }
    int getCoutAP() const { return coutAP; }
    std::string getRegles() const { return regles; }
};
