#pragma once

#include "CameraController.h"
#include "Scene.h"
#include "Tower.h"

#include "Graphics/Sprite.h"
#include <DirectXMath.h>// DirectX::XMFLOAT3, XMConvertToRadians

#include "UI.h"


// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	//UI�\��
	void RenderUIButton(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);
	
	//�f�o�b�N�\���i��ŏ����j
	void DebugDrawGUI();
private:
	enum
	{
		numEnemyTower,
		numPlayerTower,
		numFortFirst,
	};

private:

	CameraController* cameraController = nullptr;

	Sprite* gauge{};

	//�Ԃ̏����ݒ�
	static const int numFort = 3;
	Tower* fort[numFort]{};
	Tower* playerTower = {}; // �^���[�|�C���^�z���������
	Tower* enemyTower = {}; // �^���[�|�C���^�z���������

	UI* ui{};
	// �N�[���_�E���̊Ԋu�i�b�j

	const float SPAWN_COOLDOWN_TIME = 2.0f; // �Ⴆ��2�b�̃N�[���_�E��

	// �Ō�ɃX�|�[����������
	float coolDownTimeSpider = -SPAWN_COOLDOWN_TIME;
	float coolDownTimeGolem = -SPAWN_COOLDOWN_TIME;
	// �{�^�����L�����ǂ������Ǘ�����t���O
	bool isSpiderButtonEnabled = true;
	bool isGolemButtonEnabled = true;

private:
	float positionX; // �{�^����X���W
	float positionY; // �{�^����Y���W
};
