#include "TrackPosComponent.h"
#include "Track.h"
#include "Entity.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTrackPosComponent::CTrackPosComponent()
  : m_pTrack(NULL)
  , m_positionOnTrack(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

//-------------------------------------------------------------------------------------------------

CTrackPosComponent::~CTrackPosComponent()
{

}

//-------------------------------------------------------------------------------------------------

void CTrackPosComponent::Update()
{
  if (!m_pTrack)
    return;

  //m_pTrack->ProjectToTrack(m_pContainingEntity->m_prevPos, m_pContainingEntity->m_position);// , m_positionOnTrack);
}

//-------------------------------------------------------------------------------------------------

void CTrackPosComponent::SetData(CTrack *pTrack)
{
  m_pTrack = pTrack;
}

//-------------------------------------------------------------------------------------------------