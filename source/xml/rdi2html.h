#ifndef RDI2HTML_H
#define RDI2HTML_H

#include <memory>

using namespace std;

#include "rdiReader.h"

class rdi2html
{
public:
  rdi2html(const char* rdi_filepath);
  ~rdi2html();

protected:
  auto_ptr<rdiReader> m_rdiReader;

};

#endif // RDI2HTML_H
