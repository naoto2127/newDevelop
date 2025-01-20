#include "Projectile.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"

Projectile::Projectile(ProjectileManager* manager)
	: manager(manager)
{
	manager->Register(this);
}

// �f�o�b�O�v���~�e�B�u�̕`��
void Projectile::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�f�o�b�O����`��
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Projectile::Destroy()
{
	manager->Remove(this);
}

// �s��X�V����
void Projectile::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// �O�x�N�g�����Z�o
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	// ���̏�̃x�N�g���Z�o
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);

	// �E�x�N�g�����Z�o
	Right = DirectX::XMVector3Cross(Up, Front);
	Right = DirectX::XMVector3Normalize(Right);

	// ��x�N�g�����Z�o
	Up = DirectX::XMVector3Cross(Front, Right);

	// �v�Z���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// �Z�o�������x�N�g������s����쐬
	transform._11 = right.x * scale.x;		// �E�����x�N�g���� x�iXx�j
	transform._12 = right.y * scale.x;		// �E�����x�N�g���� y�iXy�j
	transform._13 = right.z * scale.x;		// �E�����x�N�g���� z�iXz�j
	transform._14 = 0.0f;		
	transform._21 = up.x * scale.y;		// ������x�N�g���� x�iYx�j
	transform._22 = up.y * scale.y;		// ������x�N�g���� y�iYy�j
	transform._23 = up.z * scale.y;		// ������x�N�g���� z�iYz�j
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;		// �O�����x�N�g���� x�iZx�j
	transform._32 = front.y * scale.z;		// �O�����x�N�g���� y�iZy�j
	transform._33 = front.z * scale.z;		// �O�����x�N�g���� z�iZz�j
	transform._34 = 0.0f;
	transform._41 = position.x;	// �ʒu x�iPx�j
	transform._42 = position.y;	// �ʒu y�iPy�j
	transform._43 = position.z;	// �ʒu z�iPz�j
	transform._44 = 1.0f;

	// ���˕���
	this->direction = front;
}
