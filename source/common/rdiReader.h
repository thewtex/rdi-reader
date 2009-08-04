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
#include <vector>
using namespace std;

#include "common/ToXMLCh.h"

class rdiReader
{
public:
  rdiReader(const char* filepath);
  ~rdiReader();

  void parse();

protected:
  const std::string m_filepath;

  /**
   * @brief for transcoding file content to the internal XMLCh*, UTF-16 strings
   */
  ToXMLCh* m_transcoder;

  class LineParser
    {
  public:
    /**
     * @brief First string is considered to contain the element names
     *
     * May be nested, sub-elements separated by '/'
     */
    vector<string> elements;
    /**
     * @brief element content
     */
    string content;
    /**
     * @brief the third field is often the units for the field, but this is
     * optional
     */
    string units;

    /**
     * @brief Internal function for splitting up and processing the content of a line in the .rdi
     * file
     *
     * @param f the .rdi file
     */
    void parse_line(istream& f);
  private:
    string m_line;
    }; // end LineParser nested class
  LineParser m_line_parser;

};

#endif // RDIREADER_H
