#pragma once
#include "Graphics/Model.h"
#include "PlayerMonster.h"
#include "Tower.h"

class Spider : public PlayerMonster
{
public:
	Spider();
	~Spider() override;
	
	void Update(float elapsedTime) override;
	
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;
	void OnDead() override;
	
	void SetTarget(Tower* target) { this->target = target; }
	void Attack()override;
private:
	Model* model = nullptr;
	Tower* target = nullptr;
	float speed = 2.0f;
	float range = 10.0f;
	bool attackFlag = false;
};