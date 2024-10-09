#ifndef WHIPLIB_COMPONENT_H
#define WHIPLIB_COMPONENT_H
//-------------------------------------------------------------------------------------------------
class CEntity;
//-------------------------------------------------------------------------------------------------

class IComponent
{
public:
  virtual bool Init() { return true; }
  virtual bool Shutdown() { return true; }
  virtual void Update() {}

private:
  friend class CEntity;

protected:
  CEntity *m_pContainingEntity;
};

//-------------------------------------------------------------------------------------------------
#endif