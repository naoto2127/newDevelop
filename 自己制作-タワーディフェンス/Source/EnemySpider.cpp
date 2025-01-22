#include "EnemySpider.h"
#include "Graphics/Model.h"

EnemySpider::EnemySpider()
{
	model = new Model("Data/Model/Spider/Spider.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	//�f�o�b�N���肪�X�P�[���ς���Ă���v�ł���悤�ɃX�P�[���ݒ�
	radius *= scale.x * 100;
	height *= scale.y * 30;

	angle.y = DirectX::XMConvertToRadians(90);

	acceleration = 2.0f;
}

EnemySpider::~EnemySpider()
{
	delete model;
}

void EnemySpider::Update(float elapsedTime)
{

	////�X�e�[�g���̍X�V����
	//switch (state)
	//{
	//case State::Idle:
	//	UpdateIdleState(elapsedTime);
	//	break;
	//case State:Attack:
	//	UpdateAttackState(elapsedTime);
	//	break;
	//}

	if (target != nullptr) {
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
		angle.y = newAngleY + DirectX::XM_PI; // // Y����]���X�V���A180�x�i�΁j�����Z�i���f�������΂Ɍ����Ă��邽�߁j
	}
	//// �V�����ʒu��ݒ�
	//SetPosition(position);

	//// ���͏����X�V
	//UpdateVelocity(elapsedTime);

	//// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}


void EnemySpider::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemySpider::OnDead()
{
	// ���M��j���iEnemy �N���X�i���N���X�j�� Destroy �֐����Ăяo���j
	Destroy();
}

void EnemySpider::Attack()
{
}

//void UpdateIdleState(float elapsedTime)
//{
//	//�^�C�}�[����
//	stateTimer -= elapsedTime;
//	if (stateTimer < 0.0f)
//	{
//		//�p�j�X�e�[�g�֑J��
//		TransitionWanderState();
//	}
//}