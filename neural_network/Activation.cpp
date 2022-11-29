#include "Activation.h"

/**
 * Applies the RELU function on the copy of the input Matrix (vector).
 * @param input an input Matrix
 * @return a Matrix that is the result of the application.
 */
Matrix do_relu (const Matrix &input)
{
  Matrix output = Matrix (input);
  int rows = input.get_rows ();
  int cols = input.get_cols ();
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
        {
          float element = input (i, j);
          if (element < 0)
            {output (i, j) = ((float) 0);}
        }
    }
  return output;
}

/**
 * Applies the SOFTMAX function on the copy of the input Matrix (vector).
 * @param input an input Matrix
 * @return a Matrix that is the result of the application.
 */
Matrix do_softmax (const Matrix &input)
{
  Matrix output = Matrix (input);
  int rows = input.get_rows ();
  int cols = input.get_cols ();
  float sum = 0;
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
        {
          sum += (std::exp (input (i, j)));
          output (i, j) = (std::exp (input (i, j)));
        }
    }
  sum = ((float) 1) / sum;
  return output * sum;
}

/**
 * Constructs an Activation object. Defines this instance's activation
 * according to the passed activation type.
 * @param act_type an ActivationType enum with one of two legal values:
 * RELU/SOFTMAX.
 */
Activation::Activation (const ActivationType act_type)
{
  if (act_type != RELU && act_type != SOFTMAX)
    {
      std::cout << TYPE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  _act_type = act_type;
}

/**
 * Returns this activation's type.
 * @return RELU/SOFTMAX.
 */
ActivationType Activation::get_activation_type () const {return _act_type;}

/**
 * Applies activation function on input. Does not change input.
 * @param input a Matrix to apply the function on.
 * @return a Matrix that is a result of the application.
 */
Matrix Activation::operator() (const Matrix &input) const
{
  if (_act_type == RELU)
    {return do_relu (input);}
  else
    {return do_softmax (input);}
}
