#ifndef _WHIPLIB_TRACKCOMPONENT_H
#define _WHIPLIB_TRACKCOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CTrack;
class CRenderer;
//-------------------------------------------------------------------------------------------------

class CTrackComponent : public IComponent
{
public:
  CTrackComponent();
  ~CTrackComponent();

  void Update() override;
  void SetData(CTrack *pTrack, CRenderer *pRenderer);

private:
  CTrack *m_pTrack;
  CRenderer *m_pRenderer;
  float m_fTimer;
};

//-------------------------------------------------------------------------------------------------
#endif