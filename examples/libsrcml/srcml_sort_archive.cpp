/**
 * @file srcml_sort_archive.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
  Example program of the use of the libsrcml C API.

  Converts an input archive into an output archive sorted by the unit filename.
*/

#include <srcml.h>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char* argv[]) {

    // open an existing archive
    srcml_archive* inputArchive = srcml_archive_create();
    srcml_archive_read_open_filename(inputArchive, "project.xml");

    // create a new output archive with the same
    // options and attributes of the input archive
    srcml_archive* sortedArchive = srcml_archive_clone(inputArchive);

    // read all the units in the archive into a container
    std::vector<srcml_unit*> units;
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {
        units.push_back(unit);
    }

    // sort the units in the container by filename
    std::sort(units.begin(), units.end(), [](const srcml_unit* a, const srcml_unit* b) {
        return std::string(srcml_unit_get_filename(a)) < std::string(srcml_unit_get_filename(b));
    });

    // open a srcML archive for output
    srcml_archive_write_open_filename(sortedArchive, "projectSorted.xml");

    // copy the files from the container to the output archive
    // free them now since we did not before
    for (auto unit : units) {
        srcml_archive_write_unit(sortedArchive, unit);
        srcml_unit_free(unit);
    }

    // close the archives
    srcml_archive_close(inputArchive);
    srcml_archive_close(sortedArchive);

    // free the archives
    srcml_archive_free(inputArchive);
    srcml_archive_free(sortedArchive);

    return 0;
}
