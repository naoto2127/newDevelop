#include "StageMain.h"

// �R���X�g���N�^
StageMain::StageMain()
{
	// �X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/ExampleStage/Ground.mdl");

	// �X�e�[�W�̃X�P�[����ݒ� (��: �S�̂̑傫���� 0.5 �{�ɂ���)
	scale = DirectX::XMFLOAT3(0.05f, 0.05f, 0.05f); // X, Y, Z �̃X�P�[��
	angle.y = DirectX::XMConvertToRadians(90);
}

StageMain::~StageMain()
{
	// �X�e�[�W���f����j��
	delete model;
}

// �X�V����
void StageMain::Update(float elapsedTime)
{
	//�s��X�V����
	UpdateTransform();

	model->UpdateTransform(transform);

}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// �V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
	shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMain::RayCast(const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}
