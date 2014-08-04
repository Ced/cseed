
   /*--------------------------------------------------------------------+
    |                              Spot                                  |
    |--------------------------------------------------------------------|
    |                             spot.c                                 |
    |--------------------------------------------------------------------|
    |                    First version: 14/01/2014                       |
    +--------------------------------------------------------------------+

 +--------------------------------------------------------------------------+
 |  / __)/ __)( ___)( ___)(  _ \                                            |
 | ( (__ \__ \ )__)  )__)  )(_) )      Chunky High-Level Compiler Seed      |
 |  \___)(___/(____)(____)(____/                                            |
 +--------------------------------------------------------------------------+
 | Copyright (C) 2014 University of Strasbourg                              |
 |                                                                          |
 | This library is free software; you can redistribute it and/or modify it  |
 | under the terms of the GNU Lesser General Public License as published by |
 | the Free Software Foundation; either version 2.1 of the License, or      |
 | (at your option) any later version.                                      |
 |                                                                          |
 | This library is distributed in the hope that it will be useful but       |
 | WITHOUT ANY WARRANTY; without even the implied warranty of               |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser  |
 | General Public License for more details.                                 |
 |                                                                          |
 | You should have received a copy of the GNU Lesser General Public License |
 | along with this software; if not, write to the Free Software Foundation, |
 | Inc., 51 Franklin Street, Fifth Floor,                                   |
 | Boston, MA  02110-1301  USA                                              |
 |                                                                          |
 | Spot, the Chunky High-Level Compiler Seed                                |
 | Written by Cedric Bastoul, Cedric.Bastoul@unistra.fr                     |
 +--------------------------------------------------------------------------*/


#include <stdlib.h>
#include <string.h>
//#include <osl/extensions/doi.h>
#include <osl/osl.h>
#include <clan/clan.h>
#define CLOOG_INT_LONG
#include <cloog/cloog.h>
#include <cloog/isl/cloog.h>
#include <spot/macros.h>
#include <isl/options.h>

osl_scop_p spot_scop_read_from_c(FILE* input, char* input_name) {
  clan_options_p clanoptions;
  osl_scop_p scop;

  clanoptions = clan_options_malloc();
  clanoptions->precision = OSL_PRECISION_MP;
  CLAN_strdup(clanoptions->name, input_name);
  scop = clan_scop_extract(input, clanoptions);
  clan_options_free(clanoptions);
  return scop;
}

void spot_scop_print_to_c(FILE* output, osl_scop_p scop) {
  CloogState* state;
  CloogOptions* options;
  CloogInput* input;
  CloogProgram* program ;
  
  state = cloog_state_malloc();
  options = cloog_options_malloc(state);
  options->openscop = 1;
  cloog_options_copy_from_osl_scop(scop, options);
  input = cloog_input_from_osl_scop(options->state, scop);
  
  /* Reading the program informations. */
  program = cloog_program_alloc(input->context, input->ud,options) ;
  free(input) ;
  
  program = cloog_program_generate(program,options);
  if (options->structure)
		cloog_program_print(stdout,program);
  cloog_program_pprint(output,program,options);
  
  cloog_program_free(program);
  options->scop = NULL; // don't free the scop  
  cloog_options_free(options);
	cloog_state_free(state); // the input is freed inside
  fclose(output); 
}

osl_names_p get_scop_names(osl_scop_p scop){

  //generate temp names
  osl_names_p names = osl_scop_names(scop);

  //if scop has names substitute them for temp names
  if(scop->context->nb_parameters){
    osl_strings_free(names->parameters);
    names->parameters = osl_strings_clone((osl_strings_p)scop->parameters->data);
  }

  osl_arrays_p arrays = osl_generic_lookup(scop->extension, OSL_URI_ARRAYS);
  if(arrays){
    osl_strings_free(names->arrays);
    names->arrays = osl_arrays_to_strings(arrays);
  }
  
	

  return names;
}

/** 
 * Convert a osl_relation_p containing the constraints of a domain
 * to an isl_set.
 * One shot only; does not take into account the next ptr.
 */
static __isl_give isl_set *osl_relation_to_isl_set(osl_relation_p relation,
        __isl_take isl_dim *dim)
{
    int i, j;
    int n_eq = 0, n_ineq = 0;
    isl_ctx *ctx;
    isl_mat *eq, *ineq;
    isl_int v;
    isl_basic_set *bset;

    isl_int_init(v);

    ctx = isl_dim_get_ctx(dim);

    for (i = 0; i < relation->nb_rows; ++i)
        if (osl_int_zero(relation->precision, relation->m[i][0]))
            n_eq++;
        else
            n_ineq++;

    eq = isl_mat_alloc(ctx, n_eq, relation->nb_columns - 1);
    ineq = isl_mat_alloc(ctx, n_ineq, relation->nb_columns - 1);

    n_eq = n_ineq = 0;
    for (i = 0; i < relation->nb_rows; ++i) {
        isl_mat **m;
        int row;

        if (osl_int_zero(relation->precision, relation->m[i][0])) {
            m = &eq;
            row = n_eq++;
        } else {
            m = &ineq;
            row = n_ineq++;
        }

        for (j = 0; j < relation->nb_columns - 1; ++j) {
            int t = osl_int_get_si(relation->precision, relation->m[i][1 + j]);
            isl_int_set_si(v, t);
            *m = isl_mat_set_element(*m, row, j, v);
        }
    }

    isl_int_clear(v);

    bset = isl_basic_set_from_constraint_matrices(dim, eq, ineq,
            isl_dim_set, isl_dim_div, isl_dim_param, isl_dim_cst);
    return isl_set_from_basic_set(bset);
}


/** 
 * Convert a osl_relation_p describing a union of domains
 * to an isl_set.
 */
static __isl_give isl_set *osl_relation_list_to_isl_set(
        osl_relation_p list, __isl_take isl_dim *dim)
{
    isl_set *set;

    set = isl_set_empty(isl_dim_copy(dim));
    for (; list; list = list->next) {
        isl_set *set_i;
        set_i = osl_relation_to_isl_set(list, isl_dim_copy(dim));
        set = isl_set_union(set, set_i);
    }

    isl_dim_free(dim);
    return set;
}


/** 
 * Set the dimension names of type "type" according to the elements
 * in the array "names".
 */
static __isl_give isl_dim *set_names(__isl_take isl_dim *dim,
        enum isl_dim_type type, char **names)
{
    int i;
    for (i = 0; i < isl_dim_size(dim, type); ++i)
        dim = isl_dim_set_name(dim, type, i, names[i]);
    return dim;
}

__isl_give isl_set * 
spot_get_isl_stmt_domain(__isl_keep isl_ctx * ctx, osl_scop_p scop, osl_statement_p stm) { 
	isl_set *context, *dom;
	isl_space *param_space;
	isl_dim *dim;
	osl_strings_p scop_params = NULL;
	
	dim = isl_dim_set_alloc(ctx, scop->context->nb_parameters, 0);
	if(scop->context->nb_parameters){
		scop_params = (osl_strings_p)scop->parameters->data;
		dim = set_names(dim, isl_dim_param, scop_params->string);
	}
	param_space = isl_space_params(isl_space_copy(dim));
	context = osl_relation_to_isl_set(scop->context, param_space);
	
	int niter = osl_statement_get_nb_iterators(stm);
	dim = isl_dim_set_alloc(ctx, scop->context->nb_parameters, niter);

	if(scop->context->nb_parameters){
		scop_params = (osl_strings_p)scop->parameters->data;
		dim = set_names(dim, isl_dim_param, scop_params->string);
	}  
	if(niter){
		osl_body_p stmt_body = osl_statement_get_body(stm);//(osl_body_p)(stm->body->data);
		dim = set_names(dim, isl_dim_set, stmt_body->iterators->string);
	} 
	dom = osl_relation_list_to_isl_set(stm->domain, isl_dim_copy(dim));
	dom = isl_set_intersect_params(dom, isl_set_copy(context));
	
	// free stuff
	isl_set_free(context);
	return dom;
}

osl_relation_p spot_isl_to_osl_dom(isl_ctx * ctx, isl_set * set) 
{
	static isl_printer *p = NULL;
	char *relstr; 
	osl_relation_p rel;
	
	if (p == NULL) {
		OSL_debug("creating printer");
		p = isl_printer_to_str(ctx);
		p = isl_printer_set_output_format(p, ISL_FORMAT_EXT_POLYLIB);
	}
	
	p = isl_printer_print_set(p, set);
	relstr = isl_printer_get_str(p);
	p = isl_printer_flush(p); 
		
	rel = osl_relation_sread_polylib(&relstr);
	rel->type = OSL_TYPE_DOMAIN;
	
	return rel;
}

void spot_add_statement(osl_scop_p scop, isl_ctx * ctx, osl_doi_p doi) {
	osl_statement_p tmp, dstmt;
	osl_body_p extb;

	// clone the original statement in order to create the new one
	tmp = scop->statement->next;
	scop->statement->next = NULL;
	dstmt = osl_statement_clone(scop->statement); 
	scop->statement->next = tmp;
	// replace the original domain by new one
	osl_relation_free(dstmt->domain);
	dstmt->domain = spot_isl_to_osl_dom(ctx, doi->user);

	extb = osl_generic_lookup(dstmt->extension, OSL_URI_BODY); 
	if (extb != NULL) { 
		osl_strings_free(extb->expression);
		OSL_debug("Extended Body not NULL");
	} else { 
		OSL_debug("Extended Body NULL");
	}
	extb->expression = osl_strings_malloc();
	osl_strings_add(extb->expression, doi->comp);
	
	osl_statement_add(&(scop->statement), dstmt);
}

void spot_compute_statements(osl_scop_p scop, osl_doi_p doi) {
	isl_set *stmt_dom;
	isl_ctx *ctx;
	isl_space *dsp;
	osl_doi_p di, dj;
	osl_statement_p tmp; 
	ctx = isl_ctx_alloc();		 
	assert(ctx);
	isl_options_set_on_error(ctx, ISL_ON_ERROR_ABORT);
	
	// get an isl set from the original domain
	stmt_dom = spot_get_isl_stmt_domain(ctx, scop, scop->statement);	
	if (stmt_dom == NULL) 
		SPOT_error("Impossible to convert statement domain to isl");
	dsp = isl_set_get_space(stmt_dom);
	
	// compute new domains and wrap them into statements
	for (di = doi; di != NULL; di = di->next) {
		int pflag = 0;
				
		if (di->user == NULL) { 
			di->user = isl_set_read_from_str(ctx, di->dom);	
			di->user = isl_set_align_params(di->user, isl_space_copy(dsp));
		}
		if (di->user == NULL) 
			SPOT_error("Impossible to read isl domain");
		
		// substract the set from the original domain
		stmt_dom = isl_set_subtract(stmt_dom, isl_set_copy(di->user));
	
		if (di->comp != NULL) {
			// solve priority overlapping 
			for (dj = doi; dj != NULL; dj = dj->next) {
				
				if (dj->user == NULL) {
					dj->user = isl_set_read_from_str(ctx, dj->dom);
					dj->user = isl_set_align_params(dj->user, isl_space_copy(dsp));
				}
				
			
				if (dj->user == NULL) 
					SPOT_error("Impossible to read isl domain");
					
				if (di->priority < dj->priority) 
					di->user = isl_set_subtract(di->user, isl_set_copy(dj->user));
				else if (di->priority == dj->priority && pflag == 0)
					pflag = 1; 
				else if (di->priority == dj->priority) 
					SPOT_error("More than one element with the same prority in the doi list"); 	
			}
			
			if (isl_set_is_empty(di->user)) 
				continue; 
	
			spot_add_statement(scop, ctx, di);		
		}
	}
	
	if (isl_set_is_empty(stmt_dom)) { 
		tmp = scop->statement;
		scop->statement = scop->statement->next;
		tmp->next = NULL;
		osl_statement_free(tmp); 
	} else { 
		osl_relation_free(scop->statement->domain); 
		scop->statement->domain = spot_isl_to_osl_dom(ctx, stmt_dom);
	} 
	
  // free isl stuff
  for (di = doi; di != NULL; di = di->next)
		isl_set_free(di->user);
	isl_set_free(stmt_dom);
	
	// TODO: figure out how to properly free isl ctx without error
	//isl_printer_free(p);
	//isl_ctx_free(ctx);
}


void spot_compute_scops(osl_scop_p scop) { 
	osl_doi_p doi; 

	while (scop != NULL) {
		// get doi extension 
		doi = osl_generic_lookup(scop->extension, OSL_URI_DOI); 
		
		if (doi == NULL) {  scop = scop->next;  continue; }

		if (scop->statement == NULL || scop->statement->next != NULL) { 
			scop = scop->next;
			continue; 
		}
		spot_compute_statements(scop, doi);
		// once applied, remove doi extension
		osl_generic_remove(&(scop->extension), OSL_URI_DOI);
		
		scop = scop->next; 
	}
} 
