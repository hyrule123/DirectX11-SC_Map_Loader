

#pragma once

#include <Engine/define.h>

//=========================================================
//This Code is Automatically generated by CodeGenerator.exe
//=========================================================


#ifndef STRKEY
#define STRKEY constexpr inline const char*
#endif

#include "pch.h"
#include "UserClassInitializer.h"

#include "strKey_Script.h"

#include <Engine/CUserClassMgr.h>
#include <Engine/CScript.h>
#define CONSTRUCTOR_T(_Type) pMgr->AddBaseScript(strKey_Script::_Type, Constructor_T<_Type>)

//Script Classes
#include "CScript_CameraMove.h"
#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Building_Prod.h"
#include "CScript_FSM_Death.h"
#include "CScript_FSM_Idle.h"
#include "CScript_FSM_Move_Ground.h"
#include "CScript_FSM_Move_Hover.h"
#include "CScript_MainCamSC_InGame.h"
#include "CScript_Mineral.h"
#include "CScript_MouseCursor.h"
#include "CScript_SCEntity.h"
#include "CScript_SCUnitVital.h"
#include "CScript_TestObj.h"
#include "CScript_TilemapUnitLoader.h"
#include "CScript_Vespene.h"
#include "CScript_VespeneSmoke.h"

void UserClassInitializer::InitScript()
{
	CUserClassMgr* pMgr = CUserClassMgr::GetInst();
	CONSTRUCTOR_T(CScript_CameraMove);
	CONSTRUCTOR_T(CScript_FSM_Attack);
	CONSTRUCTOR_T(CScript_FSM_Building_Prod);
	CONSTRUCTOR_T(CScript_FSM_Death);
	CONSTRUCTOR_T(CScript_FSM_Idle);
	CONSTRUCTOR_T(CScript_FSM_Move_Ground);
	CONSTRUCTOR_T(CScript_FSM_Move_Hover);
	CONSTRUCTOR_T(CScript_MainCamSC_InGame);
	CONSTRUCTOR_T(CScript_Mineral);
	CONSTRUCTOR_T(CScript_MouseCursor);
	CONSTRUCTOR_T(CScript_SCEntity);
	CONSTRUCTOR_T(CScript_SCUnitVital);
	CONSTRUCTOR_T(CScript_TestObj);
	CONSTRUCTOR_T(CScript_TilemapUnitLoader);
	CONSTRUCTOR_T(CScript_Vespene);
	CONSTRUCTOR_T(CScript_VespeneSmoke);
}