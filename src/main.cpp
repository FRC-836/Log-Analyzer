#include <qapplication.h>
#include <qtextedit.h>

#include "OutputManager.h"

//TEMP
#include "DsLogReader.h"
#include <fstream>

OutputManager cout(stdout);

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QTextEdit e;
  e.show();

  //TEMP
  DsLogReader reader("C:/Users/Public/Documents/FRC/Log Files/2018_04_25 19_43_46 Wed.dslog");
  std::ofstream file;
  file.open("C:/Users/Cheeto/Robotics/FRC/Tools/Log-Analyzer/build/OutputFiles/Log.csv", std::iostream::out | std::iostream::trunc);
  if (file.is_open())
  {
    auto entries = reader.getEntries();
    file << "Date-Time, Trip Time, Lost Packets, Voltage, RobotRIO CPU, CAN Utilization, ";
    for (int i = 0; i < (int)DsLogEntry::STATUS_FLAGS::NUM_STATUS_FLAGS; i++)
    {
      file << DsLogEntry::STATUS_FLAG_STR[static_cast<DsLogEntry::STATUS_FLAGS>(i)].toStdString() << ", ";
    }
    for (int i = 0; i < entries[0].m_pdpCurrents.size(); i++)
    {
      file << "PDP[" << i << "], ";
    }
    file << std::endl;
    for (int i = 0; i < entries.size(); i++)
    {
      file << entries[i].m_time.toString().toStdString() << ", " << entries[i].m_tripTime << ", " << entries[i].m_lostPackets << ", "
        << entries[i].m_voltage << ", " << entries[i].m_roboRioCpu << ", "
        << entries[i].m_canUtil << ", " << entries[i].m_statusFlags[0] << ", " 
        << entries[i].m_statusFlags[1] << ", " << entries[i].m_statusFlags[2] << ", " 
        << entries[i].m_statusFlags[3] << ", " << entries[i].m_statusFlags[4] << ", " 
        << entries[i].m_statusFlags[5] << ", " << entries[i].m_statusFlags[6] << ", " 
        << entries[i].m_statusFlags[7] << ", ";

      for (int j = 0; j < entries[i].m_pdpCurrents.size(); j++)
      {
        file << entries[i].m_pdpCurrents[j] << ", ";
      } //end  for (int i = 0; i < entries[i].m_pdpCurrents.size(); i++)
      file << std::endl;
    } //end  for (int i = 0; i < entries.size(); i++)
  } //end  if (file.is_open())
  else
  {
    cout << "can't open csv file for writing" << endl;
  }
  
  return app.exec();
}