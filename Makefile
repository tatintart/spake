# Copyright (C) 2005-2016 Xavier Lepaul <xavier@lepaul.fr>
# 
# This file is part of Spake.
# 
# Spake is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

spake: spake.o highscores.o
	gcc -Wall -lcurses -o spake spake.o highscores.o

spake.o: spake.c spake.h
	gcc -c -Wall -o spake.o spake.c

highscores.o: highscores.c highscores.h
	gcc -c -Wall -o highscores.o highscores.c

run: spake
	./spake

install: spake
	cp spake ${HOME}/bin/

clean:
	rm -f *~ *.o

realclean: clean
	rm -f spake
