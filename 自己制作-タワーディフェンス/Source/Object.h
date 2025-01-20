#pragma once

#include<DirectXMath.h>

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	//行列更新処理
	void UpdateTransform();


	bool ApplyDamage(int damage, float invincibleTime);

	void Move(float vx, float vz, float speed);

	void Turn(float elapsedTime, float vx, float vz, float speed);

	void UpdateVelocity(float elapsedTime);

	void UpdateInvincibleTimer(float elapsedTime);

	void UpdateVerticalVelocity(float elapsedFrame);

	void UpdateVerticalMove(float elapsedTime);

	void UpdateHorizontalVelocity(float elapsedFrame);

	void UpdateHorizontalMove(float elapsedTime);

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//着地したときに呼ばれる
	virtual void OnLanding(){}

	//ダメージを受けたときに呼ばれる
	virtual void OnDamaged(){}

	//死亡したときに呼ばれる
	virtual void OnDead(){}

private:
	DirectX::XMFLOAT3 position{};
	DirectX::XMFLOAT3 angle{};
	DirectX::XMFLOAT3 scale{1,1,1};
	DirectX::XMFLOAT4X4 transform{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	//半径
	float radius = 0.5f;

	//重力
	float gravity = -1.0f;

	//速力
	DirectX::XMFLOAT3 velocity{};

	//true...地面についている
	bool isGround = false;

	//高さ
	float height = 2.0f;

	//健康状態
	int health = 10;

	//最大
	int maxHealth = 10;

	//無敵時間
	float invincibleTimer = 1.0f;

	//摩擦の値
	float friction = 0.5f;

	//加速度
	float acceleration;

	//最大移動速度
	float maxMoveSpeed = 5.0f;
	//計算用の速度ベクトル（x,y）
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	//空中操作用の係数
	float airControl = 0.3f;

	float stepOffset = 1.0f;

	//傾斜率
	float slopeRate = 1.0f;
};