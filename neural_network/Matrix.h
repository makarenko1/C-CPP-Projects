// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <cmath>
#define DEFAULT_SIZE 1
#define SQUARE 2
#define SIZEOF_FLOAT 4
#define ERROR_TELLG (-1)
#define MIN_TO_PRINT 0.1
#define DOUBLE_SPACE "  "
#define DOUBLE_ASTERISK "**"
#define ALLOCATION_ERROR "Error: Allocation error!"
#define SIZE_ERROR "Error: Sizes error!"
#define STREAM_ERROR "Error: Stream error!"

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

class Matrix
{
 public:
  /**
   * The constructor for the Matrix objects. Constructs a Matrix rows * cols.
   * Initializes all the elements to 0.
   * @param rows a number of rows in the created Matrix.
   * @param cols a number of columns in the created Matrix.
   */
  Matrix (int rows, int cols);

  /**
   * The default constructor for the Matrix objects. Constructs a
   * Matrix 1 * 1. Initializes the single element in it to 0.
   */
  Matrix ();

  /**
   * The copy constructor for the Matrix objects. Constructs a Matrix from
   * another matrix.
   * @param other a reference to another matrix to construct from.
   */
  Matrix (const Matrix &other);

  /**
   * The destructor for the Matrix objects. Frees all the allocated resources.
   */
  ~Matrix ();

  /**
   * Returns the amount of rows as int.
   * @return the number of rows.
   */
  int get_rows () const;

  /**
   * Returns the amount of columns as int.
   * @return the number of columns.
   */
  int get_cols () const;

  /**
   * Transforms a matrix into its transpose matrix. Supports concatenation.
   * @return a reference to the current object that was changed.
   */
  Matrix &transpose ();

  /**
   * Transforms a matrix into a column vector. Supports concatenation.
   * @return a reference to the current object that was changed.
   */
  Matrix &vectorize ();

  /**
   * Prints matrix elements, no return value. Prints space after each
   * element (including last element in row). Prints newline after each row
   * (including last row).
   */
  void plain_print () const;

  /**
   * Returns a matrix which is the dot product of this matrix with another
   * matrix.
   * @param other another matrix to make the dot product with.
   * @return a matrix that is the dot product.
   */
  Matrix dot (const Matrix &other) const;

  /**
   * Returns the Frobenius norm of the given matrix.
   * @return the norm.
   */
  float norm () const;

  /**
   * Fills the matrix elements.
   * @param is input stream to fill from.
   * @param other the matrix to fill.
   */
  friend void read_binary_file (std::istream &is, Matrix &other);

  /**
   * Matrix addition.
   * @param lhs a Matrix on the left side.
   * @param rhs a Matrix on the right side.
   * @return a new Matrix that is a sum of lhs and rhs.
   */
  friend Matrix operator+ (const Matrix &lhs, const Matrix &rhs);

  /**
   * Matrix assignment.
   * @param other a Matrix to assign to this Matrix.
   * @return a reference to the current object that was changed.
   */
  Matrix &operator= (const Matrix &other);

  /**
   * Matrix multiplication.
   * @param lhs a Matrix on the left side.
   * @param rhs a Matrix on the right side.
   * @return a new Matrix that is a product of lhs and rhs.
   */
  friend Matrix operator* (const Matrix &lhs, const Matrix &rhs);

  /**
   * Scalar multiplication on the right.
   * @param other a Matrix to multiply.
   * @param c a scalar to multiply with.
   * @return a new Matrix that is a product of other and c (on the right).
   */
  friend Matrix operator* (const Matrix &other, float c);

  /**
   * Scalar multiplication on the left.
   * @param c a scalar to multiply with.
   * @param other a Matrix to multiply.
   * @return a new Matrix that is a product of other and c (on the left).
   */
  friend Matrix operator* (float c, const Matrix &other);

  /**
   * Matrix addition accumulation.
   * @param other a Matrix to add to this Matrix.
   * @return a reference to the current object that was changed.
   */
  Matrix &operator+= (const Matrix &other);

  /**
   * Parenthesis indexing (non-const).
   * @param i a row index.
   * @param j a column index.
   * @return a reference to the element at the ith row and jth column in
   * this Matrix.
   */
  float &operator() (int i, int j);

  /**
   * Parenthesis indexing (const).
   * @param i a row index.
   * @param j a column index.
   * @return a value of the element at the ith row and jth column in this
   * Matrix.
   */
  float operator() (int i, int j) const;

  /**
   * Brackets indexing (non-const).
   * @param i an element index.
   * @return a reference to the ith element in this Matrix.
   */
  float &operator[] (int i);

  /**
   * Brackets indexing (const).
   * @param i an element index.
   * @return a value of the ith element in this Matrix.
   */
  float operator[] (int i) const;

  /**
   * Output stream.
   * @param os an output stream.
   * @param other a Matrix to export.
   * @return an export of the other Matrix to the os stream.
   */
  friend std::ostream& operator<< (std::ostream& os, const Matrix &other);

 private:
  int _rows; // a number of rows in the Matrix.
  int _cols; // a number of columns in the Matrix.
  float *_matrix; // a dynamically allocated array of the Matrix values (1D).
};

#endif //MATRIX_H
