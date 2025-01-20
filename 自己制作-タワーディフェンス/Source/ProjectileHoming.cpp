#include "ProjectileHoming.h"

// コンストラクタ
ProjectileHoming::ProjectileHoming(ProjectileManager* manager)
	: Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	// モデルが小さいのでスケーリング
	scale.x = scale.y = scale.z = 3.0f;
}

// デストラクタ
ProjectileHoming::~ProjectileHoming()
{
	delete model;
}

void ProjectileHoming::Update(float elapsedTime)
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
	{
		float speed = moveSpeed * elapsedTime;
		position.x += direction.x * speed;// x の移動処理（位置 += 向き * 速さ）
		position.y += direction.y * speed;// y の移動処理
		position.z += direction.z * speed;// z の移動処理
	}

	// 旋回
	{
		float turnSpeed = this->turnSpeed * elapsedTime;

		// 弾の位置からターゲットまでのベクトルを計算
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);	// position を XMVECTOR に変換
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target); // target を XMVECTOR に変換
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position); // target - position

		// ゼロベクトルでないなら回転処理
		DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
		float lengthSq;
		DirectX::XMStoreFloat(&lengthSq, LengthSq);
		if (lengthSq > 0.00001f)
		{
			// ターゲットまでのベクトルを単位ベクトル化
			Vec = DirectX::XMVector3Normalize(Vec);

			// direction を XMVECTOR に変換
			DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

			// 前方方向ベクトルとターゲットまでのベクトルの内積（角度）を計算
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

			float dot;
			DirectX::XMStoreFloat(&dot, Dot);

			// ２つの単位ベクトルの角度が小さいほど
			// 1.0 に近づくという性質を利用して回転速度を調整する
			float rot = 1.0f - dot;
			if (rot > DirectX::XMConvertToRadians(180))
			{
				rot = DirectX::XMConvertToRadians(180);
			}

			// 回転角度があるなら回転処理をする
			if (fabsf(rot) > 0.0001f)
			{
				// 回転軸を計算
				DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);

				// 回転軸と回転量から回転行列を計算
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

				// 現在の行列を回転させる
				DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
				Transform = DirectX::XMMatrixMultiply(Transform, Rotation);

				// 回転後の前方方向を取り出し、単位ベクトル化する
				Direction = DirectX::XMVector3Normalize(Transform.r[2]);
				DirectX::XMStoreFloat3(&direction, Direction);

			}
		}
	}

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列を更新
	model->UpdateTransform(transform);
}

void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target)
{
	this->direction = direction;
	this->position = position;
	this->target = target;

	// 発射した瞬間に方向を計算しないといけないので UpdateTransform を呼び出しておく
	UpdateTransform();
}
