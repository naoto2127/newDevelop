#include "ProjectileStraight.h"

// コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
	: Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	// 表示サイズを調整
	scale.x = scale.y = scale.z = 3.0f;
}

// デストラクタ
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
	// 寿命処理（lifeTimer から 経過時間を引いていく）
	lifeTimer -= elapsedTime;
	// lifeTimer が 0 以下になったら
	if (lifeTimer <= 0)
	{
		// 自分を削除
		Destroy();
	}

	// 移動
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;// x の移動処理（位置 += 向き * 速さ）
	position.y += direction.y * speed;// y の移動処理
	position.z += direction.z * speed;// z の移動処理
	
	//DirectX::XMVECTOR dVec = DirectX::XMLoadFloat3(&direction);
	//DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&position);
	//DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(pVec, DirectX::XMVectorScale(dVec, speed)));

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
}

void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
