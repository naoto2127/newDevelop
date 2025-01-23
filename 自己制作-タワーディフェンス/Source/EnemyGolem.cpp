#include "EnemyGolem.h"
#include"EnemyManager.h"
#include"PlayerMonsterManager.h"

EnemyGolem::EnemyGolem()
{
	model = new Model("Data/Model/StoneGolem/StoneGolem.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.003f;

	radius = scale.x * 300;
	height = scale.y * 1000;

	acceleration = 1.5f;
}

EnemyGolem::~EnemyGolem()
{
	delete model;
}

void EnemyGolem::Update(float elapsedTime)
{
	if (attackFlag)
	{
	}
	else if (target != nullptr) {
		if (GetPosition().x != target->GetPosition().x &&
			GetPosition().y != target->GetPosition().y &&
			GetPosition().z != target->GetPosition().z)
		{
			// �v���C���[�̈ʒu���擾
			DirectX::XMFLOAT3 targetPos = target->GetPosition();
			// ���݂̈ʒu���擾
			DirectX::XMFLOAT3 currentPos = GetPosition();
			// �v���C���[�ւ̕������v�Z
			DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat3(&targetPos),
				DirectX::XMLoadFloat3(&currentPos)
			);
			dir = DirectX::XMVector3Normalize(dir);
			// �V�����ʒu���v�Z
			DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(
				DirectX::XMLoadFloat3(&currentPos),
				DirectX::XMVectorScale(dir, acceleration * elapsedTime)
			);
			// �V�����ʒu��ݒ�
			DirectX::XMFLOAT3 updatedPos;
			DirectX::XMStoreFloat3(&updatedPos, newPos);
			SetPosition(updatedPos);
			// �i�s�����Ɋ�Â��ĉ�]���v�Z
			DirectX::XMFLOAT3 forward;
			DirectX::XMStoreFloat3(&forward, dir);
			float newAngleY = atan2f(forward.x, forward.z); // Z������ɉ�]�p���v�Z
			angle.y = newAngleY; // // Y����]���X�V����
		}
	}

	// ���͏����X�V
	UpdateVelocity(elapsedTime);

	// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void EnemyGolem::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemyGolem::OnDead()
{
	// ���M��j���iEnemy �N���X�i���N���X�j�� Destroy �֐����Ăяo���j
	Destroy();
}

void EnemyGolem::Attack()
{
	int monsterCount = PlayerMonsterManager::Instance().GetMonsterCount();
	PlayerMonster* closestMonster = nullptr;
	float closestDistance = FLT_MAX; // �����l�͔��ɑ傫���l
	for (int i = 0; i < monsterCount; i++)
	{
		PlayerMonster* monster = PlayerMonsterManager::Instance().GetMonster(i);
		//�G�l�~�[�̂Ȃ��Ŏ����Ɉ�ԋ߂��G��T��
		DirectX::XMVECTOR v = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&monster->GetPosition()));
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(v));
		//��ԋ߂��G�Ƃ̋��������߂�
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestMonster = monster;
		}
	}
	// ��ԋ߂��G��������A���������͈͓��ł���΍U��
	if (closestMonster && closestDistance <= range)
	{
		attackFlag = true;
		//�U������������
		closestMonster->ApplyDamage(1, 0.5f);
	}
	else
	{
		attackFlag = false;
	}
}


