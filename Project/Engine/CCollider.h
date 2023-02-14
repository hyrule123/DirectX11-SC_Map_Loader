#pragma once
#include "CComponent.h"

class CCollider :
    public CComponent
{
private:
    CCollider() = delete;
public:
    CCollider(eCOMPONENT_TYPE _ComType, eCOLLIDER_TYPE _ColType);
    virtual ~CCollider();

private:
    eCOLLIDER_TYPE      m_eColType;
    Matrix              m_matCollider;      // Collider 의 월드행렬

    Vec3                m_vOffsetPos;
    Vec3                m_vOffsetScale;
    bool                m_bFixSize;         // 고정 사이즈를 사용
    int                 m_iCollisionCount;  // 현재 충돌중인 충돌체의 갯수를 저장

    bool                m_bNeedAABBUpdate;
    bool                m_bNeedCollUpdate;


public:
    //inline Getter
    eCOLLIDER_TYPE  GetColliderType()   const { return m_eColType; }
    Vec3            GetOffsetPos()      const { return m_vOffsetPos; }
    Vec3            GetOffsetScale()    const { return m_vOffsetScale; }
    bool            GetNeedAABBUpdate()     const { return m_bNeedAABBUpdate; }
    void            SetNeedAABBUpdate(bool _bNeedAABBUpdate) { m_bNeedAABBUpdate = _bNeedAABBUpdate; }

    bool            GetNeedCollUpdate()     const { return m_bNeedCollUpdate; }
    void            SetNeedCollUpdate(bool _bNeedCollUpdate) { m_bNeedCollUpdate = _bNeedCollUpdate; }

    void            AddCollisionCount() { ++m_iCollisionCount; }
    void            SubCollisionCount() { --m_iCollisionCount; }
    int             GetCollisionCount() const { return m_iCollisionCount; }

    int             GetLayerIndex()           { return GetOwner()->GetLayer(); }
    

    //nullptr이 반환될 수 있으므로 주의할것
    CScriptHolder* ScriptHolder()             { return GetOwner()->ScriptHolder(); }

    //inline Setter
    
    void SetColliderMatrix(const Matrix& _mat) { m_matCollider = _mat; }


public:
    void BeginCollision(CCollider* _other);
    void OnCollision(CCollider* _other);
    void EndCollision(CCollider* _other);

public:
    //충돌체 finaltick()은 transform::finaltick()이후 호출되므로 transform의 값을 마음껏 가져다 사용해도 문제 없음.
    virtual void finaltick() = 0;

    //고유 충돌체의 정보와 AABB 사각형 정보(m_RectInfo)를 업데이트 해야함.
    virtual void UpdateCollider() = 0;

};