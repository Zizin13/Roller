#ifndef _WHIPLIB_TRACKPOSCOMPONENT_H
#define _WHIPLIB_TRACKPOSCOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include "glm.hpp"
#include <string>
//-------------------------------------------------------------------------------------------------
class CTrack;
//-------------------------------------------------------------------------------------------------

class CTrackPosComponent : public IComponent
{
public:
  CTrackPosComponent();
  ~CTrackPosComponent();

  void Update() override;
  void SetData(CTrack *pTrack);

  glm::vec3 m_positionOnTrack;

private:
  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------
#endif