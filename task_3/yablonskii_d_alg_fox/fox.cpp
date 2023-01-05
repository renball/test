// Copyright 2022 Tychinin Alexey


#include "../../../modules/task_3/tychinin_a_fox/fox.h"
#include <mpi.h>

#include <random>

namespace {
    MPI_Comm GRID_COMM;
    MPI_Comm COL_COMM;
    MPI_Comm ROW_COMM;

    void createGridCommunicators(int gridSize, int procRank, int* gridCoords) {
        std::vector<int> dim_size(2, gridSize);
        std::vector<int> periodic(2, 0);
        std::vector<int> subdims(2);
        MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size.data(), periodic.data(), 0, &GRID_COMM);
        MPI_Cart_coords(GRID_COMM, procRank, 2, gridCoords);
        subdims[0] = 0;
        subdims[1] = 1;
        MPI_Cart_sub(GRID_COMM, subdims.data(), &ROW_COMM);
        subdims[0] = 1;
        subdims[1] = 0;
        MPI_Cart_sub(GRID_COMM, subdims.data(), &COL_COMM);
    }

    void blockMult(double* aBlock, double* bBlock, double* abBlock, size_t blockSize) {
        for (size_t i = 0; i < blockSize; i++) {
            for (size_t j = 0; j < blockSize; j++) {
                double tmp = 0;
                for (int k = 0; k < blockSize; k++) {
                    tmp += aBlock[i * blockSize + k] * bBlock[k * blockSize + j];
                }
                abBlock[i * blockSize + j] += tmp;
            }
        }
    }

}  

namespace prek {
    std::vector<double> Make_mat(int n) {
        std::mt19937 gen(std::random_device {}());
        std::uniform_real_distribution<> distr(-100, 100);
        std::vector<double> m(n * n);

        for (int i = 0; i < n * n; i++) m[i] = distr(gen);

        return m;
    }

    std::vector<double> s_m(const std::vector<double>& a, const std::vector<double>& b, int n) {
        std::vector<double> ab(n * n, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    ab[i * n + j] += a[i * n + k] * b[k * n + j];
                }
            }
        }

        return ab;
    }

    std::vector<double> Fox_m(std::vector<double> a, std::vector<double> b, int data) {
        int num, rank;
        MPI_Comm_size(MPI_COMM_WORLD, &num);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        MPI_Status status;
        int grid_size = static_cast<int>(sqrt(num));
        if (grid_size * grid_size != num) {
            throw "Wrong number of processes";
        }
        std::vector<int> grid_coords(2);
        createGridCommunicators(grid_size, rank, grid_coords.data());
        int block_size;
        if (rank == 0) {
            block_size = static_cast<int>(size) / grid_size;
        }
        MPI_Bcast(&block_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<double> block_a(block_size * block_size);
        std::vector<double> block_b(block_size * block_size);
        std::vector<double> block_ab(block_size * block_size, 0);
        if (rank == 0) {
            for (int i = 0; i < block_size; i++) {
                for (int j = 0; j < block_size; j++) {
                    block_a[i * block_size + j] = a[i * size + j];
                    block_b[i * block_size + j] = b[i * size + j];
                }
            }
        }
        MPI_Datatype block_mat;
        MPI_Type_vector(block_size, block_size, block_size * grid_size, MPI_DOUBLE, &block_mat);
        MPI_Type_commit(&block_mat);
        if (rank == 0) {
            for (int p = 1; p < num; p++) {
                MPI_Send(a.data() + (p % grid_size) * block_size + (p / grid_size) * size * block_size,
                    1, block_mat, p, 0, GRID_COMM);
                MPI_Send(b.data() + (p % grid_size) * block_size + (p / grid_size) * size * block_size,
                    1, block_mat, p, 1, GRID_COMM);
            }
        } else {
            MPI_Recv(block_a.data(), block_size * block_size, MPI_DOUBLE,
                     0, 0, GRID_COMM, &status);
            MPI_Recv(block_b.data(), block_size * block_size, MPI_DOUBLE,
                     0, 1, GRID_COMM, &status);
        }
        for (int i = 0; i < grid_size; i++) {
            std::vector<double> tmpblockA(block_size * block_size);
            int pivot = (grid_coords[0] + i) % grid_size;
            if (grid_coords[1] == pivot) {
                tmpblockA = block_a;
            }
            MPI_Bcast(tmpblockA.data(), block_size * block_size,
                      MPI_DOUBLE, pivot, ROW_COMM);
            blockMult(tmpblockA.data(), block_b.data(), block_ab.data(), block_size);
            int nextPr = grid_coords[0] + 1;
            if (grid_coords[0] == grid_size - 1)
                nextPr = 0;
            int prevPr = grid_coords[0] - 1;
            if (grid_coords[0] == 0)
                prevPr = grid_size - 1;
            MPI_Sendrecv_replace(block_b.data(), block_size * block_size, MPI_DOUBLE,
                                 prevPr, 0, nextPr, 0, COL_COMM, &status);
        }
        std::vector<double> resultM(size * size);
        if (rank == 0) {
            for (int i = 0; i < block_size; i++) {
                for (int j = 0; j < block_size; j++)
                    resultM[i * size + j] = block_ab[i * block_size + j];
            }
        }
        if (rank != 0) {
            MPI_Send(block_ab.data(), block_size * block_size, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        } else {
            for (int p = 1; p < num; p++) {
                MPI_Recv(resultM.data() + (p % grid_size) * block_size + (p / grid_size) * size * block_size,
                    block_size * block_size, block_mat, p, 3, MPI_COMM_WORLD, &status);
            }
        }
        MPI_Type_free(&block_mat);
        return resultM;
    }
} 

