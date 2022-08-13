from os import system

system("gcc -I /home/rony/gsl/gsl-install/include/ -c n-niveis.c")
system("gcc -L /home/rony/gsl/gsl-install/lib/ n-niveis.o -lgsl -lgslcblas -lm -o nNiveis")
system("rm n-niveis.o")
system("./nNiveis")
system("rm nNiveis")
system("xmgrace -nxy probabilidades.dat")