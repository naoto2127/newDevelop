#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"

// �X�e�[�W
class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) = 0;

private:
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,	// �E����
		0,1,0,0,	// �����
		0,0,1,0,	// �O����
		0,0,0,1
	};
};