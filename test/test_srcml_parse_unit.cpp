/*

  Test cases for srcml_parse_unit
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  const std::string src = "a;\n";
  const std::string srcml = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
  std::ofstream src_file("project.c");
  src_file << src;
  src_file.close();
  
  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_parse_unit_filename(unit, "project.c");
    assert(*unit->unit == srcml);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  unlink("project.c");
  unlink("project.xml");

  return 0;
}
