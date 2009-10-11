#include <string>

#include <gtest/gtest.h>

#include "rdiReader.h"

using namespace std;

static const string filepath = "@RDIREADER_TEST_FILEPATH@";

class rdiReaderTest: public ::testing::Test
{
protected:

  rdiReader rdi_reader;

  rdiReaderTest():
    rdi_reader(filepath.c_str())
  {}
};


TEST_F(rdiReaderTest, ImageInfoSection)
{
  auto_ptr<rdi_t> rdi_i = rdi_reader.parse();
  EXPECT_EQ(4256, rdi_i->image_info().Image_Acquisition_Size()) << "Invalid Image Acquisition Size";
}
