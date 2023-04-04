#include "pch.h"
#include "CScript_Marine.h"

#include "CFState_SCUnitIdle.h"
#include "CFState_SCUnitMove_Ground.h"
#include "CFState_SCUnit_AttackBegin.h"
#include "CFState_SCUnit_Attack.h"
#include "CFState_SCUnit_AttackEnd.h"

CScript_Marine::CScript_Marine()
	: CSCEntity(TYPE_INDEX(CScript_Marine), (UINT)FSM_SCGroundUnit::END)
{
	AddFState(FSM_SCGroundUnit::IDLE, new CFState_SCUnitIdle);

	CFState_SCUnitMove_Ground* move = new CFState_SCUnitMove_Ground;
	move->SetSpeed(100.f);
	AddFState(FSM_SCGroundUnit::MOVE, move);

	AddFState(FSM_SCGroundUnit::ATTACK_BEGIN, new CFState_SCUnit_AttackBegin);
	AddFState(FSM_SCGroundUnit::ATTACK, new CFState_SCUnit_Attack);
	AddFState(FSM_SCGroundUnit::ATTACK_END, new CFState_SCUnit_AttackEnd);
}


CScript_Marine::~CScript_Marine()
{
}
