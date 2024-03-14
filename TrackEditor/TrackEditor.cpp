#include "TrackEditor.h"
#include "MainWindow.h"
#include "qapplication.h"
#include <qstring.h>
#include "qdir.h"
//-------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  QString sAppPath = argv[0];
  sAppPath = sAppPath.left(sAppPath.lastIndexOf(QDir::separator()));

  QApplication app(argc, argv);
  CMainWindow *pMainWin = new CMainWindow(sAppPath);
  
  int iRetCode = app.exec();

  delete pMainWin;
  pMainWin = NULL;

  return iRetCode;
}

//-------------------------------------------------------------------------------------------------