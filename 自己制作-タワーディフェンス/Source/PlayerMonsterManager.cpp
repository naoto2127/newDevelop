#include "PlayerMonsterManager.h"
#include "Collision.h"
void PlayerMonsterManager::Update(float elapsedTime)
{
	for (PlayerMonster* monster : monsters)
	{
		monster->Update(elapsedTime);
	}
	// 破棄処理
	for (PlayerMonster* monster : removes)
	{
		std::vector<PlayerMonster*>::iterator it = std::find(monsters.begin(), monsters.end(), monster);
		if (it != monsters.end())
		{
			monsters.erase(it);
		}
		delete monster;
	}
	removes.clear();
	// 敵同士の衝突処理
	CollisionMonsterVsMonsters();
}
void PlayerMonsterManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (PlayerMonster* monster : monsters)
	{
		monster->Render(context, shader);
	}
}
void PlayerMonsterManager::Register(PlayerMonster* monster)
{
	monsters.emplace_back(monster);
}
void PlayerMonsterManager::Clear()
{
	for (PlayerMonster* monster : monsters)
	{
		delete monster;
	}
	monsters.clear();
}
void PlayerMonsterManager::DrawDebugPrimitive()
{
	for (PlayerMonster* monster : monsters)
	{
		monster->DrawDebugPrimitive();
	}
}
void PlayerMonsterManager::Attack()
{
	for (PlayerMonster* monster : monsters)
	{
		monster->Attack();
	}
}
void PlayerMonsterManager::Remove(PlayerMonster* monster)
{
	removes.insert(monster);
}
// 敵同士の衝突処理
void PlayerMonsterManager::CollisionMonsterVsMonsters()
{
	// 敵１を繰り返し文で回す（カウンタは int i = 0）
	int monsterCount = GetMonsterCount();
	for (int i = 0; i < monsterCount; ++i)
	{
		// 敵１を取得
		PlayerMonster* monster1 = GetMonster(i);
		// 敵２を繰り返し文で回す（カウンタ int j = i + 1）
		for (int j = i + 1; j < monsterCount; ++j)
		{
			// 敵２を取得
			PlayerMonster* monster2 = GetMonster(j);
			DirectX::XMFLOAT3 pos1 = monster1->GetPosition();
			DirectX::XMFLOAT3 pos2 = monster2->GetPosition();
			DirectX::XMVECTOR vecPos1 = XMLoadFloat3(&pos1);
			DirectX::XMVECTOR vecPos2 = XMLoadFloat3(&pos2);
			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere を呼び出す
			if (Collision::IntersectCylinderVsCylinder(
				monster1->GetPosition(),  // 敵１の位置
				monster1->GetRadius(),    // 敵１の半径
				monster1->GetHeight(),    // 敵１の高さ
				//enemy1->GetScale(),	  //敵１の大きさ
				monster2->GetPosition(),  // 敵２の位置
				monster2->GetRadius(),    // 敵２の半径
				monster2->GetHeight(),    // 敵２の高さ
				//enemy2->GetScale(),	  //敵２の大きさ
				outPosition             // outPosition
			))
			{
				// 敵1と敵2の方向ベクトルを計算
				DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecPos2, vecPos1);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));
				// 重なり量を計算
				float overlap = (monster1->GetRadius() + monster2->GetRadius()) - distance;
				if (overlap > 0)
				{
					// 正規化された解決方向
					DirectX::XMVECTOR resolveDir = DirectX::XMVector3Normalize(vecDir);
					// 両方の敵を半分ずつ動かす
					DirectX::XMVECTOR adjustment = DirectX::XMVectorScale(resolveDir, overlap * 0.5f);
					vecPos1 = DirectX::XMVectorSubtract(vecPos1, adjustment);
					vecPos2 = DirectX::XMVectorAdd(vecPos2, adjustment);
					XMStoreFloat3(&pos1, vecPos1);
					XMStoreFloat3(&pos2, vecPos2);
					monster1->SetPosition(pos1);
					monster2->SetPosition(pos2);
				}
			}
		}
	}
}