#include "Astartes.h"
#include "Demon.h"
#include "grille.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

using namespace std;

// === Aleatoire ===
int randInRange(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// === Verifie si une case est deja occupee ===
bool positionOccupee(int x, int y,
    const vector<Astartes>& escouade,
    const vector<Demon>& demons) {
    for (auto& a : escouade)
        if (a.getX() == x && a.getY() == y) return true;
    for (auto& d : demons)
        if (d.getX() == x && d.getY() == y) return true;
    return false;
}

int main() {
    srand((unsigned)time(0));

    // === CREATION DES UNITES ===
    vector<Astartes> escouade = {
        Astartes::creerTactique('A'),
        Astartes::creerAssaut('B'),
        Astartes::creerDevastator('C')
    };

    vector<Demon> demons = {
        Demon::creerMineur('d'),
        Demon::creerMajeur('D'),
        Demon::creerDestructeur('X')
    };

    // === GRILLE ===
    Grille g(escouade, demons);
    int taille = g.getTaille();

    // === PLACEMENT ALEATOIRE ===
    for (auto& a : escouade) {
        int x, y;
        do {
            x = randInRange(0, taille / 3);
            y = randInRange(0, taille / 2);
        } while (positionOccupee(x, y, escouade, demons));
        a.setPosition(x, y);
    }

    for (auto& d : demons) {
        int x, y;
        do {
            x = randInRange(taille / 2, taille - 1);
            y = randInRange(taille / 2, taille - 1);
        } while (positionOccupee(x, y, escouade, demons));
        d.setPosition(x, y);
    }

    int tour = 1;
    int apJoueur = 3;

    cout << BOLD << CYAN << "\n=== ESCARMOUCHE WARHAMMER 40K ===" << RESET << "\n";

    // === BOUCLE DE JEU ===
    while (true) {
        bool tousDemonsMorts = true;
        for (auto& d : demons)
            if (d.isAlive()) tousDemonsMorts = false;
        if (tousDemonsMorts) break;

        bool tousMorts = true;
        for (auto& a : escouade)
            if (a.isAlive()) tousMorts = false;
        if (tousMorts) break;

        cout << CYAN << "\n===== TOUR " << tour++ << " =====" << RESET << "\n";
        apJoueur = 3;

        cout << BOLD << GREEN << "\n=== ESCQUADE ASTARTES ===\n" << RESET;
        for (size_t i = 0; i < escouade.size(); ++i) {
            Astartes& a = escouade[i];
            cout << (a.isAlive() ? GREEN : RED)
                << " #" << (i + 1)
                << " [" << a.getSymbole() << "] "
                << (a.isAlive() ? "En vie" : "Mort")
                << RESET
                << " | PV=" << a.getHP()
                << " | Mouvement=" << a.getMouvement()
                << " | Pos(" << a.getX() << "," << a.getY() << ")\n";
        }

        cout << BOLD << RED << "\n=== FORCES DU CHAOS ===\n" << RESET;
        for (auto& d : demons) {
            cout << (d.isAlive() ? RED : MAGENTA)
                << " Demon [" << d.getSymbole() << "] "
                << (d.isAlive() ? "En vie" : "Mort")
                << RESET
                << " | PV=" << d.getHP()
                << " | Pos(" << d.getX() << "," << d.getY() << ")\n";
        }

        cout << string(40, '-') << "\n";

        bool finPhase = false;
        while (!finPhase && apJoueur > 0) {
            g.afficher(escouade, demons);
            cout << GREEN << "\nAP Joueur: " << apJoueur << "/3" << RESET << "\n";
            cout << CYAN << "Choisir Astartes (1-" << escouade.size() << ") ou 0 pour fin de phase: " << RESET;
            int choix;
            cin >> choix;

            if (choix == 0) {
                cout << MAGENTA << "Fin de la phase du joueur.\n" << RESET;
                break;
            }

            if (choix < 1 || choix >(int)escouade.size()) {
                cout << RED << "Choix invalide.\n" << RESET;
                continue;
            }

            int id = choix - 1;
            Astartes& a = escouade[id];
            if (!a.isAlive()) {
                cout << RED << "Cet Astartes est mort.\n" << RESET;
                continue;
            }

            // Animation de selection visuelle
            g.clignoterSelection(a, escouade, demons);

            cout << YELLOW << "\n--- Astartes #" << choix << " (" << a.getSymbole() << ") ---" << RESET << "\n";
            cout << BLUE << "1. Deplacer (1 AP)\n"
                << "2. Attaquer (selon arme)\n"
                << "3. Annuler\n> " << RESET;

            int action;
            cin >> action;

            if (action == 1) {
                if (apJoueur >= 1 && g.deplacer(a, demons, escouade, id)) {
                    apJoueur -= 1;
                }
                else {
                    cout << RED << "Deplacement impossible.\n" << RESET;
                }
            }
            else if (action == 2) {
                const vector<Arme>& armes = a.getArmes();
                Demon* cible = nullptr;
                int distanceMin = 999;

                for (auto& d : demons) {
                    if (!d.isAlive()) continue;
                    int dx = abs(a.getX() - d.getX());
                    int dy = abs(a.getY() - d.getY());
                    int dist = max(dx, dy);
                    if (dist < distanceMin) {
                        distanceMin = dist;
                        cible = &d;
                    }
                }

                if (!cible) {
                    cout << RED << "Aucune cible disponible.\n" << RESET;
                    continue;
                }

                cout << CYAN << "\n--- Choix d'arme ---" << RESET << "\n";
                for (size_t j = 0; j < armes.size(); ++j) {
                    const Arme& arme = armes[j];
                    bool aPortee = (distanceMin <= arme.getPortee());
                    string couleur = aPortee ? GREEN : RED;
                    cout << couleur << (j + 1) << ". " << arme.getNom()
                        << " (" << arme.getAtk() << " des, "
                        << arme.getHit() << "+, Dmg " << arme.getDmg()
                        << ", Portee " << arme.getPortee()
                        << ", Regles: " << arme.getRegles() << ")" << RESET << "\n";
                }
                cout << YELLOW << "0. Annuler" << RESET << "\n> ";

                int armeChoisie;
                cin >> armeChoisie;
                if (armeChoisie == 0) continue;
                if (armeChoisie < 1 || armeChoisie >(int)armes.size()) {
                    cout << RED << "Choix invalide.\n" << RESET;
                    continue;
                }

                Arme arme = armes[armeChoisie - 1];
                if (distanceMin > arme.getPortee()) {
                    cout << RED << "Cible hors de portee.\n" << RESET;
                    continue;
                }

                int coutAP = arme.getCoutAP();
                if (apJoueur < coutAP) {
                    cout << RED << "Pas assez d'AP (" << coutAP << " requis).\n" << RESET;
                    continue;
                }

                // Animation d'attaque selon type
                if (arme.getPortee() > 1)
                    g.animationTir(a, *cible, escouade, demons);
                else
                    g.animationCac(a, *cible, escouade, demons);

                a.choisirArme(armeChoisie - 1);
                a.attack(*cible);
                apJoueur -= coutAP;
            }
            else if (action == 3) {
                cout << MAGENTA << "Action annulee.\n" << RESET;
            }
        }

        // === TOUR DES DEMONS ===
        cout << RED << "\n>>> Tour des Demons <<<" << RESET << "\n";
        for (auto& d : demons) {
            if (!d.isAlive()) continue;

            vector<Astartes*> vivants;
            for (auto& a : escouade)
                if (a.isAlive()) vivants.push_back(&a);
            if (vivants.empty()) break;

            Astartes* cible = vivants[randInRange(0, vivants.size() - 1)];
            int dx = cible->getX() - d.getX();
            int dy = cible->getY() - d.getY();
            int dist = max(abs(dx), abs(dy));

            if (dist > 1) {
                d.setPosition(d.getX() + (dx > 0 ? 1 : (dx < 0 ? -1 : 0)),
                    d.getY() + (dy > 0 ? 1 : (dy < 0 ? -1 : 0)));
                cout << RED << "Le demon [" << d.getSymbole()
                    << "] avance vers " << cible->getSymbole() << "." << RESET << "\n";
            }
            else {
                g.animationCac(d, *cible, escouade, demons);
                cout << RED << "Le demon [" << d.getSymbole()
                    << "] attaque " << cible->getSymbole() << " !" << RESET << "\n";
                d.attack(*cible);
            }
        }
    }

    cout << BOLD << "\n=== Fin de la bataille ===\n" << RESET;
    bool victoire = false;
    for (auto& a : escouade)
        if (a.isAlive()) victoire = true;

    if (victoire)
        cout << GREEN << "Les Astartes remportent la victoire !\n" << RESET;
    else
        cout << RED << "Les Demons triomphent !\n" << RESET;

    return 0;
}
