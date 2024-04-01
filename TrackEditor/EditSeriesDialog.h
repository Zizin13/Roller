#ifndef _TRACKEDITOR_EDITSERIESDLG_H
#define _TRACKEDITOR_EDITSERIESDLG_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditSeriesDialog.h"
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
class CTexture;
//-------------------------------------------------------------------------------------------------
class CEditSeriesDialog : public QDialog, private Ui::EditSeriesDialog
{
  Q_OBJECT

public:
  CEditSeriesDialog(QWidget *pParent, int iTrackSize);
  ~CEditSeriesDialog();

  int GetStartChunk();
  int GetEndChunk();
  int GetInterval();
  int GetField();
  int GetValue();
  int GetIncrement();

private:
  void Validate();
  int ToInt(QString sText);
  void UpdateDialog();

  int m_iStartChunk;
  int m_iEndChunk;
  int m_iInterval;
  int m_iField;
  int m_iValue;
  int m_iIncrement;
  QString chunkFields[70] = {
    "Row 0 Col 00",
    "Row 0 Col 01",
    "Row 0 Col 02",
    "Row 0 Col 03",
    "Row 0 Col 04",
    "Row 0 Col 05",
    "Row 0 Col 06",
    "Row 0 Col 07",
    "Row 0 Col 08",
    "Row 0 Col 19",
    "Row 0 Col 10",
    "Row 0 Col 11",
    "Row 0 Col 12",
    "Row 0 Col 13",
    "Row 0 Col 14",
    "Row 0 Col 15",
    "Row 0 Col 16",
    "Row 0 Col 17",
    "Row 0 Col 18",
    "Row 0 Col 19",
    "Row 0 Col 20",
    "Row 0 Col 21",
    "Row 1 Col 00",
    "Row 1 Col 01",
    "Row 1 Col 02",
    "Row 1 Col 03",
    "Row 1 Col 04",
    "Row 1 Col 05",
    "Row 1 Col 06",
    "Row 1 Col 07",
    "Row 1 Col 08",
    "Row 1 Col 09",
    "Row 1 Col 10",
    "Row 1 Col 11",
    "Row 1 Col 12",
    "Row 1 Col 13",
    "Row 1 Col 14",
    "Row 1 Col 15",
    "Row 1 Col 16",
    "Row 1 Col 17",
    "Row 2 Col 00",
    "Row 2 Col 01",
    "Row 2 Col 02",
    "Row 2 Col 03",
    "Row 2 Col 04",
    "Row 2 Col 05",
    "Row 2 Col 06",
    "Row 2 Col 07",
    "Row 2 Col 08",
    "Row 2 Col 09",
    "Row 2 Col 10",
    "Row 2 Col 11",
    "Row 2 Col 12",
    "Row 2 Col 13",
    "Row 2 Col 14",
    "Row 2 Col 15",
    "Row 2 Col 16",
    "Row 2 Col 17",
    "Row 2 Col 18",
    "Row 2 Col 19",
    "Row 2 Col 20",
    "Row 2 Col 21",
    "Row 2 Col 22",
    "Row 2 Col 23",
    "Row 2 Col 24",
    "Row 2 Col 25",
    "Row 2 Col 26",
    "Row 2 Col 27",
    "Row 2 Col 28",
    "Row 2 Col 29",
  };
  size_t chunkFieldsSize = sizeof(CEditSeriesDialog::chunkFields) / sizeof(CEditSeriesDialog::chunkFields[0]);
};

//-------------------------------------------------------------------------------------------------
#endif