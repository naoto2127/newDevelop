#include "EnemyManager.h"
#include "Collision.h"

void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	// 破棄処理
	for (Enemy* enemy : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		delete enemy;
	}

	removes.clear();

	// 敵同士の衝突処理
	CollisionEnemyVsEnemies();
}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

void EnemyManager::Remove(Enemy* enemy)
{
	removes.insert(enemy);
}

void EnemyManager::Attack()
{
	for (Enemy* enemy : enemies)
	{
		enemy->Attack();
	}
}

// 敵同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
	// 敵１を繰り返し文で回す（カウンタは int i = 0）
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		// 敵１を取得
		Enemy* enemy1 = GetEnemy(i);

		// 敵２を繰り返し文で回す（カウンタ int j = i + 1）
		for (int j = i + 1; j < enemyCount; ++j)
		{
			// 敵２を取得
			Enemy* enemy2 = GetEnemy(j);

			DirectX::XMFLOAT3 pos1 = enemy1->GetPosition();
			DirectX::XMFLOAT3 pos2 = enemy2->GetPosition();

			DirectX::XMVECTOR vecPos1 = XMLoadFloat3(&pos1);
			DirectX::XMVECTOR vecPos2 = XMLoadFloat3(&pos2);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere を呼び出す
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),  // 敵１の位置
				enemy1->GetRadius(),    // 敵１の半径
				enemy1->GetHeight(),    // 敵１の高さ
				//enemy1->GetScale(),	  //敵１の大きさ
				enemy2->GetPosition(),  // 敵２の位置
				enemy2->GetRadius(),    // 敵２の半径
				enemy2->GetHeight(),    // 敵２の高さ
				//enemy2->GetScale(),	  //敵２の大きさ
				outPosition             // outPosition
			))
			{
				// 敵1と敵2の方向ベクトルを計算
				DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecPos2, vecPos1);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));

				// 重なり量を計算
				float overlap = (enemy1->GetRadius() + enemy2->GetRadius()) - distance;
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

					enemy1->SetPosition(pos1);
					enemy2->SetPosition(pos2);
				}
			}

		}
	}
}			
	
