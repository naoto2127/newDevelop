#pragma once

#include <DirectXMath.h>

// キャラクター
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	// 行列更新処理
	void UpdateTransform();

	// 位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// 回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// 半径取得
	float GetRadius() const { return radius; }

	// 地面に設置しているか
	bool IsGround() const { return isGround; }

	// 高さ取得
	float GetHeight() const { return height; }

	//健康状態を取得
	int GetHealth() const { return health; }

	//最大健康状態を取得
	int GetMaxHealth() const { return maxHealth; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincibleTime);

	// 衝撃を与える（衝突したときに移動する方向を与える）
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

protected:
	// 移動処理
	void Move(float vx, float vz, float speed);

	// 旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// ジャンプ処理
	void Jump(float speed);

	// 速力処理更新
	void UpdateVelocity(float elapsedTime);

	// 無敵時間更新
	void UpdateInvincibleTimer(float elapsedTime);

	// 着地したときに呼ばれる
	virtual void OnLanding() {}

	// ダメージを受けたときに呼ばれる
	virtual void OnDamaged() {}

	// 死亡したときに呼ばれる
	virtual void OnDead() {}

private:
	// 垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);

	// 垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);

	// 水平速力更新処理
	void UpdateHorizontalVelocity(float elapsedFrame);

	// 水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);

protected:
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,	// 右方向
		0,1,0,0,	// 上方向
		0,0,1,0,	// 前方向
		0,0,0,1
	};

	DirectX::XMFLOAT3 target{};

	// 半径
	float radius = 0.5f;
	
	// 重力
	float	gravity = -1.0f;

	// 速力
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// true...地面についている
	bool isGround = false;

	// 高さ
	float height = 2.0f;
	
	// 健康状態
	int health = 5;

	// 最大
	int maxHealth = 5;

	// 無敵時間
	float invincibleTimer = 1.0f;

	// 摩擦の値
	float friction = 0.5f;

	// 加速度
	float acceleration = 1.0f;

	// 最大移動速度
	float maxMoveSpeed = 5.0f;

	// 計算用の速度ベクトル（x, z）
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	// 空中操作用の係数
	float airControl = 0.3f;

	float stepOffset = 1.0f;

	//傾斜率
	float slopeRate = 1.0f;
};