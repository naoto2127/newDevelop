#include "ProjectileStraight.h"

// �R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
	: Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	// �\���T�C�Y�𒲐�
	scale.x = scale.y = scale.z = 3.0f;
}

// �f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

void ProjectileStraight::Update(float elapsedTime)
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
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;// x �̈ړ������i�ʒu += ���� * �����j
	position.y += direction.y * speed;// y �̈ړ�����
	position.z += direction.z * speed;// z �̈ړ�����
	
	//DirectX::XMVECTOR dVec = DirectX::XMLoadFloat3(&direction);
	//DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&position);
	//DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(pVec, DirectX::XMVectorScale(dVec, speed)));

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
