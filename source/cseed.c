
   /*--------------------------------------------------------------------+
    |                              CSeed                                 |
    |--------------------------------------------------------------------|
    |                             cseed.c                                |
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
 | CSeed, the Chunky High-Level Compiler Seed                               |
 | Written by Cedric Bastoul, Cedric.Bastoul@unistra.fr                     |
 +--------------------------------------------------------------------------*/


#include <stdlib.h>
#include <string.h>
#include <osl/osl.h>
#include <clan/clan.h>
#define CLOOG_INT_LONG
#include <cloog/cloog.h>
#include <cseed/macros.h>

osl_scop_p cseed_scop_read_from_c(FILE* input, char* input_name) {
  clan_options_p clanoptions;
  osl_scop_p scop;

  clanoptions = clan_options_malloc();
  clanoptions->precision = OSL_PRECISION_MP;
  CLAN_strdup(clanoptions->name, input_name);
  scop = clan_scop_extract(input, clanoptions);
  clan_options_free(clanoptions);
  return scop;
}

void cseed_scop_print_to_c(FILE* output, osl_scop_p scop) {
  CloogState* state;
  CloogOptions* options;
  CloogInput* input;
  struct clast_stmt* clast;

  state = cloog_state_malloc();
  options = cloog_options_malloc(state);
  options->openscop = 1;
  cloog_options_copy_from_osl_scop(scop, options);
  input = cloog_input_from_osl_scop(options->state, scop);
  clast = cloog_clast_create_from_input(input, options);
  clast_pprint(output, clast, 0, options);
  
  cloog_clast_free(clast);
  options->scop = NULL; // don't free the scop
  cloog_options_free(options);
  cloog_state_free(state); // the input is freed inside
}

int main(int argc, char* argv[]) {
  osl_scop_p scop;
  FILE* input;

  if ((argc < 2) || (argc > 2)) {
    CSEED_info("usage: cseed [file]");
    exit(0);
  }

  if (argc == 1)
    input = stdin;
  else
    input = fopen(argv[1], "r");

  if (input == NULL)
    CSEED_error("cannot open input file");

  scop = cseed_scop_read_from_c(input, argv[1]);
  osl_scop_print(stdout, scop);
  cseed_scop_print_to_c(stdout, scop);
  osl_scop_free(scop);
  
  fclose(input);
  return 0;
}
