// $Id: HOPSPACK_Matrix.hpp 166 2010-03-22 19:58:07Z tplante $ 
// $URL: https://software.sandia.gov/svn/hopspack/tags/dakota-6.3/src/src-shared/HOPSPACK_Matrix.hpp $ 

//@HEADER
// ************************************************************************
// 
//         HOPSPACK: Hybrid Optimization Parallel Search Package
//                 Copyright 2009-2010 Sandia Corporation
// 
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
// 
// This file is part of HOPSPACK.
//
// HOPSPACK is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library.  If not, see http://www.gnu.org/licenses/.
// 
// Questions? Contact Tammy Kolda (tgkolda@sandia.gov)
//                 or Todd Plantenga (tplante@sandia.gov) 
// 
// ************************************************************************
//@HEADER

/*!
  \file HOPSPACK_Matrix.hpp
  \brief  Class declaration of HOPSPACK::Matrix.
*/

#ifndef HOPSPACK_MATRIX_HPP
#define HOPSPACK_MATRIX_HPP

#include "HOPSPACK_common.hpp"
#include "HOPSPACK_Vector.hpp"

namespace HOPSPACK
{

//! Stores a dense matrix with indices beginning at zero
/*!  

\b Vectorized \b storage

When interfacing with BLAS and LAPACK
routines it is necessary to first write matrix elements to a vector of
columns, and/or a vector of rows.  
To enhance computational efficiency when such a copy is made, it will
automatically be stored.  If A is written to a vector by columns,
a copy is stored in fmatvec and fmatvecSet is set to true.  Similarly,
if A is written to vector by rows, a copy is stored in fmatvecT and
fmatvecTSet is set to true.  To ensure that the vectorized forms
of A are kept current, both fmatvecSet and fmatvecTSet are set to 
false upon instantiation and whenever a non-const method is called.

\warning
To ensure that vectorized storage of A is kept current, it
is crucial that every non-const method call method Matrix::matrixChanged()
which resets fmatvecSet and fmatvecTSet to false.

\see Matrix::copyToFortranVector();
 */
class Matrix
{
public:

  //! Indicator for transpose operations
  enum TransposeType {
    //! Transpose the matrix
    TRANSPOSE,
    //! Do not transpose the matrix
    NO_TRANSPOSE
  };

  //! Constructor. Creates an empty matrix.
  Matrix();
   
  //! Copy constructor.
  /*! Sets A = B (or A = B' if ttype = TRANSPOSE). */
  Matrix(const Matrix& B, TransposeType ttype = NO_TRANSPOSE);

  //! Copy constructor with scaling
  /*! Copies B into A and scales column j by s[j]. In other words,
    \f[
    A_{ij} = s_j B_{ij}.
    \f]
    If ttype = TRANSPOSE, the
    \f[
    A_{ij} = s_j B_{ji}.
    \f]
   */
  Matrix(const Matrix& B, const Vector& s, TransposeType ttype = NO_TRANSPOSE);

  //! Constructor. Creates a matrix and populates with A(i,j) = Aptr[i][j].
  Matrix(double ** const Aptr, int nrows, int ncols);

  //! Assignment operator that copies a matrix (A = B).
  Matrix &  operator= (const Matrix &  B);

  //! Destructor
  ~Matrix();

  //@{ \name Accessors

  //! Returns true if A is empty, i.e., the number of rows is zero
  bool empty() const;

  //! Returns number of rows in A.
  int getNrows() const;
  
  //! Returns number of columns in A.
  int getNcols() const;

  //! Return ith row of A (rows are numbered starting with zero).
  const Vector& getRow(int i) const;

  //@}

  //@{ \name Modifiers

  //! On exit Aptr[i] points to row i of A.
  void getModifiableRowPointers(vector< double *>& Aptr);

  //! Empty the matrix
   void clear();

  //! Append a copy of r to the bottom of A.
  void addRow(const Vector& r);

  //! Deletes the ith row of A.
  void deleteRow(int i);

  //! Append alpha * r (i.e., a scaled copy of r) to the bottom of A.
  void addRow(const Vector& r, double alpha);

  //! Append copies of the rows of matrix B to the bottom of A.
  void addMatrix(const Matrix& B);
 
  //! Append the matrix alpha * B (i.e., a scaled copy of B) to the bottom of A.
  void addMatrix(const Matrix& B, double alpha);

  //! Append matrix B scaled columnwise to the bottom of A.  
  /*! In other words the matrix \f$\hat{B}\f$ is added to the bottom of A, where
    \f[
    \hat{B}_{ij} = s_j B_{ij}.
    \f]
   */
  void addMatrix(const Matrix& B, const Vector& s);

  //! Selectivly appends the rows which are unique to B to the bottom of A.
  /*! In other words, the ith row \f$ b_i \f$ is added to the end of A if
    \f[
     \|a_j - b_i\|_2 \le \epsilon \;\;\forall\;\; a_j \in A.
    \f]
    Here \f$ a_j \f$ is used to denote the jth row of A. 
    \note It is assumed that there is no repitition amongst the rows of B.
  */
  void addUniqueRows(const Matrix& B, double epsilon);

  //! Copy transposed matrix (A = B').
  void transpose(const Matrix& B);

  //! Copy and scale matrix.
  /*! Copies B into A and scales column j by s[j]. In other words,
    \f[
    A_{ij} = s_j B_{ij}.
    \f]
   */
  void scale(const Matrix& B, const Vector& s);

  //! Scale matrix.
  /*! Scales column j by s[j]. In other words,
    \f[
    A_{ij} = s_j A_{ij}.
    \f]
   */
  void scale(const Vector& s);

  //! Sets A = I, an n by n identity matrix.
  void setToIdentity(int n);
  
  //! Normalizes the rows of A. 
  /*! \warning All rows of A with norm equal to zero are deleted from A. */
  void normalize();

  //@}

  //@{ \name Calculations involving A

  //! Compute y = Ax (or y = A'x if ttype = TRANSPOSE)
  void multVec(const Vector& x, Vector& y,
               TransposeType ttype = NO_TRANSPOSE) const;
  
  //! Update "this" matrix A to be A*B (or A*B' if ttype = TRANSPOSE).
  void multMat(const Matrix& B, TransposeType ttype = NO_TRANSPOSE);
  
  //! Compute C = A*B (or C = A*B' if ttype = TRANSPOSE), where A is "this".
  /*!
   *  Matrix C is resized to conform with the product.
   */
  void multMat(const Matrix& B, Matrix& C,
               TransposeType ttype = NO_TRANSPOSE) const;

  //! Compute the singular value decomposition (SVD) of A.
  /*! Compute \f$ A = U \Sigma V^T \f$ where U is an m x m orthogonal
    matrix, V is an n x n orthogonal matrix, and \f$ \Sigma \f$ is an m
    x n diagonal matrix where
    \f[
    \Sigma = \left[
    \begin{array}{cccc}
    s_{1} & 0 & \cdots & 0 \\
    0 & s_2 & \cdots & 0 \\
   \vdots &\vdots & \ddots &\vdots \\
    0 & 0 & 0 & s_{p}\\
    \end{array}
    \right],
    \f]
    with \f$s_1 \geq s_2 \geq \cdots \geq s_p\f$ and p = min{m,n}.
  
    \param U (output) Matrix \f$ U \f$ from the SVD
    \param s (output) Vector for diagonal of \f$ \Sigma \f$ from the SVD
    \param VT (output) Matrix \f$ V^T \f$ from the SVD (note this is transposed)

    \note Method requires that HOPSPACK be configured with LAPACK.
   */
  void svd(Matrix& U, Vector &s, Matrix& VT) const;

  //! On exit the rows of ZT form an orthonormal basis for null(A).
  /*!  
    \param ZT (output) ZT is a matrix of size \f$ n_z \times n \f$ where
    n is the number of columns in A and 
      \f$ n_z = n - {\rm rank}(A) \f$ .
    \param tol (input) Any singular value of A that is less than this value is
    considered to be zero for the purposes of determining the rank.

    \note Method requires that HOPSPACK be configured with LAPACK.
  */
  void nullSpace(Matrix& ZT, double tol) const;

  //! Computes right inverse and null space basis matrix for A (if possible).
  /*! Let A be an \f$m \times n\f$ matrix. We assume \f$n < m\f$ and that A
    has full row rank (i.e., rank(A) = m); if not, the method returns
    false because a right inverse cannot be computed.  Otherwise,
    matrices R and N are computed such that R is a right-inverse of A,
    i.e., \f$AR = I\f$, and the columns of N are an orthonormal basis for
    the null space of A so that \f$AN = 0\f$.

      \param RT (output) The transpose of R, the right inverse of A

      \param NT (output) The transpose  N, whose columns span the null space of A

      \param tol (input) Used in the rank determination, i.e., only
                   singular values greater than or equal to tol are
                   considered to be non-zero.

      \return True if the right inverse of A exists; false otherwise.

      \note Method requires that HOPSPACK be configured with LAPACK.
  */
  bool getRightInvAndNullBasis(Matrix& RT, Matrix& NT, double tol) const;

  //! Find and delete dependent rows of A and corresponding entries in b.
  /*!
    \param b Righthand-side vector in system Ax=b.
    \param epsilon Threshold used to identify linearly dependent rows.

    On exit, Ax = b forms a consistent system.
    A consistent system is formed by deleting rows of A and entries
    of b corresponding to dependent rows.  

    Dependent rows are determined as follows:

    An LQ factorization of A is first computed, where L denotes a lower
    triangular matrix and Q an orthogonal matrix. In exact arithmetic, the
    nonzero diagonal entries of L correspond to a linearly independent subset
    of the rows of A.  To allow for numerical round off, row \f$ a_i^T \f$
    and entry \f$ b_i \f$ are deleted from A and b respectively if
    \f[
      |L(i,i)| < \epsilon. 
    \f]

    \note Method requires that HOPSPACK be configured with LAPACK.
   */
  void pruneDependentRows(Vector& b, double epsilon);


  //! Solve a special equality constrained least-squares problem.
  /*! 
    This method solves the  following problem for "this" matrix \f$ A \f$
    and constant vectors \f$ b \f$ and \f$ \hat x \f$:
    \f[
    \begin{array}{ll}
      {\rm minimize}   & \|\hat x - x\|_2  \\
      {\rm subject to} & A x = b
    \end{array}
    \f]
    LAPACK function DGGLSE is used.  It is assumed that \f$ A \f$ is full rank.
    
    @param[in,out] cX  On entry, x denotes current point \f$ \hat x \f$.
                       On exit, x is the solution to the optimization problem.
    @param[in]     cB  The right-hand side vector to system \f$ A x = b \f$.
    @return            True if successful.  False also prints an error message.

    @note Method requires that HOPSPACK be configured with LAPACK.
  */
  bool  specialConstrainedLSQR (      Vector &  cX,
                                const Vector &  cB) const;


  //! Solve a general equality constrained least-squares problem.
  /*! 
    This method solves the  following problem for "this" matrix \f$ A \f$:
    \f[
    \begin{array}{ll}
      {\rm minimize}   & \| c - D x \|_2  \\
      {\rm subject to} & A x = b
    \end{array}
    \f]
    using LAPACK function DGGLSE.  It is assumed that \f$ A \f$ is full rank
    and \f$ D \f$ is diagonal.
    
    @param[in]  cC  The cost vector \f$ c \f$.
    @param[in]  cD  The diagonal cost matrix \f$ D \f$.
    @param[in]  cB  The right-hand side vector to system \f$ A x = b \f$.
    @param[out] cX  The solution to the optimization problem, assuming
                    the method returns true.
    @return         True if successful.  False also prints an error message.

    @note Method requires that HOPSPACK be configured with LAPACK.
  */
  bool  generalConstrainedLSQR (const Vector &  cC,
                                const Vector &  cD,
                                const Vector &  cB,
                                      Vector &  cX) const;


  //! Solve a least-squares problem without constraints.
  /*! 
    This method solves the  following problem for "this" matrix \f$ A \f$:
    \f[
    \begin{array}{ll}
      {\rm minimize} & \| c - A x \|_2  \\
    \end{array}
    \f]
    using LAPACK function DGELSS.  It is assumed that \f$ A \f$ is full rank.
    
    @param[in]  cC  The cost vector \f$ c \f$.
    @param[out] cX  The solution to the optimization problem, assuming
                    the method returns true.
    @return         True if successful.  False also prints an error message.

    @note Method requires that HOPSPACK be configured with LAPACK.
  */
  bool  generalLS (const Vector &  cC,
                         Vector &  cX) const;

  //@}

  //@{ \name Printing

  //! Print object to the given stream
  void leftshift(ostream& stream) const;

  //! Print object to the given stream
  void formattedPrint(const string  &  sPrefix,
                            ostream &  stream) const;

  //@}

private:

  //! Sets fmatvecSet and fmatvecTSet to false.
  /*! Should be called inside ever non-const method. */
  void matrixChanged();

  /*! \brief Private member variables fmatvec and fmatvecTSet should
    never be access directly.  Instead method getMatrixVector() should
    be called.  If ttype equals NO_TRANSPOSE, A is returns as a single vector
    stored column-wise.  If ttype equals TRANSPOSE, A is returns as a single vector stored
    row-wise.
  */
  const Vector& getMatrixVector(TransposeType ttype = NO_TRANSPOSE) const;
  
  //! Converts A to a Fortran-style vector
  /*! Converts A to a vector stored columnwise so that the vector representation is
    \f[
    \left[
    \begin{array}{c}
    a_{11} \\
    a_{21} \\
    \vdots \\
    a_{m1} \\
    a_{12} \\
    a_{22} \\
    \vdots \\
    a_{m2} \\
    a_{13} \\
    \vdots \\
    \vdots \\
    a_{mn}
    \end{array}
    \right]
    \f]
    The index mapping is:
    \f[
    (i,j) \rightarrow (j * m) + i
    \f]
    Store A' instead if ttype = TRANSPOSE
  */
  void copyToFortranVector(Vector& Avec, TransposeType ttype = NO_TRANSPOSE) const;

  //! Unpack A from a Fortran-style vector.
  /*!
    Assumes A' is stored in the vector if ttype = TRANSPOSE
   */
  void copyFromFortranVector(const Vector& Avec, int nrows, int ncols, TransposeType ttype = NO_TRANSPOSE);
 
  //! Resize A to have m rows and n columns
  void resize(int m, int n);
  
  //! Sets A equal to the specified block-row submatrix of B.
  /*!
    \param istart Starting row of submatrix.
    \param iextent Number of rows past istart to copy from B.
    \param B Matrix to be copied from.

    On exit A equals the block-row submatrix of B designated by
    rows \a istart through \a istart + \a iextent of B.  Essentially,
    \f[
      A(i,j) = B(istart+i, j), \; 0 \le i < iextent, \; 0 \le j < n_B
    \f]   
    where \f$ n_B \f$ denotes the number of columns in \f$ B\f$.  Hence
    \f$ A \f$ is an (iextent X \f$n_B\f$) matrix on exit.
  */
  void copySubMatrix(int istart, int iextent, const Matrix& B);
   
#if defined(HAVE_LAPACK)

  //! Performs a matrix-vector  multiply with BLAS subroutine DGEMV.
  /*! Computes y = Ax (or y = A'x  if ttype = TRANSPOSE). */
  void multVecWithBlas(const Vector& x, Vector& y,
                       TransposeType ttype = NO_TRANSPOSE) const;

  //! Performs a matrix multiply using BLAS routine DGEMM.
  /*! On exit C = A*B if ttype = NO_TRANSPOSE, and C = A*B' otherwise. */
  void multMatWithBlas(const HOPSPACK::Matrix& Bin,
                             HOPSPACK::Matrix& C,
                       const TransposeType ttype) const;

#else
  
  //! Performs a matrix-vector multiply without BLAS subroutines.
  /*! Computes y = Ax (or y = A'x  if ttype = TRANSPOSE). */
  void multVecWithoutBlas(const Vector& x, Vector& y,
                          TransposeType ttype = NO_TRANSPOSE) const;

  //! Performs a matrix multiply by hand without BLAS subroutines.
  /*! On exit C = A*B if ttype = NO_TRANSPOSE, and C = A*B' otherwise. */
  void multMatWithoutBlas(const HOPSPACK::Matrix& Bin,
                                HOPSPACK::Matrix& C,
                          const TransposeType ttype) const;

#endif


  /*! \brief The matrix A.

  The matrix is stored rowwise, i.e., matrix[i] is row i of A. and matrix[i][j] is A_ij.

   */

  vector<Vector> matrix;

  //@{ \name Vectorized matrix storage. 
  
  /*! Stores a current copy of A as a vector of columns if fmatvecSet=true */
  mutable Vector fmatvec;
  
  /*! If true, fmatvec is up to date.  If false, fmatvex must be updated.  */
  mutable bool fmatvecSet;
  
  /*! Stores a current copy of A as a vector of rows if fmatvecT=true */
  mutable Vector fmatvecT;
  
  /*! If true, fmatvecT is up to date.  If false, fmatvecT must be updated. */
  mutable bool fmatvecTSet;
  
  //@}
  
};

}


#endif
