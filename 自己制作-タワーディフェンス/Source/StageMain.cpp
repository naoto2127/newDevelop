#include "StageMain.h"

// コンストラクタ
StageMain::StageMain()
{
	// ステージモデルを読み込み
	model = new Model("Data/Model/ExampleStage/Ground.mdl");

	// ステージのスケールを設定 (例: 全体の大きさを 0.5 倍にする)
	scale = DirectX::XMFLOAT3(0.05f, 0.05f, 0.05f); // X, Y, Z のスケール
	angle.y = DirectX::XMConvertToRadians(90);
}

StageMain::~StageMain()
{
	// ステージモデルを破棄
	delete model;
}

// 更新処理
void StageMain::Update(float elapsedTime)
{
	//行列更新処理
	UpdateTransform();

	model->UpdateTransform(transform);

}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// シェーダーにモデルを描画してもらう
	shader->Draw(dc, model);
}

//レイキャスト
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
