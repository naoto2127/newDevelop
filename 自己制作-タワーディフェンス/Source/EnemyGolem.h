#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "Tower.h"

// �S�[����
class EnemyGolem : public Enemy
{
public:
	EnemyGolem();
	~EnemyGolem() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void OnDead() override;

	//�ڕW�ʒu�ݒ�
	void SetTarget(Tower* target) { this->target = target; }

private:
	Model* model = nullptr;
	
	Tower* target = nullptr;//�ǐՑΏ�
	float speed = 2.0f;//�ړ����x
};