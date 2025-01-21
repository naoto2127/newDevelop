#pragma once

#include <DirectXMath.h>

// �L�����N�^�[
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	// �s��X�V����
	void UpdateTransform();

	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// ��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// ��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// �X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// ���a�擾
	float GetRadius() const { return radius; }

	// �n�ʂɐݒu���Ă��邩
	bool IsGround() const { return isGround; }

	// �����擾
	float GetHeight() const { return height; }

	//���N��Ԃ��擾
	int GetHealth() const { return health; }

	//�ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return maxHealth; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincibleTime);

	// �Ռ���^����i�Փ˂����Ƃ��Ɉړ����������^����j
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

protected:
	// �ړ�����
	void Move(float vx, float vz, float speed);

	// ���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// �W�����v����
	void Jump(float speed);

	// ���͏����X�V
	void UpdateVelocity(float elapsedTime);

	// ���G���ԍX�V
	void UpdateInvincibleTimer(float elapsedTime);

	// ���n�����Ƃ��ɌĂ΂��
	virtual void OnLanding() {}

	// �_���[�W���󂯂��Ƃ��ɌĂ΂��
	virtual void OnDamaged() {}

	// ���S�����Ƃ��ɌĂ΂��
	virtual void OnDead() {}

private:
	// �������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);

	// �������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);

protected:
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,	// �E����
		0,1,0,0,	// �����
		0,0,1,0,	// �O����
		0,0,0,1
	};

	DirectX::XMFLOAT3 target{};

	// ���a
	float radius = 0.5f;
	
	// �d��
	float	gravity = -1.0f;

	// ����
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// true...�n�ʂɂ��Ă���
	bool isGround = false;

	// ����
	float height = 2.0f;
	
	// ���N���
	int health = 5;

	// �ő�
	int maxHealth = 5;

	// ���G����
	float invincibleTimer = 1.0f;

	// ���C�̒l
	float friction = 0.5f;

	// �����x
	float acceleration = 1.0f;

	// �ő�ړ����x
	float maxMoveSpeed = 5.0f;

	// �v�Z�p�̑��x�x�N�g���ix, z�j
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	// �󒆑���p�̌W��
	float airControl = 0.3f;

	float stepOffset = 1.0f;

	//�X�Η�
	float slopeRate = 1.0f;
};