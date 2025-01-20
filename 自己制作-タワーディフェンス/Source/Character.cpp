#include "Character.h"
#include "StageManager.h" 
#include "Stage.h"
#include "TowerManager.h"

void Character::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// �ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

/// <summary>
/// �_���[�W����
/// </summary>
/// <param name="damage">�^����_���[�W</param>
/// <param name="invincibleTime">�X�V���閳�G����</param>
/// <returns></returns>
bool Character::ApplyDamage(int damage, float invincibleTime)
{
	// �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage == 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health <= 0) return false;

	// ���G���Ԓ��̓_���[�W��^���Ȃ�
	if (this->invincibleTimer > 0.0f) return false;

	// ���G���Ԃ��Đݒ�
	this->invincibleTimer = invincibleTime;

	// �_���[�W����
	health -= damage;

	// ���S�ʒm
	if (health <= 0)
	{
		// ���S�����Ƃ��ɌĂяo���ׂ��֐����Ăяo��
		OnDead();
	}
	// �_���[�W�ʒm
	else
	{
		// �_���[�W���󂯂��Ƃ��ɌĂяo���ׂ��֐����Ăяo��
		OnDamaged();
	}

	// ���N��Ԃ��ύX������ true ��Ԃ�
	return true;
}

/// <summary>
/// �Ռ���^����i�Փ˂����Ƃ��Ɉړ����������^����j
/// </summary>
/// <param name="impulse">�ړ����鑬�x�x�N�g��</param>
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// ���݂̑��͂ɏՓ˂������Ɉړ����鑬�x�x�N�g�������Z
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

void Character::Move(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;

	// �ő呬�x�ݒ�
	maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;

	// �i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O�����������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);




	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (frontX * vx) + (frontZ * vz);

	// ���ϒl�� -1.0 �` 1.0 �ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق� 1.0 �ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rot > speed)
		rot = speed;



	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontZ * vx) - (frontX * vz);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		// �����v���
		angle.y -= rot;
	}
	else
	{
		// ���v���
		angle.y += rot;
	}


}

void Character::Jump(float speed)
{
	// ������̗͂�ݒ�
	velocity.y = speed;
}

void Character::UpdateVelocity(float elapsedTime)
{
	// �o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);

	// �������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);
	
	// �����ړ��X�V����
	UpdateVerticalMove(elapsedTime);

	// �����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);
}

// ���G���Ԃ̍X�V
void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (this->invincibleTimer > 0.0f)
	{
		this->invincibleTimer -= elapsedTime;
	}
}

// �������͍X�V����
// �c�����̑��x�̍X�V�̌v�Z�����Ă��鏈���̕���
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	// �d�͏���
	velocity.y += gravity * elapsedFrame;
}

// �����ړ��X�V����
// �c�����̈ړ��̍X�V�̌v�Z�����Ă��鏈���̕���
void Character::UpdateVerticalMove(float elapsedTime)
{
	//���������̈ړ���
	float my = velocity.y * elapsedTime;

	//������
	if (my < 0.0f)
	{
		//���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		//���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x, position.y + my - 0.5f, position.z };

		//���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//�n�ʂɐڒn���Ă���
			position.y = hit.position.y;

			//�X�Η��̌v�Z
			//�ʂ̖@������ xz ���ʂ̌X�����v�Z
			float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x 
				+ hit.normal.z * hit.normal.z);
			//�X�Η��i�������ʂ̌X���Ƃ��̌�������X�Η����v�Z�����]�j
			slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

			//���n����
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			//�󒆂ɕ����Ă���
			position.y += my;
			isGround = false;
		}
	}
	//�㏸��
	else if (my > 0.0f)
	{
		position.y += my;
		isGround = false;
	}

}

void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ ���ʂ̑��͂���������

	// ���x�̕��ʂł��� XZ �̒������v�Z
	DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

	DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
	float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));

	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsedFrame;

		// �󒆂ɂ���Ƃ��͖��C�͂����炷
		if (isGround == false) friction *= airControl;

		// ���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			// ���x��P�ʃx�N�g����
			auto v = DirectX::XMVector2Normalize(velVel);

			// �P�ʃx�N�g�����������x�� friction ���X�P�[�����O
			v = DirectX::XMVectorScale(v, friction);

			// �X�P�[�����O�����x�N�g���𑬓x�Ɍ��Z
			DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

			velocity.x = vel.x;
			velocity.z = vel.y;
		}
		// �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			// ���x�x�N�g���� 0 �ɂ���
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	if (length <= maxMoveSpeed)
	{
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			float acceleration = this->acceleration * elapsedFrame;

			// �󒆂ɂ���Ƃ��͉����͂����炷
			if (isGround == false) acceleration *= airControl;

			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			// �󒆂ɂ���Ƃ��͉����͂����炷
			if (isGround == false) acceleration *= airControl;

			if (length > maxMoveSpeed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * maxMoveSpeed; 
				velocity.z = vz * maxMoveSpeed; 
			}

			//�����ŃK�^�K�^���Ȃ��悤�ɂ���
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= length * slopeRate * elapsedFrame;
			}
		}
	}



#if false
	// XZ ���ʂ̑��͂������̏���
	if (length <= maxMoveSpeed)
	{
		DirectX::XMFLOAT2 moveVec = { moveVecX , moveVecZ };
		DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

		// �ړ������x�N�g���̒����v�Z���Ă���iLength �֐����g���Ă��悢�j
		DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&moveVec);
		float moveVecLength = DirectX::XMVectorGetX(DirectX::XMVector2Length(vec));

		//float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ�
		if (moveVecLength > 0.0f)
		{
			// ��������
			// �����͂̌v�Z
			float acceleration = this->acceleration * elapsedFrame;

			// �󒆂ɂ���Ƃ��͉����͂����炷
			if (isGround == false) acceleration *= airControl;

			// �ړ��x�N�g���ɂ���������
			DirectX::XMVECTOR velVec = DirectX::XMLoadFloat2(&vel);
			velVec = DirectX::XMVectorAdd(velVec, DirectX::XMVectorScale(vec, acceleration));
			// �X�P�[�����O�����x�N�g�������x�x�N�g��

			DirectX::XMStoreFloat2(&vel, velVec);
			velocity.x = vel.x;
			velocity.z = vel.y;

			// �ő呬�x�����i�v�Z�������x�ōēx���������߂�j
			float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVec));
			if (length > maxMoveSpeed)
			{
				// ���x��P�ʃx�N�g����
				auto v = DirectX::XMVector2Normalize(velVel);

				// �P�ʃx�N�g�����������x�� maxMoveSpeed ���X�P�[�����O
				v = DirectX::XMVectorScale(v, maxMoveSpeed);

				// �X�P�[�����O�����x�N�g�������x�x�N�g��
				DirectX::XMStoreFloat2(&vel, v);

				velocity.x = vel.x;
				velocity.z = vel.y;
			}


		}
	}
#endif
	// �ړ������x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//// �ړ�����
	//position.x += velocity.x * elapsedTime;
	//position.z += velocity.z * elapsedTime;

#if 1
	//�������͗ʌv�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		//�����ړ��l
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;


		//DirectX::XMFLOAT3 proposedPosition = { position.x + mx, position.y, position.z + mz };
		//// Tower �̏Փ˔���
		//for (const auto& tower : TowerManager::Instance().GetAllTowers())
		//{
		//	if (tower->CheckCollision(proposedPosition, radius, height))
		//	{
		//		// �Փ˂��Ă���ꍇ�A�ړ����L�����Z���܂��͕␳
		//		return; // �ړ����L�����Z��
		//	}
		//}

		//���C�̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz};

		//���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//�ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			//�ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			//�␳�ʒu�̌v�Z
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			position.x = collectPosition.x;
			position.z = collectPosition.z;

			//�ǂ�������փ��C�L���X�g
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				//�ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
				position.x = collectPosition.x;
				position.z = collectPosition.z;
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
			}
		}
		else
		{
			//�ړ�
			position.x += mx;
			position.z += mz;
		}
	}

#endif

}
