#include <iostream>

using namespace std;

/**
 * @brief print program usage
 *
 * @param program_name
 *
 * @return exit error num
 */
int usage(char* program_name)
{
    cerr << "usage: " << program_name << " filename.rdi [output_filename.html]" << endl;
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
    if(argc == 2)
      out_file = out_file.substr(0, out_file.length()-4) + ".xml";
    else
      out_file = string(argv[2]);
  }
  catch (const std::exception& e)
    {
    cerr << "Error: "  << e.what() << endl;
    return 1;
    }
  return 0;
}
