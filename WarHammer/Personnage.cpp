#include "Personnage.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

// === Petites pauses pour les animations ===
static void pauseCourt() {
    this_thread::sleep_for(chrono::milliseconds(120));
}

// === Animation de tir ===
static void effetTir(const string& arme) {
    string projectile;
    if (arme.find("Bolter") != string::npos) projectile = "[BANG]";
    else if (arme.find("Plasma") != string::npos) projectile = "[ZAP]";
    else if (arme.find("Missile") != string::npos) projectile = "[BOOM]";
    else projectile = "[SHOT]";

    for (int i = 0; i < 5; ++i) {
        cout << projectile << " ";
        pauseCourt();
    }
    cout << endl;
}

// === Animation de combat au corps a corps ===
static void effetCac(const string& arme) {
    string impact;
    if (arme.find("Sword") != string::npos) impact = "[SLASH]";
    else if (arme.find("Fist") != string::npos) impact = "[PUNCH]";
    else if (arme.find("Axe") != string::npos) impact = "[SMASH]";
    else impact = "[HIT]";

    for (int i = 0; i < 3; ++i) {
        cout << impact << " ";
        pauseCourt();
    }
    cout << endl;
}

// === Animation de selection ===
static void effetSelection(char symbole) {
    cout << "\033[36m"; // Cyan
    for (int i = 0; i < 3; ++i) {
        cout << "\r> [" << symbole << "] Pret a agir <";
        pauseCourt();
        cout << "\r                        ";
        pauseCourt();
    }
    cout << "\r> [" << symbole << "] Selectionne.\033[0m" << endl;
    this_thread::sleep_for(chrono::milliseconds(300));
}

// === Constructeur ===
Personnage::Personnage(int a, int h, int df, int dg, int hp, int mv, char s)
    : atk(a), hit(h), def(df), dgt(dg), pv(hp), mvt(mv), symbole(s),
    armeActive(0), costMove(1), costAttack(1), x(0), y(0) {
}

Personnage::~Personnage() {}

// === Attaque ===
void Personnage::attack(Personnage& target) {
    if (armes.empty()) {
        cout << "Aucune arme equipee." << endl;
        return;
    }

    Arme arme = armes[armeActive];
    cout << endl << symbole << " attaque " << target.getSymbole()
        << " avec " << arme.getNom() << " !" << endl;

    // Animation selon le type d'arme
    if (arme.getPortee() > 1) effetTir(arme.getNom());
    else effetCac(arme.getNom());

    // Jets pour toucher
    int touches = Des::lancer(arme.getAtk(), arme.getHit(), arme.getNom());

    // Jets de defense
    int saves = Des::lancer(touches, target.getDef(), "Sauvegarde");

    int netHits = max(0, touches - saves);
    int damage = netHits * arme.getDmg();

    cout << "-> " << netHits << " touches passent la defense, "
        << damage << " degats infliges." << endl;

    target.takeDamage(damage);
}

// === Degats ===
void Personnage::takeDamage(int amount) {
    pv -= amount;
    if (pv < 0) pv = 0;
    cout << symbole << " subit " << amount << " points de degats (PV restants: " << pv << ")." << endl;
}

// === Gestion des armes ===
void Personnage::ajouterArme(const Arme& a) {
    armes.push_back(a);
}

void Personnage::choisirArme(int index) {
    if (index >= 0 && index < (int)armes.size()) {
        armeActive = index;
    }
}

Arme Personnage::getArmeActive() const {
    if (armes.empty()) return Arme("Aucune", 0, 0, 0, 0, 0, "");
    return armes[armeActive];
}
