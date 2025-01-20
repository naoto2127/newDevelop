#pragma once

#include <vector>
#include "Tower.h"
#include <set>

// �G�l�~�[�Ǘ�
class TowerManager
{
private:
	TowerManager() {}
	~TowerManager() {}

public:
	// �B��̃C���X�^���X�擾
	static TowerManager& Instance()
	{
		static TowerManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �G�l�~�[�o�^
	void Register(Tower* tower);

	// �G�l�~�[�S�폜
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�^���[�f�o�b�N�`��
	void DrawDebugGUI();

	// �^���[���擾
	int GetTowerCount() const { return static_cast<int>(towers.size()); }

	// �^���[�擾
	Tower* GetTower(int index) { 
		if (index < 0 || index >= towers.size()) return nullptr; // �͈͊O�`�F�b�N
		return towers[index]; 
	}

	//// �^���[�ꗗ���擾
	//const std::vector<Tower*>& GetAllTowers() const { return towers; }


	// �e�ۍ폜
	void Remove(Tower* tower);

private:
	std::vector<Tower*>	towers;

	std::set<Tower*> removes;
};