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
	//オブジェクト更新処理
	UpdateTransform();

	//モデル行列更新処理
	model->UpdateTransform(transform);
}

void Tower::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void Tower::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//行列更新関数
void Tower::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Tower::DrawDebugGUI()
{
	// トランスフォーム
	if (ImGui::CollapsingHeader("Tower", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 位置
		ImGui::InputFloat3("Position", &position.x);
		//回転
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// スケール
		ImGui::InputFloat3("Scale", &scale.x);
	}
}

bool Tower::CheckCollision(const DirectX::XMFLOAT3& charPos, float charRadius, float charHeight)
{
	// キャラクターの位置との差分
	float dx = charPos.x - position.x;
	float dz = charPos.z - position.z;
	float distXZ = sqrtf(dx * dx + dz * dz);

	// 高さ判定
	float charBottom = charPos.y;
	float charTop = charPos.y + charHeight;
	float towerBottom = position.y;
	float towerTop = position.y + height;

	bool heightOverlap = (charBottom < towerTop) && (charTop > towerBottom);

	// 半径と高さの条件を満たす場合に衝突と判定
	return distXZ < (radius + charRadius) && heightOverlap;
}


