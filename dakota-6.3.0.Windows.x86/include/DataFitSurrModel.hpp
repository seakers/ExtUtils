/*  _______________________________________________________________________

    DAKOTA: Design Analysis Kit for Optimization and Terascale Applications
    Copyright 2014 Sandia Corporation.
    This software is distributed under the GNU Lesser General Public License.
    For more information, see the README file in the top Dakota directory.
    _______________________________________________________________________ */

//- Class:       DataFitSurrModel
//- Description: A model which manages a surrogate relationship between a
//-              data fit approximation (local, multipoint, or global) and
//-              a truth model.
//- Owner:       Mike Eldred
//- Checked by:
//- Version: $Id: DataFitSurrModel.hpp 7029 2010-10-22 00:17:02Z mseldre $

#ifndef DATA_FIT_SURR_MODEL_H
#define DATA_FIT_SURR_MODEL_H

#include "dakota_data_types.hpp"
#include "SurrogateModel.hpp"
#include "DakotaInterface.hpp"
#include "DakotaIterator.hpp"
#include "ProblemDescDB.hpp"
#include "ParallelLibrary.hpp"


namespace Dakota {

/// Derived model class within the surrogate model branch for managing
/// data fit surrogates (global and local)

/** The DataFitSurrModel class manages global or local approximations
    (surrogates that involve data fits) that are used in place of an
    expensive model.  The class contains an approxInterface (required
    for both global and local) which manages the approximate function
    evaluations, an actualModel (optional for global, required for
    local) which provides truth evaluations for building the
    surrogate, and a daceIterator (optional for global, not used for
    local) which selects parameter sets on which to evaluate
    actualModel in order to generate the necessary data for building
    global approximations. */

class DataFitSurrModel: public SurrogateModel
{
public:
  
  //
  //- Heading: Constructors and destructor
  //

  /// constructor
  DataFitSurrModel(ProblemDescDB& problem_db);
  /// alternate constructor for instantiations on the fly
  DataFitSurrModel(Iterator& dace_iterator, Model& actual_model,
		   const ActiveSet& set, const String& approx_type,
		   const UShortArray& approx_order, short corr_type,
		   short corr_order, short data_order, short output_level,
		   const String& point_reuse,
		   const String& import_build_points_file = String(),
		   unsigned short import_build_format = TABULAR_ANNOTATED,
		   bool import_build_active_only = false,
		   const String& export_approx_points_file = String(),
		   unsigned short export_approx_format = TABULAR_ANNOTATED);
  /// destructor
  ~DataFitSurrModel();

  //
  //- Heading: Member functions
  //

  /// set pointsTotal and pointsManagement mode
  void total_points(int points);

protected:

  //
  //- Heading: Virtual function redefinitions
  //

  // Perform the response computation portions specific to this derived 
  // class.  In this case, it simply employs approxInterface.map()/synch()/
  // synch_nowait() where approxInterface is a local, multipoint, or global
  // approximation.
  //
  /// portion of compute_response() specific to DataFitSurrModel
  void derived_compute_response(const ActiveSet& set);
  /// portion of asynch_compute_response() specific to DataFitSurrModel
  void derived_asynch_compute_response(const ActiveSet& set);
  /// portion of synchronize() specific to DataFitSurrModel
  const IntResponseMap& derived_synchronize();
  /// portion of synchronize_nowait() specific to DataFitSurrModel
  const IntResponseMap& derived_synchronize_nowait();

  /// return daceIterator
  Iterator& subordinate_iterator();
  /// return this model instance
  Model& surrogate_model();
  /// return actualModel
  Model& truth_model();
  /// return actualModel (and optionally its sub-models)
  void derived_subordinate_models(ModelList& ml, bool recurse_flag);
  /// pass request to actualModel if recursing and then update from it
  void update_from_subordinate_model(size_t depth =
				     std::numeric_limits<size_t>::max());
  /// return approxInterface
  Interface& derived_interface();

  /// set the relative weightings for multiple objective functions or least
  /// squares terms and optionally recurses into actualModel
  void primary_response_fn_weights(const RealVector& wts,
				   bool recurse_flag = true);

  /// set responseMode and pass any bypass request on to actualModel for
  /// any lower-level surrogates.
  void surrogate_response_mode(short mode);

  /// (re)set the surrogate index set in SurrogateModel::surrogateFnIndices
  /// and ApproximationInterface::approxFnIndices
  void surrogate_function_indices(const IntSet& surr_fn_indices);

  /// Builds the local/multipoint/global approximation using
  /// daceIterator/actualModel to generate new data points
  void build_approximation();
  /// Builds the local/multipoint/global approximation using
  /// daceIterator/actualModel to generate new data points that
  /// augment the vars/response anchor point
  bool build_approximation(const Variables& vars,
			   const IntResponsePair& response_pr);

  /// replaces the approximation data with daceIterator results and
  /// rebuilds the approximation if requested
  void update_approximation(bool rebuild_flag);
  /// replaces the anchor point, and rebuilds the approximation if requested
  void update_approximation(const Variables& vars,
			    const IntResponsePair& response_pr,
			    bool rebuild_flag);
  /// replaces the current points array and rebuilds the approximation
  /// if requested
  void update_approximation(const VariablesArray& vars_array,
			    const IntResponseMap& resp_map, bool rebuild_flag);
  /// replaces the current points array and rebuilds the approximation
  /// if requested
  void update_approximation(const RealMatrix& samples,
			    const IntResponseMap& resp_map, bool rebuild_flag);

  /// appends daceIterator results to a global approximation and rebuilds
  /// it if requested
  void append_approximation(bool rebuild_flag);
  /// appends a point to a global approximation and rebuilds it if requested
  void append_approximation(const Variables& vars,
			    const IntResponsePair& response_pr,
			    bool rebuild_flag);
  /// appends an array of points to a global approximation and rebuilds it
  /// if requested
  void append_approximation(const VariablesArray& vars_array,
			    const IntResponseMap& resp_map, bool rebuild_flag);
  /// appends a matrix of points to a global approximation and rebuilds it
  /// if requested
  void append_approximation(const RealMatrix& samples,
			    const IntResponseMap& resp_map, bool rebuild_flag);

  /// remove approximation data added on previous append_approximation() call
  /// or a specified number of points
  void pop_approximation(bool save_surr_data, bool rebuild_flag = false);

  /// restore a previous approximation data state
  void restore_approximation();
  /// query for whether a trial increment is restorable
  bool restore_available();

  /// finalize data fit by applying all previous trial increments
  void finalize_approximation();

  /// store the current data fit approximation for later combination
  void store_approximation();
  /// combine the current data fit approximation with one previously stored
  void combine_approximation(short corr_type);

  /// retrieve the SharedApproxData from approxInterface
  SharedApproxData& shared_approximation();
  /// retrieve the set of Approximations from approxInterface
  std::vector<Approximation>& approximations();
  /// return the approximation coefficients from each Approximation
  /// (request forwarded to approxInterface)
  const RealVectorArray& approximation_coefficients(bool normalized = false);
  /// set the approximation coefficients within each Approximation
  /// (request forwarded to approxInterface)
  void approximation_coefficients(const RealVectorArray& approx_coeffs,
				  bool normalized = false);
  /// return the approximation variance from each Approximation
  /// (request forwarded to approxInterface)
  const RealVector& approximation_variances(const Variables& vars);
  /// return the approximation data from a particular Approximation
  /// (request forwarded to approxInterface)
  const Pecos::SurrogateData& approximation_data(size_t index);

  /// update component parallel mode for supporting parallelism in actualModel
  void component_parallel_mode(short mode);

  IntIntPair estimate_partition_bounds(int max_eval_concurrency);

  /// set up actualModel for parallel operations
  void derived_init_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
				  bool recurse_flag = true);
  /// set up actualModel for serial operations.
  void derived_init_serial();
  /// set active parallel configuration within actualModel
  void derived_set_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
				 bool recurse_flag = true);
  /// deallocate communicator partitions for the DataFitSurrModel
  /// (request forwarded to actualModel)
  void derived_free_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
				  bool recurse_flag = true);

  /// Service actualModel job requests received from the master.
  /// Completes when a termination message is received from stop_servers().
  void serve_run(ParLevLIter pl_iter, int max_eval_concurrency);
  /// Executed by the master to terminate actualModel server operations
  /// when DataFitSurrModel iteration is complete.
  void stop_servers();

  /// update the Model's inactive view based on higher level (nested)
  /// context and optionally recurse into actualModel
  void inactive_view(short view, bool recurse_flag = true);

  /// return the approxInterface identifier
  const String& interface_id() const;
  /// return the current evaluation id for the DataFitSurrModel
  int evaluation_id() const;

  /// set the evaluation counter reference points for the DataFitSurrModel
  /// (request forwarded to approxInterface and actualModel)
  void set_evaluation_reference();
  /// request fine-grained evaluation reporting within approxInterface
  /// and actualModel
  void fine_grained_evaluation_counters();
  /// print the evaluation summary for the DataFitSurrModel
  /// (request forwarded to approxInterface and actualModel)
  void print_evaluation_summary(std::ostream& s, bool minimal_header = false,
				bool relative_count = true) const;

  //
  //- Heading: Data members
  //
  const bool exportSurrogate;

private:

  //
  //- Heading: Convenience functions
  //

  /// optionally read surrogate data points from provided file
  void import_points(unsigned short tabular_format, bool active_only);
  /// initialize file stream for exporting surrogate evaluations
  void initialize_export();
  /// finalize file stream for exporting surrogate evaluations
  void finalize_export();
  /// initialize manageRecasting and recastFlags for data import/export
  void manage_data_recastings();
  /// initialize file stream for exporting surrogate evaluations
  void export_point(int eval_id, const Variables& vars, const Response& resp);

  /// Common code for processing of approximate response maps shared by
  /// derived_synchronize() and derived_synchronize_nowait()
  void derived_synchronize_approx(const IntResponseMap& approx_resp_map,
				  IntResponseMap& approx_resp_map_rekey);

  /// Updates fit arrays for global approximations
  void update_global();
  /// Updates fit arrays for local or multipoint approximations
  void update_local_multipoint();
  /// Builds a global approximation using daceIterator
  void build_global();
  /// Builds a local or multipoint approximation using actualModel
  void build_local_multipoint();

  /// update actualModel with data from current variables/labels/bounds/targets
  void update_actual_model();
  /// update current variables/labels/bounds/targets with data from actualModel
  void update_from_actual_model();

  /// test if c_vars and d_vars are within [c_l_bnds,c_u_bnds] and
  /// [d_l_bnds,d_u_bnds]
  bool inside(const RealVector& c_vars, const IntVector& di_vars,
	      const RealVector& dr_vars);

  //
  //- Heading: Data members
  //

  /// number of calls to derived_compute_response()/
  /// derived_asynch_compute_response()
  int surrModelEvalCntr;

  /// total points the user specified to construct the surrogate
  int pointsTotal;
  /// configuration for points management in build_global()
  short pointsManagement;

  /// type of point reuse for approximation builds: \c all, \c region
  /// (default if points file), or \c none (default if no points file)
  String pointReuse;
  /// flag indicating need to manage data recastings when importing
  /// build data or exporting approximate evaluations
  bool manageRecasting;
  /// a key indicating which models within a model recursion involve recasting
  BoolDeque recastFlags;
  /// file name from \c import_build_points_file specification
  String importPointsFile;
  /// file name from \c export_approx_points_file specification
  String exportPointsFile;
  /// file export format for variables and approximate responses
  unsigned short exportFormat;
  /// output file stream for \c export_approx_points_file specification
  std::ofstream exportFileStream;
  /// array of variables sets read from the \c import_build_points_file
  VariablesList reuseFileVars;
  /// array of response sets read from the \c import_build_points_file
  ResponseList reuseFileResponses;

  /// manages the building and subsequent evaluation of the approximations
  /// (required for both global and local)
  Interface approxInterface;

  /// the truth model which provides evaluations for building the surrogate
  /// (optional for global, required for local)
  /** actualModel is unrestricted in type; arbitrary nestings are possible. */
  Model actualModel;

  /// selects parameter sets on which to evaluate actualModel in order
  /// to generate the necessary data for building global approximations
  /// (optional for global since restart data may also be used)
  Iterator daceIterator;
};


/** Virtual destructor handles referenceCount at base Model level. */
inline DataFitSurrModel::~DataFitSurrModel()
{ finalize_export(); }


inline void DataFitSurrModel::total_points(int points)
{ pointsTotal = points; if (points > 0) pointsManagement = TOTAL_POINTS; }


inline Iterator& DataFitSurrModel::subordinate_iterator()
{ return daceIterator; }


inline Model& DataFitSurrModel::surrogate_model()
{
  // return by reference: OK to return letter instance
  return *this;

  // return by value: letter instance must be returned within an envelope for
  // representation sharing/reference counting to work properly
  //Model surr_model;            // empty envelope
  //surr_model.assign_rep(this); // populate letter, increment reference count
  //return surr_model;
}


inline Model& DataFitSurrModel::truth_model()
{ return actualModel; }


inline void DataFitSurrModel::
derived_subordinate_models(ModelList& ml, bool recurse_flag)
{
  if (!actualModel.is_null()) {
    ml.push_back(actualModel);
    if (recurse_flag)
      actualModel.derived_subordinate_models(ml, recurse_flag);
  }
}


inline void DataFitSurrModel::update_from_subordinate_model(size_t depth)
{
  if (!actualModel.is_null()) {
    // data flows from the bottom-up, so recurse first
    if (depth > 0)
      actualModel.update_from_subordinate_model(depth - 1);
    // now pull the latest updates from actualModel
    update_from_actual_model();
  }
}


inline Interface& DataFitSurrModel::derived_interface()
{ return approxInterface; }


inline void DataFitSurrModel::
primary_response_fn_weights(const RealVector& wts, bool recurse_flag)
{
  primaryRespFnWts = wts;
  if (recurse_flag && !actualModel.is_null())
    actualModel.primary_response_fn_weights(wts, recurse_flag);
}


inline void DataFitSurrModel::surrogate_response_mode(short mode)
{
  responseMode = mode;
  if (mode == BYPASS_SURROGATE) // recurse in this case
    actualModel.surrogate_response_mode(mode);
}


inline void DataFitSurrModel::
surrogate_function_indices(const IntSet& surr_fn_indices)
{
  surrogateFnIndices = surr_fn_indices;
  approxInterface.approximation_function_indices(surr_fn_indices);
}


inline bool DataFitSurrModel::restore_available()
{ return approxInterface.restore_available(); }


inline SharedApproxData& DataFitSurrModel::shared_approximation()
{ return approxInterface.shared_approximation(); }


inline std::vector<Approximation>& DataFitSurrModel::approximations()
{ return approxInterface.approximations(); }


inline const RealVectorArray& DataFitSurrModel::
approximation_coefficients(bool normalized)
{ return approxInterface.approximation_coefficients(normalized); }


inline void DataFitSurrModel::
approximation_coefficients(const RealVectorArray& approx_coeffs,
			   bool normalized)
{
  approxInterface.approximation_coefficients(approx_coeffs, normalized);

  // Surrogate data is being imported.  Update state to suppress automatic
  // surrogate construction.
  approxBuilds++;
  if (strbegins(surrogateType, "global_"))
    update_global();
  else
    update_local_multipoint();
}


inline const RealVector& DataFitSurrModel::
approximation_variances(const Variables& vars)
{ return approxInterface.approximation_variances(vars); }


inline const Pecos::SurrogateData& DataFitSurrModel::
approximation_data(size_t index)
{ return approxInterface.approximation_data(index); }


inline IntIntPair DataFitSurrModel::
estimate_partition_bounds(int max_eval_concurrency)
{
  // support DB-based and on-the-fly instantiations for DataFitSurrModel
  if (!daceIterator.is_null()) {
    probDescDB.set_db_list_nodes(daceIterator.method_id());
    return daceIterator.estimate_partition_bounds();
  }
  else if (!actualModel.is_null()) {
    int am_max_conc = approxInterface.minimum_points(false)
                    * actualModel.derivative_concurrency(); // local/multipt
    probDescDB.set_db_model_nodes(actualModel.model_id());
    return actualModel.estimate_partition_bounds(am_max_conc);
  }
  else
    return IntIntPair(1, 1);
}


inline void DataFitSurrModel::derived_init_serial()
{
  //approxInterface.init_serial();

  if (!actualModel.is_null())
    actualModel.init_serial();
}


inline void DataFitSurrModel::
derived_set_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
			  bool recurse_flag)
{
  // allow recursion to progress - don't store/set/restore
  //parallelLib.parallel_configuration_iterator(modelPCIter);
  //approxInterface.set_communicators(messageLengths);
  // asynchEvalFlag and evaluationCapacity updates not required for DFS
  // (refer to {Recast,HierarchSurr}Model::derived_set_communicators())
  //set_ie_asynchronous_mode(max_eval_concurrency);

  miPLIndex = modelPCIter->mi_parallel_level_index(pl_iter);// run time setting

  if (recurse_flag) {
    if (!daceIterator.is_null())
      daceIterator.set_communicators(pl_iter);
    else if (!actualModel.is_null())
      actualModel.init_communicators(pl_iter,
	daceIterator.maximum_evaluation_concurrency()); // set in init_comms
  }
}


inline void DataFitSurrModel::
derived_free_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
			   bool recurse_flag)
{
  // allow recursion to progress - don't store/set/restore
  //parallelLib.parallel_configuration_iterator(modelPCIter);
  //approxInterface.free_communicators();

  if (recurse_flag) {
    if (!daceIterator.is_null())
      daceIterator.free_communicators(pl_iter);
    else if (!actualModel.is_null())
      actualModel.free_communicators(pl_iter,
	daceIterator.maximum_evaluation_concurrency()); // set in init_comms
  }
}


inline void DataFitSurrModel::
serve_run(ParLevLIter pl_iter, int max_eval_concurrency)
{
  // don't recurse, as actualModel.serve() will set actualModel comms
  set_communicators(pl_iter, max_eval_concurrency, false);

  if (!actualModel.is_null())
    actualModel.serve_run(pl_iter,
			  daceIterator.maximum_evaluation_concurrency());
}


inline void DataFitSurrModel::stop_servers()
{
  if (!actualModel.is_null())
    actualModel.stop_servers();
}


inline void DataFitSurrModel::inactive_view(short view, bool recurse_flag)
{
  currentVariables.inactive_view(view);
  userDefinedConstraints.inactive_view(view);
  if (recurse_flag && !actualModel.is_null())
    actualModel.inactive_view(view, recurse_flag);
}


inline const String& DataFitSurrModel::interface_id() const
{ return approxInterface.interface_id(); }


/** return the DataFitSurrModel evaluation count.  Due to possibly
    intermittent use of surrogate bypass, this is not the same as
    either the approxInterface or actualModel model evaluation counts.
    It also does not distinguish duplicate evals. */
inline int DataFitSurrModel::evaluation_id() const
{ return surrModelEvalCntr; }


inline void DataFitSurrModel::set_evaluation_reference()
{
  approxInterface.set_evaluation_reference();

  // don't recurse this, since the eval reference is for the top level iteration
  //if (!actualModel.is_null())
  //  actualModel.set_evaluation_reference();

  // may want to add this in time
  //surrModelEvalRef = surrModelEvalCntr;
}


inline void DataFitSurrModel::fine_grained_evaluation_counters()
{
  approxInterface.fine_grained_evaluation_counters(numFns);
  if (!actualModel.is_null())
    actualModel.fine_grained_evaluation_counters();
}


inline void DataFitSurrModel::
print_evaluation_summary(std::ostream& s, bool minimal_header,
			 bool relative_count) const
{
  approxInterface.print_evaluation_summary(s, minimal_header, relative_count);
  if (!actualModel.is_null()) {
    if (daceIterator.is_null())
      actualModel.print_evaluation_summary(s, minimal_header, relative_count);
    else // daceIterator resets the eval reference -> don't use a relative count
      actualModel.print_evaluation_summary(s, minimal_header, false);
  }
}

} // namespace Dakota

#endif
