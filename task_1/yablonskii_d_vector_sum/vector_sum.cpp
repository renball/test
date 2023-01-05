// Copyright 2023 Yablonskiy Dmitriy
#include <mpi.h>
#include <math.h>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/yablonskii_d_vector_sum/vector_sum.h"

std::vector<int> rand_vec(int s) {

    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> v(s);

    for (int i = 0; i < s; i++) {v[i] = (gen() % 10)+3;}
    return v;
}

int s_stan(std::vector<int> v) {
    int s = 0;

    for (int i = 0; i < v.size(); i++) {s = s + v[i];}

    return s;
}

int s_par(std::vector<int> vec, int s) {
    int rank, col_prc;
    MPI_Comm_size(MPI_COMM_WORLD, &col_prc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int d = s / col_prc;
    if (d == 0) {d = 1;}

    int t;

    if (col_prc < s) {t = col_prc;}
    else {t = s;}

    std::vector<int> n_v(d);

    if (rank == 0) {
        n_v = std::vector<int>(vec.begin(), vec.begin() + d);
        for (size_t i = 1; i < t; i++) {
            MPI_Send(vec.data() + i * d, d, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else if (rank < s) {
        MPI_Status status;
        MPI_Recv(n_v.data(), d, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        if (s % t > 0) {
            size_t j = 0;
            n_v.resize(n_v.size() + s % col_prc);
            
            for (int i = (s - s % col_prc); i < s; i++) {
                n_v[j + d] = vec[i];
                j++;
            }

        }
    }

    int fullsum=0,cursum=0;
    
    if (rank < s) {cursum = sum(n_v);}

    MPI_Reduce(&cursum, &fullsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    return fullsum;
}
