
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xsd/cxx/tree/exceptions.hxx>

#include "rdiReader.h"

/**
 * @brief print program usage
 *
 * @param program_name
 *
 * @return exit error num
 */
int usage(char* program_name)
{
    cerr << "usage: " << program_name << " filename.rdi [output_filename.xml]" << endl;
    return 1;
}


int main(int argc, char* argv[])
{
  if(argc < 2)
    return usage(argv[0]);
  string out_file(argv[1]);
  if(argc > 3 ||
     out_file.length() < 4 ||
     !(out_file.substr(out_file.length()-4) == ".rdi") ||
     out_file == "--help"
     )
    return usage(argv[0]);

  try
  {
    rdiReader rdi_reader(argv[1]);

    auto_ptr<rdi_t> rdi_i = rdi_reader.parse();

    if(argc == 2)
      out_file = out_file.substr(0, out_file.length()-4) + ".xml";
    else
      out_file = string(argv[2]);
    std::ofstream ofs(out_file.c_str());
    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "rdi.xsd";
    rdi(ofs, *rdi_i, map);
  }
  // @todo move some of the exception handling up into rdiReader excecution,
  // throw a general more general exception with informative e.what() after
  // caught
  catch (const ifstream::failure& e)
    {
    cerr << "Error: failure opening/reading file." << endl;
    return 1;
    }
  // xerces-C++
  catch (const xercesc::XMLException& e)
    {
    char* msg = xercesc::XMLString::transcode(e.getMessage());
    cerr << "Error: In Xerces-C++ xml processing: " << msg << endl;
    xercesc::XMLString::release(&msg);
    return 1;
    }
  catch (const xercesc::DOMException& e)
    {

    cerr << "Error: In Xerces-C++ DOM processing, code: " << e.code << endl;
    cerr << "See http://xerces.apache.org/xerces-c/apiDocs-3/classDOMException.html for an explanation." << endl;
    return 1;
    }
  catch (const xsd::cxx::tree::expected_element<char>& e)
    {
    cerr << "Error: " << e.what() << ": '" << e.name() << "' in namespace: " << e.namespace_() << endl;
    return 1;
    }
  catch (const std::exception& e)
    {
    cerr << "Error: "  << e.what() << endl;
    return 1;
    }
  return 0;
}

