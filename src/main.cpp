#include <qapplication.h>
#include <qtextedit.h>

#include "OutputManager.h"

//TEMP
#include "DsLogReader.h"

OutputManager cout(stdout);

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  cout << "hello world" << endl;

  QTextEdit e;
  e.show();
  
  return app.exec();
}