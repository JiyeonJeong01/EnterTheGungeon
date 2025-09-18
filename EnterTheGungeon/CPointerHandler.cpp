#include "pch.h"
#include "CPointerHandler.h"
#include "CInputManager.h"
#include "CCollider.h"

CPointerHandler::CPointerHandler(CObject* pObj) : CUIComponent(pObj)
{
}

CPointerHandler::~CPointerHandler()
{
}

void CPointerHandler::Initialize()
{
	rDetectRange = pOwner->Get_Collider()->Get_PCollider();

}

void CPointerHandler::Update_PointerState()
{
	bPrevInRange = bCurrInRange;
	bCurrInRange = PtInRect(rDetectRange, MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition());

	bPrevClick = bCurrClick;
	bCurrClick = bCurrInRange && MANAGER(CInputManager*, M_INPUT)->Get_Key(VK_LBUTTON);

}
