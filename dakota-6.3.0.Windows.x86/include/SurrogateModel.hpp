/*  _______________________________________________________________________

    DAKOTA: Design Analysis Kit for Optimization and Terascale Applications
    Copyright 2014 Sandia Corporation.
    This software is distributed under the GNU Lesser General Public License.
    For more information, see the README file in the top Dakota directory.
    _______________________________________________________________________ */

//- Class:       SurrogateModel
//- Description: A model which provides a surrogate for a truth model.
//- Owner:       Mike Eldred
//- Checked by:
//- Version: $Id: SurrogateModel.hpp 7024 2010-10-16 01:24:42Z mseldre $

#ifndef SURROGATE_MODEL_H
#define SURROGATE_MODEL_H

#include "DakotaModel.hpp"
#include "DakotaInterface.hpp"
#include "DiscrepancyCorrection.hpp"
#include "DataMethod.hpp"


namespace Dakota {

class ProblemDescDB;
class ParallelLibrary;

/// Base class for surrogate models (DataFitSurrModel and HierarchSurrModel).

/** The SurrogateModel class provides common functions to derived
    classes for computing and applying corrections to approximations. */

class SurrogateModel: public Model
{
public:

protected:

  //
  //- Heading: Constructor and destructor
  //

  /// constructor
  SurrogateModel(ProblemDescDB& problem_db);
  /// alternate constructor
  SurrogateModel(ProblemDescDB& problem_db, ParallelLibrary& parallel_lib,
		 const SharedVariablesData& svd, const SharedResponseData& srd,
		 const ActiveSet& set, short output_level);
  /// destructor
  ~SurrogateModel();

  //
  //- Heading: Virtual function redefinitions
  //

  /// return truth_model()
  Model& subordinate_model();

  /// return responseMode
  short surrogate_response_mode() const;

  /// return miPLIndex
  size_t mi_parallel_level_index() const;

  /// return deltaCorr
  DiscrepancyCorrection& discrepancy_correction();

  //
  //- Heading: Member functions
  //

  /// verify compatibility between SurrogateModel attributes and
  /// attributes of the submodel (DataFitSurrModel::actualModel or
  /// HierarchSurrModel::highFidelityModel)
  void check_submodel_compatibility(const Model& sub_model);

  /// evaluate whether a rebuild of the approximation should be
  /// forced based on changes in the inactive data
  bool force_rebuild();

  /// distributes the incoming orig_asv among actual_asv and approx_asv
  void asv_mapping(const ShortArray& orig_asv, ShortArray& actual_asv,
		   ShortArray& approx_asv, bool build_flag);
  /// reconstitutes a combined_asv from actual_asv and approx_asv
  void asv_mapping(const ShortArray& actual_asv, const ShortArray& approx_asv,
		   ShortArray& combined_asv);
  /// overlays actual_response and approx_response to update combined_response
  void response_mapping(const Response& actual_response,
                        const Response& approx_response,
                        Response& combined_response);

  //
  //- Heading: Data
  //

  /// for mixed response sets, this array specifies the response function
  /// subset that is approximated
  IntSet surrogateFnIndices;

  /// map of surrogate responses used in derived_synchronize() and
  /// derived_synchronize_nowait() functions
  IntResponseMap surrResponseMap;

  /// map of raw continuous variables used by apply_correction().
  /// Model::varsList cannot be used for this purpose since it does
  /// not contain lower level variables sets from finite differencing.
  IntVariablesMap rawVarsMap;

  /// map from actualModel/highFidelityModel evaluation ids to
  /// DataFitSurrModel.hppierarchSurrModel ids
  IntIntMap truthIdMap;
  /// map from approxInterface/lowFidelityModel evaluation ids to
  /// DataFitSurrModel.hppierarchSurrModel ids
  IntIntMap surrIdMap;

  /// map of approximate responses retrieved in derived_synchronize_nowait()
  /// that could not be returned since corresponding truth model response
  /// portions were still pending.
  IntResponseMap cachedApproxRespMap;

  /// an enumeration that controls the response calculation mode in
  /// {DataFit,Hierarch}SurrModel approximate response computations
  /** SurrBasedLocalMinimizer toggles this mode since compute_correction()
      does not back out old corrections. */
  short responseMode;

  /// number of calls to build_approximation()
  /** used as a flag to automatically build the approximation if one of the
      derived compute_response functions is called prior to
      build_approximation(). */
  size_t approxBuilds;

  /// the index of the active metaiterator-iterator parallelism level
  /// (corresponding to ParallelConfiguration::miPLIters) used at runtime
  size_t miPLIndex;

  /// stores a reference copy of active continuous lower bounds when the
  /// approximation is built; used to detect when a rebuild is required.
  RealVector referenceCLBnds;
  /// stores a reference copy of active continuous upper bounds when the
  /// approximation is built; used to detect when a rebuild is required.
  RealVector referenceCUBnds;
  /// stores a reference copy of active discrete int lower bounds when the
  /// approximation is built; used to detect when a rebuild is required.
  IntVector referenceDILBnds;
  /// stores a reference copy of active discrete int upper bounds when the
  /// approximation is built; used to detect when a rebuild is required.
  IntVector referenceDIUBnds;
  /// stores a reference copy of active discrete real lower bounds when the
  /// approximation is built; used to detect when a rebuild is required.
  RealVector referenceDRLBnds;
  /// stores a reference copy of active discrete real upper bounds when the
  /// approximation is built; used to detect when a rebuild is required.
  RealVector referenceDRUBnds;

  /// stores a reference copy of the inactive continuous variables when the
  /// approximation is built using a Distinct view; used to detect when a
  /// rebuild is required.
  RealVector referenceICVars;
  /// stores a reference copy of the inactive discrete int variables when
  /// the approximation is built using a Distinct view; used to detect when
  /// a rebuild is required.
  IntVector referenceIDIVars;
  /// stores a reference copy of the inactive discrete real variables when
  /// the approximation is built using a Distinct view; used to detect when
  /// a rebuild is required.
  RealVector referenceIDRVars;

  /// manages construction and application of correction functions that
  /// are applied to a surrogate model (DataFitSurr or HierarchSurr) in
  /// order to reproduce high fidelity data.
  DiscrepancyCorrection deltaCorr;

private:

  //
  //- Heading: Data
  //

  /// copy of the truth model variables object used to simplify conversion 
  /// among differing variable views in force_rebuild()
  Variables   truthModelVars;
  /// copy of the truth model constraints object used to simplify conversion 
  /// among differing variable views in force_rebuild()
  Constraints truthModelCons;
};


inline SurrogateModel::~SurrogateModel()
{ } // Virtual destructor handles referenceCount at Strategy level.


inline Model& SurrogateModel::subordinate_model()
{ return truth_model(); }


inline short SurrogateModel::surrogate_response_mode() const
{ return responseMode; }


inline size_t SurrogateModel::mi_parallel_level_index() const
{ return miPLIndex; }


inline DiscrepancyCorrection& SurrogateModel::discrepancy_correction()
{ return deltaCorr; }

} // namespace Dakota

#endif
