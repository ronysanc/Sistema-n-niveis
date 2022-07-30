import os

os.system("gcc -I /home/rony/gsl/gsl-install/include/ -c Eigen.c")
os.system("gcc -L /home/rony/gsl/gsl-install/lib/ Eigen.o -lgsl -lgslcblas -lm -o Eigen.out")
os.system("./Eigen.out")

os.system("gcc main.c -lm -o main.out")
os.system("./main.out")