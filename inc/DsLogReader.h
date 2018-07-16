#ifndef LOG_ANALYZER_DS_LOG_READER_H
#define LOG_ANALYZER_DS_LOG_READER_H

#include <qvector.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qfile.h>
#include <qbytearray.h>

/**
 * class for containing a single entry in a DriverStation log file
 */
class DsLogEntry
{
public:
  enum class STATUS_FLAGS
  {
    BROWNOUT,
    WATCHDOG,
    DS_TELE,
    DS_AUTO,
    DS_DISABLED,
    ROBOT_TELE,
    ROBOT_AUTO,
    ROBOT_DISABLED
  };

  const double m_tripTime;
  const double m_lostPackets;
  const double m_voltage;
  const double m_roboRioCpu;
  const double m_canUtil;
  const double m_wifiDb;
  const double m_bandwith;
  const double m_pdpResistance;
  const double m_pdpVoltage;
  const double m_pdpTemp;
  const QVector<bool> m_statusFlags;
  const int m_pdpId;
  const QVector<double> m_pdpCurrents;
  const QDateTime m_time;

  DsLogEntry(double trip, double packets, double vol, double rrCPU,
             const QVector<bool>& flags, double can, double dB, double band, int pdp,
             const QVector<double>& pdpCurrents, double res, double pdpV, double temp,
             const QDateTime& time)
    : m_tripTime(trip), m_lostPackets(packets), m_voltage(vol), m_roboRioCpu(rrCPU),
      m_canUtil(can), m_wifiDb(dB), m_bandwith(band), m_pdpResistance(res),
      m_pdpVoltage(pdpV), m_pdpTemp(temp), m_statusFlags(flags), m_pdpId(pdp),
      m_pdpCurrents(pdpCurrents), m_time(time)
  {
  }
};

class DsLogReader
{
private:
  //member variables
  QVector<DsLogEntry> m_entries;
  int m_version;
  QDateTime m_startTime;

  //private functions
  virtual void readFile(const QString& path)
  {
    QFile log(path);
    if (log.open(QIODevice::ReadOnly))
    {
      QByteArray bytes = log.readAll();
      //m_version = reader.ReadInt32();
      //if (Version == 3)
      //{
      //  StartTime = FromLVTime(reader.ReadInt64(), reader.ReadUInt64());
      //  int i = 0;
      //  while (reader.BaseStream.Position != reader.BaseStream.Length)
      //  {
      //    Entries.Add(new Entry(TripTimeToDouble(reader.ReadByte()), PacketLossToDouble(reader.ReadSByte()), VoltageToDouble(reader.ReadUInt16()), RoboRioCPUToDouble(reader.ReadByte()), StatusFlagsToBooleanArray(reader.ReadByte()), CANUtilToDouble(reader.ReadByte()), WifidBToDouble(reader.ReadByte()), BandwidthToDouble(reader.ReadUInt16()), reader.ReadByte(), PDPValuesToArrayList(reader.ReadBytes(21)), reader.ReadByte(), reader.ReadByte(), reader.ReadByte(), StartTime.AddMilliseconds(20 * i++)));
      //  }
      //}
    } //end  if (log.exists(path)
    else
    {
    } //end  els
  }


public:
  //constructors
  DsLogReader(const QString& path)
  {
    readFile(path);
  }

  //public functions


  //getters

  //setters
};

#endif