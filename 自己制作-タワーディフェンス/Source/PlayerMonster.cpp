#include "PlayerMonster.h"
#include "Graphics/Graphics.h"
#include "PlayerMonsterManager.h"

void PlayerMonster::Destroy()
{
	PlayerMonsterManager::Instance().Remove(this);
}

void PlayerMonster::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
