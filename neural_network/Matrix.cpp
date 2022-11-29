#include "Matrix.h"

/**
 * Prints the given error message to the error stream and exits the program
 * with code 1 (EXIT_FAILURE).
 * @param str an error message.
 */
void treat_error_matrix (const std::string &str)
{
  std::cerr << str << std::endl;
  std::exit (EXIT_FAILURE);
}

/**
 * The constructor for the Matrix objects. Constructs a Matrix rows * cols.
 * Initializes all the elements to 0.
 * @param rows a number of rows in the created Matrix.
 * @param cols a number of columns in the created Matrix.
 */
Matrix::Matrix (const int rows, const int cols)
{
  if (rows < DEFAULT_SIZE || cols < DEFAULT_SIZE)
    {treat_error_matrix (SIZE_ERROR);}
  _rows = rows;
  _cols = cols;
  _matrix = new (std::nothrow) float [_rows * _cols] ();
  if (_matrix == nullptr)
    {treat_error_matrix (ALLOCATION_ERROR);}
}

/**
 * The default constructor for the Matrix objects. Constructs a
 * Matrix 1 * 1. Initializes the single element in it to 0.
 */
Matrix::Matrix () : _rows (DEFAULT_SIZE), _cols (DEFAULT_SIZE)
{
  _matrix = new (std::nothrow) float [_rows * _cols] ();
  if (_matrix == nullptr)
    {treat_error_matrix (ALLOCATION_ERROR);}
}

/**
 * The copy constructor for the Matrix objects. Constructs a Matrix from
 * another matrix.
 * @param other a reference to another matrix to construct from.
 */
Matrix::Matrix (const Matrix &other) : _rows (other._rows), _cols (other._cols)
{
  _matrix = new (std::nothrow) float [_rows * _cols];
  if (_matrix == nullptr)
    {treat_error_matrix (ALLOCATION_ERROR);}
  for (int i = 0; i < _rows; i++)
    {
      for (int j = 0; j < _cols; j++)
        {
          _matrix[i * _cols + j] = other._matrix[i * _cols + j];
        }
    }
}

/**
 * The destructor for the Matrix objects. Frees all the allocated resources.
 */
Matrix::~Matrix () {delete[] _matrix;}

/**
 * Returns the amount of rows as int.
 * @return the number of rows.
 */
int Matrix::get_rows () const  {return _rows;}

/**
 * Returns the amount of columns as int.
 * @return the number of columns.
 */
int Matrix::get_cols () const  {return _cols;}

/**
 * Transforms a matrix into its transpose matrix. Supports concatenation.
 * @return a reference to the current object that was changed.
 */
Matrix & Matrix::transpose ()
{
  auto *new_matrix = new (std::nothrow) float [_rows * _cols];
  if (new_matrix == nullptr)
    {treat_error_matrix (ALLOCATION_ERROR);}
  for (int i = 0; i < _rows; i++)
    {
      for (int j = 0; j < _cols; j++)
        {
          new_matrix[j * _rows + i] = _matrix[i * _cols + j];
        }
    }
  delete[] _matrix;
  _matrix = new_matrix;
  int temp = _rows;
  _rows = _cols;
  _cols = temp;
  return *this;
}

/**
 * Transforms a matrix into a column vector. Supports concatenation.
 * @return a reference to the current object that was changed.
 */
Matrix & Matrix::vectorize ()
{
  _rows *= _cols;
  _cols = DEFAULT_SIZE;
  return *this;
}

/**
 * Prints matrix elements, no return value. Prints space after each
 * element (including last element in row). Prints newline after each row
 * (including last row).
 */
void Matrix::plain_print () const
{
  for (int i = 0; i < _rows; i++)
    {
      for (int j = 0; j < _cols; j++)
        {
          std::cout << _matrix[j * _rows + i] << " ";
        }
      std::cout << std::endl;
    }
}

/**
 * Returns a matrix which is the dot product of this matrix with another
 * matrix.
 * @param other another matrix to make the dot product with.
 * @return a matrix that is the dot product.
 */
Matrix Matrix::dot (const Matrix &other) const
{
  if (_rows != other._rows || _cols != other._cols)
    {treat_error_matrix (SIZE_ERROR);}
  Matrix new_matrix = Matrix (_rows, _cols);
  for (int i = 0; i < new_matrix._rows; i++)
    {
      for (int j = 0; j < new_matrix._cols; j++)
        {
          new_matrix._matrix[i * new_matrix._cols + j] =
              _matrix[i * _cols + j] * other._matrix[i * other._cols + j];
        }
    }
  return new_matrix;
}

/**
 * Returns the Frobenius norm of the given matrix.
 * @return the norm.
 */
float Matrix::norm () const
{
  float sum = 0;
  for (int i = 0; i < _rows; i++)
    {
      for (int j = 0; j < _cols; j++)
        {sum += (float) pow (_matrix[i * _cols + j], SQUARE);}
    }
  return ((float) sqrt ((double) sum));
}

/**
 * Fills the matrix elements.
 * @param is input stream to fill from.
 * @param other the matrix to fill.
 */
void read_binary_file (std::istream &is, Matrix &other)
{
  if (!is)
    {treat_error_matrix (STREAM_ERROR);}
  is.seekg(0, std::istream::end); // go to end of file
  int n_bytes = is.tellg (); // tell how many bytes in the file.
  int num_expected = other._rows * other._cols * SIZEOF_FLOAT;
  if ((n_bytes == ERROR_TELLG) || n_bytes < num_expected)
    {treat_error_matrix (STREAM_ERROR);}
  is.seekg(0, std::istream::beg); // go to beginning of file
  is.read ((char *) other._matrix, num_expected);
  if (!is.good ())
    {treat_error_matrix (STREAM_ERROR);}
}

/**
 * Matrix addition.
 * @param lhs a Matrix on the left side.
 * @param rhs a Matrix on the right side.
 * @return a new Matrix that is a sum of lhs and rhs.
 */
Matrix operator+ (const Matrix &lhs, const Matrix &rhs)
{
  if (lhs._rows != rhs._rows || lhs._cols != rhs._cols)
    {treat_error_matrix (SIZE_ERROR);}
  Matrix new_matrix = Matrix (lhs._rows, lhs._cols);
  for (int i = 0; i < new_matrix._rows; i++)
    {
      for (int j = 0; j < new_matrix._cols; j++)
        {
          new_matrix._matrix[i * new_matrix._cols + j] =
              lhs._matrix[i * lhs._cols + j] + rhs._matrix[i * rhs._cols + j];
        }
    }
  return new_matrix;
}

/**
 * Matrix assignment.
 * @param other a Matrix to assign to this Matrix.
 * @return a reference to the current object that was changed.
 */
Matrix & Matrix::operator= (const Matrix &other)
{
  if (this != &other)
    {
      delete[] _matrix;
      _rows = other._rows;
      _cols = other._cols;
      _matrix = new (std::nothrow) float [_rows * _cols];
      if (_matrix == nullptr)
        {treat_error_matrix (ALLOCATION_ERROR);}
      for (int i = 0; i < _rows; i++)
        {
          for (int j = 0; j < _cols; j++)
            {
              _matrix[i * _cols + j] = other._matrix[i * _cols + j];
            }
        }
    }
  return *this;
}

/**
 * Matrix multiplication.
 * @param lhs a Matrix on the left side.
 * @param rhs a Matrix on the right side.
 * @return a new Matrix that is a product of lhs and rhs.
 */
Matrix operator* (const Matrix &lhs, const Matrix &rhs)
{
  if (lhs._cols != rhs._rows)
    {treat_error_matrix (SIZE_ERROR);}
  Matrix new_matrix = Matrix (lhs._rows, rhs._cols);
  for (int i = 0; i < lhs._rows; i++)
    {
      for (int j = 0; j < rhs._cols; j++)
        {
          for (int k = 0; k < lhs._cols; k++)
            {
              new_matrix._matrix[i * new_matrix._cols + j] +=
                  lhs._matrix[i * lhs._cols + k] * rhs._matrix[k * rhs._cols
                  + j];
            }
        }
    }
  return new_matrix;
}

/**
 * Scalar multiplication on the right.
 * @param other a Matrix to multiply.
 * @param c a scalar to multiply with.
 * @return a new Matrix that is a product of other and c (on the right).
 */
Matrix operator* (const Matrix &other, const float c)
{
  Matrix new_matrix = Matrix (other._rows, other._cols);
  for (int i = 0; i < new_matrix._rows; i++)
    {
      for (int j = 0; j < new_matrix._cols; j++)
        {
          new_matrix._matrix[i * new_matrix._cols + j] =
              other._matrix[i * other._cols + j] * c;
        }
    }
  return new_matrix;
}

/**
 * Scalar multiplication on the left.
 * @param c a scalar to multiply with.
 * @param other a Matrix to multiply.
 * @return a new Matrix that is a product of other and c (on the left).
 */
Matrix operator* (const float c, const Matrix &other)
{
  return other * c;
}

/**
 * Matrix addition accumulation.
 * @param other a Matrix to add to this Matrix.
 * @return a reference to the current object that was changed.
 */
Matrix & Matrix::operator+= (const Matrix &other)
{
  if (_rows != other._rows || _cols != other._cols)
    {treat_error_matrix (SIZE_ERROR);}
  for (int i = 0; i < _rows; i++)
    {
      for (int j = 0; j < _cols; j++)
        {
          _matrix[i * _cols + j] += other._matrix[i * other._cols + j];
        }
    }
  return *this;
}

/**
 * Parenthesis indexing (non-const).
 * @param i a row index.
 * @param j a column index.
 * @return a reference to the element at the ith row and jth column in
 * this Matrix.
 */
float & Matrix::operator() (const int i, const int j)
{
  if (i < 0 || j < 0 || i >= _rows || j >= _cols)
    {treat_error_matrix (SIZE_ERROR);}
  return _matrix[i * _cols + j];
}

/**
 * Parenthesis indexing (const).
 * @param i a row index.
 * @param j a column index.
 * @return a value of the element at the ith row and jth column in this
 * Matrix.
 */
float Matrix::operator() (const int i, const int j) const
{
  if (i < 0 || j < 0 || i >= _rows || j >= _cols)
    {treat_error_matrix (SIZE_ERROR);}
  return _matrix[i * _cols + j];
}

/**
 * Brackets indexing (non-const).
 * @param i an element index.
 * @return a reference to the ith element in this Matrix.
 */
float & Matrix::operator[] (const int i)
{
  if (i < 0 || i >= _rows * _cols)
    {treat_error_matrix (SIZE_ERROR);}
  return _matrix[i];
}

/**
 * Brackets indexing (const).
 * @param i an element index.
 * @return a value of the ith element in this Matrix.
 */
float Matrix::operator[] (const int i) const
{
  if (i < 0 || i >= _rows * _cols)
    {treat_error_matrix (SIZE_ERROR);}
  return _matrix[i];
}

/**
 * Output stream.
 * @param os an output stream.
 * @param other a Matrix to export.
 * @return an export of the other Matrix to the os stream.
 */
std::ostream& operator<< (std::ostream& os, const Matrix &other)
{
  if (!os)
    {treat_error_matrix (STREAM_ERROR);}
  for (int i = 0; i < other._rows; i++)
    {
      for (int j = 0; j < other._cols; j++)
        {
          if (other(i, j) >= MIN_TO_PRINT)
            {os << DOUBLE_SPACE;}
          else
            {os << DOUBLE_ASTERISK;}
        }
      os << std::endl;
    }
  return os;
}
