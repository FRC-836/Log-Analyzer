#include "DsLogReader.h"

const QMap<DsLogEntry::STATUS_FLAGS, QString> DsLogEntry::STATUS_FLAG_STR = {
  {STATUS_FLAGS::BROWNOUT,         "Brownout"},
  {STATUS_FLAGS::WATCHDOG,         "Watchdog"},
  {STATUS_FLAGS::DS_TELE,          "DS_Tele"},
  {STATUS_FLAGS::DS_AUTO,          "DS_Auto"},
  {STATUS_FLAGS::DS_DISABLED,      "DS_Disabled"},
  {STATUS_FLAGS::ROBOT_AUTO,       "Robot_Auto"},
  {STATUS_FLAGS::ROBOT_TELE,       "Robot_Tele"},
  {STATUS_FLAGS::ROBOT_DISABLED,   "Robot_Disabled"},
  {STATUS_FLAGS::NUM_STATUS_FLAGS, "Invalid"},
};

//private functions
void DsLogReader::readFile(const QString& path)
{
  BinaryReader reader(path);
  if (reader.readyToRead())
  {
    m_version = reader.readInt32();
    if (m_version == 3)
    {
      auto unixTime = reader.readUint64();
      auto ummm = reader.readUint64();
      m_startTime = FromLVTime(unixTime, ummm);
      int i = 0;
      while (reader.getPos() < reader.size())
      {
        auto tripTime = reader.readUint8();
        auto packetLoss = reader.readInt8();
        auto voltage = reader.readUint16();
        auto cpu = reader.readUint8();
        auto statusFlags = reader.readUint8();
        auto canUtil = reader.readUint8();
        auto wifi = reader.readUint8();
        auto bandwidth = reader.readUint16();
        auto pdpId = reader.readUint8();
        auto currents = reader.readBytes(21);
        auto res = reader.readUint8();
        auto pdpv = reader.readUint8();
        auto temp = reader.readUint8();
        m_entries.push_back(DsLogEntry(TripTimeToDouble(tripTime),
                                       PacketLossToDouble(packetLoss),
                                       VoltageToDouble(voltage), 
                                       RoboRioCPUToDouble(cpu), 
                                       StatusFlagsToBooleanArray(statusFlags), 
                                       CANUtilToDouble(canUtil), 
                                       WifidBToDouble(wifi), 
                                       BandwidthToDouble(bandwidth), 
                                       pdpId, 
                                       PDPValuesToArrayList(currents), 
                                       res, 
                                       pdpv, 
                                       temp, 
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
  //have to use this instead of msecsFromEpoc because QT Epoch 1970
  auto epoch = QDateTime(QDate(1904, 1, 1), QTime(0, 0, 0), Qt::UTC);
  epoch = epoch.addSecs(unixTime);
  epoch = epoch.toTimeZone(QTimeZone::systemTimeZone());

  return epoch.addSecs(((double)ummm / std::numeric_limits<std::uint64_t>::max()));
}
double DsLogReader::TripTimeToDouble(std::uint8_t b)
{
  return (double)b * 0.5;
}
double DsLogReader::PacketLossToDouble(std::int8_t b)
{
  return (double)(b * 4) * .01;
}
double DsLogReader::VoltageToDouble(std::uint16_t i)
{
  return (double)i * .00390625;
}
double DsLogReader::RoboRioCPUToDouble(std::uint8_t b)
{
  return ((double)b * 0.5) * .01;
}
QVector<bool> DsLogReader::StatusFlagsToBooleanArray(std::uint8_t b)
{
  QVector<bool> statusFlags;
  for (int i = 0; i < (int)DsLogEntry::STATUS_FLAGS::NUM_STATUS_FLAGS; i++)
  {
    statusFlags.push_back(!(b & 0x80));
    b = b << 1;
  } //end  for (int i = 0; i < (int)DsLogEntry::STATUS_FLAGS::NUM_STATUS_FLAGS; i++
  return statusFlags;
}
double DsLogReader::CANUtilToDouble(std::uint8_t b)
{
  return ((double)b * 0.5) * .01;
}
double DsLogReader::WifidBToDouble(std::uint8_t b)
{
  return ((double)b * 0.5) * .01;
}
double DsLogReader::BandwidthToDouble(std::uint16_t i)
{
  return (double)i * .00390625;
}
QVector<double> DsLogReader::PDPValuesToArrayList(QVector<std::uint8_t> ba)
{
  QVector<double> d(16);
  for (int s = 0; s < 5; s++)
  {
    if (s % 2 == 0)
    {
      QVector<std::uint8_t> b5(5);
      for (int i = 0; i < 5; i++)
      {
        b5[i] = ba[s * 4 + i];
      } //end  for (int i = 0; i < 5; i++
      for (int n = 0; n < 4; ++n)
      {
        if (n == 0)
        {
          d[(s * 3) + n] = (double)(static_cast<std::uint16_t>(b5[0] << 2) + static_cast<std::uint16_t>(b5[1] >> 6)) * .125;
        } //end  if (n == 0
        else
        {
          d[(s * 3) + n] = (double)(static_cast<std::uint16_t>(((std::uint16_t)((std::uint8_t)(b5[n] << (n * 2)))) << 2) + static_cast<std::uint16_t>(b5[n + 1] >> (6 - (n * 2)))) * .125;
        } //end  els
      } //end  for (int n = 0; n < 4; ++n
    } //end  if (s % 2 == 0
    else
    {
      QVector<std::uint8_t> b3(3);
      for (int i = 0; i < 3; i++)
      {
        b3[i] = ba[s * 4 + 1 + i];
      } //end  for (int i = 0; i < 3; i++
      for (int n = 0; n < 2; ++n)
      {
        if (n == 0)
        {
          d[((s * 3) + 1) + n] = (double)(static_cast<uint16_t>(b3[0] << 2) + static_cast<std::uint16_t>(b3[1] >> 6)) * .125;
        } //end  if (n == 0
        else
        {
          d[((s * 3) + 1) + n] = (double)(static_cast<uint16_t>(((std::uint16_t)((std::uint8_t)(b3[1] << 2))) << 2) + static_cast<std::uint16_t>(b3[2] >> 4)) * .125;
        } //end  els
      } //end  for (int n = 0; n < 2; ++n
    } //end  els
  } //end  for (int s = 0; s < 5; s++
  return d;
}

//constructors
DsLogReader::DsLogReader(const QString& path)
{
  readFile(path);
}

QVector<DsLogEntry> DsLogReader::getEntries() const
{
  return m_entries;
}
