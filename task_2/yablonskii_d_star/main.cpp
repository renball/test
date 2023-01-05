// Copyright Goncharova Anna

#include <gtest/gtest.h>
#include "../../../modules/task_2/goncharova_a_star/star.h"
#include <gtest-mpi-listener.hpp>

TEST(Test_Star1, make_star_top) {
    ASSERT_NO_THROW(Making_star(MPI_COMM_WORLD));
}

TEST(Test_Star2, n_t_star_top) {
    ASSERT_NO_THROW(Is_this_a_star_mmm(MPI_COMM_WORLD));
}

TEST(Test_Star3, c_r_Star_top) {
    int rank, zveno;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &zveno);

    int* idzv = new int[zveno];
    int* rebro = new int[2 * zveno];
    for (int i = 0; i < zveno; i++) {
        idzv[i] = 2 + i * 2;
        rebro[i * 2] = (zveno - 1 + i) % zveno;
        rebro[(i * 2) + 1] = (zveno + 1 + i) % zveno;
    }
    MPI_Comm comm;
    MPI_Graph_create(MPI_COMM_WORLD, zveno, idzv, rebro, 0, &comm);
    if (rank == 0 && zveno != 1) EXPECT_FALSE(Is_this_a_star_mmm(comm));
}

TEST(Test_Star4, IsThisStar) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm star = Making_star(MPI_COMM_WORLD);
    if (rank == 0) EXPECT_TRUE(Is_this_a_star_mmm(star));
}

TEST(Test_Star5, IsThis_NOT_Star) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) EXPECT_FALSE(Is_this_a_star_mmm(MPI_COMM_WORLD));
}

int main(int argc, char **argv) {
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
