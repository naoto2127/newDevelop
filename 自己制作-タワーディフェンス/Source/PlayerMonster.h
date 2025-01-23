#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
// �v���C���[�̃����X�^�[
class PlayerMonster : public Character
{
public:
	PlayerMonster() {}
	~PlayerMonster() override {}
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
	virtual void Attack() = 0;
	void Destroy();
	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();
	// ���̑��̃����o�֐��i�ʒu�A���a�j
	DirectX::XMFLOAT3 GetPosition() const { return position; }
	float GetRadius() const { return radius; }
};