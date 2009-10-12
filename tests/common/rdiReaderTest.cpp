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
  EXPECT_EQ(string("plaque_pat_143_right"), rdi_i->image_info().Study_Name()) << "Invalid Study Name";
  EXPECT_EQ(246, rdi_i->image_info().Image_Frames()) << "Invalid Image Frames";
  EXPECT_EQ(250, rdi_i->image_info().Image_Lines()) << "Invalid Image Lines";
  EXPECT_EQ(4256, rdi_i->image_info().Image_Acquisition_Size()) << "Invalid Image Acquisition Size";

}
