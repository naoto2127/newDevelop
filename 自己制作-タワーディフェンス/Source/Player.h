#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Tower.h"

#include "Effect.h"

// �v���C���[
class Player : public Character
{
public:
	Player();
	~Player() override;

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �f�o�b�O�p GUI �`��
	void DrawDebugGUI();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

private:
	// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;

	// �ړ����͏���
	void InputMove(float elapsedTime);

	// �e�ۂƓG�̏Փˏ���
	void CollisionProjectilesVsEnemies();

	// �W�����v���͏���
	void InputJump();

	// �e�ۓ��͏���
	void InputProjectile();

protected:
	// ���n�����Ƃ��ɌĂ΂��
	void OnLanding() override;

private:
	Model*	model = nullptr;
	float	moveSpeed = 5.0f;
	float	turnSpeed = DirectX::XMConvertToRadians(720);
	float	jumpSpeed = 20.0f;
	int		jumpCount = 0;
	int		jumpLimit = 2;

	ProjectileManager	projectileManager;
	Effect*				hitEffect = nullptr;
};