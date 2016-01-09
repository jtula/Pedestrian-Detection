/*
 * treinaSVM.cpp
 *
 *  Created on: 5/9/2015
 *      Author: José A Tula Leyva
 */
#include "treinaSVM.h"

/*
 * Function to get the SVM vector
 */
void TrainSVM::getSVMVector(std::vector<float>& hog_vector) const {

    int sv_count = get_support_vector_count();
    const CvSVMDecisionFunc* df = decision_func;
    const double* alphas = df[0].alpha;
    double rho = df[0].rho;
    int var_count = get_var_count();
    hog_vector.resize(var_count, 0);

    for (int i = 0; i < sv_count; ++i) {

      float myalpha = alphas[i];
      const float* v = get_support_vector(i);

      for (int j = 0; j < var_count; j++,v++) {

        hog_vector[j] += (-myalpha) * (*v);
      }

    }

    hog_vector.push_back(rho);
}
