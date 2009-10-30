#ifndef __itkVisualSonicsSeriesReader_txx
#define __itkVisualSonicsSeriesReader_txx

#include "itkVisualSonicsSeriesReader.h"

#include "itkVisualSonicsImageIO.h"

namespace itk
{


template <class TOutputImage>
VisualSonicsSeriesReader< TOutputImage >
::VisualSonicsSeriesReader()
{
  this->SetImageIO( VisualSonicsImageIO::New() );
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::GenerateOutputInformation(void)
{
  typename TOutputImage::Pointer output = this->GetOutput();

  ImageRegionType largestRegion;
  typename TOutputImage::PointType     origin;

  origin.Fill( 0.0 );

  std::string key("ITK_ImageOrigin");
  // Clear the eventual previous content of the MetaDictionary array
  if( this->m_MetaDataDictionaryArray.size() )
    {
    for(unsigned int i=0; i<this->m_MetaDataDictionaryArray.size(); i++)
      {
      // each element is a raw pointer, delete them.
      delete this->m_MetaDataDictionaryArray[i];
      }
    }
  this->m_MetaDataDictionaryArray.clear();

  if ( this->m_FileNames.size() == 0 )
    {
    itkExceptionMacro(<< "At least one filename is required." );
    }

  SpacingType spacing = this->GenerateSubRegions();

  typename SubRegionsVectorType::const_iterator sub_it = m_SubRegionsVector.begin();
  IndexType index = sub_it->GetIndex();
  SizeType  size  = sub_it->GetSize();
  for( ++sub_it;
    sub_it != m_SubRegionsVector.end();
    ++sub_it )
    {
    size[0] += sub_it->GetSize()[0];
    }
  largestRegion.SetIndex( index );
  largestRegion.SetSize(  size  );

  output->SetOrigin( origin );
  output->SetSpacing( spacing );
  output->SetLargestPossibleRegion( largestRegion );

}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::EnlargeOutputRequestedRegion( DataObject* output )
{
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "SubRegionsVector Size: " << m_SubRegionsVector.size() << std::endl;
  for( size_t i = 0; i < m_SubRegionsVector.size(); i++ )
    {
    os << indent << "SubRegionsVector[" << i << "]:" << std::endl;
    m_SubRegionsVector[i].Print(os, indent.GetNextIndent() );
    }
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::GenerateData()
{
}


template < class TOutputImage >
typename VisualSonicsSeriesReader< TOutputImage >::SpacingType
VisualSonicsSeriesReader< TOutputImage >
::GenerateSubRegions()
{
  m_SubRegionsVector.resize( this->m_FileNames.size() );

  const int numberOfFiles = static_cast<int>( this->m_FileNames.size());
  const int FileName = ( this->m_ReverseOrder ? numberOfFiles - 1: 0 );

  typename ReaderType::Pointer first_reader = ReaderType::New();
  first_reader->SetFileName( this->m_FileNames[FileName].c_str() );
  first_reader->SetImageIO( this->m_ImageIO );

  first_reader->UpdateOutputInformation();
  SpacingType spacing = first_reader->GetOutput()->GetSpacing();

  m_SubRegionsVector[0] = first_reader->GetOutput()->GetLargestPossibleRegion();

  SizeType first_size  = first_reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  IndexType first_index = first_reader->GetOutput()->GetLargestPossibleRegion().GetIndex();

  DictionaryRawPointer newDictionary = new DictionaryType;
  *newDictionary = first_reader->GetImageIO()->GetMetaDataDictionary();
  this->m_MetaDataDictionaryArray.push_back( newDictionary );

  const MetaDataObjectBase* radius_meta_object_base = (*newDictionary)["Radius"];
  const MetaDataObject< double >* radius_meta_object = dynamic_cast< const MetaDataObject< double >* >( radius_meta_object_base );
  double previous_radius_plus_extent = radius_meta_object->GetMetaDataObjectValue() + (first_index[0] + first_size[0]) * spacing[0];

  SizeType  next_size;
  IndexType next_index;
  ImageRegionType next_region;
  double next_radius;

  // axial overlap in distance [m]
  double overlap;
  unsigned int overlap_size;

  for ( int i = 1; i != numberOfFiles; ++i )
    {
    const int iFileName = ( this->m_ReverseOrder ? numberOfFiles - i - 1: i );

    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( this->m_FileNames[iFileName].c_str() );
    reader->SetImageIO( this->m_ImageIO );

    reader->UpdateOutputInformation();

    next_size  = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
    next_index = reader->GetOutput()->GetLargestPossibleRegion().GetIndex();

    if( next_size[1]  != first_size[1] ||
	next_size[2]  != first_size[2] ||
	next_index[1] != first_index[1] ||
	next_index[2] != first_index[2] )
      {
      itkExceptionMacro( << "Images have inconsistent sizes or indices in the lateral or elevational directions." );
      }

    newDictionary = new DictionaryType;
    *newDictionary = reader->GetImageIO()->GetMetaDataDictionary();
    this->m_MetaDataDictionaryArray.push_back( newDictionary );

    radius_meta_object_base = (*newDictionary)["Radius"];
    radius_meta_object      = dynamic_cast< const MetaDataObject< double >* >( radius_meta_object_base );

    next_radius = radius_meta_object->GetMetaDataObjectValue();

    overlap = previous_radius_plus_extent - next_radius;

    if( overlap <= 0.0 )
      {
      itkExceptionMacro( << "VisualSonics sub images do not overlap." );
      }

    overlap_size = static_cast< unsigned int >( overlap / spacing[0] );
    previous_radius_plus_extent = next_radius + (next_index[0] + next_size[0]) * spacing[0];

    next_index[0] = next_index[0] + overlap_size;
    next_size[0]  = next_size[0]  - overlap_size;

    next_region.SetSize( next_size );
    next_region.SetIndex( next_index );

    m_SubRegionsVector[i] = next_region;
    }

  return spacing;
}

} // end namespace itk

#endif // inclusion guard
