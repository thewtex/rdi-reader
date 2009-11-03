#include "ToXMLCh.h"

#include <cstring>
#include <stdexcept>
using namespace std;

#include <xercesc/util/PlatformUtils.hpp>

/**
 * @brief buffer length for transcoding
 */
const static unsigned int MAX_TRANSCODING_LENGTH = 4096;

ToXMLCh::ToXMLCh():
  m_maxChars(MAX_TRANSCODING_LENGTH)
{
  XMLTransService::Codes failReason;
  m_transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(
    "ISO-8859-1", failReason, MAX_TRANSCODING_LENGTH);
  if(failReason != XMLTransService::Ok)
    throw(logic_error("Could not create transcoder."));

  m_toFill = new XMLCh[MAX_TRANSCODING_LENGTH];
  m_charSizes = new unsigned char[MAX_TRANSCODING_LENGTH];
}


ToXMLCh::~ToXMLCh()
{
  delete m_transcoder;
  delete[] m_toFill;
  delete[] m_charSizes;
}


void ToXMLCh::setInput(const char* to_translate)
{
  m_srcData = reinterpret_cast<const XMLByte*>(to_translate);
  // +1 for null char
  m_srcCount = static_cast<XMLSize_t>(strlen(to_translate)+1);
}


const XMLCh* ToXMLCh::getOutput()
{
  m_transcoder->transcodeFrom(m_srcData,
    m_srcCount,
    m_toFill,
    m_maxChars,
    m_bytesEaten,
    m_charSizes);

  return const_cast<const XMLCh*>(m_toFill);
}


const XMLCh* ToXMLCh::transcode(const char* to_transcode)
{
  this->setInput(to_transcode);
  return this->getOutput();
}
