#ifndef _WHIPLIB_ENTITY_H
#define _WHIPLIB_ENTITY_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class IComponent;
//-------------------------------------------------------------------------------------------------

class CEntity
{
public:
  CEntity();

  bool Init();
  bool Shutdown();
  void AddComponent(IComponent *pComponent);
  void Update();
  glm::vec3 GetOrientation();
  glm::vec3 GetUp();

  template<class T>
  T *GetComponent() const;

  glm::vec3 m_position;
  glm::mat4 m_rotationMat;
  bool m_bAcceptControls;

private:
  static const int s_iMaxComponents = 10;
  int m_iNumComponents;
  IComponent *m_components[s_iMaxComponents];
};

//-------------------------------------------------------------------------------------------------

template<class T>
T *CEntity::GetComponent() const
{
  for (int i = 0; i < m_iNumComponents; ++i) {
    //downcast to requested class
    T *pRet = dynamic_cast<T *>(m_components[i]);

    if (pRet != NULL) {
      return pRet;
    }
  }

  //type not found
  return NULL;
}

//-------------------------------------------------------------------------------------------------
#endif