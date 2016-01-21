/*  _______________________________________________________________________

    DAKOTA: Design Analysis Kit for Optimization and Terascale Applications
    Copyright 2014 Sandia Corporation.
    This software is distributed under the GNU Lesser General Public License.
    For more information, see the README file in the top Dakota directory.
    _______________________________________________________________________ */

//- Class:       RecastModel
//- Description: A model mapping variables into responses using
//-              primary/secondary function pointers.
//- Owner:       Mike Eldred
//- Checked by:
//- Version: $Id: RecastModel.hpp 7024 2010-10-16 01:24:42Z mseldre $

#ifndef RECAST_MODEL_H
#define RECAST_MODEL_H

#include "DakotaModel.hpp"
#include "DakotaInterface.hpp"
#include "ParallelLibrary.hpp"
#include "DataModel.hpp"


namespace Dakota {

/// Derived model class which provides a thin wrapper around a sub-model
/// in order to recast the form of its inputs and/or outputs.

/** The RecastModel class uses function pointers to allow recasting of
    the subModel input/output into new problem forms.  This is
    currently used to recast SBO approximate subproblems, but can be
    used for multiobjective, input/output scaling, and other problem
    modifications in the future. 

    For now, making the assumption that variables mappings are ordered
    by submodel active continous, discrete int, discrete string,
    discrete real variables, even though all current use cases are
    continuous only */
class RecastModel: public Model
{
public:
  
  //
  //- Heading: Constructor and destructor
  //

  /// standard constructor
  RecastModel(const Model& sub_model, const Sizet2DArray& vars_map_indices,
	      const SizetArray& vars_comps_total, const BitArray& all_relax_di,
	      const BitArray& all_relax_dr, bool nonlinear_vars_mapping,
	      void (*variables_map)      (const Variables& recast_vars,
					  Variables& sub_model_vars),
	      void (*set_map)            (const Variables& recast_vars,
					  const ActiveSet& recast_set,
					  ActiveSet& sub_model_set),
	      const Sizet2DArray& primary_resp_map_indices,
	      const Sizet2DArray& secondary_resp_map_indices,
	      size_t recast_secondary_offset, short recast_resp_order,
	      const BoolDequeArray& nonlinear_resp_mapping,
	      void (*primary_resp_map)   (const Variables& sub_model_vars,
					  const Variables& recast_vars,
					  const Response& sub_model_response,
					  Response& recast_response),
	      void (*secondary_resp_map) (const Variables& sub_model_vars,
					  const Variables& recast_vars,
					  const Response& sub_model_response,
					  Response& recast_response));
  /// alternate constructor
  RecastModel(const Model& sub_model, //size_t num_deriv_vars,
	      const SizetArray& vars_comps_totals, const BitArray& all_relax_di,
	      const BitArray& all_relax_dr,    size_t num_recast_primary_fns,
	      size_t num_recast_secondary_fns, size_t recast_secondary_offset,
	      short recast_resp_order);

  /// destructor
  ~RecastModel();

  //
  //- Heading: Member functions
  //

  /// completes initialization of the RecastModel after alternate construction
  void initialize(const Sizet2DArray& vars_map_indices,
		  bool nonlinear_vars_mapping,
		  void (*variables_map)     (const Variables& recast_vars,
					     Variables& sub_model_vars),
		  void (*set_map)           (const Variables& recast_vars,
					     const ActiveSet& recast_set,
					     ActiveSet& sub_model_set),
		  const Sizet2DArray& primary_resp_map_indices,
		  const Sizet2DArray& secondary_resp_map_indices,
		  const BoolDequeArray& nonlinear_resp_mapping,
		  void (*primary_resp_map)  (const Variables& sub_model_vars,
					     const Variables& recast_vars,
					     const Response& sub_model_response,
					     Response& recast_response),
		  void (*secondary_resp_map)(const Variables& sub_model_vars,
					     const Variables& recast_vars,
					     const Response& sub_model_response,
					     Response& recast_response));

  /// provide optional inverse mappings
  void inverse_mappings(
    void (*inv_vars_map)     (const Variables& sub_model_vars,
			      Variables& recast_vars),
    void (*inv_set_map)      (const Variables& sub_model_vars,
			      const ActiveSet& sub_model_set,
			      ActiveSet& recast_set),
    void (*inv_pri_resp_map) (const Variables& recast_vars,
			      const Variables& sub_model_vars,
			      const Response& recast_resp,
			      Response& sub_model_resp),
    void (*inv_sec_resp_map) (const Variables& recast_vars,
			      const Variables& sub_model_vars,
			      const Response& recast_resp,
			      Response& sub_model_resp));

  /// perform transformation of Variables (recast --> sub-model)
  void transform_variables(const Variables& recast_vars,
			   Variables& sub_model_vars);
  /// into sub_model_set for use with subModel.
  void transform_set(const Variables& recast_vars, const ActiveSet& recast_set,
		     ActiveSet& sub_model_set);
  /// perform transformation of Response (sub-model --> recast)
  void transform_response(const Variables& recast_vars,
			  const Variables& sub_model_vars,
			  const Response& sub_model_resp,
			  Response& recast_resp);

  /// perform inverse transformation of Variables (sub-model --> recast)
  void inverse_transform_variables(const Variables& sub_model_vars,
				   Variables& recast_vars);
  /// into sub_model_set for use with subModel.
  void inverse_transform_set(const Variables& sub_model_vars,
			     const ActiveSet& sub_model_set,
			     ActiveSet& recast_set);
  /// perform inverse transformation of Response (recast --> sub-model)
  void inverse_transform_response(const Variables& sub_model_vars,
				  const Variables& recast_vars,
				  const Response& recast_resp,
				  Response& sub_model_resp);

  /// override the submodel's derivative estimation behavior
  void submodel_supports_derivative_estimation(bool sed_flag);

protected:

  //
  //- Heading: Virtual function redefinitions
  //

  /// portion of compute_response() specific to RecastModel
  /// (forward to subModel.compute_response())
  void derived_compute_response(const ActiveSet& set);
  /// portion of asynch_compute_response() specific to RecastModel
  /// (forward to subModel.asynch_compute_response())
  void derived_asynch_compute_response(const ActiveSet& set);
  /// portion of synchronize() specific to RecastModel
  /// (forward to subModel.synchronize())
  const IntResponseMap& derived_synchronize();
  /// portion of synchronize_nowait() specific to RecastModel
  /// (forward to subModel.synchronize_nowait())
  const IntResponseMap& derived_synchronize_nowait();

  /// return sub-iterator, if present, within subModel
  Iterator& subordinate_iterator();
  /// return subModel
  Model& subordinate_model();
  /// return surrogate model, if present, within subModel
  Model& surrogate_model();
  /// return truth model, if present, within subModel
  Model& truth_model();
  /// add subModel to list and recurse into subModel
  void derived_subordinate_models(ModelList& ml, bool recurse_flag);
  /// pass request to subModel if recursing and then update from it
  void update_from_subordinate_model(size_t depth =
				     std::numeric_limits<size_t>::max());
  /// return subModel interface
  Interface& derived_interface();

  /// set the relative weightings for multiple objective functions or least
  /// squares terms and optionally recurses into subModel
  void primary_response_fn_weights(const RealVector& wts,
				   bool recurse_flag = true);

  /// update the subModel's surrogate response function indices
  /// (DataFitSurrModel::surrogateFnIndices)
  void surrogate_function_indices(const IntSet& surr_fn_indices);

  /// update the subModel's surrogate response mode
  /// (SurrogateModel::responseMode)
  void surrogate_response_mode(short mode);

  /// builds the subModel approximation
  void build_approximation();
  /// builds the subModel approximation
  bool build_approximation(const Variables& vars,
			   const IntResponsePair& response_pr);

  /// replaces data in the subModel approximation
  void update_approximation(bool rebuild_flag);
  /// replaces data in the subModel approximation
  void update_approximation(const Variables& vars,
			    const IntResponsePair& response_pr,
			    bool rebuild_flag);
  /// replaces data in the subModel approximation
  void update_approximation(const VariablesArray& vars_array,
			    const IntResponseMap& resp_map, bool rebuild_flag);

  /// appends data to the subModel approximation
  void append_approximation(bool rebuild_flag);
  /// appends data to the subModel approximation
  void append_approximation(const Variables& vars,
			    const IntResponsePair& response_pr,
			    bool rebuild_flag);
  /// appends data to the subModel approximation
  void append_approximation(const VariablesArray& vars_array,
			    const IntResponseMap& resp_map, bool rebuild_flag);

  void pop_approximation(bool save_surr_data, bool rebuild_flag = false);
  void restore_approximation();
  bool restore_available();
  void finalize_approximation();

  void store_approximation();
  void combine_approximation(short corr_type);

  /// retrieve the set of Approximations from the subModel
  std::vector<Approximation>& approximations();
  /// retrieve the approximation coefficients from the subModel
  const RealVectorArray& approximation_coefficients(bool normalized = false);
  /// set the approximation coefficients within the subModel
  void approximation_coefficients(const RealVectorArray& approx_coeffs,
				  bool normalized = false);
  /// retrieve the approximation variances from the subModel
  const RealVector& approximation_variances(const Variables& vars);
  /// retrieve the approximation data from the subModel
  const Pecos::SurrogateData& approximation_data(size_t index);

  /// RecastModel only supports parallelism in subModel, so this
  /// virtual function redefinition is simply a sanity check.
  void component_parallel_mode(short mode);

  /// return subModel's MI parallel level index
  size_t mi_parallel_level_index() const;

  /// return subModel local synchronization setting
  short local_eval_synchronization();
  /// return subModel local evaluation concurrency
  int local_eval_concurrency();
  /// flag which prevents overloading the master with a multiprocessor
  /// evaluation (request forwarded to subModel)
  bool derived_master_overload() const;

  IntIntPair estimate_partition_bounds(int max_eval_concurrency);

  /// set up RecastModel for parallel operations (request forwarded to subModel)
  void derived_init_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
				  bool recurse_flag = true);
  /// set up RecastModel for serial operations (request forwarded to subModel).
  void derived_init_serial();
  /// set active parallel configuration within subModel
  void derived_set_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
				 bool recurse_flag = true);
  /// deallocate communicator partitions for the RecastModel (request forwarded
  /// to subModel)
  void derived_free_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
				  bool recurse_flag = true);

  /// Service subModel job requests received from the master.
  /// Completes when a termination message is received from stop_servers().
  void serve_run(ParLevLIter pl_iter, int max_eval_concurrency);
  /// executed by the master to terminate subModel server operations
  /// when RecastModel iteration is complete.
  void stop_servers();

  /// update the Model's inactive view based on higher level (nested)
  /// context and optionally recurse into subModel
  void inactive_view(short view, bool recurse_flag = true);

  /// return the subModel interface identifier
  const String& interface_id() const;
  /// return the current evaluation id for the RecastModel (request
  /// forwarded to subModel)
  int evaluation_id() const;
  /// set the evaluation counter reference points for the RecastModel
  /// (request forwarded to subModel)
  void set_evaluation_reference();
  /// request fine-grained evaluation reporting within subModel
  void fine_grained_evaluation_counters();
  /// print the evaluation summary for the RecastModel (request
  /// forwarded to subModel)
  void print_evaluation_summary(std::ostream& s, bool minimal_header = false,
				bool relative_count = true) const;

  /// set the hierarchical eval ID tag prefix
  void eval_tag_prefix(const String& eval_id_str);

  /// RecastModel may need to map variables, asv before DB lookup, or
  /// responses after lookup
  bool db_lookup(const Variables& search_vars, 
		 const ActiveSet& search_set, Response& found_resp);

private:

  //
  //- Heading: Convenience member functions
  //

  /// code shared among constructors to initialize base class data from submodel
  void initialize_data_from_submodel();

  /// update current variables/labels/bounds/targets from subModel
  void update_from_sub_model();

  //
  //- Heading: Data members
  //

  /// the sub-model underlying the function pointers
  Model subModel;

  /// For each subModel variable, identifies the indices of the recast
  /// variables used to define it (maps RecastModel variables to
  /// subModel variables; data is packed with only the variable indices
  /// employed rather than a sparsely filled N_sm x N_r matrix).
  Sizet2DArray varsMapIndices;
  /// boolean set to true if the variables mapping involves a nonlinear
  /// transformation.  Used in transform_set() to manage the requirement for
  /// gradients within the Hessian transformations.  This does not require
  /// a BoolDeque for each individual variable, since response gradients and
  /// Hessians are managed per function, not per variable.
  bool nonlinearVarsMapping;

  /// set to true if non-NULL primaryRespMapping or secondaryRespMapping
  /// are supplied
  bool respMapping;
  /// For each recast primary function, identifies the indices of the
  /// subModel functions used to define it (maps subModel response
  /// to RecastModel Response).
  Sizet2DArray primaryRespMapIndices;
  /// For each recast secondary function, identifies the indices of
  /// the subModel functions used to define it (maps subModel response
  /// to RecastModel response).
  Sizet2DArray secondaryRespMapIndices;
  /// array of BoolDeques, one for each recast response function.  Each
  /// BoolDeque defines which subModel response functions contribute to the
  /// recast function using a nonlinear mapping.  Used in transform_set() to
  /// augment the subModel function value/gradient requirements.
  BoolDequeArray nonlinearRespMapping;

  /// map of recast active set passed to derived_asynch_compute_response().
  /// Needed for currentResponse update in synchronization routines.
  IntActiveSetMap recastSetMap;
  /// map of recast variables used by derived_asynch_compute_response().
  /// Needed for primaryRespMapping() and secondaryRespMapping() in
  /// synchronization routines.
  IntVariablesMap recastVarsMap;
  /// map of subModel variables used by derived_asynch_compute_response().
  /// Needed for primaryRespMapping() and secondaryRespMapping() in
  /// synchronization routines.
  IntVariablesMap subModelVarsMap;
  /// map of recast responses used by RecastModel::derived_synchronize()
  /// and RecastModel::derived_synchronize_nowait()
  IntResponseMap recastResponseMap;

  /// holds pointer for variables mapping function passed in ctor/initialize
  void (*variablesMapping)     (const Variables& recast_vars,
			        Variables& sub_model_vars);
  /// holds pointer for set mapping function passed in ctor/initialize
  void (*setMapping)           (const Variables& recast_vars,
				const ActiveSet& recast_set,
			        ActiveSet& sub_model_set);
  /// holds pointer for primary response mapping function passed in
  /// ctor/initialize
  void (*primaryRespMapping)   (const Variables& sub_model_vars,
				const Variables& recast_vars,
				const Response& sub_model_response,
				Response& recast_response);
  /// holds pointer for secondary response mapping function passed in
  /// ctor/initialize
  void (*secondaryRespMapping) (const Variables& sub_model_vars,
				const Variables& recast_vars,
				const Response& sub_model_response,
				Response& recast_response);

  // flag indicating that inverse mappings have been published
  //bool inverseMapFlag;
  /// holds pointer for optional inverse variables mapping function
  /// passed in inverse_mappings()
  void (*invVarsMapping)    (const Variables& sub_model_vars,
			     Variables& recast_vars);
  /// holds pointer for optional inverse set mapping function passed
  /// in inverse_mappings()
  void (*invSetMapping)     (const Variables& sub_model_vars,
			     const ActiveSet& sub_model_set,
			     ActiveSet& recast_set);
  /// holds pointer for optional inverse primary response mapping
  /// function passed in inverse_mappings()
  void (*invPriRespMapping) (const Variables& recast_vars,
			     const Variables& sub_model_vars,
			     const Response& recast_resp,
			     Response& sub_model_resp);
  /// holds pointer for optional inverse secondary response mapping
  /// function passed in inverse_mappings()
  void (*invSecRespMapping) (const Variables& recast_vars,
			     const Variables& sub_model_vars,
			     const Response& recast_resp,
			     Response& sub_model_resp);
};


inline RecastModel::~RecastModel()
{ } // Virtual destructor handles referenceCount at Strategy level.


inline void RecastModel::submodel_supports_derivative_estimation(bool sed_flag)
{ subModel.supports_derivative_estimation(sed_flag); }


inline Iterator& RecastModel::subordinate_iterator()
{ return subModel.subordinate_iterator(); }


inline Model& RecastModel::subordinate_model()
{ return subModel; }


inline Model& RecastModel::surrogate_model()
{ return subModel.surrogate_model(); }


inline Model& RecastModel::truth_model()
{ return subModel.truth_model(); }


inline void RecastModel::
derived_subordinate_models(ModelList& ml, bool recurse_flag)
{
  ml.push_back(subModel);
  if (recurse_flag)
    subModel.derived_subordinate_models(ml, true);
}


inline void RecastModel::update_from_subordinate_model(size_t depth)
{
  // data flows from the bottom-up, so recurse first
  if (depth > 0)
    subModel.update_from_subordinate_model(depth - 1);
  // now pull the latest updates from subModel
  update_from_sub_model();
}


inline Interface& RecastModel::derived_interface()
{ return subModel.derived_interface(); }


inline void RecastModel::
primary_response_fn_weights(const RealVector& wts, bool recurse_flag)
{
  primaryRespFnWts = wts;
  if (recurse_flag && !primaryRespMapping)
    subModel.primary_response_fn_weights(wts, recurse_flag);
}


inline void RecastModel::surrogate_response_mode(short mode)
{ if (mode == BYPASS_SURROGATE) subModel.surrogate_response_mode(mode); }


inline void RecastModel::
surrogate_function_indices(const IntSet& surr_fn_indices)
{ subModel.surrogate_function_indices(surr_fn_indices); }


inline void RecastModel::build_approximation()
{ subModel.build_approximation(); }


inline bool RecastModel::
build_approximation(const Variables& vars, const IntResponsePair& response_pr)
{ return subModel.build_approximation(vars, response_pr); }


inline void RecastModel::update_approximation(bool rebuild_flag)
{ subModel.update_approximation(rebuild_flag); }


inline void RecastModel::
update_approximation(const Variables& vars, const IntResponsePair& response_pr,
		     bool rebuild_flag)
{ subModel.update_approximation(vars, response_pr, rebuild_flag); }


inline void RecastModel::
update_approximation(const VariablesArray& vars_array,
		     const IntResponseMap& resp_map, bool rebuild_flag)
{ subModel.update_approximation(vars_array, resp_map, rebuild_flag); }


inline void RecastModel::append_approximation(bool rebuild_flag)
{ subModel.append_approximation(rebuild_flag); }


inline void RecastModel::
append_approximation(const Variables& vars, const IntResponsePair& response_pr,
		     bool rebuild_flag)
{ subModel.append_approximation(vars, response_pr, rebuild_flag); }


inline void RecastModel::
append_approximation(const VariablesArray& vars_array,
		     const IntResponseMap& resp_map, bool rebuild_flag)
{ subModel.append_approximation(vars_array, resp_map, rebuild_flag); }


inline void RecastModel::
pop_approximation(bool save_surr_data, bool rebuild_flag)
{ subModel.pop_approximation(save_surr_data, rebuild_flag); }


inline void RecastModel::restore_approximation()
{ subModel.restore_approximation(); }


inline bool RecastModel::restore_available()
{ return subModel.restore_available(); }


inline void RecastModel::finalize_approximation()
{ subModel.finalize_approximation(); }


inline void RecastModel::store_approximation()
{ subModel.store_approximation(); }


inline void RecastModel::combine_approximation(short corr_type)
{ subModel.combine_approximation(corr_type); }


inline std::vector<Approximation>& RecastModel::approximations()
{ return subModel.approximations(); }


inline const RealVectorArray& RecastModel::
approximation_coefficients(bool normalized)
{ return subModel.approximation_coefficients(normalized); }


inline void RecastModel::
approximation_coefficients(const RealVectorArray& approx_coeffs,
			   bool normalized)
{ subModel.approximation_coefficients(approx_coeffs, normalized); }


inline const RealVector& RecastModel::
approximation_variances(const Variables& vars)
{ return subModel.approximation_variances(vars); }


inline const Pecos::SurrogateData& RecastModel::approximation_data(size_t index)
{ return subModel.approximation_data(index); }


inline void RecastModel::component_parallel_mode(short mode)
{
  //if (mode != SUB_MODEL) {
  //  Cerr << "Error: RecastModel only supports the SUB_MODEL component "
  //       << "parallel mode." << std::endl;
  //  abort_handler(-1);
  //}

  // Since we don't want the thin recast wrapper interfering with subModel
  // parallelism, we forward the parallel mode to the subModel.  This differs
  // from all other derived Model implementations (which utilize the mode
  // locally and do not forward it).
  subModel.component_parallel_mode(mode);
}


inline size_t RecastModel::mi_parallel_level_index() const
{ return subModel.mi_parallel_level_index(); }


inline short RecastModel::local_eval_synchronization()
{ return subModel.local_eval_synchronization(); }


inline int RecastModel::local_eval_concurrency()
{ return subModel.local_eval_concurrency(); }


inline bool RecastModel::derived_master_overload() const
{ return subModel.derived_master_overload(); }


inline IntIntPair RecastModel::
estimate_partition_bounds(int max_eval_concurrency)
{ return subModel.estimate_partition_bounds(max_eval_concurrency); }


inline void RecastModel::
derived_init_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
			   bool recurse_flag)
{
  if (recurse_flag)
    subModel.init_communicators(pl_iter, max_eval_concurrency);
}


inline void RecastModel::derived_init_serial()
{ subModel.init_serial(); }


inline void RecastModel::
derived_set_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
			  bool recurse_flag)
{
  if (recurse_flag) {
    subModel.set_communicators(pl_iter, max_eval_concurrency);

    // RecastModels do not utilize default set_ie_asynchronous_mode() as
    // they do not define the ie_parallel_level
    asynchEvalFlag     = subModel.asynch_flag();
    evaluationCapacity = subModel.evaluation_capacity();
  }
}


inline void RecastModel::
derived_free_communicators(ParLevLIter pl_iter, int max_eval_concurrency,
			   bool recurse_flag)
{
  if (recurse_flag)
    subModel.free_communicators(pl_iter, max_eval_concurrency);
}


inline void RecastModel::
serve_run(ParLevLIter pl_iter, int max_eval_concurrency)
{
  // don't recurse, as subModel.serve() will set subModel comms
  set_communicators(pl_iter, max_eval_concurrency, false);

  subModel.serve_run(pl_iter, max_eval_concurrency); // sets subModel comms
}


inline void RecastModel::stop_servers()
{ subModel.stop_servers(); }


inline void RecastModel::inactive_view(short view, bool recurse_flag)
{
  currentVariables.inactive_view(view);
  userDefinedConstraints.inactive_view(view);
  if (recurse_flag)
    subModel.inactive_view(view, recurse_flag);
}


inline const String& RecastModel::interface_id() const
{ return subModel.interface_id(); }


inline int RecastModel::evaluation_id() const
{ return subModel.evaluation_id(); }


inline void RecastModel::set_evaluation_reference()
{ subModel.set_evaluation_reference(); }


inline void RecastModel::fine_grained_evaluation_counters()
{ subModel.fine_grained_evaluation_counters(); }


inline void RecastModel::
print_evaluation_summary(std::ostream& s, bool minimal_header,
			 bool relative_count) const
{ subModel.print_evaluation_summary(s, minimal_header, relative_count); }

/** RecastModel just forwards any tags to its subModel */
inline void RecastModel::eval_tag_prefix(const String& eval_id_str)
{ subModel.eval_tag_prefix(eval_id_str); }

} // namespace Dakota

#endif
