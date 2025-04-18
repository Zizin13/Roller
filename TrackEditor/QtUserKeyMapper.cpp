#include "QtUserKeyMapper.h"
#include <Qt>
#include "qevent.h"
#include "GameInput.h"
#include "qcursor.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CQtUserKeyMapper::CQtUserKeyMapper()
  : IKeyMapper()
  , m_ullActionsPressed(0)
{ }

//-------------------------------------------------------------------------------------------------

void CQtUserKeyMapper::QtMousePressEvent(QMouseEvent *pEvent)
{
  (void)(pEvent);
  m_ullActionsPressed |= ACTION_CLICK;
}

//-------------------------------------------------------------------------------------------------

void CQtUserKeyMapper::QtMouseReleaseEvent(QMouseEvent *pEvent)
{
  (void)(pEvent);
  m_ullActionsPressed &= ~ACTION_CLICK;
}

//-------------------------------------------------------------------------------------------------

void CQtUserKeyMapper::QtKeyPressEvent(QKeyEvent *pEvent)
{
  switch (pEvent->key()) {
    case Qt::Key_W:
      m_ullActionsPressed |= ACTION_FORWARD;
      break;
    case Qt::Key_A:
      m_ullActionsPressed |= ACTION_LEFT;
      break;
    case Qt::Key_S:
      m_ullActionsPressed |= ACTION_BACKWARD;
      break;
    case Qt::Key_D:
      m_ullActionsPressed |= ACTION_RIGHT;
      break;
    case Qt::Key_R:
    case Qt::Key_E:
      m_ullActionsPressed |= ACTION_UP;
      break;
    case Qt::Key_F:
    case Qt::Key_Q:
      m_ullActionsPressed |= ACTION_DOWN;
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void CQtUserKeyMapper::QtKeyReleaseEvent(QKeyEvent *pEvent)
{
  switch (pEvent->key()) {
    case Qt::Key_W:
      m_ullActionsPressed &= ~ACTION_FORWARD;
      break;
    case Qt::Key_A:
      m_ullActionsPressed &= ~ACTION_LEFT;
      break;
    case Qt::Key_S:
      m_ullActionsPressed &= ~ACTION_BACKWARD;
      break;
    case Qt::Key_D:
      m_ullActionsPressed &= ~ACTION_RIGHT;
      break;
    case Qt::Key_R:
    case Qt::Key_E:
      m_ullActionsPressed &= ~ACTION_UP;
      break;
    case Qt::Key_F:
    case Qt::Key_Q:
      m_ullActionsPressed &= ~ACTION_DOWN;
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void CQtUserKeyMapper::Update(uint64 &ullActionsPressed, glm::vec2 &mousePos)
{
  ullActionsPressed = m_ullActionsPressed;

  QPoint pos = QCursor::pos();
  mousePos.x = (float)pos.x();
  mousePos.y = (float)pos.y();
}

//-------------------------------------------------------------------------------------------------