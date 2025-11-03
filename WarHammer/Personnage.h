#pragma once
#include "Armes.h"
#include "Des.h"
#include <vector>
#include <string>

class Personnage {
protected:
    int atk;        // Nombre de dés à lancer
    int hit;        // Seuil minimum pour réussir un jet (ex: 3 => 3+)
    int def;        // Seuil de défense
    int dgt;        // Dégâts infligés par réussite
    int pv;         // Points de vie
    int mvt;        // Distance max de déplacement
    int costMove;   // Coût d'une action de déplacement (AP)
    int costAttack; // Coût d'une action d'attaque (AP)
    char symbole;   // Lettre affichée sur la carte
    int x, y;       // Position sur la grille

    std::vector<Arme> armes;
    int armeActive;

public:
    Personnage(int a, int h, int df, int dg, int hp, int mv, char s);

    virtual ~Personnage();

    // --- Combat ---
    virtual void attack(Personnage& target);
    void takeDamage(int amount);
    bool isAlive() const { return pv > 0; }

    // --- Getters ---
    int getHP() const { return pv; }
    int getDef() const { return def; }
    int getDmg() const { return dgt; }
    int getMouvement() const { return mvt; }
    char getSymbole() const { return symbole; }

    int getX() const { return x; }
    int getY() const { return y; }

    // --- Setters ---
    void setPosition(int nx, int ny) { x = nx; y = ny; }

    void setCostMove(int c) { costMove = c; }
    void setCostAttack(int c) { costAttack = c; }
    int getCostMove() const { return costMove; }
    int getCostAttack() const { return costAttack; }

    // --- Armes ---
    void ajouterArme(const Arme& a);
    void choisirArme(int index);
    Arme getArmeActive() const;
    const std::vector<Arme>& getArmes() const { return armes; }
};
