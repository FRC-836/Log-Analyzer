#include "DsLogReader.h"

//private functions
void DsLogReader::readFile(const QString& path)
{
  BinaryReader reader(path);
  if (reader.readyToRead())
  {
    m_version = reader.readInt32();
    if (m_version == 3)
    {
      m_startTime = FromLVTime(reader.readInt64(), reader.readUint64());
      int i = 0;
      while (reader.getPos() < reader.size())
      {
        m_entries.push_back(DsLogEntry(TripTimeToDouble(reader.readUint8()), 
                                       PacketLossToDouble(reader.readInt8()), 
                                       VoltageToDouble(reader.readUint16()), 
                                       RoboRioCPUToDouble(reader.readUint8()), 
                                       StatusFlagsToBooleanArray(reader.readUint8()), 
                                       CANUtilToDouble(reader.readUint8()), 
                                       WifidBToDouble(reader.readUint8()), 
                                       BandwidthToDouble(reader.readUint16()), 
                                       reader.readUint8(), 
                                       PDPValuesToArrayList(reader.readBytes(21)), 
                                       reader.readUint8(), 
                                       reader.readUint8(), 
                                       reader.readUint8(), 
                                       m_startTime.addMSecs(20 * i++)));
      } //end  while (reader.getPos() < reader.size()
    }
  } //end  if (reader.readyToRead()
  else
  {
  } //end  els
}
QDateTime DsLogReader::FromLVTime(long unixTime, std::uint64_t ummm)
{
  var epoch = new DateTime(1904, 1, 1, 0, 0, 0, DateTimeKind.Utc);
  epoch = epoch.AddSeconds(unixTime);
  epoch = TimeZoneInfo.ConvertTimeFromUtc(epoch, TimeZoneInfo.Local);

  return epoch.AddSeconds(((double)ummm / UInt64.MaxValue));
}
double DsLogReader::TripTimeToDouble(std::uint8_t b)
{
  return (double)b * 0.5d;
}

double DsLogReader::PacketLossToDouble(std::int8_t b)
{
  return (double)(b * 4) * .01;
}

double DsLogReader::VoltageToDouble(std::uint16_t i)
{
  return (double)i * .00390625d;
}

double DsLogReader::RoboRioCPUToDouble(std::uint8_t b)
{
  return ((double)b * 0.5d) * .01d;
}

QVector<bool> DsLogReader::StatusFlagsToBooleanArray(std::uint8_t b)
{
  byte[] bytes = { b };
  return bytes.SelectMany(GetBits).ToArray();
}

double DsLogReader::CANUtilToDouble(std::uint8_t b)
{
  return ((double)b * 0.5d) * .01d;
}

double DsLogReader::WifidBToDouble(std::uint8_t b)
{
  return ((double)b * 0.5d) * .01d;
}

double DsLogReader::BandwidthToDouble(std::uint16_t i)
{
  return (double)i * .00390625d;
}
QVector<double> DsLogReader::PDPValuesToArrayList(QVector<std::uint8_t> ba)
{
  double[] d = new double[16];
  for (int s = 0; s < 5; s++)
  {
    if (s % 2 == 0)
    {
      byte[] b5 = new byte[5];
      Array.Copy(ba, s * 4, b5, 0, 5);
      for (int n = 0; n < 4; ++n)
      {
        if (n == 0)
        {
          d[(s * 3) + n] = (double)(Convert.ToUInt16(b5[0] << 2) + Convert.ToUInt16(b5[1] >> 6)) * .125d;
        }
        else
        {
          d[(s * 3) + n] = (double)(Convert.ToUInt16(((UInt16)((byte)(b5[n] << (n * 2)))) << 2) + Convert.ToUInt16(b5[n + 1] >> (6 - (n * 2)))) * .125d;
        }
      }
    }
    else
    {
      byte[] b3 = new byte[3];
      Array.Copy(ba, (s * 4) + 1, b3, 0, 3);
      for (int n = 0; n < 2; ++n)
      {
        if (n == 0)
        {
          d[((s * 3) + 1) + n] = (double)(Convert.ToUInt16(b3[0] << 2) + Convert.ToUInt16(b3[1] >> 6)) * .125d;
        }
        else
        {
          d[((s * 3) + 1) + n] = (double)(Convert.ToUInt16(((UInt16)((byte)(b3[1] << 2))) << 2) + Convert.ToUInt16(b3[2] >> 4)) * .125d;
        }
      }
    }
  }



  return d;
}

QVector<bool> DsLogReader::GetBits(std::uint8_t b)
{
  for (int i = 0; i < 8; i++)
  {
    yield return !((b & 0x80) != 0);
    b *= 2;
  }
}


//constructors
DsLogReader::DsLogReader(const QString& path)
{
  readFile(path);
}
