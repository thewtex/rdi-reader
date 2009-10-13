
#set(spirit_parse_source_dir ${RDI_READER_SOURCE_DIR}/source/common/spirit_parse)

#set(base_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    #${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/ImageReader.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    #)
#include_directories( ${RDI_READER_SOURCE_DIR}/source )

#add_library( vs_common SHARED ${base_srcs} )
##project( PARAMETRIC_US )

include_directories( ${RDI_READER_SOURCE_DIR}/source )
include_directories( ${RDI_READER_SOURCE_DIR}/source/itk )

find_package( ITK )
if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
else( ITK_FOUND )
  message( FATAL_ERROR "ITK not found. Please set ITK_DIR." )
endif( ITK_FOUND )


#add_executable( start
  #start.cpp
  #)

#target_link_libraries( start
  #ITKCommon
  #ITKIO
  #ITKAlgorithms
  #boost_filesystem
  #parse_vs_rdi
  #)
