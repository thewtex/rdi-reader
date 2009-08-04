/**
 * @file ToXMLCh.h
 * @brief Translate from UTF8/ASCII to the XMLCh used by XercesC++ which is
 * UTF-16
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @version
 * @date 2009-08-04
 */

#ifndef TOXMLCH_H
#define TOXMLCH_H

#include <xercesc/util/TransService.hpp>
using namespace xercesc;

class ToXMLCh
{
public:
  ToXMLCh();
  ~ToXMLCh();

  void setInput(const char* to_transcode);
  const XMLCh* getOutput();

  /**
   * @brief convenience function: call setInput(to_translate), then getOutput()
   *
   * @param to_translate
   *
   * @return
   */
  const XMLCh* transcode(const char* to_transcode);

protected:
  XMLTranscoder* m_transcoder;

  // following the naming convention from XMLTranscoder::transcodeFrom docs...
  const XMLByte* m_srcData;
  XMLSize_t m_srcCount;
  XMLCh* m_toFill;
  const XMLSize_t m_maxChars;
  XMLSize_t m_bytesEaten;
  unsigned char* m_charSizes;
};

#endif // TOXMLCH_H
