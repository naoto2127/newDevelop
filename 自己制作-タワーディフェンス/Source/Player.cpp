#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include <Graphics/DebugRenderer.h>
#include <Graphics/Graphics.h>
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "Tower.h"
#include "TowerManager.h"

Player::Player()
{
	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	// �q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Hit.efk");
}

Player::~Player()
{
	delete hitEffect;

	delete model;
}

void Player::Update(float elapsedTime)
{
	// �ړ����͏���
	InputMove(elapsedTime);

	// �W�����v���͏���
	//InputJump();

	// �e�ۓ��͏���
	//InputProjectile();

	// ���͏����X�V
	UpdateVelocity(elapsedTime);

	// �e�ۍX�V����
	//projectileManager.Update(elapsedTime);

	// �v���C���[�ƓG�Ƃ̏Փˏ���
	CollisionPlayerVsEnemies();

	// �e�ۂƓG�̏Փˏ���
	CollisionProjectilesVsEnemies();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s����X�V
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	// �e�ە`�揈��
	projectileManager.Render(dc, shader);
}

// �f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
	//ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	//{
	//}
	//ImGui::End();
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::InputFloat3("Position", &position.x);
			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			// �X�P�[��
			ImGui::InputFloat3("Scale", &scale.x);
		}

}

// �f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	projectileManager.DrawDebugPrimitive();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// ���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J�����̕������擾
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// �ړ��x�N�g���́@XZ ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	// �J�����E�����x�N�g���� XZ �̒P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;	// �E�����x�N�g���� X
	float cameraRightZ = cameraRight.z;	// �E�����x�N�g���� Z
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// �J�����O�����x�N�g���� XZ �P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;	// �O�����x�N�g���� X
	float cameraFrontZ = cameraFront.z;	// �O�����x�N�g���� Z
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);// XZ �̕�����
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f��
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);	// �����ړ�
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);	// �����ړ�													// �����ړ�
	vec.y = 0.0f;

	return vec;
}



void Player::InputMove(float elapsedTime)
{
	// �i�s�����x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// �ړ�����
	Move(moveVec.x, moveVec.z, moveSpeed);

	// ���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}



void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// ���ׂĂ̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i) 
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		// �Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylinderVsCylinder(
			GetPosition(),// �v���C���[�̈ʒu
			GetRadius(),// �v���C���[�̔��a
			GetHeight(), // �v���C���[�̍���
			enemy->GetPosition(),// �G�̈ʒu
			enemy->GetRadius(),// �G�̔��a
			enemy->GetHeight(), // �G�̍���
			outPosition
		))
		{
			// �����o����̈ʒu�ݒ�ioutPosition ��G�̈ʒu�ɐݒ�j
			enemy->SetPosition(outPosition);
		}
	}
}

// �e�ۂƓG�̏Փˏ���
void Player::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// ���ׂĂ̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓˏ���
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);

		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			// �Փˏ���
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition
			))
			{
				// �_���[�W��^����
				if (enemy->ApplyDamage(1, 0.5f))
				{
					// ������΂�
					{
						// ������΂��ړ������̑��x�x�N�g��
						DirectX::XMFLOAT3 impulse;
						// ������΂���
						const float power = 10.0f;

						// �G�̈ʒu
						DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
						// �e�̈ʒu
						DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
						// �e����G�ւ̕����x�N�g�����v�Z�i�G�̈ʒu - �e�̈ʒu�j
						auto v = DirectX::XMVectorSubtract(eVec, pVec);
						// �����x�N�g���𐳋K��
						v = DirectX::XMVector3Normalize(v);

						DirectX::XMFLOAT3 vec;
						DirectX::XMStoreFloat3(&vec, v);

						// ������΂��ړ������̑��x�x�N�g���� power ���|���Đݒ�
						impulse.x = power * vec.x;
						impulse.y = power * 0.5;
						impulse.z = power * vec.z;

						enemy->AddImpulse(impulse);
					}

					// �q�b�g�G�t�F�N�g�Đ�
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					// �e�۔j��
					projectile->Destroy();
				}
			}
		}
	}
}

void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		// �W�����v�J�E���g���W�����v���~�b�g��菬�������
		if (jumpCount < jumpLimit)
		{
			// �W�����v�J�E���g��1���₵��
			jumpCount++;
			// �W�����v
			Jump(jumpSpeed);
		}
	}
}

// �e�ۓ��͏���
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���i�e�۔���
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		// �O�����i���f���� 0.01f ���Ă���̂ōs��̊g�k�� 0.01 �{����Ă���B�Ȃ̂őO���������擾����ہA
		// 100 �{�ɖ߂��Ă���擾���Ă��悢��j
		DirectX::XMFLOAT3 dir;
		dir.x = transform._31 * 100.0f;
		dir.y = 0.0f;
		dir.z = transform._33 * 100.0f;
		// 100 �{����̂��C���������Ƃ����̂ł���Ή��� y ������ɂ��ĕ��ʂ̌������v�Z���Ă��ǂ�
		//dir.x = sinf(angle.y);
		//dir.y = 0.0f;
		//dir.z = cosf(angle.y);
		
		// ���ˈʒu�i�v���C���[�̍�������j
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;
		// ����
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
	}

	// �ǔ��e�۔���
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		// �O����
		DirectX::XMFLOAT3 dir;
		dir.x = transform._31 * 100.0f;
		dir.y = 0.0f;
		dir.z = transform._33 * 100.0f;

		// ���ˈʒu�i�v���C���[�̍�������j
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;

		// �^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O���j
		DirectX::XMFLOAT3 target;
		target.x = pos.x + dir.x * 1000.0f;
		target.y = pos.y + dir.y * 1000.0f;
		target.z = pos.z + dir.z * 1000.0f;

		// ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
		float dist = FLT_MAX;
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)
		{
			// �G�Ƃ̋�������

			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			// �����̈ʒu�iposition �� XMVECTOR �ɕϊ��j
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			// �G�̈ʒu�ienemy->GetPosition() �� XMVECTOR �ɕϊ��j
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			// ��������G�܂ł̃x�N�g�����v�Z�inemy->GetPosition() - position�j
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
			// ���̃x�N�g���̒������v�Z
			DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
			float d;
			DirectX::XMStoreFloat(&d, D);

			// ���ׂĂ̓G�̒��ň�ԋ߂��G���^�[�Q�b�g�Ƃ��Đݒ�
			if (d < dist)
			{
				dist = d;
				target = enemy->GetPosition();
				target.y += enemy->GetHeight() * 0.5f;
			}
		}

		// ����
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos, target);
	}
}

void Player::OnLanding()
{
	// �W�����v�񐔂̃��Z�b�g
	jumpCount = 0;
}
