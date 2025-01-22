#pragma once

#include "Graphics/Model.h"
#include "PlayerMonster.h"
#include "Tower.h"

// 蜘蛛
class Spider : public PlayerMonster
{
public:
	Spider();
	~Spider() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void OnDead() override;

	//目標位置設定
	void SetTarget(Tower* target) { this->target = target; }

	void Attack()override;

private:
	Model* model = nullptr;

	Tower* target = nullptr;//追跡対象
	float speed = 2.0f;//移動速度

	float range = 10.0f;

	bool attackFlag = false;
};