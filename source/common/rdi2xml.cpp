
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xsd/cxx/tree/exceptions.hxx>

#include "rdiReader.h"


int main(int argc, char* argv[])
{
  string out_file(argv[1]);
  if(argc < 2 ||
      argc > 3 ||
      out_file.substr(out_file.length()-5) == ".rdi")
    {
    cerr << "usage: " << argv[0] << " filename.rdi <output_filename.xml>" << endl;
    return 1;
    }

  try
  {
    rdiReader rdi_reader(argv[1]);

    auto_ptr<rdi_t> rdi_i = rdi_reader.parse();

    // debug
    cout << "Acquisition Mode: " << rdi_i->image_info().Acquisition_Mode() << endl;
    cout << "Image Frames: "  << rdi_i->image_info().Image_Frames() << endl;

    // end debug

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
    cerr << "Error: " << e.what() << ": '" << e.name() << "' in " << e.namespace_() << endl;
    return 1;
    }
  catch (const std::exception& e)
    {
    cerr << "Error: "  << e.what() << endl;
    return 1;
    }
  return 0;
}

