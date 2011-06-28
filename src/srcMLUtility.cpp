/*
  srcMLUtility.cpp

  Copyright (C) 2004-2011  SDML (www.sdml.info)

  This file is part of the srcML translator.

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

  Class for straightforward translation
*/

#include "srcMLUtility.hpp"
#include <cstring>
#include "srcmlns.hpp"
#include <sys/stat.h>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <sys/errno.h>
#else
#include <direct.h>
#endif

#include <libxml/parserInternals.h>

#include "Options.hpp"

#include "ProcessUnit.hpp"
#include "ExtractUnitsSrc.hpp"
#include "CountUnits.hpp"
#include "Properties.hpp"
#include "ListUnits.hpp"
#include "ExtractUnitsXML.hpp"

#include "SAX2ExtractUnitsSrc.hpp"

#include "SAX2UnitDOMXPath.hpp"
#include "SAX2UnitDOMXSLT.hpp"
#include "SAX2UnitDOMRelaxNG.hpp"

#include "srcexfun.hpp"

#include <libexslt/exslt.h>

#include "libxml_archive_read.hpp"
#include "libxml_archive_write.hpp"

// local function forward declarations
static xmlParserCtxtPtr srcMLCreateURLParserCtxt(const char * infile);
static void srcMLParseDocument(xmlParserCtxtPtr ctxt);

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* encoding, OPTION_TYPE& op)
  : infile(infilename), output_encoding(encoding), options(op), units(0) {


  // assume totaling for numeric results
  op |= OPTION_XPATH_TOTAL;

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";
}
// destructor
srcMLUtility::~srcMLUtility() {

}

int srcMLUtility::curunits() const {
  return units;
}

// attribute
const char* srcMLUtility::attribute(const char* attribute_name) {

  const char* pos = find(attrv, attribute_name);

  return pos ? pos : 0;
}

// prefix of given namespace
// return blank for default namespace
const char* srcMLUtility::namespace_ext(const char* uri) {

  static std::string s;

  const char* pos = find(nsv, uri);

  if (!pos)
    return 0;

  if (strncmp(pos, "xmlns:", 6) == 0)
    return pos + 6;
  else if (strncmp(pos, "xmlns", 5) == 0)
    return pos + 5;
  else
    return pos;
}

// move to a particular nested unit
void srcMLUtility::move_to_unit(int unitnumber, srcMLUtility&su, OPTION_TYPE options, int optioncount, int optionorder[]) {

  // setup parser
  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  Properties process(su, nsv, attrv, optioncount, optionorder);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unitnumber);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit >= 1 && state.count != state.unit)
     throw OutOfRangeUnitError(state.count);

  units = state.count;

  /*
  // output entire unit element
  xmlSAXHandler sax = SAX2Properties::factory();

  SAX2Properties state(unitnumber, options, nsv, attrv, su, optioncount, optionorder);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  srcMLParseDocument(ctxt);

  // encoding is entered as a property
  //  const char* encoding = (const char*) (state.ctxt->encoding ? state.ctxt->encoding : state.ctxt->input->encoding);
  //  if (encoding)
  //    attrv.insert(attrv.end(), PROPERTIES_TYPE::value_type(".encoding", encoding));

  // don't let the context free the static sax handler
  ctxt->sax = NULL;

  // now free the context
  xmlFreeParserCtxt(ctxt);

  */
}


// count of nested units
int srcMLUtility::unit_count() {

  // setup parser
  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return -1;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  CountUnits process;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  return state.count;
}

// extract a given unit
void srcMLUtility::extract_xml(const char* ofilename, int unit) {

  // setup parser
  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsXML process(0 /* to_directory is stdout */, ofilename, output_encoding);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
void srcMLUtility::extract_element(const char* element, const char* filename) {
  /*
  xmlTextReaderPtr reader = xmlNewTextReaderFilename(infile);

  const char* paths[] = { NULL };

  // perform xpath evaluation
  //  srceval(element, paths, "", "", reader, filename);

  xmlFreeTextReader(reader);
  */
  /*
  int unit;

  // output entire unit element
  xmlSAXHandler sax = SAX2CopyElement::factory();

  SAX2CopyElement state(element, filename, unit, options);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  srcMLParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
  */
}

// extract a given unit
void srcMLUtility::extract_text(const char* to_dir, const char* ofilename, int unit) {

#if 0
  if (xmlRegisterOutputCallbacks(archiveWriteMatch_src2srcml, archiveWriteOpen, archiveWrite, archiveWriteClose) < 0) {
      fprintf(stderr, "%s: failed to register archive handler\n", "FOO");
      exit(1);
    }

  if (archiveWriteMatch_src2srcml(ofilename)) {
    archiveWriteOutputFormat(ofilename);

    archiveWriteRootOpen(ofilename);
  }
#endif

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsSrc process(0 /* to_directory is stdout */, ofilename, output_encoding);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt);

#if 0
  if (archiveWriteMatch_src2srcml(ofilename))
    archiveWriteRootClose(0);
#endif
  
  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// expand the compound srcML to individual files
void srcMLUtility::expand(const char* root_filename, const char* format, const char* to_directory) {

  if (xmlRegisterOutputCallbacks(archiveWriteMatch_srcml2src, archiveWriteOpen, archiveWrite, archiveWriteClose) < 0) {
      fprintf(stderr, "%s: failed to register archive handler\n", "FOO");
      exit(1);
    }

  archiveWriteOutputFormat(format);

  archiveWriteRootOpen(root_filename);

  // setup parser
  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsSrc process(to_directory, root_filename, output_encoding);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt);

  archiveWriteRootClose(0);
  
  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);
}

// list the elements
void srcMLUtility::list() {

  // setup parser
  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ListUnits process;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);
}
/*
// namespaces and prefixes
const PROPERTIES_TYPE* srcMLUtility::getNS() const {
  return nsv;
}
*/

// set the input filename after the constructor
void srcMLUtility::set_input_filename(const char* infilename) {
  infile = infilename;
}


// xpath evaluation of the nested units
void srcMLUtility::xpath(const char* ofilename, const char* context_element, const char* xpaths[]) {

  xmlSAXHandler sax = SAX2UnitDOMXPath::factory();
  
  SAX2UnitDOMXPath state(context_element, xpaths, ofilename, options);

  // relative xpath changed to at any level
  std::string s = state.fxpath[0];
  //  if (s[0] != '/')
  //    s = "//" + s;

  // compile the xpath that will be applied to each unit
  state.compiled_xpath = xmlXPathCompile(BAD_CAST s.c_str());
  if (state.compiled_xpath == 0) {
    return;
  }

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  srcMLParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

// xslt evaluation of the nested units
void srcMLUtility::xslt(const char* context_element, const char* ofilename, const char* xslts[], const char* params[], int paramcount) {

  xmlSAXHandler sax = SAX2UnitDOMXSLT::factory();
  
  SAX2UnitDOMXSLT state(context_element, xslts, ofilename, params, paramcount, options);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  // allow for all exslt functions
  exsltRegisterAll();

  xsltsrcMLRegister();

  // parse the stylesheet
  state.xslt = xsltParseStylesheetFile(BAD_CAST xslts[0]);
  // TODO: error return

  srcMLParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

// relaxng evaluation of the nested units
void srcMLUtility::relaxng(const char* ofilename, const char** xslts) {

  xmlSAXHandler sax = SAX2UnitDOMRelaxNG::factory();
  
  SAX2UnitDOMRelaxNG state(0, xslts, ofilename, 0);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  // parse the stylesheet
  state.relaxng = xmlRelaxNGNewParserCtxt(state.fxslt[0]);
  state.rng = xmlRelaxNGParse(state.relaxng);
  state.rngptr = xmlRelaxNGNewValidCtxt(state.rng);

  srcMLParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}


// process srcML document with error reporting
static void srcMLParseDocument(xmlParserCtxtPtr ctxt) {

  // process the document
  int status;
  if ((status = xmlParseDocument(ctxt)) == -1) {

    // report error
    xmlErrorPtr ep = xmlCtxtGetLastError(ctxt);
    char* partmsg = strdup(ep->message);
    partmsg[strlen(partmsg) - 1] = '\0';
    fprintf(stderr, "%s: %s in '%s'\n", "srcml2src", partmsg, ep->file);
    exit(1);
  }

}

// create srcml parser with error reporting
static xmlParserCtxtPtr srcMLCreateURLParserCtxt(const char * infile) {

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) {

    // report error
    xmlErrorPtr ep = xmlGetLastError();
    fprintf(stderr, "%s: %s", "srcml2src", ep->message);
    exit(1);
  }

  return ctxt;

}
