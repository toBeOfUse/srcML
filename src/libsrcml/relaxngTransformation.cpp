/**
 * @file relaxngTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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

 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <relaxngTransformation.hpp>

/**
 * relaxngTransformation
 * @param options list of srcML options
 * @param stylesheet an relaxng stylesheet
 * @param params relaxng parameters
 *
 * Constructor.
 */
relaxngTransformation::relaxngTransformation(/* OPTION_TYPE& options, */ xmlDocPtr relaxng) {

    relaxng_parser_ctxt = xmlRelaxNGNewDocParserCtxt(relaxng);
    rng = xmlRelaxNGParse(relaxng_parser_ctxt);
    rngctx = xmlRelaxNGNewValidCtxt(rng);
}

/**
 * ~relaxngTransformation
 *
 * Destructor.  Closes dynamically loaded library.
 */
relaxngTransformation::~relaxngTransformation() {

    xmlRelaxNGFreeValidCtxt(rngctx);
    xmlRelaxNGFree(rng);
    xmlRelaxNGFreeParserCtxt(relaxng_parser_ctxt);
}

/**
 * apply
 *
 * Apply relaxng program, writing results.
 * 
 * @returns true on success false on failure.
 */
TransformationResult relaxngTransformation::relaxngTransformation::apply(xmlDocPtr doc, int /* position */) {

    int n = xmlRelaxNGValidateDoc(rngctx, doc);
    if (n == 0)
        return TransformationResult();

    // transformation result is nodeset with single unit, and the unit is wrapped
    return TransformationResult(xmlXPathNodeSetCreate(doc->children), true);
}
