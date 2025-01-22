#include "PlayerMonsterManager.h"
#include "Collision.h"

void PlayerMonsterManager::Update(float elapsedTime)
{
	for (PlayerMonster* monster : monsters)
	{
		monster->Update(elapsedTime);
	}

	// �j������
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

	// �G���m�̏Փˏ���
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

// �G���m�̏Փˏ���
void PlayerMonsterManager::CollisionMonsterVsMonsters()
{
	// �G�P���J��Ԃ����ŉ񂷁i�J�E���^�� int i = 0�j
	int monsterCount = GetMonsterCount();
	for (int i = 0; i < monsterCount; ++i)
	{
		// �G�P���擾
		PlayerMonster* monster1 = GetMonster(i);

		// �G�Q���J��Ԃ����ŉ񂷁i�J�E���^ int j = i + 1�j
		for (int j = i + 1; j < monsterCount; ++j)
		{
			// �G�Q���擾
			PlayerMonster* monster2 = GetMonster(j);

			DirectX::XMFLOAT3 pos1 = monster1->GetPosition();
			DirectX::XMFLOAT3 pos2 = monster2->GetPosition();

			DirectX::XMVECTOR vecPos1 = XMLoadFloat3(&pos1);
			DirectX::XMVECTOR vecPos2 = XMLoadFloat3(&pos2);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ���Ăяo��
			if (Collision::IntersectCylinderVsCylinder(
				monster1->GetPosition(),  // �G�P�̈ʒu
				monster1->GetRadius(),    // �G�P�̔��a
				monster1->GetHeight(),    // �G�P�̍���
				//enemy1->GetScale(),	  //�G�P�̑傫��
				monster2->GetPosition(),  // �G�Q�̈ʒu
				monster2->GetRadius(),    // �G�Q�̔��a
				monster2->GetHeight(),    // �G�Q�̍���
				//enemy2->GetScale(),	  //�G�Q�̑傫��
				outPosition             // outPosition
			))
			{
				// �G1�ƓG2�̕����x�N�g�����v�Z
				DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecPos2, vecPos1);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));

				// �d�Ȃ�ʂ��v�Z
				float overlap = (monster1->GetRadius() + monster2->GetRadius()) - distance;
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

					monster1->SetPosition(pos1);
					monster2->SetPosition(pos2);
				}
			}

		}
	}
}

