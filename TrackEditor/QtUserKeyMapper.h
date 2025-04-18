#ifndef _TRACKEDITOR_QTUSERKEYMAPPER_H
#define _TRACKEDITOR_QTUSERKEYMAPPER_H
//-------------------------------------------------------------------------------------------------
#include "KeyMapper.h"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class QKeyEvent;
class QMouseEvent;
//-------------------------------------------------------------------------------------------------

class CQtUserKeyMapper : public IKeyMapper
{
public:
  CQtUserKeyMapper();

  void QtMousePressEvent(QMouseEvent *pEvent);
  void QtMouseReleaseEvent(QMouseEvent *pEvent);
  void QtKeyPressEvent(QKeyEvent *pEvent);
  void QtKeyReleaseEvent(QKeyEvent *pEvent);
  void Update(uint64 &ullActionsPressed, glm::vec2 &mousePos) override;

private:
  uint64 m_ullActionsPressed;
};

//-------------------------------------------------------------------------------------------------
#endif