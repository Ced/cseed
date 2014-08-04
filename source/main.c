
   /*--------------------------------------------------------------------+
    |                              Spot                                  |
    |--------------------------------------------------------------------|
    |                             main.c                                 |
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
#include <osl/osl.h>
#include <spot/spot.h>

int main(int argc, char* argv[]) {
  osl_scop_p scop;
  FILE* input;

  if ((argc < 2) || (argc > 2)) {
    SPOT_info("usage: spot [file]");
    exit(0);
  }

  if (argc == 1)
    input = stdin;
  else
    input = fopen(argv[1], "r");

  if (input == NULL)
    SPOT_error("cannot open input file");

  scop = spot_scop_read_from_c(input, argv[1]);
	if (scop == NULL) { 
		SPOT_info("Null SCOP, exiting..");
		return 0;
	}
	spot_compute_scops(scop);
	spot_scop_print_to_c(stdout, scop);
	osl_scop_free(scop);
  
  fclose(input);
  return 0;
}
