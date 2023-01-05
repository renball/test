// Copyright 2022 Tychinin Alexey


#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_3/tychinin_a_fox/fox.h"
#include <gtest-mpi-listener.hpp>

TEST(Fox, Matrix1X1) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    size_t size = 1;
    int blocks = static_cast<int>(sqrt(num));
    if (blocks * blocks == num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (rank == 0) {
            a = prek::generateMatrix(size);
            b = prek::generateMatrix(size);
        }
        ab_fox = prek::foxMult(a, b, size);
        if (rank == 0) {
            std::vector<double> ab_seq = prek::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix5X5) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    size_t size = 5;
    int blocks = static_cast<int>(sqrt(num));
    if (blocks * blocks == num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (rank == 0) {
            a = prek::generateMatrix(size);
            b = prek::generateMatrix(size);
        }
        ab_fox = prek::foxMult(a, b, size);
        if (rank == 0) {
            std::vector<double> ab_seq = prek::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix10X10) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    size_t size = 10;
    int blocks = static_cast<int>(sqrt(num));
    if (blocks * blocks == num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (rank == 0) {
            a = prek::generateMatrix(size);
            b = prek::generateMatrix(size);
        }
        ab_fox = prek::foxMult(a, b, size);
        if (rank == 0) {
            std::vector<double> ab_seq = prek::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix20X20) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    size_t size = 20;
    int blocks = static_cast<int>(sqrt(num));
    if (blocks * blocks == num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (rank == 0) {
            a = prek::generateMatrix(size);
            b = prek::generateMatrix(size);
        }
        ab_fox = prek::foxMult(a, b, size);
        if (rank == 0) {
            std::vector<double> ab_seq = prek::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix50X50) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    size_t size = 50;
    int blocks = static_cast<int>(sqrt(num));
    if (blocks * blocks == num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (rank == 0) {
            a = prek::generateMatrix(size);
            b = prek::generateMatrix(size);
        }
        ab_fox = prek::foxMult(a, b, size);
        if (rank == 0) {
            std::vector<double> ab_seq = prek::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Fox, Matrix100X100) {
    int rank, num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    size_t size = 100;
    int blocks = static_cast<int>(sqrt(num));
    if (blocks * blocks == num) {
        std::vector<double> a, b, ab_fox;
        int add = size % blocks;
        if (add) {
            size += blocks - (size % blocks);
        }
        if (rank == 0) {
            a = prek::generateMatrix(size);
            b = prek::generateMatrix(size);
        }
        ab_fox = prek::foxMult(a, b, size);
        if (rank == 0) {
            std::vector<double> ab_seq = prek::simpleMult(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
