
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
#include <osl/scop.h>
#include <osl/statement.h>
#include <osl/relation.h>
#include <cseed/macros.h>

int main(int argc, char * argv[]) {
  osl_scop_p scop;
  osl_statement_p statement;
  osl_relation_p scattering;
  FILE * input;
  int nb_statements;

  if ((argc > 2) || ((argc == 2) && !strcmp(argv[1], "-h")))  {
    CSEED_info("usage: cseed [file]");
    exit(0);
  }

  if (argc == 1)
    input = stdin;
  else
    input = fopen(argv[1], "r");

  if (input == NULL)
    CSEED_error("cannot open input file");

  scop = osl_scop_read(input);
  statement = scop->statement;
  nb_statements = osl_statement_number(statement);
  srand(time(0));

  while (statement != NULL) {
    scattering = statement->scattering;
    if (scattering != NULL)
      osl_int_set_si(scattering->precision,
	             scattering->m[0], scattering->nb_columns - 1,
		     rand() % nb_statements);
    else
      CSEED_warning("no scattering");

    statement = statement->next;
  }
  
  osl_scop_print(stdout, scop);
  osl_scop_free(scop);
  fclose(input);
}
