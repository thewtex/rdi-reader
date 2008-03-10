/*! @file   parse_vi_rdi.cpp
 *  @brief  a library call for parsing a VisualSonics .rdi file
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date   2007 February 4
 */

#ifndef	  PARSE_VS_RDI_H
#define	  PARSE_VS_RDI_H


#include <string>

#include "rdiParserData.h"

#pragma GCC visibility push(default)
/*! parse the metadata out of a VisualSonics .rdi file
 * omit the '.rdi' extension in the filename
 * @param filename
 */
visual_sonics::rdiParserData parse_vs_rdi(const std::string& filename);
#pragma GCC visibility pop

#endif // inclusion guard

