#include "pch.h"
#include "CIntroScene.h"
#include "CInputManager.h"
#include "CSceneManager.h"


CIntroScene::CIntroScene()
{
}

CIntroScene::~CIntroScene()
{
    Release();
}

void CIntroScene::Initialize()
{
    hVideo = MCIWndCreate(g_hWnd,
        nullptr,
        WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR,
        L"../Video/IntroVideo00.wmv");

    MoveWindow(hVideo, 0, 0, WINCX, WINCY, FALSE);

    MCIWndPlay(hVideo);

}

void CIntroScene::Update()
{
    if ( MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RETURN) ||
        MCIWndGetLength(hVideo) <= MCIWndGetPosition(hVideo))
    {
        MANAGER(CSceneManager*, M_SCENE)->Change_Scene(SC_STORE);
    }
}

void CIntroScene::Late_Update()
{
}

void CIntroScene::Render(HDC hDC)
{
}

void CIntroScene::Release()
{
    MCIWndClose(hVideo);
}
