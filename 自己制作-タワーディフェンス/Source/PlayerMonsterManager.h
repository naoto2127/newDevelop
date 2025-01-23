#pragma once
#include <vector>
#include "PlayerMonster.h"
#include <set>
// �v���C���[�̃����X�^�[�Ǘ�
class PlayerMonsterManager
{
private:
	PlayerMonsterManager() {}
	~PlayerMonsterManager() {}
public:
	// �B��̃C���X�^���X�擾
	static PlayerMonsterManager& Instance()
	{
		static PlayerMonsterManager instance;
		return instance;
	}
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	// �v���C���[�̃����X�^�[�o�^
	void Register(PlayerMonster* monster);
	// �v���C���[�̃����X�^�[�S�폜
	void Clear();
	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	// �v���C���[�̃����X�^�[���擾
	int GetMonsterCount() const { return static_cast<int>(monsters.size()); }
	// �v���C���[�̃����X�^�[�擾
	PlayerMonster* GetMonster(int index) { return monsters.at(index); }
	void Attack();
	// �e�ۍ폜
	void Remove(PlayerMonster* monster);
private:
	// �v���C���[�̃����X�^�[���m�̏Փˏ���
	void CollisionMonsterVsMonsters();
private:
	std::vector<PlayerMonster*>	monsters;
	std::set<PlayerMonster*> removes;
};