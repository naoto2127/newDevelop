#include "EnemyGolem.h"

EnemyGolem::EnemyGolem()
{
	model = new Model("Data/Model/StoneGolem/StoneGolem.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.003f;

	radius = scale.x * 300;
	height = scale.y * 1000;

	acceleration = 1.5f;
}

EnemyGolem::~EnemyGolem()
{
	delete model;
}

void EnemyGolem::Update(float elapsedTime)
{
	if (target != nullptr) {
		// プレイヤーの位置を取得
		DirectX::XMFLOAT3 targetPos = target->GetPosition();

		// 現在の位置を取得
		DirectX::XMFLOAT3 currentPos = GetPosition();

		// プレイヤーへの方向を計算
		DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&targetPos),
			DirectX::XMLoadFloat3(&currentPos)
		);
		dir = DirectX::XMVector3Normalize(dir);

		// 新しい位置を計算
		DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(
			DirectX::XMLoadFloat3(&currentPos),
			DirectX::XMVectorScale(dir, acceleration * elapsedTime)
		);

		// 新しい位置を設定
		DirectX::XMFLOAT3 updatedPos;
		DirectX::XMStoreFloat3(&updatedPos, newPos);
		SetPosition(updatedPos);

		// 進行方向に基づいて回転を計算
		DirectX::XMFLOAT3 forward;
		DirectX::XMStoreFloat3(&forward, dir);

		float newAngleY = atan2f(forward.x, forward.z); // Z軸を基準に回転角を計算
		angle.y = newAngleY; // // Y軸回転を更新する
	}

	// 速力処理更新
	UpdateVelocity(elapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
}

void EnemyGolem::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemyGolem::OnDead()
{
	// 自信を破棄（Enemy クラス（基底クラス）の Destroy 関数を呼び出す）
	Destroy();
}


