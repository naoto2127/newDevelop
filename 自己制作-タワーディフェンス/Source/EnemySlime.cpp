#include "EnemySlime.h"

EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	radius = 50.0f * scale.x;
	height = 100.0f * scale.y;
}

EnemySlime::~EnemySlime()
{
	delete model;
}

void EnemySlime::Update(float elapsedTime)
{
	// 速力処理更新
	UpdateVelocity(elapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
}

void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemySlime::OnDead()
{
	// 自信を破棄（Enemy クラス（基底クラス）の Destroy 関数を呼び出す）
	Destroy();
}


