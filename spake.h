/*
 * Copyright (C) 2005-2016 Xavier Lepaul <xavier@lepaul.fr>
 *
 * This file is part of Spake.
 *
 * Spake is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SPAKE_H
#define SPAKE_H

#define min(x,y) x<y?x:y
#define max(x,y) x<y?y:x

/* sous-fenêtres */
WINDOW *menubar,*fenetreJeu,*blocJeu,*dialog;

/* les tableaux */
int largeur, hauteur;
int *tableau;

char vaisseau;
int score;

/* initialisation de l'affichage */
void init_curses();

/* fermeture des fenêtres, libération de mémoire... */
void end_curses();

/* génération d'un tableau du jeu */
void gen_tableau(int niveau);

/* dessins */
void draw_tableau();
void draw_cadre();
void animation_de_fin();

/* évenements du jeu */
void partie();
void view_highscores();
void help();

#endif
