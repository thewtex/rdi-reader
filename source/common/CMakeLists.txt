#set(spirit_parse_source_dir ${RDI_READER_SOURCE_DIR}/source/common/spirit_parse)




#set(parse_vs_rdi_lib_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    #${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/ImageReader.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
				    #${RDI_READER_SOURCE_DIR}/source/common/parse_vs_rdi.cpp
                                    #${RDI_READER_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    #${RDI_READER_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
				    #${RDI_READER_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
				    #${RDI_READER_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
				    #)


#add_library( parse_vs_rdi SHARED ${parse_vs_rdi_lib_srcs} )

if(ANALYZE_RDI_FORMAT)
  add_subdirectory(formats)
endif(ANALYZE_RDI_FORMAT)
