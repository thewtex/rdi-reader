/**
 * @file rdiReader.h
 * @brief Read the rdi file contents into the XML data object represention
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @version
 * @date 2009-07-13
 */

#ifndef RDIREADER_H
#define RDIREADER_H

#include <string>

#include <xercesc/util/TransService.hpp>
using namespace xercesc;

class rdiReader
{
public:
  rdiReader(const char* filepath);
  ~rdiReader();

  void parse();

protected:
  const std::string m_filepath;
};

#endif // RDIREADER_H
