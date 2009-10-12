/**
 * @file rdiReader.h
 * @brief Read the rdi file contents into the XML data object represention
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @version
 * @date 2009-07-13
 */

#ifndef RDIREADER_H
#define RDIREADER_H

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "common/formats/rdi.hxx"
#include "common/ToXMLCh.h"

class rdiReader
{
public:
  rdiReader(const char* filepath);
  ~rdiReader();

  std::auto_ptr<rdi_t> parse();

protected:
  const std::string m_filepath;

  /**
   * @brief for transcoding file content to the internal XMLCh*, UTF-16 strings
   */
  ToXMLCh* m_transcoder;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument>
  parse_IMAGE_INFO_section(std::ifstream& infile,
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc);
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument>
  parse_IMAGE_DATA_section(std::ifstream& infile,
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc);
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument>
  parse_IMAGE_PARAMETERS_section(std::ifstream& infile,
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc);

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
     *
     * @return if there is another valid line to get parsed
     *
     * Requires the first " to be stripped and strips " from the next line
     */
    bool parse_line(istream& f);
  private:
    /**
     * @brief purify the proposed element name so that it is a valid XML element
     * name
     *
     * @param element_name
     *
     * @return purified element name
     */
    string make_element_name_valid(const string& element_name);
    string m_subline;
    }; // end LineParser nested class
  LineParser m_line_parser;

};

#endif // RDIREADER_H
