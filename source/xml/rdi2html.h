#ifndef RDI2HTML_H
#define RDI2HTML_H

#include <memory>
#include <string>

using namespace std;

#include "rdiReader.h"

class rdi2html
{
public:
  rdi2html(const char* rdi_filepath, const string& out_filepath);
  ~rdi2html();

  void transform();

protected:
  auto_ptr<rdiReader> m_rdiReader;

  const string m_out_filepath;
};

#endif // RDI2HTML_H
