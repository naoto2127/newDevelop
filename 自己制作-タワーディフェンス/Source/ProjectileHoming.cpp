#include "ProjectileHoming.h"

// �R���X�g���N�^
ProjectileHoming::ProjectileHoming(ProjectileManager* manager)
	: Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	// ���f�����������̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 3.0f;
}

// �f�X�g���N�^
ProjectileHoming::~ProjectileHoming()
{
	delete model;
}

void ProjectileHoming::Update(float elapsedTime)
{
	// ���������ilifeTimer ���� �o�ߎ��Ԃ������Ă����j
	lifeTimer -= elapsedTime;
	// lifeTimer �� 0 �ȉ��ɂȂ�����
	if (lifeTimer <= 0)
	{
		// �������폜
		Destroy();
	}

	// �ړ�
	{
		float speed = moveSpeed * elapsedTime;
		position.x += direction.x * speed;// x �̈ړ������i�ʒu += ���� * �����j
		position.y += direction.y * speed;// y �̈ړ�����
		position.z += direction.z * speed;// z �̈ړ�����
	}

	// ����
	{
		float turnSpeed = this->turnSpeed * elapsedTime;

		// �e�̈ʒu����^�[�Q�b�g�܂ł̃x�N�g�����v�Z
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);	// position �� XMVECTOR �ɕϊ�
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target); // target �� XMVECTOR �ɕϊ�
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position); // target - position

		// �[���x�N�g���łȂ��Ȃ��]����
		DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
		float lengthSq;
		DirectX::XMStoreFloat(&lengthSq, LengthSq);
		if (lengthSq > 0.00001f)
		{
			// �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
			Vec = DirectX::XMVector3Normalize(Vec);

			// direction �� XMVECTOR �ɕϊ�
			DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

			// �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���v�Z
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

			float dot;
			DirectX::XMStoreFloat(&dot, Dot);

			// �Q�̒P�ʃx�N�g���̊p�x���������ق�
			// 1.0 �ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
			float rot = 1.0f - dot;
			if (rot > DirectX::XMConvertToRadians(180))
			{
				rot = DirectX::XMConvertToRadians(180);
			}

			// ��]�p�x������Ȃ��]����������
			if (fabsf(rot) > 0.0001f)
			{
				// ��]�����v�Z
				DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);

				// ��]���Ɖ�]�ʂ����]�s����v�Z
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

				// ���݂̍s�����]������
				DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
				Transform = DirectX::XMMatrixMultiply(Transform, Rotation);

				// ��]��̑O�����������o���A�P�ʃx�N�g��������
				Direction = DirectX::XMVector3Normalize(Transform.r[2]);
				DirectX::XMStoreFloat3(&direction, Direction);

			}
		}
	}

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s����X�V
	model->UpdateTransform(transform);
}

void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target)
{
	this->direction = direction;
	this->position = position;
	this->target = target;

	// ���˂����u�Ԃɕ������v�Z���Ȃ��Ƃ����Ȃ��̂� UpdateTransform ���Ăяo���Ă���
	UpdateTransform();
}
