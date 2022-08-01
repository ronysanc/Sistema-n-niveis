#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<gsl/gsl_eigen.h>
#include<gsl/gsl_complex_math.h>

#define quantidadeNiveis 2
const int meioDaRede = (int)(quantidadeNiveis/2);

typedef struct {
  double complex c[quantidadeNiveis];
} estadoQuantico;

double complex autovalor[quantidadeNiveis];
estadoQuantico autovetor[quantidadeNiveis];
gsl_matrix_complex *matrizHamiltoniana;

void computarMatrizHamiltoniana();
void diagonalizarMatrizHamiltoniana();
double probabilidadeDoSitio(int, estadoQuantico);

int main() {
  double tempoMaximo = 2.0, dt = 0.05;
  estadoQuantico estadoInicial, estadoAutodecomposicao;
  FILE *arquivoDeResultados = fopen("probabilidades.dat", "w");

  // configurar o estado inicial da rede
  for (int i = 0; i < quantidadeNiveis; i++)
    estadoInicial.c[i] = 0.0;
  estadoInicial.c[meioDaRede] = 1.0f + 0.0i;
  // computar a matriz do hamiltoniano do sistema nos estados atomicos
  computarMatrizHamiltoniana();
  // diagonalizar a matriz hamiltoniana
  diagonalizarMatrizHamiltoniana();
  // computar o estado inicial com base nos autoestados
  for (int i = 0; i < quantidadeNiveis; i++)
  {
    estadoAutodecomposicao.c[i] = 0.0f + 0.0i;
    for (int j = 0; j < quantidadeNiveis; j++)
      estadoAutodecomposicao.c[i] +=
        autovetor[i].c[j]*estadoInicial.c[j];
  }
  // efetuar a evolução temporal do sistema no cenário de Schrödinger
  for (double tempo = 0.0; tempo < tempoMaximo; tempo+=dt)
  {
    // obter o estado no tempo futuro
    for (int i = 0; i < quantidadeNiveis; i++)
      estadoAutodecomposicao.c[i] *= cexp(-I*autovalor[i]*dt);
    // armazenar o valor da probabilidade de estar num estado num tempo
    fprintf(arquivoDeResultados, "%g ", tempo);
    for (int i = 0; i < quantidadeNiveis; i++)
    {
      fprintf(
        arquivoDeResultados,
        "%g ",
        probabilidadeDoSitio(i, estadoAutodecomposicao)
      );
    }
    fprintf(arquivoDeResultados, "\n");
  }

  return 0;
}

void computarMatrizHamiltoniana() {
  gsl_complex energiaFundamental;
  gsl_complex hopping;

  // computar os termos de potencial e hopping
  GSL_SET_COMPLEX(&energiaFundamental, 1.0, 0.0);
  GSL_SET_COMPLEX(&hopping, 1.0, 1.0);
  // alocar a memória necessária para a execução
  matrizHamiltoniana =
    gsl_matrix_complex_alloc(quantidadeNiveis,quantidadeNiveis);
  // computar os elementos da matriz hamiltoniana
  gsl_matrix_complex_set_zero(matrizHamiltoniana);
  gsl_matrix_complex_set(matrizHamiltoniana,0,0,energiaFundamental);
  for (int i = 1; i < quantidadeNiveis; i++)
  {
    // elementos da diagonal
    gsl_matrix_complex_set(
      matrizHamiltoniana,
      i,i,
      energiaFundamental
    );
    // elementos da subdiagonal
    gsl_matrix_complex_set(
      matrizHamiltoniana,
      i,i-1,
      hopping
    );
    gsl_matrix_complex_set(
      matrizHamiltoniana,
      i-1,i,
      gsl_complex_conjugate(hopping)
    );
  }
  return;
}
void diagonalizarMatrizHamiltoniana() {
  gsl_vector *gslAutovalores = gsl_vector_alloc(quantidadeNiveis);
  gsl_matrix_complex *gslAutovetores = 
    gsl_matrix_complex_alloc(quantidadeNiveis, quantidadeNiveis);
  gsl_eigen_hermv_workspace *gslWorkspace =
    gsl_eigen_hermv_alloc(quantidadeNiveis);

  // obter autovalores e autovetores pelo método na biblioteca gsl
  gsl_eigen_hermv(
    matrizHamiltoniana,
    gslAutovalores,
    gslAutovetores,
    gslWorkspace
  );
  gsl_eigen_hermv_free(gslWorkspace);
  // obter os autovalores e autovetores do hamiltoniano
  for (int i = 0; i < quantidadeNiveis; i++)
  {
    autovalor[i] = (double complex)gsl_vector_get(gslAutovalores, i);
    for (int j = 0; j < quantidadeNiveis; j++)
      autovetor[i].c[j] =
        gsl_matrix_complex_get(gslAutovetores,i,j);
  }
  // liberar a memória alocada
  gsl_vector_free(gslAutovalores);
  gsl_matrix_complex_free(gslAutovetores);
  gsl_matrix_complex_free(matrizHamiltoniana);
  return;
}
double probabilidadeDoSitio(int i, estadoQuantico Psi) {
  double complex densidadeProbabilidade = 0.0f + 0.0i;
  for (int j = 0; j < quantidadeNiveis; j++)
    densidadeProbabilidade += Psi.c[j]*autovetor[j].c[i];
  return (double)(
    creal(densidadeProbabilidade)*creal(densidadeProbabilidade) +
    cimag(densidadeProbabilidade)*cimag(densidadeProbabilidade)
  );
}