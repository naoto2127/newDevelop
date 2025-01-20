#pragma once

#include <d3d11.h>
#include "Graphics/Shader.h"
#include "Player.h"
#include "Object.h"

class Tower : public Object
{
public:

	Tower();
	virtual ~Tower() { delete model; }

	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�N�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�^���[�s�񏈗��֐�
	void UpdateTransform();

	//�^���[�f�o�b�N�`��
	void DrawDebugGUI();

	//�^���[�̓����蔻��iCharacter�p�j
	bool CheckCollision(const DirectX::XMFLOAT3& charPos, float charRadius, float charHeight);

	//Tower�̈ʒu���擾
	DirectX::XMFLOAT3 GetPosition() { return position; }

	//Tower�̈ʒu��ݒ�
	DirectX::XMFLOAT3 SetPosition(DirectX::XMFLOAT3 p) { return position = p; }
	
	//Tower�̉�]��ݒ�
	DirectX::XMFLOAT3 SetRotation(DirectX::XMFLOAT3 r) { return angle = r; }
	
	//Tower�̔��a���擾
	float GetRadius() { return radius; }
private:
	Model* model = nullptr;

	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,	// �E����
		0,1,0,0,	// �����
		0,0,1,0,	// �O����
		0,0,0,1
	};

	// ���a
	float radius = 100.0f;
	// ����
	float height = 280.0f;
};