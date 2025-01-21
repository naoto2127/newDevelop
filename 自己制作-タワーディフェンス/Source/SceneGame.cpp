#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Collision.h"
#include "Camera.h"
#include "StageManager.h"
#include "StageMain.h"
#include "EnemySpider.h"
#include "EnemyGolem.h"
#include "EnemyManager.h"
#include <EffectManager.h>
#include "TowerManager.h"
#include "Tower.h"
#include "Input/Input.h"


// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	// �J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		{ 0,10,-10 },
		{ 0, 0, 0 },
		{ 0, 1, 0 }
	);
	camera.SetPspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// �J�����R���g���[���[������
	cameraController = new CameraController();
	cameraController->SetRange(40);
	cameraController->SetAngle({ DirectX::XMConvertToRadians(60), 0, 0 });
	

	//�^���[������
	playerTower = new Tower(); // �^���[����
	playerTower->SetPosition({ 0, 0, -25 }); // �ʒu�ݒ�
	playerTower->SetRotation({ 0, DirectX::XMConvertToRadians(90), 0 }); // ��]�ݒ�

	// �^���[���Ǘ�����d�g�݂ɓo�^
	TowerManager::Instance().Register(playerTower);

	enemyTower = new Tower(); // �^���[����
	enemyTower->SetPosition({ 0, 0, 25 }); // �ʒu�ݒ�
	enemyTower->SetRotation({ 0, DirectX::XMConvertToRadians(-90), 0 }); // ��]�ݒ�

	// �^���[���Ǘ�����d�g�݂ɓo�^
	TowerManager::Instance().Register(enemyTower);

	//�Ԃ̏����ݒ�
	for (int i = 0; i < numFort; i++)
	{
		fort[i] = new Tower();
		fort[i]->SetPosition({ static_cast<float>(i) * 15 - 15,0,0 });
		TowerManager::Instance().Register(fort[i]);
	}

	{
		//�Q�[�W�X�v���C�g
		gauge = new Sprite();
		
		//UI�{�^���\��
		ui = new UI();

		// �X�p�C�_�[�X�|�[���R�[���o�b�N��ݒ肷��
		ui->SetSpawnSpiderCallback([this]() {
			if (ui->GetButtonSpider())
			{
				EnemySpider* spider = new EnemySpider();
				spider->SetPosition(playerTower->GetPosition());
				spider->SetTarget(fort[0]);
				EnemyManager::Instance().Register(spider);

				// �Ō�ɃX�|�[���������Ԃ����Z�b�g
				ui->SetCooldownTimerSpider({});

				// �{�^���𖳌������ăN�[���_�E����
				ui->SetButtonSpider(false);

			}
		});

		// �S�[�����X�|�[���R�[���o�b�N��ݒ肷��
		ui->SetSpawnGolemCallback([this]() {
			if (ui->GetButtonGolem())
			{
				EnemyGolem* golem = new EnemyGolem();
				golem->SetPosition(playerTower->GetPosition());
				golem->SetTarget(fort[2]);
				EnemyManager::Instance().Register(golem);

				// �Ō�ɃX�|�[���������Ԃ����Z�b�g
				ui->SetCooldownTimerGolem({});

				// �{�^���𖳌������ăN�[���_�E����
				ui->SetButtonGolem(false);

			}
		});
	}

}

// �I����
void SceneGame::Finalize()
{
	//�Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	// �G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�X�e�[�W������
	StageManager::Instance().Clear();

	// �J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�^���[�I����
	TowerManager::Instance().Clear();

	if (ui)
	{
		delete ui;
		ui = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	// �J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = playerTower->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//UI�\��
	ui->Update(elapsedTime);
	
	//�Ԃ̍X�V����
	TowerManager::Instance().Update(elapsedTime);

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);
	
	// �G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	//�����蔻�菈��
	//player->CollisionPlayerVsEnemies();

	// �G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		// �V�F�[�_�̊J�n
		shader->Begin(dc, rc);

		// �G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		//�^���[�`��
		TowerManager::Instance().Render(dc, shader);

		// �V�F�[�_�̏I��
		shader->End(dc);
	}

	// 3D �G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		// �G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//�^���[�f�o�b�N�`��
		TowerManager::Instance().DrawDebugPrimitive();
	}

	// 2D�X�v���C�g�`��
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);

		if (ui)
		{
			//UI�\��
			ui->Render();
		}

	}

	// 2D�f�o�b�OGUI�`��
	{
		if (ImGui::TreeNode("Object"))
		{
			if (ImGui::TreeNode("TowerManager"))
			{
				//�Ԃ̃f�o�b�N�`��@
				TowerManager::Instance().DrawDebugGUI();

				ImGui::TreePop();
			}

			//UI�̃f�o�b�N�`��
			ui->DrawDebugGUI();

			ImGui::TreePop();
		}
	}
}

void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�S�Ă̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//�G�l�~�[�̓���̃��[���h���W���v�Z
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();

		//XMVECTOR�ɕϊ�
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		//���[���h���W����X�N���[�����W�֕ϊ�
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//�X�N���[�����W���@XMFLOAT2�@�ɕϊ�
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		//��������̓Q�[�W�̕\������
		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		//���݁@HP�@/�@�ő�HP����Q�[�W�̒����̔䗦���v�Z
		float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

		//�Q�[�W��`��
		gauge->Render(dc,
			screenPosition.x - gaugeWidth * 0.5f,//�v�Z�����QD�ʒu�𒆐S�ɕ\��
			screenPosition.y - gaugeHeight * 0.5f,//�v�Z�����QD�ʒu�𒆐S�ɕ\��
			gaugeWidth * healthRate,//�Q�[�W���@���Q�[�W�̒����̔䗦
			gaugeHeight,//�Q�[�W�̍���
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f
		);
	}
	//�G�l�~�[�z�u����
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//�}�E�X�J�[�\�����W���擾
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());

		DirectX::XMVECTOR ScreenPosition, WorldPosition;

		//���C�̎n�_���v�Z�i����0.0f�j
		screenPosition.z = 0.0f;
		//���C�̎n�_��XMVECTOR�ɕϊ�
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		//���C�̎n�_���Q�c���W����R�c���W�ɕϊ�
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//�n�_�̈ʒu��rayStart�ɓ���Ȃ����AXMFLOAT3�ɕϊ�
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//���C�̏I�_���Z�o�iz��1.0f�j
		screenPosition.z = 1.0f;
		//���C�̏I�_��XMVECTOR�ɕϊ�
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		//���C�̏I�_���QD���W����RD���W�ɕϊ�
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//�I�_�̈ʒu��rayEnd�ɓ���Ȃ���XMFLOAT3�ɕϊ�
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		// �^���[�̈ʒu���擾
		Tower* targetTower = TowerManager::Instance().GetTower(numEnemyTower); // 0 �͍ŏ��̃^���[���w��
		
		//���C�L���X�g
		HitResult hit;
		if (StageManager::Instance().RayCast(rayStart, rayEnd, hit))
		{

			DirectX::XMFLOAT3 towerPosition = targetTower->GetPosition();
			//�G�𐶐���
			EnemySpider* spider = new EnemySpider();
			//�G�̈ʒu��hit.position�ɓo�^����
			spider->SetPosition(hit.position);
			//���̓G��player�ɒǐՂ�����
			spider->SetTarget(enemyTower);
			//EnemyManager�ɓo�^����
			EnemyManager::Instance().Register(spider);
		}
	}
}