#include "TrackComponent.h"
#include "Track.h"
#include "Entity.h"
#include "ShapeComponent.h"
#include "Renderer.h"
#include "GameClock.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTrackComponent::CTrackComponent()
  : m_pTrack(NULL)
  , m_pRenderer(NULL)
  , m_fTimer(0.0f)
{
}

//-------------------------------------------------------------------------------------------------

CTrackComponent::~CTrackComponent()
{

}

//-------------------------------------------------------------------------------------------------

void CTrackComponent::Update()
{
  if (!m_pTrack || !m_pRenderer)
    return;

  m_fTimer += CGameClock::GetGameClock().DeltaTimeLastFrame();

  if (m_fTimer > 0.028f) {
    m_fTimer = m_fTimer - 0.028f;
    CShapeComponent *pTrackShapeComponent = m_pContainingEntity->GetComponent<CShapeComponent>();
    if (pTrackShapeComponent) {
      m_pTrack->UpdateStunts();
      m_pRenderer->MakeTrackShape(&pTrackShapeComponent->m_pShapeData, m_pTrack);
    }
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackComponent::SetData(CTrack *pTrack, CRenderer *pRenderer)
{
  m_pTrack = pTrack;
  m_pRenderer = pRenderer;
}

//-------------------------------------------------------------------------------------------------