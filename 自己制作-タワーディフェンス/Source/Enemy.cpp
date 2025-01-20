#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}

void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
