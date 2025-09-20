#include "pch.h"
#include "Vignette.h"

using namespace Gdiplus;

void Vignette::Initialize()
{
    GdiplusStartupInput input;
    GdiplusStartup(&gdiplusToken, &input, NULL);

#undef new
    vignette = new Image(L"../Sprites/Back/Vignette.png", FALSE); // 비네팅 PNG 로드
#define new DBG_NEW
}

void Vignette::Render(HDC hDC, Image* background)
{
    if (!vignette || !background) return;

    // 캐시 백버퍼가 없으면 생성
    if (!cachedScreen)
    {
#undef new

        cachedScreen = new Bitmap(WINCX, WINCY, PixelFormat32bppARGB);
#define new DBG_NEW

        Graphics gCached(cachedScreen);

        // 배경 먼저 그리기
        gCached.DrawImage(background, 0, 0, WINCX, WINCY);

        // 비네팅 합성 (원본 크기 그대로)
        gCached.SetCompositingMode(CompositingModeSourceOver);
        gCached.DrawImage(vignette, 0, 0, WINCX, WINCY);
    }

    // HDC로 한 번에 출력
    Graphics g(hDC);
    g.DrawImage(cachedScreen, 0, 0, WINCX, WINCY);
}

void Vignette::Release()
{
    if (vignette)
    {
        delete vignette;
        vignette = nullptr;
    }

    if (cachedScreen)
    {
        delete cachedScreen;
        cachedScreen = nullptr;
    }

    GdiplusShutdown(gdiplusToken);
}
