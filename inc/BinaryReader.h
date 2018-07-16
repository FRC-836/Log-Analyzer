#ifndef LOG_ANALYZER_BINARY_READER_H
#define LOG_ANALYZER_BINARY_READER_H

#include <stdint.h>

#include <qfile.h>
#include <qbytearray.h>
#include <qstring.h>
#include <qvector.h>

class BinaryReader
{
protected:
  //member variables
  int m_pos ; //current position in the byte array
  QByteArray m_bytes; //bytes read from the binary file
  bool m_error;

public:
  //constructors
  BinaryReader(const QString& file);
  virtual ~BinaryReader() {}

  //public functions
  virtual QVector<std::uint8_t> readBytes(int numBytes);
  virtual std::uint8_t readUint8();
  virtual std::uint16_t readUint16();
  virtual std::uint32_t readUint32();
  virtual std::uint64_t readUint64();
  virtual std::int8_t readInt8();
  virtual std::int16_t readInt16();
  virtual std::int32_t readInt32();
  virtual std::int64_t readInt64();
  virtual bool readyToRead() const;

  //getters
  virtual int getPos() const;

  //setters
};

#endif