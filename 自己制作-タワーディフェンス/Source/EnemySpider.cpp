#include "EnemySpider.h"
#include "Graphics/Model.h"

EnemySpider::EnemySpider()
{
	model = new Model("Data/Model/Spider/Spider.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//デバック判定がスケール変わっても一致できるようにスケール設定
	radius *= scale.x * 100;
	height *= scale.y * 30;

	angle.y = DirectX::XMConvertToRadians(90);

	acceleration = 2.0f;
}

EnemySpider::~EnemySpider()
{
	delete model;
}

void EnemySpider::Update(float elapsedTime)
{

	////ステート毎の更新処理
	//switch (state)
	//{
	//case State::Idle:
	//	UpdateIdleState(elapsedTime);
	//	break;
	//case State:Attack:
	//	UpdateAttackState(elapsedTime);
	//	break;
	//}

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
		angle.y = newAngleY + DirectX::XM_PI; // // Y軸回転を更新し、180度（π）を加算（モデルが反対に向いているため）
	}
	//// 新しい位置を設定
	//SetPosition(position);

	//// 速力処理更新
	//UpdateVelocity(elapsedTime);

	//// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
}


void EnemySpider::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void EnemySpider::OnDead()
{
	// 自信を破棄（Enemy クラス（基底クラス）の Destroy 関数を呼び出す）
	Destroy();
}

void EnemySpider::Attack()
{
}

//void UpdateIdleState(float elapsedTime)
//{
//	//タイマー処理
//	stateTimer -= elapsedTime;
//	if (stateTimer < 0.0f)
//	{
//		//徘徊ステートへ遷移
//		TransitionWanderState();
//	}
//}