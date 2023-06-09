#pragma once

#include <Engine/CFSM.h>

#include "define_SCUnit.h"

class CScript_FSM_Attack_BeginEnd;

class CScript_FSM_Attack :
    public CFSM
{
public:
    CScript_FSM_Attack(const string& _strKey);
    virtual ~CScript_FSM_Attack();

    CScript_FSM_Attack(const CScript_FSM_Attack& _other) = default;
    CLONE(CScript_FSM_Attack);

public:
    virtual void init() override;

    virtual void EnterState(const tFSM_Event& _tEvent) override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override;

    void Attack(CGameObject* _pTarget);

private:
    //Intantiate, 업그레이드 확인용으로 사용
    string m_strWeaponName;
    GETTER_SETTER(const string&, m_strWeaponName, WeaponName);

private: 
    UINT m_uDefaultDMG;
    GETTER_SETTER(UINT, m_uDefaultDMG, DefaultDMG);

private:
    UINT m_uDMGAddedPerUpgrade;
    GETTER_SETTER(UINT, m_uDMGAddedPerUpgrade, DMGPerUpgrade);

private:
    //1당 32 픽셀
    UINT m_uWeaponRange;
    GETTER_SETTER(UINT, m_uWeaponRange, WeaponRange);

private:
    //공격 대상
    CGameObject* m_pTarget;
    SETTER(CGameObject*, m_pTarget, Target);


    //공격 시작/종료 모션 담당 클래스
private:
    CScript_FSM_Attack_BeginEnd* m_pAtkBeginEnd;

private:


public:
    void SetAll(const string& _strWeaponName, UINT _uDefaultDMG, UINT _uDMGAddedPerUpgrade, UINT _uWeaponRange);
};

inline void CScript_FSM_Attack::SetAll(const string& _strWeaponName, UINT _uDefaultDMG, UINT _uDMGAddedPerUpgrade, UINT _uWeaponRange)
{
    m_strWeaponName = _strWeaponName;
    m_uDefaultDMG = _uDefaultDMG;
    m_uDMGAddedPerUpgrade = _uDMGAddedPerUpgrade;
    m_uWeaponRange = _uWeaponRange;
}


