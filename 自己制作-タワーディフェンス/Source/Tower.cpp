#include "Tower.h"
#include "Graphics/Graphics.h"
#include "TowerManager.h"

Tower::Tower()
{
	model = new Model("Data/Model/Object/tower.mdl");
	scale = { 0.02f,0.02f,0.02f };
	position.z = -25.0f;

	radius *= scale.x;
	height *= scale.y;
}


void Tower::Update(float elapsedTime)
{
	//�I�u�W�F�N�g�X�V����
	UpdateTransform();

	//���f���s��X�V����
	model->UpdateTransform(transform);
}

void Tower::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void Tower::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�s��X�V�֐�
void Tower::UpdateTransform()
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

void Tower::DrawDebugGUI()
{
	// �g�����X�t�H�[��
	if (ImGui::CollapsingHeader("Tower", ImGuiTreeNodeFlags_DefaultOpen))
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

bool Tower::CheckCollision(const DirectX::XMFLOAT3& charPos, float charRadius, float charHeight)
{
	// �L�����N�^�[�̈ʒu�Ƃ̍���
	float dx = charPos.x - position.x;
	float dz = charPos.z - position.z;
	float distXZ = sqrtf(dx * dx + dz * dz);

	// ��������
	float charBottom = charPos.y;
	float charTop = charPos.y + charHeight;
	float towerBottom = position.y;
	float towerTop = position.y + height;

	bool heightOverlap = (charBottom < towerTop) && (charTop > towerBottom);

	// ���a�ƍ����̏����𖞂����ꍇ�ɏՓ˂Ɣ���
	return distXZ < (radius + charRadius) && heightOverlap;
}


