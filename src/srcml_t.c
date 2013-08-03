/*
  srcml_t.c

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Test program for the syntax of the srcml.h functions
*/

#include "srcml.h"

int main() {

    /*
      Easy translation
    */

    /* Translate a single file to srcML */
    srcml("a.cpp", "a.cpp.xml");

    /* Translate a single srcML file back to a source code file */
    /* Note:  name of output file could come from srcML */
    srcml("a.cpp.xml", "a.cpp");

    /*
      Using old forms
    */

    /* Translate a single file to srcML */
    src2srcml("a.cpp", "a.cpp.xml");

    /* Translate a single srcML file back to a source code file */
    /* Note:  name of output file could come from srcML */
    /* NOTE:  NOT SURE WHY THIS CAN"T BE FOUND with gcc */
//    srcml2src("a.cpp.xml", "a.cpp");

    /*
      Full options, src2srcml
    */

    /* Translate a single file to srcML */
    src2srcml_filename_filename("a.cpp", "a.cpp.xml", 0, 0, 0, 0);

    /*
      Full options, srcml2src
    */

    /* Translate a srcML file to a source code file */
    srcml2src_filename_filename("a.cpp.xml", "a.cpp", 0, 0, 0, 0);

    /*
      Create an archive, file by file
    */

    /* create a new srcml archive */
    struct srcml_archive* archive = src2srcml_new_archive();

    /* setup options for srcml archive */
    src2srcml_set_directory(archive, "newstuff");

    /* open a srcML archive for output */
    src2srcml_open_filename(archive, "project.xml");

    /* setup options for srcml unit */
    src2srcml_unit_set_language(archive, "C");

    /* Translate to srcml and append to the archive */
    src2srcml_unit_filename(archive, "a.c");

    /* setup options for srcml unit */
    src2srcml_unit_set_language(archive, "C++");

    /* Translate to srcml and append to the archive */
    src2srcml_unit_filename(archive, "a.c");

    /* close the srcML archive */
    src2srcml_close(archive);

    /* free the srcML archive data */
    src2srcml_free(archive);



    return 0;
}
