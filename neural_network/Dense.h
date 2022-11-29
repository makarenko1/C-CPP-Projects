//Dense.h

#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"

class Dense
{
 public:
  /**
   * Constructs a new layer with given parameters.
   * @param w the Matrix of weights of the current layer.
   * @param bias the Matrix of bias of the current layer.
   * @param act_type the activation type of the current layer.
   */
  Dense (const Matrix &w, const Matrix &bias, ActivationType act_type);

  /**
   * Returns the weights of this layer. Forbids modification.
   * @return the Matrix of weights.
   */
  Matrix get_weights () const;

  /**
   * Returns the bias of the current layer. Forbids modification.
   * @return the Matrix of bias.
   */
  Matrix get_bias () const;

  /**
   * Returns the Activation of the current layer. Forbids modification.
   * @return the Activation object.
   */
  Activation get_activation () const;

  /**
   * Applies the layer on input and returns the output Matrix. Does not
   * change input.
   * @param input a Matrix of input (the result of the previous layer).
   * @return the result of act_func (w * input + bias).
   */
  Matrix operator() (const Matrix &input) const;

 private:
  const Matrix _weights; // the Matrix of weights of the current layer.
  const Matrix _bias; // the Matrix of bias of the current layer.
  const Activation _activation; // the Activation object of the current layer.
};

#endif //C___PROJECT_DENSE_H
