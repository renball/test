// Copyright Anna Goncharova

#include "../../../modules/task_2/goncharova_a_star/star.h"

MPI_Comm Making_star(const MPI_Comm com) {

    int zveno;
    MPI_Comm_size(com, &zveno);

    int *idzv = new int[zveno];
    idzv[0] = zveno - 1;

    for (int i = 1; i < zveno; i++) {idzv[i] = idzv[0] + i;}

    int r_size = 2 * (zveno - 1);
    int *rebro = new int[r_size];

    for (int i = 0; i < zveno - 1; i++) {rebro[i] = i + 1;}
    
    for (int i = 0; i < r_size; i++) {rebro[i] = 0;}

    int t;
    t = 0;

    MPI_Comm star;
    MPI_Graph_create(com, zveno, idzv, rebro, t, &star);

    return star;
}

bool Is_this_a_star_mmm(const MPI_Comm check_com) {
    int status;
    MPI_Topo_test(check_com, &status);
    if (status != MPI_GRAPH) return false;

    int zveno;
    int n_r;
    MPI_Graphdims_get(check_com, &zveno, &n_r);

    int r_size = 2 * (zveno - 1);
    if (n_r != r_size) return false;

    int *idzv = new int[zveno];
    int *rebro = new int[r_size];
    MPI_Graph_get(check_com, zveno, r_size, idzv, rebro);


    for (int i = 0; i < zveno; i++) {if (idzv[i] != i + zveno - 1) return false;}
    for (int i = 0; i < zveno - 1; i++) {if (rebro[i] != i + 1) return false;}
    for (int i = 0; i < r_size; i++) {if (rebro[i] != 0) return false;}

    return true;
}
