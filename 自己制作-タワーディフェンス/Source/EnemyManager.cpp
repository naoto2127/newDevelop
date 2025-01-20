#include "EnemyManager.h"
#include "Collision.h"

void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	// �j������
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

	// �G���m�̏Փˏ���
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

// �G���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{
	// �G�P���J��Ԃ����ŉ񂷁i�J�E���^�� int i = 0�j
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		// �G�P���擾
		Enemy* enemy1 = GetEnemy(i);

		// �G�Q���J��Ԃ����ŉ񂷁i�J�E���^ int j = i + 1�j
		for (int j = i + 1; j < enemyCount; ++j)
		{
			// �G�Q���擾
			Enemy* enemy2 = GetEnemy(j);

			DirectX::XMFLOAT3 pos1 = enemy1->GetPosition();
			DirectX::XMFLOAT3 pos2 = enemy2->GetPosition();

			DirectX::XMVECTOR vecPos1 = XMLoadFloat3(&pos1);
			DirectX::XMVECTOR vecPos2 = XMLoadFloat3(&pos2);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ���Ăяo��
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),  // �G�P�̈ʒu
				enemy1->GetRadius(),    // �G�P�̔��a
				enemy1->GetHeight(),    // �G�P�̍���
				//enemy1->GetScale(),	  //�G�P�̑傫��
				enemy2->GetPosition(),  // �G�Q�̈ʒu
				enemy2->GetRadius(),    // �G�Q�̔��a
				enemy2->GetHeight(),    // �G�Q�̍���
				//enemy2->GetScale(),	  //�G�Q�̑傫��
				outPosition             // outPosition
			))
			{
				// �G1�ƓG2�̕����x�N�g�����v�Z
				DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecPos2, vecPos1);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));

				// �d�Ȃ�ʂ��v�Z
				float overlap = (enemy1->GetRadius() + enemy2->GetRadius()) - distance;
				if (overlap > 0)
				{
					// ���K�����ꂽ��������
					DirectX::XMVECTOR resolveDir = DirectX::XMVector3Normalize(vecDir);

					// �����̓G�𔼕���������
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
	
