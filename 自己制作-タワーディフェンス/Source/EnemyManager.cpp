#include "EnemyManager.h"
#include "Collision.h"

void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	// ”jŠüˆ—
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

	// “G“¯m‚ÌÕ“Ëˆ—
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

// “G“¯m‚ÌÕ“Ëˆ—
void EnemyManager::CollisionEnemyVsEnemies()
{
	// “G‚P‚ğŒJ‚è•Ô‚µ•¶‚Å‰ñ‚·iƒJƒEƒ“ƒ^‚Í int i = 0j
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		// “G‚P‚ğæ“¾
		Enemy* enemy1 = GetEnemy(i);

		// “G‚Q‚ğŒJ‚è•Ô‚µ•¶‚Å‰ñ‚·iƒJƒEƒ“ƒ^ int j = i + 1j
		for (int j = i + 1; j < enemyCount; ++j)
		{
			// “G‚Q‚ğæ“¾
			Enemy* enemy2 = GetEnemy(j);

			DirectX::XMFLOAT3 pos1 = enemy1->GetPosition();
			DirectX::XMFLOAT3 pos2 = enemy2->GetPosition();

			DirectX::XMVECTOR vecPos1 = XMLoadFloat3(&pos1);
			DirectX::XMVECTOR vecPos2 = XMLoadFloat3(&pos2);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ‚ğŒÄ‚Ño‚·
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),  // “G‚P‚ÌˆÊ’u
				enemy1->GetRadius(),    // “G‚P‚Ì”¼Œa
				enemy1->GetHeight(),    // “G‚P‚Ì‚‚³
				//enemy1->GetScale(),	  //“G‚P‚Ì‘å‚«‚³
				enemy2->GetPosition(),  // “G‚Q‚ÌˆÊ’u
				enemy2->GetRadius(),    // “G‚Q‚Ì”¼Œa
				enemy2->GetHeight(),    // “G‚Q‚Ì‚‚³
				//enemy2->GetScale(),	  //“G‚Q‚Ì‘å‚«‚³
				outPosition             // outPosition
			))
			{
				// “G1‚Æ“G2‚Ì•ûŒüƒxƒNƒgƒ‹‚ğŒvZ
				DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecPos2, vecPos1);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));

				// d‚È‚è—Ê‚ğŒvZ
				float overlap = (enemy1->GetRadius() + enemy2->GetRadius()) - distance;
				if (overlap > 0)
				{
					// ³‹K‰»‚³‚ê‚½‰ğŒˆ•ûŒü
					DirectX::XMVECTOR resolveDir = DirectX::XMVector3Normalize(vecDir);

					// —¼•û‚Ì“G‚ğ”¼•ª‚¸‚Â“®‚©‚·
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
	
