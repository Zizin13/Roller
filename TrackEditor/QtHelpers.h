#ifndef _TRACKEDITOR_QTHELPERS_H
#define _TRACKEDITOR_QTHELPERS_H
//-------------------------------------------------------------------------------------------------
class QLineEdit;
class QString;
class QComboBox;
class QPushButton;
class QCheckBox;
class CTexture;
class QWidget;
//-------------------------------------------------------------------------------------------------

class QtHelpers
{
public:
  static bool UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  static bool UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue);
  static bool UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex);
  static void UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex);
  static void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue);
  static void UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue);
  static void UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue);
};

//-------------------------------------------------------------------------------------------------
#endif