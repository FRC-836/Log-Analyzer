#include "BinaryReader.h"

//constructors
BinaryReader::BinaryReader(const QString& file)
{
  m_pos = 0;
  QFile toRead(file);
  if (
}

//public functions
QVector<std::uint8_t> BinaryReader::readBytes(int numBytes)
{
}
std::uint8_t BinaryReader::readUint8()
{
}
std::uint16_t BinaryReader::readUint16()
{
}
std::uint32_t BinaryReader::readUint32()
{
}
std::uint64_t BinaryReader::readUint64()
{
}
std::int8_t BinaryReader::readInt8()
{
}
std::int16_t BinaryReader::readInt16()
{
}
std::int32_t BinaryReader::readInt32()
{
}
std::int64_t BinaryReader::readInt64()
{
}

//getters
int BinaryReader::getPos() const
{
}
