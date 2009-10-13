#include "rdi2html.h"

#include <stdexcept>
#include <string>

using namespace std;

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>

XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XercesDOMSupport)
XALAN_USING_XALAN(XercesDOMWrapperParsedSource)
XALAN_USING_XALAN(XercesParserLiaison)
XALAN_USING_XALAN(XSLTInputSource)

#include "rdiReader.h"

const static char* STYLESHEET =
#include "rdi2html.xsl.cstringliteral"

rdi2html::rdi2html(const char* rdi_filepath, const string& out_filepath):
  m_rdiReader(new rdiReader(rdi_filepath)),
  m_out_filepath(out_filepath)
{
  // Initialize Xalan.
  XalanTransformer::initialize();
}


rdi2html::~rdi2html()
{
  // Terminate Xalan...
  XalanTransformer::terminate();
}


void rdi2html::transform()
{
  XalanTransformer transformer;

  istringstream xsl_stream(STYLESHEET);
  const XSLTInputSource stylesheet_input_source(xsl_stream);
  const XalanCompiledStylesheet* stylesheet = 0;
  if(transformer.compileStylesheet(stylesheet_input_source, stylesheet) != 0)
    {
    throw logic_error(
      string("An error occurred compiling the stylesheet: ") +
      string(transformer.getLastError())
      );
    }

  xml_schema::dom::auto_ptr< xercesc::DOMDocument> domdoc = m_rdiReader->parse_to_DOMDocument();
  domdoc->normalize();

  XercesParserLiaison parser_liaison;
  XercesDOMSupport DOM_support(parser_liaison);

  const XercesDOMWrapperParsedSource parsed_source(
    domdoc.get(),
    parser_liaison,
    DOM_support
  );

  if(transformer.transform(parsed_source, stylesheet, m_out_filepath.c_str()) != 0)
    {
    throw logic_error(
      string("An error occurred compiling the stylesheet: ") +
      string(transformer.getLastError())
      );
    }
}

