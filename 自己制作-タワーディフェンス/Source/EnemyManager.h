#pragma once

#include <vector>
#include "Enemy.h"
#include <set>
#include"Tower.h"

// �G�l�~�[�Ǘ�
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:
	// �B��̃C���X�^���X�擾
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �G�l�~�[�o�^
	void Register(Enemy* enemy);

	// �G�l�~�[�S�폜
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// �G�l�~�[�擾
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	/*void Attack();*/

	// �e�ۍ폜
	void Remove(Enemy* enemy);

	void SpawnEnemy(Tower* target,Tower*spawnTower);



private:
	// �G�l�~�[���m�̏Փˏ���
	void CollisionEnemyVsEnemies();

private:
	std::vector<Enemy*>	enemies;

	std::set<Enemy*> removes;
};