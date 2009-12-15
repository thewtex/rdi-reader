#include <string>

#include <gtest/gtest.h>

#include "rdiReader.h"

using namespace std;

string filepath;

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
  auto_ptr<rdi_t> rdi_i = rdi_reader.parse_to_rdi_t();
  EXPECT_EQ(string("pat147"), rdi_i->image_info().Study_Name()) << "Invalid Study Name";
  EXPECT_EQ(29, rdi_i->image_info().Image_Frames()) << "Invalid Image Frames";
  EXPECT_EQ(50, rdi_i->image_info().Image_Lines()) << "Invalid Image Lines";
  EXPECT_EQ(4480, rdi_i->image_info().Image_Acquisition_Size()) << "Invalid Image Acquisition Size";
}

TEST_F(rdiReaderTest, ImageParametersSection)
{
  auto_ptr<rdi_t> rdi_i = rdi_reader.parse_to_rdi_t();
  EXPECT_EQ(string("Rat Cardiology"), rdi_i->image_parameters().RF_Mode().ActiveProbe().Notes()) << "Invalid Active Probe Notes";
  EXPECT_EQ(string("µs"), rdi_i->image_parameters().RF_Mode().ActiveProbe().Sample_Time().units()) << "Invalid Sample Time units";
  EXPECT_FLOAT_EQ(154.0, rdi_i->image_parameters().RF_Mode().ActiveProbe().Sample_Time()) << "Invalid Sample Time";
  EXPECT_EQ(string("mm"), rdi_i->image_parameters().RF_Mode().ActiveProbe().Encoder_Range_Max().units()) << "Invalid Encoder Range Max units";
  EXPECT_FLOAT_EQ(13.75, rdi_i->image_parameters().RF_Mode().ActiveProbe().Encoder_Range_Max()) << "Invalid Encoder Range Max";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    filepath = argv[1];

    return RUN_ALL_TESTS();
}
