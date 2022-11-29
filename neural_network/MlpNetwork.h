//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Dense.h"
#include "Digit.h"

#define MLP_SIZE 4
#define NUM_DIGITS 10
#define DIMENSION_ERROR "Error: Invalid Matrix dimensions!"

//
const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64, 128},
                                    {20, 64},
                                    {10, 20}};
const matrix_dims bias_dims[]    = {{128, 1},
                                    {64, 1},
                                    {20, 1},
                                    {10, 1}};

class MlpNetwork
{
 public:
  /**
   * Constructs the network of layers (stores an array of weights and an
   * array of biases such that the elements at each index i correspond to
   * the i+1 level).
   * @param weights an array of weight Matrices.
   * @param biases an array of bias Matrices.
   */
  MlpNetwork (const Matrix weights[MLP_SIZE], const Matrix biases[MLP_SIZE]);

  /**
   * Applies the entire network on input.
   * @param input an input Matrix
   * @return a digit struct such that it is the result of the application of
   * the entire network on the input.
   */
  digit operator() (const Matrix &input) const;

 private:
  Matrix _weights[MLP_SIZE]; // the array of weight Matrices.
  Matrix _biases[MLP_SIZE]; // the array of bias Matrices.
};

#endif // MLPNETWORK_H
