#include "grille.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

using namespace std;

static void clearConsole() {
    system(CLEAR);
}

// === Constructeur ===
Grille::Grille(const vector<Astartes>& escouade,
    const vector<Demon>& demons) {
    taille = max(8, (int)(escouade.size() + demons.size()) + 5);
    cases = vector<vector<char>>(taille, vector<char>(taille, '.'));
}

// === Mise a jour de la grille ===
void Grille::majPositions(const vector<Astartes>& escouade,
    const vector<Demon>& demons) {
    for (int i = 0; i < taille; ++i)
        for (int j = 0; j < taille; ++j)
            cases[i][j] = '.';

    for (const auto& a : escouade)
        if (a.isAlive())
            cases[a.getY()][a.getX()] = a.getSymbole();

    for (const auto& d : demons)
        if (d.isAlive())
            cases[d.getY()][d.getX()] = d.getSymbole();
}

// === Affichage standard ===
void Grille::afficher(const vector<Astartes>& escouade,
    const vector<Demon>& demons,
    int idActif) {
    majPositions(escouade, demons);
    cout << "\n    ";
    for (int j = 0; j < taille; ++j) cout << j << " ";
    cout << "\n";
    for (int i = 0; i < taille; ++i) {
        cout << i << " | ";
        for (int j = 0; j < taille; ++j) {
            char c = cases[i][j];
            bool estAstartes = false, estDemon = false;
            for (size_t k = 0; k < escouade.size(); ++k)
                if (escouade[k].getX() == j && escouade[k].getY() == i && escouade[k].isAlive())
                    estAstartes = true;
            for (auto& d : demons)
                if (d.getX() == j && d.getY() == i && d.isAlive())
                    estDemon = true;

            if (estAstartes) cout << "\033[32m" << c << " " << "\033[0m";
            else if (estDemon) cout << "\033[31m" << c << " " << "\033[0m";
            else cout << c << " ";
        }
        cout << "\n";
    }
}

// === Affichage de la portee ===
void Grille::afficherPortee(const Astartes& p,
    const vector<Astartes>& escouade,
    const vector<Demon>& demons) {
    majPositions(escouade, demons);
    cout << "\nPortee de deplacement (" << p.getMouvement() << " cases):\n\n";
    cout << "    ";
    for (int j = 0; j < taille; ++j) cout << j << " ";
    cout << "\n";
    for (int i = 0; i < taille; ++i) {
        cout << i << " | ";
        for (int j = 0; j < taille; ++j) {
            bool occupe = false;
            for (auto& a : escouade)
                if (a.getX() == j && a.getY() == i && a.isAlive())
                    occupe = true;
            for (auto& d : demons)
                if (d.getX() == j && d.getY() == i && d.isAlive())
                    occupe = true;

            int dist = abs(p.getX() - j) + abs(p.getY() - i);
            if (i == p.getY() && j == p.getX())
                cout << "\033[32m" << p.getSymbole() << " " << "\033[0m";
            else if (occupe)
                cout << ". ";
            else if (dist <= p.getMouvement())
                cout << "\033[33m*\033[0m ";
            else
                cout << ". ";
        }
        cout << "\n";
    }
}

// === Rendu intermediaire pour animation ===
void Grille::renderFrame(const vector<Astartes>& escouade,
    const vector<Demon>& demons,
    const vector<pair<int, int>>& trace,
    int idActif) {
    majPositions(escouade, demons);
    for (auto& pt : trace) {
        int tx = pt.first, ty = pt.second;
        if (tx >= 0 && tx < taille && ty >= 0 && ty < taille)
            if (cases[ty][tx] == '.') cases[ty][tx] = '+';
    }
    clearConsole();
    afficher(escouade, demons, idActif);
    this_thread::sleep_for(chrono::milliseconds(100));
}

// === Animation: clignotement de selection ===
void Grille::clignoterSelection(const Astartes& perso,
    const vector<Astartes>& escouade,
    const vector<Demon>& demons) {
    for (int i = 0; i < 4; ++i) {
        clearConsole();
        if (i % 2 == 0)
            afficher(escouade, demons);
        else {
            vector<Astartes> copie = escouade;
            for (auto& a : copie)
                if (&a == &perso)
                    ; // temporairement cache
            afficher(copie, demons);
        }
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

// === Animation: tir (projectile * vers la cible) ===
void Grille::animationTir(const Astartes& tireur,
    const Personnage& cible,
    vector<Astartes>& escouade,
    vector<Demon>& demons) {
    int x1 = tireur.getX(), y1 = tireur.getY();
    int x2 = cible.getX(), y2 = cible.getY();

    int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
    int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);

    int cx = x1, cy = y1;
    while (cx != x2 || cy != y2) {
        cx += dx; cy += dy;
        if (cx == x2 && cy == y2) break;

        vector<pair<int, int>> trace = { {cx, cy} };
        clearConsole();
        majPositions(escouade, demons);
        if (cy >= 0 && cy < taille && cx >= 0 && cx < taille)
            cases[cy][cx] = '*';
        afficher(escouade, demons);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    clearConsole();
    afficher(escouade, demons);
}

// === Animation: melee (impact sur la cible) ===
void Grille::animationCac(const Personnage& attaquant,
    const Personnage& cible,
    vector<Astartes>& escouade,
    vector<Demon>& demons) {
    for (int i = 0; i < 4; ++i) {
        clearConsole();
        majPositions(escouade, demons);
        if (i % 2 == 0)
            cases[cible.getY()][cible.getX()] = '*';
        afficher(escouade, demons);
        this_thread::sleep_for(chrono::milliseconds(150));
    }
    clearConsole();
    afficher(escouade, demons);
}

// === Deplacement avec trainee jaune de "+" ===
bool Grille::deplacer(Astartes& p,
    const vector<Demon>& demons,
    vector<Astartes>& escouade,
    int idActif) {

    int nx, ny;
    while (true) {
        afficherPortee(p, escouade, demons);
        cout << "\nEntrez les nouvelles coordonnees (x y) ou -1 pour annuler: ";
        cin >> nx;
        if (nx == -1) return false;
        cin >> ny;

        int distTot = abs(p.getX() - nx) + abs(p.getY() - ny);
        if (nx < 0 || nx >= taille || ny < 0 || ny >= taille) {
            cout << "Coordonnees hors de la grille.\n";
            continue;
        }
        if (distTot > p.getMouvement()) {
            cout << "Trop loin. Mouvement max = " << p.getMouvement() << "\n";
            continue;
        }

        bool occupe = false;
        for (auto& a : escouade)
            if (&a != &p && a.getX() == nx && a.getY() == ny && a.isAlive())
                occupe = true;
        for (auto& d : demons)
            if (d.getX() == nx && d.getY() == ny && d.isAlive())
                occupe = true;

        if (occupe) {
            cout << "Case occupee.\n";
            continue;
        }

        int cx = p.getX(), cy = p.getY();
        vector<pair<int, int>> trace;

        // === Deplacement en X ===
        while (cx != nx) {
            trace.push_back({ cx, cy });
            cx += (nx > cx) ? 1 : -1;
            p.setPosition(cx, cy);

            clearConsole();
            majPositions(escouade, demons);

            // Ajout des + jaunes
            for (auto& t : trace) {
                int tx = t.first, ty = t.second;
                if (tx >= 0 && tx < taille && ty >= 0 && ty < taille)
                    if (cases[ty][tx] == '.')
                        cases[ty][tx] = '+'; // stock brut, on colore à l'affichage
            }

            // Affichage manuel avec couleurs
            cout << "\n    ";
            for (int j = 0; j < taille; ++j) cout << j << " ";
            cout << "\n";
            for (int i = 0; i < taille; ++i) {
                cout << i << " | ";
                for (int j = 0; j < taille; ++j) {
                    char c = cases[i][j];
                    if (c == '+') cout << "\033[33m+\033[0m ";
                    else if (c == p.getSymbole()) cout << "\033[32m" << c << "\033[0m ";
                    else {
                        bool demon = false;
                        for (auto& d : demons)
                            if (d.getX() == j && d.getY() == i && d.isAlive()) demon = true;
                        if (demon) cout << "\033[31m" << c << "\033[0m ";
                        else cout << c << " ";
                    }
                }
                cout << "\n";
            }

            this_thread::sleep_for(chrono::milliseconds(120));
        }

        // === Deplacement en Y ===
        while (cy != ny) {
            trace.push_back({ cx, cy });
            cy += (ny > cy) ? 1 : -1;
            p.setPosition(cx, cy);

            clearConsole();
            majPositions(escouade, demons);

            // Ajout des + jaunes
            for (auto& t : trace) {
                int tx = t.first, ty = t.second;
                if (tx >= 0 && tx < taille && ty >= 0 && ty < taille)
                    if (cases[ty][tx] == '.')
                        cases[ty][tx] = '+';
            }

            // Affichage manuel avec couleurs
            cout << "\n    ";
            for (int j = 0; j < taille; ++j) cout << j << " ";
            cout << "\n";
            for (int i = 0; i < taille; ++i) {
                cout << i << " | ";
                for (int j = 0; j < taille; ++j) {
                    char c = cases[i][j];
                    if (c == '+') cout << "\033[33m+\033[0m ";
                    else if (c == p.getSymbole()) cout << "\033[32m" << c << "\033[0m ";
                    else {
                        bool demon = false;
                        for (auto& d : demons)
                            if (d.getX() == j && d.getY() == i && d.isAlive()) demon = true;
                        if (demon) cout << "\033[31m" << c << "\033[0m ";
                        else cout << c << " ";
                    }
                }
                cout << "\n";
            }

            this_thread::sleep_for(chrono::milliseconds(120));
        }

        // Nettoyage final
        clearConsole();
        afficher(escouade, demons);
        return true;
    }
}