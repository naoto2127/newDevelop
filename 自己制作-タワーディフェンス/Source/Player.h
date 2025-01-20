#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Tower.h"

#include "Effect.h"

// プレイヤー
class Player : public Character
{
public:
	Player();
	~Player() override;

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// デバッグ用 GUI 描画
	void DrawDebugGUI();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

private:
	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	// 移動入力処理
	void InputMove(float elapsedTime);

	// 弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();

	// ジャンプ入力処理
	void InputJump();

	// 弾丸入力処理
	void InputProjectile();

protected:
	// 着地したときに呼ばれる
	void OnLanding() override;

private:
	Model*	model = nullptr;
	float	moveSpeed = 5.0f;
	float	turnSpeed = DirectX::XMConvertToRadians(720);
	float	jumpSpeed = 20.0f;
	int		jumpCount = 0;
	int		jumpLimit = 2;

	ProjectileManager	projectileManager;
	Effect*				hitEffect = nullptr;
};