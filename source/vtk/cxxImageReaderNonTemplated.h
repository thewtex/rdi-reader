/*! @file  cxxImageReaderNonTemplated.h
 *  @brief cxx::ImageReader without templates because vtk is template discrimatory
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 May 19
 *
 */

#ifndef CXXIMAGEREADERNONTEMPLATED_H
#define CXXIMAGEREADERNONTEMPLATED_H


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;



#include "cxx/ImageReader.h"


namespace visual_sonics
{
  namespace vtk
  {
    //! Base class for inclusion as a class member
    class cxxImageReaderNonTemplatedBase
    {
    public:
    virtual void* get_ir() = 0;

    virtual ~cxxImageReaderNonTemplatedBase(){};

    protected:
    //! to be reinterpreted as the appropriately typed cxx::ImageReader
    void* its_ir;
    };




    //! Derived class for creating specific cxx::ImageReaders
    template< class ImageDataOutT, class CoordT > 
    class cxxImageReaderNonTemplatedDerived: public cxxImageReaderNonTemplatedBase
    {
      public:
      cxxImageReaderNonTemplatedDerived( const bf::path& in_file_path ) 
      { 
	its_ir = new visual_sonics::cxx::ImageReader<ImageDataOutT, CoordT>(in_file_path);
      }
      virtual ~cxxImageReaderNonTemplatedDerived()
      {
	delete its_ir;
      }

      virtual visual_sonics::cxx::ImageReader<ImageDataOutT, CoordT>* get_ir()
      {
	return reinterpret_cast< visual_sonics::cxx::ImageReader<ImageDataOutT, CoordT> > ( its_ir );
      }
    };

  }
}


#endif // CXXIMAGEREADERNONTEMPLATED_H inclusion guard

