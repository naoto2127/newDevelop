#pragma once

#include<DirectXMath.h>

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	//�s��X�V����
	void UpdateTransform();


	bool ApplyDamage(int damage, float invincibleTime);

	void Move(float vx, float vz, float speed);

	void Turn(float elapsedTime, float vx, float vz, float speed);

	void UpdateVelocity(float elapsedTime);

	void UpdateInvincibleTimer(float elapsedTime);

	void UpdateVerticalVelocity(float elapsedFrame);

	void UpdateVerticalMove(float elapsedTime);

	void UpdateHorizontalVelocity(float elapsedFrame);

	void UpdateHorizontalMove(float elapsedTime);

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//���n�����Ƃ��ɌĂ΂��
	virtual void OnLanding(){}

	//�_���[�W���󂯂��Ƃ��ɌĂ΂��
	virtual void OnDamaged(){}

	//���S�����Ƃ��ɌĂ΂��
	virtual void OnDead(){}

private:
	DirectX::XMFLOAT3 position{};
	DirectX::XMFLOAT3 angle{};
	DirectX::XMFLOAT3 scale{1,1,1};
	DirectX::XMFLOAT4X4 transform{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	//���a
	float radius = 0.5f;

	//�d��
	float gravity = -1.0f;

	//����
	DirectX::XMFLOAT3 velocity{};

	//true...�n�ʂɂ��Ă���
	bool isGround = false;

	//����
	float height = 2.0f;

	//���N���
	int health = 10;

	//�ő�
	int maxHealth = 10;

	//���G����
	float invincibleTimer = 1.0f;

	//���C�̒l
	float friction = 0.5f;

	//�����x
	float acceleration;

	//�ő�ړ����x
	float maxMoveSpeed = 5.0f;
	//�v�Z�p�̑��x�x�N�g���ix,y�j
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	//�󒆑���p�̌W��
	float airControl = 0.3f;

	float stepOffset = 1.0f;

	//�X�Η�
	float slopeRate = 1.0f;
};