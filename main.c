#include <stdio.h>
#include <math.h>
#include <complex.h>

int main(int argc, char *argv[])
{
    const int quantidadeNiveis = 2;
    double autovalor[quantidadeNiveis], probabilidadeEstado[quantidadeNiveis];
    double complex evolucaoTemporal[quantidadeNiveis];
    double complex estadoInicial[quantidadeNiveis];
    double complex mudancaBase[quantidadeNiveis][quantidadeNiveis];
    double realAutovalor, cmplAutovalor;
    double const Omega = 1.0, dt = 0.5;

    FILE *eigenFile = fopen("Diagonal.dat", "r");

//Ler os autovalores no arquivo
    for (int i = 0; i < quantidadeNiveis; i++)
        fscanf(eigenFile, "%lf\t", &autovalor[i]);
//Ler a matriz de mudança de base da base canônica
//à base de autoestados
    for (int i = 0; i < quantidadeNiveis; i++)
    {
        for (int j = 0; j < quantidadeNiveis; j++)
        {
            fscanf(eigenFile, "(%lf, %lf)\t", &realAutovalor, &cmplAutovalor);
            mudancaBase[i][j] = CMPLX(realAutovalor, cmplAutovalor);
            printf("(%lf, %lf)\t", creal(mudancaBase[i][j]), cimag(mudancaBase[i][j]));
        }
        printf("\n");
    }
//Configurar o estado inicial
    for (int i = 0; i < quantidadeNiveis; i++)
        estadoInicial[i] = 0.0;
    estadoInicial[0] = 1.0f + 0.0i;
//Efetuar a evolução temporal dos estados segundo o
//cenário de Schrödinger
    for (double T = 0.0; T <= 2.0 * M_PI / Omega; T += dt)
    {
    //Cálculo na base dos autoestados
        for (int i = 0; i < quantidadeNiveis; i++)
            evolucaoTemporal[i] = cexp(-I * autovalor[i] * T);
    //Obter a probabilidade dos estados da base
    //dos estados atomicos
        for (int i = 0; i < quantidadeNiveis; i++)
        {
            //TODO: Cálculo da evolução temporal
            probabilidadeEstado[i] = 0.0;
        }
    }
}