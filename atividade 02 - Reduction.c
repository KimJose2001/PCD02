#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#define N 2048
#define EX 2000
#define SRAND_VALUE 1985

int **grid;
int **newgrid;
int cont;

void MatrizVazia(){
    int i, j;
    grid = (int **)malloc(N*sizeof(int*));
    newgrid = (int **)malloc(N*sizeof(int*));
    for(i = 0; i<N; i++){
        grid[i] = (int*)malloc(N*sizeof(int));
        newgrid[i] = (int*)malloc(N*sizeof(int));
        for (j = 0; j < N; j++){
            grid[i][j] = 0;
            newgrid[i][j] = 0;
        }
    }
}

void Populacao(){
    int i,j;
    cont = 0;
    srand(SRAND_VALUE);
    for(i= 0; i<N; i++){
        for (j = 0; j < N; j++){  //Percorre o Vetor de Inteiros atual.
            grid[i][j] = rand() % 2;
            cont += grid[i][j];
        }
    }
    printf("Geracao Inicial: %d", cont);
}

void ImprimeMatriz(int **M, int m, int n){
    int i,j;
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++)
            printf(" %2d ",M[i][j]);
        printf("\n");
    }
}

void LiberarMatriz(int **Matriz){
    int i;
    for(i = 0; i < N; i++)
        free(Matriz[i]);
    free(Matriz);
}

int numVizinhos(int **Matriz, int linha, int coluna){
    int vizinhos = 0;
    int x_up, y_up;
    int x_right, y_right;
    int x_left, y_left;
    int x_down, y_down;
    int x_ul, y_ul;
    int x_ur, y_ur;
    int x_dl, y_dl;
    int x_dr, y_dr;

    if(linha == 0){
        x_up = N - 1;
        x_down = linha + 1;
        x_ul = N - 1;
        x_ur = N - 1;
        x_dl = linha + 1;
        x_dr = linha + 1;
    }
    else if(linha == N - 1){
        x_up = linha - 1;
        x_down = 0;
        x_ul = linha - 1;
        x_ur = linha - 1;
        x_dl = 0;
        x_dr = 0;
    }
    else{
        x_up = linha - 1;
        x_down = linha + 1;
        x_ul = linha - 1;
        x_ur = linha - 1;
        x_dl = linha + 1;
        x_dr = linha + 1;
    }
    y_up = coluna;
    y_down = coluna;
    if(coluna == 0){
        y_left = N - 1;
        y_right = coluna + 1;
        y_ul = N - 1;
        y_dl = N - 1;
        y_ur = coluna + 1;
        y_dr = coluna + 1;
    }
    else if(coluna == N - 1){
        y_left = coluna - 1;
        y_right = 0;
        y_ul = coluna - 1;
        y_dl = coluna - 1;
        y_ur = 0;
        y_dr = 0;
    }
    else{
        y_left = coluna - 1;
        y_right = coluna + 1;
        y_ul = coluna - 1;
        y_dl = coluna - 1;
        y_ur = coluna + 1;
        y_dr = coluna + 1;
    }
    x_left = linha;
    x_right = linha;
    vizinhos = Matriz[x_up][y_up] + Matriz[x_down][y_down] + Matriz[x_left][y_left] + Matriz[x_right][y_right]
               +Matriz[x_ul][y_ul] + Matriz[x_ur][y_ur] + Matriz[x_dl][y_dl] + Matriz[x_dr][y_dr];
    return vizinhos;
}

int novoEstado(int linha, int coluna){
    int i = linha;
    int j = coluna;
    if(grid[i][j] == 1){
        if(numVizinhos(grid, i, j) < 2)
            return 0;
        else if( numVizinhos(grid, i, j)== 2 || numVizinhos(grid, i, j) == 3)
            return 1;
        else if(numVizinhos(grid, i, j) >= 4)
            return 0;
    }
    else{
        if(numVizinhos(grid, i, j) == 3)
            return 1;
        else
            return 0;
    }
}

void novaGeracao(){
    int i, j, hcont;
    cont=0;
    #pragma omp parallel for private(i, j, hcont) reduction(+:cont)
    for(i = 0; i < N; i++){
        hcont = 0;
        for(j = 0; j < N; j++){
            if(grid[i][j] == 1)
                hcont += 1;
            newgrid[i][j] = novoEstado(i, j);
        }
        cont += hcont;
    }
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j ++)
            grid[i][j] = newgrid[i][j];
    }
}


void ImprimeVizinhos(int **Matriz){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++)
            printf("\nNumero de vizinhos de (%d, %d): %d", i, j, numVizinhos(Matriz, i, j));
    }
}

int main (int argc, char *argv[]) {
    double start, end;
    int i;
    omp_set_num_threads(4);
    MatrizVazia();
    Populacao();
    start = omp_get_wtime();
    for(i=0; i <= EX; i++){
        novaGeracao();
    }
    printf("\nUltima Geracao: %d", cont);
    end = omp_get_wtime();
    printf("\n\ntempo decorrido threads: %.2f segundos ", (end-start));
    LiberarMatriz(grid);
    LiberarMatriz(newgrid);
    return 0;
}
