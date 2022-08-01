from os import system

system("gcc -I /home/carlos/gsl/gsl-install/include/ -c n-niveis.c")
system("gcc -L /home/carlos/gsl/gsl-install/lib/ n-niveis.o -lgsl -lgslcblas -lm -o nNiveis")
system("rm n-niveis.o")
system("./nNiveis")
system("rm nNiveis")