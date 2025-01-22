#include "PlayerMonsterManager.h"
#include "Collision.h"

void PlayerMonsterManager::Update(float elapsedTime)
{
	for (PlayerMonster* monster : monsters)
	{
		monster->Update(elapsedTime);
	}

	// ”jŠüˆ—
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

	// “G“¯m‚ÌÕ“Ëˆ—
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

// “G“¯m‚ÌÕ“Ëˆ—
void PlayerMonsterManager::CollisionMonsterVsMonsters()
{
	// “G‚P‚ğŒJ‚è•Ô‚µ•¶‚Å‰ñ‚·iƒJƒEƒ“ƒ^‚Í int i = 0j
	int monsterCount = GetMonsterCount();
	for (int i = 0; i < monsterCount; ++i)
	{
		// “G‚P‚ğæ“¾
		PlayerMonster* monster1 = GetMonster(i);

		// “G‚Q‚ğŒJ‚è•Ô‚µ•¶‚Å‰ñ‚·iƒJƒEƒ“ƒ^ int j = i + 1j
		for (int j = i + 1; j < monsterCount; ++j)
		{
			// “G‚Q‚ğæ“¾
			PlayerMonster* monster2 = GetMonster(j);

			DirectX::XMFLOAT3 pos1 = monster1->GetPosition();
			DirectX::XMFLOAT3 pos2 = monster2->GetPosition();

			DirectX::XMVECTOR vecPos1 = XMLoadFloat3(&pos1);
			DirectX::XMVECTOR vecPos2 = XMLoadFloat3(&pos2);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ‚ğŒÄ‚Ño‚·
			if (Collision::IntersectCylinderVsCylinder(
				monster1->GetPosition(),  // “G‚P‚ÌˆÊ’u
				monster1->GetRadius(),    // “G‚P‚Ì”¼Œa
				monster1->GetHeight(),    // “G‚P‚Ì‚‚³
				//enemy1->GetScale(),	  //“G‚P‚Ì‘å‚«‚³
				monster2->GetPosition(),  // “G‚Q‚ÌˆÊ’u
				monster2->GetRadius(),    // “G‚Q‚Ì”¼Œa
				monster2->GetHeight(),    // “G‚Q‚Ì‚‚³
				//enemy2->GetScale(),	  //“G‚Q‚Ì‘å‚«‚³
				outPosition             // outPosition
			))
			{
				// “G1‚Æ“G2‚Ì•ûŒüƒxƒNƒgƒ‹‚ğŒvZ
				DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecPos2, vecPos1);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));

				// d‚È‚è—Ê‚ğŒvZ
				float overlap = (monster1->GetRadius() + monster2->GetRadius()) - distance;
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

					monster1->SetPosition(pos1);
					monster2->SetPosition(pos2);
				}
			}

		}
	}
}

