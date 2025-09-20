#pragma once

class Vignette
{
public:
    ULONG_PTR gdiplusToken;

private:
    Gdiplus::Image* vignette = nullptr;       // 원본 비네팅 PNG
    Gdiplus::Bitmap* cachedScreen = nullptr;  // 미리 합성한 백버퍼

public:
    void Initialize();
    void Render(HDC hDC, Gdiplus::Image* background);
    void Release();
};
