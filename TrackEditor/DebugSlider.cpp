#include "DebugSlider.h"
#include <QVBoxLayout>
#include <qslider.h>
#include <qlabel.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CDebugSlider::CDebugSlider(float value, float min, float max, bool textOnLeft, float granularity)
{
  QLayout *layout;
  this->min = min;
  this->max = max;
  sliderGranularity = granularity;
  setLayout(layout = textOnLeft ? (QLayout *)new QHBoxLayout : new QVBoxLayout);
  layout->addWidget(label = new QLabel);
  label->setMinimumWidth(35);
  layout->addWidget(slider = new QSlider);
  label->setAlignment(Qt::AlignCenter);
  slider->setOrientation(Qt::Horizontal);
  slider->setMinimum(0);
  slider->setMaximum(sliderGranularity);
  connect(slider, SIGNAL(valueChanged(int)),
          this, SLOT(sliderValueChanged()));
  setValue(value);
}

//-------------------------------------------------------------------------------------------------

float CDebugSlider::value() const
{
  return min + (max - min) * (slider->value() / sliderGranularity);
}

//-------------------------------------------------------------------------------------------------

void CDebugSlider::setValue(float newValue)
{
  float spot = (newValue - min) / (max - min);
  slider->setValue(spot * sliderGranularity);
}

//-------------------------------------------------------------------------------------------------

void CDebugSlider::sliderValueChanged()
{
  label->setText(QString::number(value()));
  emit valueChanged(value());
}

//-------------------------------------------------------------------------------------------------
