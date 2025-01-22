#pragma once

#include "Graphics/Model.h"
#include "PlayerMonster.h"
#include "Tower.h"

// �w�
class Spider : public PlayerMonster
{
public:
	Spider();
	~Spider() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void OnDead() override;

	//�ڕW�ʒu�ݒ�
	void SetTarget(Tower* target) { this->target = target; }

	void Attack()override;

private:
	Model* model = nullptr;

	Tower* target = nullptr;//�ǐՑΏ�
	float speed = 2.0f;//�ړ����x

	float range = 10.0f;

	bool attackFlag = false;
};