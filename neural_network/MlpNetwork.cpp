#include "MlpNetwork.h"

/**
 * Constructs the network of layers (stores an array of weights and an
 * array of biases such that the elements at each index i correspond to
 * the i+1 level).
 * @param weights an array of weight Matrices.
 * @param biases an array of bias Matrices.
 */
MlpNetwork::MlpNetwork (const Matrix weights[MLP_SIZE], const Matrix
biases[MLP_SIZE])
{
  for (int i = 0; i < MLP_SIZE; i++)
    {
      _weights[i] = Matrix (weights[i]);
      _biases[i] = Matrix (biases[i]);
    }
}

/**
 * Prints the given error message to the error stream and exits the program
 * with code 1 (EXIT_FAILURE).
 * @param str an error message.
 */
void treat_error_mlp (const std::string &str)
{
  std::cerr << str << std::endl;
  std::exit (EXIT_FAILURE);
}

/**
 * Creates and returns the struct of the digit (index in the result) with
 * the best probability (value at that index in the result).
 * @param result the result Matrix of the application of the entire network
 * on the input Matrix.
 * @return a digit struct such that it is the result of the application of
 * the entire network on the input.
 */
digit get_digit (const Matrix &result)
{
  unsigned int best_value = 0;
  float best_probability = result[0];
  for (int i = 1; i < NUM_DIGITS; i++)
    {
      if (result[i] > best_probability)
        {
          best_value = i;
          best_probability = result[i];
        }
    }
  digit result_digit;
  result_digit.value = best_value;
  result_digit.probability = best_probability;
  return result_digit;
}

/**
 * Applies the entire network on input.
 * @param input an input Matrix
 * @return a digit struct such that it is the result of the application of
 * the entire network on the input.
 */
digit MlpNetwork::operator() (const Matrix &input) const
{
  Matrix result = input;
  ActivationType act_type = RELU;
  for (int i = 0; i < MLP_SIZE; i++)
    {
      if (i == MLP_SIZE - 1)
        {act_type = SOFTMAX;}
      Dense level = Dense (_weights[i], _biases[i], act_type);
      if (result.get_cols () != 1) // check if the result is a vector.
        {treat_error_mlp (DIMENSION_ERROR);}
      result = level (result);
    }
  if (result.get_rows () != NUM_DIGITS || result.get_cols () != 1)
    {treat_error_mlp (DIMENSION_ERROR);}
  return get_digit (result);
}
