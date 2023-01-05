// Copyright 2023 Yablonskiy Dmitriy
#include <gtest/gtest.h>
#include "./vector_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Par_Vec, TS_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vecmain;
    const int vec_size = 5;
    if (rank == 0) {
        vecmain = { 0, 1, 2, 3, 4};
    }

    int fullsum;
    fullsum = s_par(vecmain, vec_size);

    if (rank == 0) {
        int truesum;
        truesum = 10;
        ASSERT_EQ(truesum, fullsum);
    }
}

TEST(Par_Vec, TS_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vecmain;
    const int vec_size = 10;

    if (rank == 0) {
        vecmain = random(vec_size);
    }

    int fullsum;
    fullsum = s_par(vecmain, vec_size);

    if (rank == 0) {
        int truesum;
        truesum = sum(vecmain);
        ASSERT_EQ(truesum, fullsum);
    }
}

TEST(Par_Vec, TS_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vecmain;
    const int vec_size = 200;

    if (rank == 0) {
        vecmain = random(vec_size);
    }

    int fullsum;
    fullsum = s_par(vecmain, vec_size);

    if (rank == 0) {
        int truesum;
        truesum = sum(vecmain);
        ASSERT_EQ(truesum, fullsum);
    }
}

TEST(Par_Vec, TS_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vecmain;
    const int vec_size = 3000;

    if (rank == 0) {
        vecmain = random(vec_size);
    }

    int fullsum;
    fullsum = s_par(vecmain, vec_size);

    if (rank == 0) {
        int truesum;
        truesum = sum(vecmain);
        ASSERT_EQ(truesum, fullsum);
    }
}

TEST(Par_Vec, TS_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vecmain;
    const int vec_size = 40000;

    if (rank == 0) {
        vecmain = random(vec_size);
    }

    int fullsum;
    fullsum = s_par(vecmain, vec_size);

    if (rank == 0) {
        int truesum;
        truesum = sum(vecmain);
        ASSERT_EQ(truesum, fullsum);
    }
}

TEST(Par_Vec, TS_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vecmain;
    const int vec_size = 500000;

    if (rank == 0) {
        vecmain = random(vec_size);
    }

    int fullsum;
    fullsum = s_par(vecmain, vec_size);
    
    if (rank == 0) {
        int truesum;
        truesum = sum(vecmain);
        ASSERT_EQ(truesum, fullsum);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
