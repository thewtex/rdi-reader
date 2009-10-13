#set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)




#set(parse_vs_rdi_lib_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    #${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReader.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/parse_vs_rdi.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
				    #)


#add_library( parse_vs_rdi SHARED ${parse_vs_rdi_lib_srcs} )

if(ANALYZE_RDI_FORMAT)
  add_subdirectory(formats)
endif(ANALYZE_RDI_FORMAT)
