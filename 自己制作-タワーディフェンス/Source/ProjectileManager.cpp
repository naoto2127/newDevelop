#include "ProjectileManager.h"

// �R���X�g���N�^
ProjectileManager::ProjectileManager()
{
}

// �f�X�g���N�^
ProjectileManager::~ProjectileManager()
{
	Clear();
}

// �X�V����
void ProjectileManager::Update(float elapsedTime)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Update(elapsedTime);
	}

	// �j������
	for (Projectile* projectile : removes)
	{
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
	
		if (it != projectiles.end())
		{
			projectiles.erase(it);
		}

		delete projectile;
	}

	// �j�����X�g���N���A
	removes.clear();






	//std::vector<Projectile*>::iterator it = projectiles.begin();
	//while (it != projectiles.end())
	//{
	//	if ((*it)->isRemove == true)
	//	{
	//		delete (*it);
	//		it = projectiles.erase(it);
	//	}
	//	else
	//	{
	//		it++;
	//	}
	//}



}

void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Projectile* projectile : projectiles)
	{
		projectile->Render(dc, shader);
	}
}

void ProjectileManager::DrawDebugPrimitive()
{
	for (Projectile* projectile : projectiles)
	{
		projectile->DrawDebugPrimitive();
	}
}

void ProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
	for (Projectile* projectile : projectiles)
	{
		delete projectile;
	}
	projectiles.clear();
}

void ProjectileManager::Remove(Projectile* projectile)
{
	// �j�����X�g�ɒǉ�
	removes.insert(projectile);
}


