#include<stdio.h>
#include<stdlib.h>
#include<gsl/gsl_eigen.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen("Diagonal.dat", "w");

    const int n = 2; // Matrix range

    gsl_complex g0, g1, e, zero;
    gsl_matrix_complex *Hamiltonian = gsl_matrix_complex_alloc(n,n);

    gsl_vector *AutoValores = gsl_vector_alloc(n);                    // EigenValues list
    gsl_matrix_complex *AutoVetores = gsl_matrix_complex_alloc(n, n); // EigenVectors list

    gsl_eigen_hermv_workspace *W = gsl_eigen_hermv_alloc(n);

    // Hamiltonian configuration

    GSL_SET_COMPLEX(&e, 0, 0);     // Fundamental energy
    GSL_SET_COMPLEX(&g0, 0, -1);   // Hopping energy
    GSL_SET_COMPLEX(&g1, 0, 1);
    GSL_SET_COMPLEX(&zero,0,0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i==j)
                gsl_matrix_complex_set(Hamiltonian,i,j,e);
            else if (i==j+1)
                gsl_matrix_complex_set(Hamiltonian,i,j,g1);
            else if (i==j-1)
                gsl_matrix_complex_set(Hamiltonian,i,j,g0);
            else
                gsl_matrix_complex_set(Hamiltonian,i,j,zero);
        }
    }

    gsl_eigen_hermv(Hamiltonian, AutoValores, AutoVetores, W);
    gsl_eigen_hermv_free(W);

    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("Autovalor = %g,\n", gsl_vector_get(AutoValores, i));
        printf("Autovetor = (");
        for (int j = 0; j < n - 1; j++)
            printf("(%g, %g), ",gsl_matrix_complex_get(AutoVetores,i,j).dat[0],gsl_matrix_complex_get(AutoVetores,i,j).dat[1]);
        printf("(%g, %g))\n\n",gsl_matrix_complex_get(AutoVetores,i,n-1).dat[0],gsl_matrix_complex_get(AutoVetores,i,n-1).dat[1]);
    }

    for (int i = 0; i < n; i++)
        fprintf(file, "%g\t", gsl_vector_get(AutoValores, i));
    fprintf(file, "\n");
    

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
            fprintf(file, "(%g, %g)\t",gsl_matrix_complex_get(AutoVetores,i,j).dat[0],gsl_matrix_complex_get(AutoVetores,i,j).dat[1]);
        fprintf(file, "(%g, %g)\n",gsl_matrix_complex_get(AutoVetores,i,n-1).dat[0],gsl_matrix_complex_get(AutoVetores,i,n-1).dat[1]);
    }

    fclose(file);

    return 0;
}