#include "BinaryReader.h"

//constructors
BinaryReader::BinaryReader(const QString& file)
{
  //init variables
  m_pos = 0;
  m_error = false;

  //read the file
  QFile toRead(file);
  if (toRead.open(QIODevice::ReadOnly))
  {
    m_bytes = toRead.readAll();
  } //end  if (toRead.open(QIODevice::ReadOnly)
  else
  {
    //error opening file
    m_error = true;
  } //end  els
}

//public functions
QVector<std::uint8_t> BinaryReader::readBytes(int numBytes)
{
  if (!m_error)
  {
    QVector<std::uint8_t> bytesRead;
    int i = 0;
    while (i < numBytes && m_pos < m_bytes.size())
    {
      //read byte from the current position in the file
      bytesRead.push_back(static_cast<std::uint8_t>(m_bytes.at(m_pos)));
      i++; //update the current iteration to track number of bytes read
      m_pos++; //update position in the file
    } //end  while (i < numBytes && m_pos < m_bytes.size()
    return bytesRead;
  } //end  if (!m_error
  else
  {
    return QVector<std::uint8_t>();
  } //end  els
}
std::uint8_t BinaryReader::readUint8()
{
  return static_cast<std::uint8_t>(readBytes(1)[0]);
}
std::uint16_t BinaryReader::readUint16()
{
  auto bytesRead = readBytes(2);
  return static_cast<std::uint16_t>((bytesRead[0] << 8) | bytesRead[1]);
}
std::uint32_t BinaryReader::readUint32()
{
  auto bytesRead = readBytes(4);
  return static_cast<std::uint32_t>((bytesRead[0] << 24) | (bytesRead[1] << 16) | 
                                    (bytesRead [2] << 8) | bytesRead[3]);
}
std::uint64_t BinaryReader::readUint64()
{
  auto bytesRead = readBytes(8);
  std::uint64_t toReturn = 0;
  for (int i = 7; i >= 0; i--)
  {
    toReturn |= (bytesRead[7 - i] << (i * 8));
  } //end  for (int i = 7; i >= 0; i--
  return toReturn;
}
std::int8_t BinaryReader::readInt8()
{
  return static_cast<std::int8_t>(readBytes(1)[0]);
}
std::int16_t BinaryReader::readInt16()
{
  auto bytesRead = readBytes(2);
  return static_cast<std::int16_t>((bytesRead[0] << 8) | bytesRead[1]);
}
std::int32_t BinaryReader::readInt32()
{
  auto bytesRead = readBytes(4);
  return static_cast<std::int32_t>((bytesRead[0] << 24) | (bytesRead[1] << 16) | 
                                   (bytesRead [2] << 8) | bytesRead[3]);
}
std::int64_t BinaryReader::readInt64()
{
  auto bytesRead = readBytes(8);
  std::int64_t toReturn = 0;
  for (int i = 7; i >= 0; i--)
  {
    toReturn |= (bytesRead[7 - i] << (i * 8));
  } //end  for (int i = 7; i >= 0; i--
  return toReturn;
}
bool BinaryReader::readyToRead() const
{
  return !m_error;
}
bool BinaryReader::eof() const
{
  return m_pos >= m_bytes.size();
}

//getters
int BinaryReader::getPos() const
{
  return m_pos;
}
