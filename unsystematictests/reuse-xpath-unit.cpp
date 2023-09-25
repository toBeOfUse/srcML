#include <srcml.h>
#include <iostream>

int main(){
    srcml_archive* archive = srcml_archive_create();
    srcml_archive_enable_hash(archive);
    srcml_archive_read_open_filename(archive, "project.xml");
    srcml_unit* unit = srcml_archive_read_unit(archive);
    std::cout << "Original unit:" << std::endl << srcml_unit_get_srcml(unit) << std::endl;
    std::cout << "Original unit hash:" << srcml_unit_get_hash(unit) << std::endl;

    srcml_transform_result* result = nullptr;
    srcml_append_transform_xpath(archive, "//src:type");
    srcml_unit_apply_transforms(archive, unit, &result);
    srcml_unit* resultUnit = srcml_transform_get_unit(result, 0);
    std::cout << "Result:" << std::endl << srcml_unit_get_srcml(resultUnit) << std::endl;
    const char * resultHash = srcml_unit_get_hash(resultUnit);
    if (resultHash){
        std::cout << "Result hash: "<<resultHash << std::endl;
    } else {
        std::cout << "Result hash is nullptr" << std::endl;
    }
    srcml_clear_transforms(archive);

    srcml_transform_result* result2 = nullptr;
    srcml_append_transform_xpath(archive, "//name");
    srcml_unit_apply_transforms(archive, resultUnit, &result2);
    srcml_unit* resultUnit2 = srcml_transform_get_unit(result2, 0);
    std::cout << "Second result:" << std::endl << srcml_unit_get_srcml(resultUnit2) << std::endl;
}
