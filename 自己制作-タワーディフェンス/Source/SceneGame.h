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

	void RenderEnemyArrows(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

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

	//�i�s�����̖��\��
	Sprite* uiArrows{};

	//�Ԃ̏����ݒ�
	static const int numFort = 3;
	Tower* fort[numFort]{};
	Tower* playerTower = {}; // �^���[�|�C���^�z���������
	Tower* enemyTower = {}; // �^���[�|�C���^�z���������

	UI* ui{};

	DirectX::XMFLOAT3 target;
	float range;


private:
	float positionX; // �{�^����X���W
	float positionY; // �{�^����Y���W
};
