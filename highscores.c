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
#include <curses.h>
#include "highscores.h"
#include "spake.h"

int top[10];
char *top_string[10];
char *top_name[10];

void load_highscore()
{
  int i;
  FILE *fichier;
  char *filename = (char*) malloc(50*sizeof(char));
  sprintf(filename,"%s/.spake_scores",getenv("HOME"));
  fichier = fopen(filename,"r");
  if (fichier != NULL)
    {
      const char *format = "%s %s\n";
      for (i=0;i<10;i++)
        {
          top_name[i]=(char*) malloc(15*sizeof(char));
          top_string[i]=(char*) malloc(15*sizeof(char));
          fscanf(fichier,format,top_name[i],top_string[i]);
          top[i] = atoi(top_string[i]);
          //top[i]=100;
        }
      fclose(fichier);
    }
  else
    for (i=0;i<10;i++)
      {
        top_name[i] = "x";
        top[i] = 0;
      }
}

int new_highscore()
{
  int i;
  for (i=0;i<10;i++)
      {
        if (score>top[i])
          return i+1;
      }
  // si on arrive là, c'est mort
  return 0;
}

void add_highscore(char *nick)
{
  int i,position;
  char *filename = (char*) malloc(50*sizeof(char));
  sprintf(filename,"%s/.spake_scores",getenv("HOME"));
  for (i=8;i>-1;i--)
    {
      if (top[i]<score)
        {
          top_name[i+1]=top_name[i];
          top[i+1]=top[i];
        }
      else
        break;
    }
  position=i+1;
  top[position]=score;
  top_name[position]=nick;
  FILE *fichier = fopen(filename,"w");
  for (i=0;i<10;i++)
    fprintf(fichier,"%s %d\n",top_name[i],top[i]);
  fclose(fichier);
}

void view_highscores()
{
  int i;
  wclear(dialog);
  wprintw(dialog,
          "  rg   joueur        score  \n"
          "----------------------------\n");
  for (i=0;i<10;i++)
    wprintw(dialog,"  %2d   %-10s   %6d  \n",i+1,top_name[i],top[i]);
  wrefresh(dialog);
}

void highlight_highscores(int position)
{
  view_highscores();
  wattron(dialog,COLOR_PAIR(3));
  mvwaddch(dialog,position+1,1,'@');
  wattroff(dialog,COLOR_PAIR(3));
  wrefresh(dialog);
}
