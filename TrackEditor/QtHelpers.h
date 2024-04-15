#ifndef _TRACKEDITOR_QTHELPERS_H
#define _TRACKEDITOR_QTHELPERS_H
//-------------------------------------------------------------------------------------------------
#include "QImage.h"
//-------------------------------------------------------------------------------------------------
class QLineEdit;
class QString;
class QComboBox;
class QPushButton;
class QCheckBox;
class CTexture;
class QWidget;
struct tTile;
//-------------------------------------------------------------------------------------------------

namespace QtHelpers
{
  extern bool UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  extern bool UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue);
  extern bool UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex);
  extern void UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex);
  extern void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue);
  extern void UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue);
  extern void UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue);
  extern QImage GetQImageFromTile(const tTile &tile);
};

//-------------------------------------------------------------------------------------------------
#endif