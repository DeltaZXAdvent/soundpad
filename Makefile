soundpad : soundpad.c
	cc -o soundpad soundpad.c -lpulse-simple -lpulse
main : main.cc
	g++ -o main main.cc -lpulse
tags:
	ctags /usr/include/stdio.h
