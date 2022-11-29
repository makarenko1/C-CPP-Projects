#include "Dense.h"

/**
 * Constructs a new layer with given parameters.
 * @param w the Matrix of weights of the current layer.
 * @param bias the Matrix of bias of the current layer.
 * @param act_type the activation type of the current layer.
 */
Dense::Dense (const Matrix &w, const Matrix &bias, const ActivationType
act_type) : _weights (Matrix (w)), _bias (Matrix (bias)), _activation
(Activation (act_type)) {}

/**
 * Returns the weights of this layer. Forbids modification.
 * @return the Matrix of weights.
 */
Matrix Dense::get_weights () const {return _weights;}

/**
 * Returns the bias of the current layer. Forbids modification.
 * @return the Matrix of bias.
 */
Matrix Dense::get_bias () const {return _bias;}

/**
 * Returns the Activation of the current layer. Forbids modification.
 * @return the Activation object.
 */
Activation Dense::get_activation () const {return _activation;}

/**
 * Applies the layer on input and returns the output Matrix. Does not
 * change input.
 * @param input a Matrix of input (the result of the previous layer).
 * @return the result of act_func (w * input + bias).
 */
Matrix Dense::operator() (const Matrix &input) const
{
  return _activation (((_weights * input) + _bias));
}
