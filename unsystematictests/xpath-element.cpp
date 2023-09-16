#include <srcml.h>
#include <iostream>

int main(int argc, char * argv[]) {

    // open the input archive
    srcml_archive* inputArchive = srcml_archive_create();
    srcml_archive_read_open_filename(inputArchive, "project.xml");

    // add the xpath transformation
    srcml_append_transform_xpath_element(inputArchive, "//src:unit", "q", "http://myqueries.org/xml", "result");

    // open the output archive
    srcml_archive* outputArchive = srcml_archive_clone(inputArchive);
    srcml_archive_write_open_filename(outputArchive, "xpath.xml");

    // apply the transformation to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {
        std::cout << "read a unit"<<std::endl;

        // apply the transforms
        // since an XPath transformation can produce multiple units
        // from a single input unit, this requires using the srcml_transform_result
        srcml_transform_result* result = nullptr;
        std::cout << "applying transform"<<std::endl;
        srcml_unit_apply_transforms(inputArchive, unit, &result);

        std::cout << "got "<<srcml_transform_get_unit_size(result)<<" units as a result of the transform"<<std::endl;
        
        // write the transform result
        for (int i = 0; i < srcml_transform_get_unit_size(result); ++i) {
            std::cout << "outputting unit"<<std::endl;
            srcml_archive_write_unit(outputArchive, srcml_transform_get_unit(result, i));
        }

        // free the transformation result
        srcml_transform_free(result);

        // free the input unit
        srcml_unit_free(unit);
    }

    // close the archives
    srcml_archive_close(inputArchive);
    srcml_archive_close(outputArchive);

    // free the archives
    srcml_archive_free(inputArchive);
    srcml_archive_free(outputArchive);

    return 0;
}
