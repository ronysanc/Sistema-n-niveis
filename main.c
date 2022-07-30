#include<stdio.h>
#include<math.h>
#include<complex.h>

int main(int argc, char const *argv[])
{
    FILE* file01=fopen("Diagonal.dat", "r");

    const int n = 2;
    double autovalores[n];
    double real = 0.0, imag = 0.0;
    double complex matrix[n][n];
    double complex arg = 0.0 + 0.0I;

    for (int i = 0; i < n; i++)
    {
        fscanf(file01, "%lf\t", &autovalores[i]);    
    }
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(file01, "(%lf, %lf)\t", &real, &imag);
            matrix[i][j] = CMPLX(real, imag);
        }
    }

    for (int i = 0; i < n; i++)
    {
        printf("%lf \t", autovalores[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("(%lf, %lf)\t", creal(matrix[i][j]), cimag(matrix[i][j]));
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i==j){
                arg = -I*autovalores[i];
                printf("(%lf, %lf)\t", cexp(arg));}
            else
                printf("(%lf, %lf)\t", 0.0, 0.0);
        }
        printf("\n");
    }


    printf("%lf + %lfi\n", creal(arg), cimag(arg));

    return 0;
}
