#include "QtHelpers.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "Track.h"
#include "Texture.h"
#include "EditSurfaceDialog.h"
#include "Texture.h"
#include "Palette.h"
#include "MainWindow.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

bool QtHelpers::UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue)
{
  pLineEdit->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    pLineEdit->setText("");
    pLineEdit->setPlaceholderText(sValue);
    //pLineEdit->setStyleSheet("background-color: rgb(255,0,0)");
  } else {
    pLineEdit->setPlaceholderText("");
    pLineEdit->setText(sValue);
    pLineEdit->setStyleSheet("");
  }
  pLineEdit->blockSignals(false);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

bool QtHelpers::UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue)
{
  std::string sVal = sValue.toLatin1().constData();
  pComboBox->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    if (pComboBox->findData(MIXED_DATA) == -1)
      pComboBox->addItem(MIXED_DATA, MIXED_DATA);
    pComboBox->setCurrentIndex(pComboBox->findData(MIXED_DATA));
  } else {
    pComboBox->removeItem(pComboBox->findData(MIXED_DATA));
    pComboBox->setCurrentIndex(pComboBox->findData(sValue.toInt()));
  }
  pComboBox->blockSignals(false);
  return (sValue.compare(MIXED_DATA) == 0);
}

//-------------------------------------------------------------------------------------------------

bool QtHelpers::UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex)
{
  pLineEdit->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    pLineEdit->setText("");
    pLineEdit->setPlaceholderText(sValue);
    //pLineEdit->setStyleSheet("background-color: rgb(255,0,0)");
  } else {
    pLineEdit->setPlaceholderText("");
    pLineEdit->setText(sValue);
    pLineEdit->setStyleSheet("");
  }
  pLineEdit->blockSignals(false);
  UpdateSignButtonDisplay(pPushButton, pCheckbox, pLineEdit, pTex);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

void QtHelpers::UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex)
{
  pCheckbox->blockSignals(true);
  if (pLineEdit->text().compare("-1") == 0) {
    pPushButton->setIcon(QIcon());
    pPushButton->setText("<none>");
    pCheckbox->setEnabled(false);
    pCheckbox->setChecked(false);
  } else if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
    pPushButton->setIcon(QIcon());
    pPushButton->setText(MIXED_DATA);
    pCheckbox->setEnabled(false);
    pCheckbox->setChecked(false);
  } else {
    int iValue = pLineEdit->text().toInt();
    unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
    bool bChecked = uiSignedBitVal & SURFACE_FLAG_APPLY_TEXTURE;
    int iBldIndex = CTrack::GetIntValueFromSignedBit(uiSignedBitVal & SURFACE_TEXTURE_INDEX);
    if (iBldIndex < pTex->m_iNumTiles) {
      QPixmap pixmap;
      pixmap.convertFromImage(QtHelpers::GetQImageFromTile(pTex->m_pTileAy[iBldIndex]));
      pPushButton->setIcon(pixmap);
      pPushButton->setText("");
    }
    pCheckbox->setEnabled(true);
    pCheckbox->setChecked(bChecked);
  }
  pCheckbox->blockSignals(false);
}

//-------------------------------------------------------------------------------------------------

void QtHelpers::UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue)
{
  QString sLineEditVal = pLineEdit->text();
  if (sLineEditVal.isEmpty())
    sLineEditVal = "0";

  if (sLineEditVal.compare(sValue) != 0) {
    if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pLineEdit->setStyleSheet("");
    } else {
      bEdited = true;
      pLineEdit->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pLineEdit->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void QtHelpers::UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue)
{
  QString sCbxVal = pComboBox->currentData().toString();
  if (sCbxVal.compare(sValue) != 0 && pComboBox->currentIndex() != -1) {
    if (sCbxVal.compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pComboBox->setStyleSheet("");
    } else {
      bEdited = true;
      pComboBox->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pComboBox->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void QtHelpers::UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue)
{
  QString sLineEditVal = pLineEdit->text();
  if (sLineEditVal.isEmpty())
    sLineEditVal = "0";

  if (sLineEditVal.compare(sValue) != 0) {
    if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pWidget->setStyleSheet("");
    } else {
      bEdited = true;
      pWidget->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pWidget->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void QtHelpers::UpdateTextures(QLabel *pLblTex1, QLabel *pLblTex2, CTexture *pTex, CPalette *pPal, int iSurface)
{
  QPixmap pixmap;
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iSurface);
  int iIndex = (int)(uiSignedBitVal & SURFACE_TEXTURE_INDEX);
  QSize size((int)(TILE_WIDTH * g_pMainWindow->GetDesktopScale() / 200.0),
             (int)(TILE_HEIGHT * g_pMainWindow->GetDesktopScale() / 200.0));
  if (pLblTex1) {
    pLblTex1->setMinimumSize(size);
    pLblTex1->setText("");
    pLblTex1->setPixmap(QPixmap());
  }
  if (pLblTex2) {
    pLblTex2->setMinimumSize(size);
    pLblTex2->setText("");
    pLblTex2->setPixmap(QPixmap());
  }
  if (iSurface != -1) {
    if (pTex && uiSignedBitVal & SURFACE_FLAG_APPLY_TEXTURE) {
      if (iIndex < pTex->m_iNumTiles) {
        pixmap.convertFromImage(QtHelpers::GetQImageFromTile(pTex->m_pTileAy[iIndex], true));
        if (pLblTex1)
          pLblTex1->setPixmap(pixmap);

        if (uiSignedBitVal & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
          if (uiSignedBitVal & SURFACE_FLAG_PAIR_NEXT_TEX)
            pixmap.convertFromImage(QtHelpers::GetQImageFromTile(pTex->m_pTileAy[iIndex + 1], true));
          if (pLblTex2)
            pLblTex2->setPixmap(pixmap);
        }
      }
    } else if (uiSignedBitVal & SURFACE_FLAG_TRANSPARENT) {
      if (iIndex < g_transparencyAyCount)
        pLblTex1->setText(g_transparencyAy[iIndex].c_str());
    } else {
      if (pPal && iIndex < (int)pPal->m_paletteAy.size()) {
        QPixmap pixmap;
        pixmap.convertFromImage(QtHelpers::GetQImageFromColor(pPal->m_paletteAy[iIndex]));
        if (pLblTex1)
          pLblTex1->setPixmap(pixmap);
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------

QImage QtHelpers::GetQImageFromTile(const tTile &tile, bool bScale)
{
  QImage image(TILE_WIDTH, TILE_HEIGHT, QImage::Format_RGB32);
  for (int i = 0; i < TILE_WIDTH; ++i) {
    for (int j = 0; j < TILE_HEIGHT; ++j)
      image.setPixelColor(i, j, QColor(tile.data[i][j].r,
                                       tile.data[i][j].g,
                                       tile.data[i][j].b));
  }
  if (bScale)
    return image.scaled((int)(TILE_WIDTH * g_pMainWindow->GetDesktopScale() / 200.0),
                        (int)(TILE_HEIGHT * g_pMainWindow->GetDesktopScale() / 200.0));
  else
    return image;
}

//-------------------------------------------------------------------------------------------------

QImage QtHelpers::GetQImageFromColor(const glm::vec3 &color, bool bScale)
{
  QImage image(TILE_WIDTH, TILE_HEIGHT, QImage::Format_RGB32);
  for (int i = 0; i < TILE_WIDTH; ++i) {
    for (int j = 0; j < TILE_HEIGHT; ++j)
      image.setPixelColor(i, j, QColor(color.r,
                                       color.g,
                                       color.b));
  }
  if (bScale)
    return image.scaled((int)(TILE_WIDTH * g_pMainWindow->GetDesktopScale() / 200.0),
                        (int)(TILE_HEIGHT * g_pMainWindow->GetDesktopScale() / 200.0));
  else
    return image;
}

//-------------------------------------------------------------------------------------------------