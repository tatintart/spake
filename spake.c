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

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#include "spake.h"
#include "highscores.h"

void anim_debut()
{
  int delai=75000;
  int i;
  wprintw(dialog,"                @\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog," @@@            @\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog,"@   @           @\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog,"@     @@@   @@  @  @  @@\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog," @@@  @  @    @ @ @  @  @\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog,"    @ @  @  @@@ @@   @@@@\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog,"@   @ @  @ @  @ @ @  @\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog," @@@  @@@   @@@ @  @  @@@\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog,"      @\n");
  wrefresh(dialog);usleep(delai);
  wprintw(dialog,"      @\n");
  wrefresh(dialog);usleep(delai);
  sleep(1);
  wmove(dialog,0,0);
  for (i=0;i<10;i++)
    {
      wprintw(dialog,"                         \n");
      wrefresh(dialog);usleep(delai);
    }
  usleep(500000);
}

int main()
{
  char key;

  largeur=21;
  hauteur=7;
  tableau = (int*) malloc(largeur*hauteur*sizeof(int));
  vaisseau=')';// D)]#->??*

  load_highscore();
  init_curses();
  srand((unsigned int) time(NULL));
  
  anim_debut();

  help();

  /* Boucle principale, menu */
  do {
    key=wgetch(menubar);
    switch(key)
      {
      case 'n':
        partie();
        break;
      case 's':
        view_highscores();
        break;
      case 'a':
        help();
        break;
      case 'q':
        end_curses();
        return 0;
      }
  } while (1);
    
}

void init_curses()
{
  int bkgd_color;

  /* initialisation de la bibliothèque */
  initscr();

  /* utilisation des couleurs & définition des paires de couleurs */
  start_color();
  if (use_default_colors()==OK)
    bkgd_color=-1;
  else
    bkgd_color=COLOR_BLACK;
  init_pair(1,COLOR_BLACK,COLOR_WHITE);
  init_pair(2,COLOR_WHITE,bkgd_color);
  init_pair(3,COLOR_RED,bkgd_color);
  init_pair(4,COLOR_GREEN,bkgd_color);
  init_pair(5,COLOR_BLUE,bkgd_color);

  /* on n'affiche pas de curseur, ni les caractères tapés */
  curs_set(0);
  noecho();

  /* création des sous-fenêtres */
  menubar = newwin(1,80,0,0);
  wbkgd(menubar,COLOR_PAIR(1));
  waddstr(menubar," SPAKE | (n)ouveau jeu | meilleurs (s)cores | (a)ide | (q)uitter");
  wrefresh(menubar);

  blocJeu = newwin(23,78,2,2);

  fenetreJeu = subwin(blocJeu,hauteur,largeur,3,3);
  nodelay(fenetreJeu,true);
  keypad(fenetreJeu,1);

  dialog = newwin(23,78,2,2);
}

void end_curses()
{
  delwin(menubar);
  delwin(fenetreJeu);
  endwin();
}

void help()
{
  wclear(dialog);
  wprintw(dialog,
          "--  SPAKE, sur une idée de etk  --\n"
          "    --------------------------\n"
          "\n"
          "Spake, c'est pas compliqué\n"
          "\n"
          "  But du jeu :\n"
          "    - ramasser les  ");
  wattron(dialog,COLOR_PAIR(4));
  waddch(dialog,'x');
  wattroff(dialog,COLOR_PAIR(4));
  wprintw(dialog,
          "\n"
          "    - éviter les    ");
  wattron(dialog,COLOR_PAIR(3));
  waddch(dialog,'+');
  wattroff(dialog,COLOR_PAIR(3));
  wprintw(dialog,"\n"
          "\n"
          "  Contrôle du vaisseau :\n"
          "   - flèche haute pour monter\n"
          "   - flèche basse pour descendre\n");
  wrefresh(dialog);
}

void partie()
{
  /* boucle de partie */
  int xpos;
  int ypos = 3;
  int perdu = 0;
  int niveau = 0;
  score=0;
  draw_cadre();
  wmove(blocJeu,9,6);
  wprintw(blocJeu,"%7d",score);
  wrefresh(blocJeu);

  do {
    niveau++;
    wclear(fenetreJeu);
    wmove(fenetreJeu,0,0);
    wmove(blocJeu,9,1);
    wprintw(blocJeu,"%3d",niveau);
    wrefresh(blocJeu);
    gen_tableau(niveau);
    //draw_tableau();
    usleep(500000);
    for (xpos=0;xpos<largeur;xpos++)
      {
        switch(wgetch(fenetreJeu))
          {
          case KEY_UP:
            ypos=max(0,ypos-1);
            break;
          case KEY_DOWN:
            ypos=min(ypos+1,hauteur-1);
            break;
          case 'q':
            end_curses();
            exit(0);
          }
        switch (tableau[ypos*largeur+xpos])
          {
          case 1:
            perdu = 1;
            wattron(fenetreJeu,COLOR_PAIR(3));
            mvwaddch(fenetreJeu,ypos,xpos,'@');
            wattroff(fenetreJeu,COLOR_PAIR(3));
            wrefresh(fenetreJeu);
            /* XXX pabo */
            goto finDePartie;
          case 2:
            score+=42*niveau;
            wmove(blocJeu,9,6);
            wprintw(blocJeu,"%7d",score);
            wrefresh(blocJeu);
          default:
            mvwaddch(fenetreJeu,ypos,xpos,vaisseau);
          }
                
        wrefresh(fenetreJeu);
        // pour rajouter une 'trainée'
        //mvwaddch(fenetreJeu,ypos,xpos,'=');
        usleep(125000);
      }
  } while (!perdu);


 finDePartie:
  animation_de_fin();
  if (new_highscore())
    {
      sleep(1);
      int position = new_highscore();
      char *nick = (char*)malloc(10*sizeof(char));
      /*wmove(fenetreJeu,5,8);
        wprintw(fenetreJeu,"YEAH !",score);*/
      wclear(dialog);
      wprintw(dialog,"Vous entrez dans les meilleurs scores !\n"
              "\n"
              "  Score      : %d\n"
              "  Position   : %d\n"
              "  Votre nick : ",
              score,position);
      curs_set(1);
      echo();
      wgetnstr(dialog,nick,10);
      if (strlen(nick)==0)
        nick = "(noname)";
      add_highscore(nick);
      curs_set(0);
      noecho();
      highlight_highscores(position);
    }
  else
    {
    }
}

void gen_tableau(int niveau)
{
  int nb_mines=(niveau/2)+(int) (((float) 10*rand())/RAND_MAX );
  int nb_points=11;
  int i,x,y;
  int sleep_time=1000000/(nb_points+nb_mines);

  for(i=0;i<largeur*hauteur;i++)
    tableau[i]=0;

  for (i=0;i<nb_mines;i++)
    {
      x=5+(int)( ((float) (largeur-5)*rand())/RAND_MAX );
      y=(int)( ((float) hauteur*rand())/RAND_MAX );
      tableau[y*largeur+x]=1;
      wattron(fenetreJeu,COLOR_PAIR(3));
      mvwaddch(fenetreJeu,y,x,'+');
      wattroff(fenetreJeu,COLOR_PAIR(3));
      wrefresh(fenetreJeu);
        usleep(sleep_time);
    }

  for (i=0;i<nb_points;i++)
    {
      x=1+(int)( ((float) (largeur-1)*rand())/RAND_MAX );
      y=(int)( ((float) hauteur*rand())/RAND_MAX );
      tableau[y*largeur+x]=2;
      wattron(fenetreJeu,COLOR_PAIR(4));
      mvwaddch(fenetreJeu,y,x,'x');
      wattroff(fenetreJeu,COLOR_PAIR(4));
      wrefresh(fenetreJeu);
      usleep(sleep_time);
    }

}

void draw_tableau()
{
  int y,x;
  for (y=0;y<hauteur;y++)
    {
      for (x=0;x<largeur;x++)
        {
          switch(tableau[y*largeur+x])
            {
            case 1:
              wattron(fenetreJeu,COLOR_PAIR(3));
              mvwaddch(fenetreJeu,y,x,'+');
              wattroff(fenetreJeu,COLOR_PAIR(3));
              break;
            case 2:
              wattron(fenetreJeu,COLOR_PAIR(4));
              mvwaddch(fenetreJeu,y,x,'x');
              wattroff(fenetreJeu,COLOR_PAIR(4));
              break;
            }
          wrefresh(fenetreJeu);
          usleep(5000);
        }
    }
}

void repeat_largeur(char abeu)
{
  int i;
  for (i=0;i<largeur;i++)
    {
      waddch(blocJeu,abeu);
    }
}

void draw_cadre()
{
  int i;
  wclear(blocJeu);
  wattron(blocJeu,COLOR_PAIR(5));
  waddch(blocJeu,'+');
  repeat_largeur('-');
  waddstr(blocJeu,"+\n");
  for (i=0;i<hauteur;i++)
    {
      waddch(blocJeu,'|');
      repeat_largeur(' ');
      waddstr(blocJeu,"|\n");
    }
  waddch(blocJeu,'+');
  repeat_largeur('-');
  waddstr(blocJeu,"+\n");
  wattroff(blocJeu,COLOR_PAIR(5));
  wrefresh(blocJeu);
}

void animation_de_fin()
{
  int i,j;
  wmove(fenetreJeu,0,0);
  wattron(fenetreJeu,COLOR_PAIR(5));
  for (i=0;i<hauteur;i++)
    {
      for (j=0;j<largeur;j++)
        {
          waddch(fenetreJeu,'@');
          wrefresh(fenetreJeu);
          usleep(5000);
        }
    }
  //wattroff(fenetreJeu,COLOR_PAIR(5));
  wattron(fenetreJeu,COLOR_PAIR(3));
  wmove(fenetreJeu,2,3);
  wprintw(fenetreJeu,"  GAME OVER !  ");
  wmove(fenetreJeu,4,3);
  wprintw(fenetreJeu,"  %-7d pts  ",score);
  wrefresh(fenetreJeu);
  wattroff(fenetreJeu,COLOR_PAIR(3));
}
