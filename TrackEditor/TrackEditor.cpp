#include "TrackEditor.h"
#include "MainWindow.h"
#include "qapplication.h"
#include "qdesktopwidget.h"
#include <qstring.h>
#include "qdir.h"
#include <QtCore>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#if defined(IS_WINDOWS)
  #ifndef QT_DLL
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
    Q_IMPORT_PLUGIN(QICOPlugin)
  #endif
#endif
//-------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
#if defined(_DEBUG) && defined(IS_WINDOWS)
  _set_error_mode(_OUT_TO_MSGBOX);
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  QString sAppPath = argv[0];
  sAppPath = sAppPath.left(sAppPath.lastIndexOf(QDir::separator()));

  QApplication app(argc, argv);
  float fScale = app.desktop()->logicalDpiX() / 96.0 * 100.0;
  CMainWindow *pMainWin = new CMainWindow(sAppPath, fScale);
  
  int iRetCode = app.exec();

  delete pMainWin;
  pMainWin = NULL;

  QObjectList list = QPluginLoader::staticInstances();
  while (!list.empty()) {
    QObjectList::iterator it = list.begin();
    QObject *pPlugin = *it;
    while (it != list.end()) {
      if (*it == pPlugin)
        it = list.erase(it);
      else
        ++it;
    }
    delete pPlugin;
  }

  return iRetCode;
}

//-------------------------------------------------------------------------------------------------