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
  QString GetStartValue();
  QString GetEndValue();
  QString GetIncrement();

private:
  void Validate();
  bool IsDirectionValid(double dStart, double dEnd, double dIncrement);
  int ToInt(QString sText);

  int m_iStartChunk;
  int m_iEndChunk;
  int m_iInterval;
  int m_iField;
  QString m_sStartValue;
  QString m_sEndValue;
  QString m_sIncrement;
  QString chunkFields[81] = {
    "Left shoulder width",
    "Left lane width",
    "Right lane width",
    "Right shoulder width",
    "Left shoulder height",
    "Right shoulder height",
    "Length",
    "Yaw",
    "Pitch",
    "Roll",
    "AI Line 1",
    "AI Line 2",
    "AI Line 3",
    "AI Line 4",
    "Track grip",
    "Left shoulder grip",
    "Right shoulder grip",
    "Row 0 Col 17",
    "Row 0 Col 18",
    "Audio above trigger",
    "Audio trigger speed",
    "Audio below trigger",
    "Left surface type",
    "Center surface type",
    "Right surface type",
    "Left wall type",
    "Right wall type",
    "Roof type",
    "Left upper outer wall type",
    "Left lower outer wall type",
    "Outer floor type",
    "Right lower outer wall type",
    "Right upper outer wall type",
    "Environment floor type",
    "Sign type",
    "Sign horizontal offset",
    "Sign horizontal offset",
    "Sign yaw",
    "Sign pitch",
    "Sign roll",
    "Left upper outer wall angle",
    "Left lower outer wall angle",
    "Row 2 Col 02",
    "Row 2 Col 03",
    "Right lower outer wall angle",
    "Right upper outer wall angle",
    "Left upper outer wall height",
    "Left lower outer wall height",
    "Row 2 Col 09",
    "Row 2 Col 08",
    "Right lower outer wall height",
    "Right upper outer wall height",
    "Roof height",
    "Draw order 1",
    "Draw order 2",
    "Draw order 3",
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
    "Sign texture",
    "Back texture",
    "Stunt 0",
    "Stunt 1",
    "Stunt 2",
    "Stunt 3",
    "Stunt 4",
    "Stunt 5",
    "Stunt 6",
    "Stunt 7",
    "Stunt 8"
  };
  size_t chunkFieldsSize = sizeof(CEditSeriesDialog::chunkFields) / sizeof(CEditSeriesDialog::chunkFields[0]);
};

//-------------------------------------------------------------------------------------------------
#endif