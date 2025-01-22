#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

#include "Tower.h"

// �w�
class EnemySpider : public Enemy
{
public:
	EnemySpider();
	~EnemySpider() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//���S����
	void OnDead() override;


	//�ڕW�ʒu�ݒ�
	void SetTarget(Tower* target) { this->target = target; }

private:
	//�A�j���[�V�����X�V����
	void UpdateAnimation();

	//�����ɓ���������U���A�j���[�V��������
	bool IsColliding();


	//�X�e�[�g
	enum class State
	{

		Idle,
		Walk,
		Attack,
		Damage,
		Death,
	};

	State	state = State::Idle;
private:
	Model* model = nullptr;

	Tower* target = nullptr;//�ǐՑΏ�

	float				stateTimer = 0.0f;

};