#include "rdi2html.h"

#include <stdexcept>
#include <string>

using namespace std;

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)

#include "rdiReader.h"

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

  const XSLTInputSource stylesheet_input_source("rdi2html.xsl");
  const XalanCompiledStylesheet* stylesheet = 0;
  if(transformer.compileStylesheet(stylesheet_input_source, stylesheet) != 0)
    {
    throw logic_error(
      string("An error occurred compiling the stylesheet: ") +
      string(transformer.getLastError())
      );
    }
  const XSLTResultTarget  theResultTarget(m_out_filepath.c_str());
}

