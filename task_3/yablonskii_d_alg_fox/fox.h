// Copyright 2022 Tychinin Alexey


#ifndef MODULES_TASK_3_TYCHININ_A_FOX_FOX_H_
#define MODULES_TASK_3_TYCHININ_A_FOX_FOX_H_

#include <vector>

namespace task3 {
    std::vector<double> Make_mat(int n);

    std::vector<double> s_m(const std::vector<double>& a, const std::vector<double>& b, int n);

    std::vector<double> Fox_m(std::vector<double> a, std::vector<double> b, int data);
}  // namespace task3


#endif  //  MODULES_TASK_3_TYCHININ_A_FOX_FOX_H_
