#include "ResourceTest.h"

#include "Engine/Resource/ResourceManager.h"
#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/RenderSystem.h"
#include <windows.h>

#if ENABLE_RESOURCE_TEST
TestPlayer g_Player;

void Test_Initialize()
{
    // 텍스처 풀에 등록 (파일 경로는 본인 환경에 맞게)
    ResourceManager::GetInstance()->LoadTexture(L"PlayerTex", L"Resources/character_antonio.png");

    g_Player.textureKey = L"PlayerTex";
    g_Player.position = { 400.0f, 300.0f };
    g_Player.rotation = 0.0f;
    g_Player.scaleX = 1.0f;
    g_Player.scaleY = 1.0f;
    g_Player.zOrder = 10;
    g_Player.opacity = 1.0f;
    g_Player.flipX = false;
    g_Player.flipY = false;
}

void Test_Update(float dt)
{
    g_Player.rotation += 90.0f * dt;
    if (g_Player.rotation >= 360.0f) g_Player.rotation -= 360.0f;

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)  g_Player.position.x -= 200.0f * dt;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) g_Player.position.x += 200.0f * dt;
    if (GetAsyncKeyState(VK_UP) & 0x8000)    g_Player.position.y -= 200.0f * dt;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)  g_Player.position.y += 200.0f * dt;

    if (GetAsyncKeyState('F') & 0x0001) g_Player.flipX = !g_Player.flipX;
    if (GetAsyncKeyState('G') & 0x0001) g_Player.flipY = !g_Player.flipY;
}

void Test_Render()
{
    ID2D1Bitmap* pBitmap = ResourceManager::GetInstance()->GetTexture(g_Player.textureKey);
    if (!pBitmap) return;

    RenderCommand cmd;
    cmd.pTexture = pBitmap;
    cmd.position = g_Player.position;
    cmd.rotation = g_Player.rotation;
    cmd.scaleX = g_Player.scaleX;
    cmd.scaleY = g_Player.scaleY;
    cmd.zOrder = g_Player.zOrder;
    cmd.opacity = g_Player.opacity;
    cmd.flipX = g_Player.flipX;
    cmd.flipY = g_Player.flipY;

    RenderSystem::GetInstance()->SubmitCommand(cmd);
}
#endif  // ENABLE_RESOURCE_TEST