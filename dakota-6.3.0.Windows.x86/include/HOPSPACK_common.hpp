// $Id: HOPSPACK_common.hpp.cmake 216 2013-11-13 23:34:51Z tplante $
// $URL: https://software.sandia.gov/svn/hopspack/tags/dakota-6.3/src/src-shared/HOPSPACK_common.hpp.cmake $

//@HEADER
// ************************************************************************
// 
//         HOPSPACK: Hybrid Optimization Parallel Search Package
//                 Copyright 2009-2012 Sandia Corporation
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
  @file HOPSPACK_common.hpp
  @brief Include standard header files and defines, specific to a platform.
*/

#ifndef HOPSPACK_COMMON_HPP
#define HOPSPACK_COMMON_HPP


//----------------------------------------------------------------------
//  Platform/build specific symbols
//
//  Lines with a cmakedefine directive are replaced at build time with
//  either "#define symbol" or "#undef symbol"
//----------------------------------------------------------------------

//---- DEFINED IF THIS EXECUTABLE IS USING MPI.
/* #undef HAVE_MPI */

//---- DEFINED IF THIS EXECUTABLE IS USING MULTI-THREAD UTILITIES.
/* #undef HAVE_MT */

//---- DEFINED IF THIS EXECUTABLE INCLUDES AN LAPACK LIBRARY.
#define HAVE_LAPACK

//---- DEFINED IF THIS EXECUTABLE LINKS WITH A BLAS LIBRARY USING F2C WRAPPERS.
/* #undef HAVE_BLAS_F2C_WRAPPERS */

//---- DEFINED IF REAL TIME SYSTEM UTILITIES ARE FOUND.
#define HAVE_REALTIME_CLOCK

//---- DEFINED IF FORTRAN COMPILER FOUND (HELPS WITH BLAS/LAPACK LINKING)
#define HAVE_FORTRAN_COMPILER

//---- DEFINED IF CDDLIB AVAILABLE FOR DEGENERATE LINEAR CONSTRAINTS.
/* #undef HAVE_CDDLIB */


#if defined(_WIN32)
  #if (_MSC_VER >= 1400)
    //---- RECENT WINDOWS MSVC COMPILERS OFFER SECURE STRING FUNCTIONS.
    #define HAVE_MSVC_SECURE_STRING_FNS
  #endif
#endif


//----------------------------------------------------------------------
//  General include files
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include <float.h>       //-- FOR DBL_MAX, DBL_EPSILON


//----------------------------------------------------------------------
//  Define enumerations in the HOPSPACK namespace
//----------------------------------------------------------------------

//---- HOPSPACK_DBL_MAX is also used to define HOPSPACK::dne().
#if defined(DBL_MAX)
  #define HOPSPACK_DBL_MAX  DBL_MAX
#else
  #define HOPSPACK_DBL_MAX  1.0e308
#endif



//----------------------------------------------------------------------
//  Define enumerations in the HOPSPACK namespace
//----------------------------------------------------------------------

using namespace std;

namespace HOPSPACK
{
    /**
     *  \typedef FinalOptStateType
     *  Enumeration to identify the final state of HOPSPACK optimization.
     *  The value is set by the Mediator, which may not reflect more detailed
     *  status information known to the citizen solvers.
     */
    enum FinalOptStateType
    {
        //! Optimization found a feasible point, possibly optimal.
        FINALSTATE_FEASIBLE = 0,

        //! Optimization failed to find a feasible point.
        FINALSTATE_INFEASIBLE,

        //! Optimization did not run, usually due to a configuration error.
        FINALSTATE_ERROR
    };

    /**
     *  \typedef EvalRequestType
     *  Enumeration to identify the type of evaluation request.
     *  "F" means the objective function(s), "C" means all nonlinear constraints
     *  (equalities and inequalities), and "grad" means gradients for a set
     *  of functions.
     */
    enum EvalRequestType
    {
        //! Request is for objective function value(s) only.
        EVALREQTYPE_F = 0,

        //! Request is for objective(s) and nonlinear constraints.
        EVALREQTYPE_FC,

        //! Request is for objective(s) and their gradients.
        EVALREQTYPE_F_GRAD,

        //! Request is for objective(s), nonlinear constraints, and gradients.
        EVALREQTYPE_FC_GRAD
    };

    //---- EXCEPTIONS THAT CAN BE THROWN.
    const char  INTERNAL_ERROR[]     = "FATAL ERROR -- HOPSPACK INTERNAL ERROR";
    const char  MPI_PVM_ERROR[]      = "MPI/PVM COMMUNICATION ERROR";
    const char  LAPACK_BUILD_ERROR[] = "FATAL ERROR -- MUST BUILD WITH LAPACK";
}


#endif     //-- HOPSPACK_COMMON_HPP
