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

  cout << "hello world" << endl;

  QTextEdit e;
  e.show();

  //TEMP
  DsLogReader reader("C:/Users/Public/Documents/FRC/Log Files/2018_04_25 19_43_46 Wed.dslog");
  std::ofstream file;
  file.open("C:/Users/Cheeto/Robotics/FRC/Tools/Log-Analyzer/build/OutputFiles/Log.csv", std::iostream::out | std::iostream::app);
  if (file.is_open())
  {
    auto entries = reader.getEntries();
    file << "Trip Time, Lost Packets, Voltage, RobotRIO CPU, CAN Utilization, wifi Db, bandwidth, PDP Resistance, PDP Voltage, PDP Temperature, Brownout, Watchdog, DS_Teleop, DS_Autonomous, DS_Disabled, Robot_Teleop, Robot_Autonomous, Robot_Disabled, ";
    for (int i = 0; i < entries[0].m_pdpCurrents.size(); i++)
    {
      file << "PDP[" << i << "], ";
    }
    file << "Time" << std::endl;
    for (int i = 0; i < entries.size(); i++)
    {
      file << entries[i].m_tripTime << ", " << entries[i].m_lostPackets << ", "
        << entries[i].m_voltage << ", " << entries[i].m_roboRioCpu << ", "
        << entries[i].m_canUtil << ", " << entries[i].m_wifiDb << ", "
        << entries[i].m_bandwith << ", " << entries[i].m_pdpResistance << ", "
        << entries[i].m_pdpVoltage << ", " << entries[i].m_pdpTemp << ", "
        << entries[i].m_statusFlags[0] << ", " << entries[i].m_statusFlags[1] << ", "
        << entries[i].m_statusFlags[2] << ", " << entries[i].m_statusFlags[3] << ", "
        << entries[i].m_statusFlags[4] << ", " << entries[i].m_statusFlags[5] << ", "
        << entries[i].m_statusFlags[6] << ", " << entries[i].m_statusFlags[7] << ", "
        << entries[i].m_pdpId << ", ";

      for (int j = 0; j < entries[i].m_pdpCurrents.size(); j++)
      {
        file << entries[i].m_pdpCurrents[j] << ", ";
      } //end  for (int i = 0; i < entries[i].m_pdpCurrents.size(); i++)
      file << entries[i].m_time.toString().toStdString() << std::endl;
    } //end  for (int i = 0; i < entries.size(); i++)
  } //end  if (file.is_open())
  else
  {
    cout << "can't open csv file for writing" << endl;
  }
  
  return app.exec();
}