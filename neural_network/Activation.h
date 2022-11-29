//Activation.h

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#define TYPE_ERROR "Error: Invalid type!"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    RELU,
    SOFTMAX
};

class Activation
{
 public:
  /**
   * Constructs an Activation object. Defines this instance's activation
   * according to the passed activation type.
   * @param act_type an ActivationType enum with one of two legal values:
   * RELU/SOFTMAX.
   */
  explicit Activation (ActivationType act_type);

  /**
   * Returns this activation's type.
   * @return RELU/SOFTMAX.
   */
  ActivationType get_activation_type () const;

  /**
   * Applies activation function on input. Does not change input.
   * @param input a Matrix to apply the function on.
   * @return a Matrix that is a result of the application.
   */
  Matrix operator() (const Matrix &input) const;

 private:
  ActivationType _act_type; // one of two legal values: RELU/SOFTMAX.
};

#endif //ACTIVATION_H
