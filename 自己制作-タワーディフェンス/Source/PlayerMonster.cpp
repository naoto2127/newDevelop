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

	// �Փ˔���p�̃f�o�b�O����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
