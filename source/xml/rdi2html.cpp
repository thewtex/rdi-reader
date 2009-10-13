#include "rdi2html.h"

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

XALAN_USING_XALAN(XalanTransformer)

#include "rdiReader.h"

rdi2html::rdi2html(const char* rdi_filepath):
  m_rdiReader(new rdiReader(rdi_filepath))
{
  // Initialize Xalan.
  XalanTransformer::initialize();
}


rdi2html::~rdi2html()
{
  // Terminate Xalan...
  XalanTransformer::terminate();
}
