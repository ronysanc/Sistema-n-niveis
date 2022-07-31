#include <stdio.h>
#include <math.h>
#include <complex.h>

int main(int argc, char *argv[])
{
    const int quantidadeNiveis = 2;
    double autovalor[quantidadeNiveis], probabilidadeEstado[quantidadeNiveis];
    double complex evolucaoTemporal[quantidadeNiveis];
    double complex estadoInicial[quantidadeNiveis];
    double complex estadoInicialAutoestados[quantidadeNiveis];
    double complex mudancaBase[quantidadeNiveis][quantidadeNiveis];
    double realAutovalor, cmplAutovalor;
    double const Tmax = 2.0 * M_PI / 1.0, dt = 0.5;

    FILE *eigenFile = fopen("Diagonal.dat", "r");
    FILE *resultFile = fopen("Dados.dat", "w");

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
        }
    }
//Configurar o estado inicial
    for (int i = 0; i < quantidadeNiveis; i++)
    {
        estadoInicial[i] = 0.0;
    }
    //Distúrbio no meio da rede
    estadoInicial[(int)floor((double)quantidadeNiveis / 2.0)] = 1.0f + 0.0i;
    //Obter a expansão nos autoestados
    for (int i = 0; i < quantidadeNiveis; i++)
    {
        estadoInicialAutoestados[i] = 0.0;
        for (int j = 0; j < quantidadeNiveis; j++)
            estadoInicialAutoestados[i] += mudancaBase[i][j]*estadoInicial[j];
    }
//Efetuar a evolução temporal dos estados segundo o cenário de Schrödinger
    for (double T = 0.0; T <= Tmax; T += dt)
    {
    //Cálculo na base dos autoestados
        for (int i = 0; i < quantidadeNiveis; i++)
        {
            evolucaoTemporal[i] = cexp(-I * autovalor[i] * T);
            estadoInicialAutoestados[i] *= evolucaoTemporal[i];
        }
    //Obter a probabilidade dos estados da base dos estados atomicos
        fprintf(resultFile, "%lf ", T);
        for (int i = 0; i < quantidadeNiveis; i++)
        {
            //Obter a densidade de probabilidade
            probabilidadeEstado[i] = 0.0;
            for (int j = 0; j < quantidadeNiveis; j++)
                probabilidadeEstado[i] += 
                    estadoInicialAutoestados[j]*mudancaBase[i][j];
            //Obter o módulo quadrado da densidade de probabilidade
            probabilidadeEstado[i] = 
                conj(probabilidadeEstado[i])*probabilidadeEstado[i];
            fprintf(resultFile, "%lf ", probabilidadeEstado[i]);
        }
        fprintf(resultFile, "\n");
    }
    return 0;
}