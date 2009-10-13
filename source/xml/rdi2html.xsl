<?xml version='1.0' encoding='UTF-8'?>


<xsl:stylesheet version='1.0'
  xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match='/rdi'>
  <html>
    <body>
      <h1>VisualSonics rdi file contents</h1>
      <xsl:apply-templates select='image_info'/>
      <table border='1'>
      </table>
    </body>
  </html>
</xsl:template>

<xsl:template match='image_info'>
  <h2>Image Info Section</h2>
  <table border='1'>
    <tr bgcolor='#9acd32'>
      <th>Parameter</th>
      <th>Value</th>
    </tr>
    <xsl:for-each select='./*'>
    <tr>
      <td><xsl:value-of select='local-name()'/></td>
      <td><xsl:value-of select='.'/></td>
    </tr>
    </xsl:for-each>
  </table>
</xsl:template>

</xsl:stylesheet>
