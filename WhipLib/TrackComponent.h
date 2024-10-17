#ifndef _WHIPLIB_TRACKCOMPONENT_H
#define _WHIPLIB_TRACKCOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CTrack;
//-------------------------------------------------------------------------------------------------

class CTrackComponent : public IComponent
{
public:
  CTrackComponent();
  ~CTrackComponent();

  void Update() override;
  void SetData(CTrack *pTrack);

  glm::vec3 m_positionOnTrack;

private:
  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------
#endif