#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

#include "Tower.h"

// 蜘蛛
class EnemySpider : public Enemy
{
public:
	EnemySpider();
	~EnemySpider() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//死亡処理
	void OnDead() override;


	//目標位置設定
	void SetTarget(Tower* target) { this->target = target; }

private:
	//アニメーション更新処理
	void UpdateAnimation();

	//何かに当たったら攻撃アニメーションする
	bool IsColliding();


	//ステート
	enum class State
	{

		Idle,
		Walk,
		Attack,
		Damage,
		Death,
	};

	State	state = State::Idle;
private:
	Model* model = nullptr;

	Tower* target = nullptr;//追跡対象

	float				stateTimer = 0.0f;

};