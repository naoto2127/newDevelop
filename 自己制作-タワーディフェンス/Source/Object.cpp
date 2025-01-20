#include "Object.h"
#include "StageManager.h" 
#include "Stage.h"

void Object::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

/// <summary>
/// ダメージ処理
/// </summary>
/// <param name="damage">与えるダメージ</param>
/// <param name="invincibleTime">更新する無敵時間</param>
/// <returns></returns>
bool Object::ApplyDamage(int damage, float invincibleTime)
{
	// ダメージが０の場合は健康状態を変更する必要がない
	if (damage == 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (health <= 0) return false;

	// 無敵時間中はダメージを与えない
	if (this->invincibleTimer > 0.0f) return false;

	// 無敵時間を再設定
	this->invincibleTimer = invincibleTime;

	// ダメージ処理
	health -= damage;

	// 死亡通知
	if (health <= 0)
	{
		// 死亡したときに呼び出すべき関数を呼び出す
		OnDead();
	}
	// ダメージ通知
	else
	{
		// ダメージを受けたときに呼び出すべき関数を呼び出す
		OnDamaged();
	}

	// 健康状態が変更したら true を返す
	return true;
}

void Object::Move(float vx, float vz, float speed)
{
	// 移動方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;

	// 最大速度設定
	maxMoveSpeed = speed;
}

void Object::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;

	// 進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;

	// 自身の回転値から前方方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);




	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);

	// 内積値は -1.0 ～ 1.0 で表現されており、２つの単位ベクトルの角度が
	// 小さいほど 1.0 に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;
	if (rot > speed)
		rot = speed;



	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontZ * vx) - (frontX * vz);

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		// 反時計回り
		angle.y -= rot;
	}
	else
	{
		// 時計回り
		angle.y += rot;
	}


}

void Object::UpdateVelocity(float elapsedTime)
{
	// 経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	// 垂直速力更新処理
	UpdateVerticalVelocity(elapsedFrame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(elapsedFrame);

	// 垂直移動更新処理
	UpdateVerticalMove(elapsedTime);

	// 水平移動更新処理
	UpdateHorizontalMove(elapsedTime);
}

// 無敵時間の更新
void Object::UpdateInvincibleTimer(float elapsedTime)
{
	if (this->invincibleTimer > 0.0f)
	{
		this->invincibleTimer -= elapsedTime;
	}
}

// 垂直速力更新処理
// 縦方向の速度の更新の計算をしている処理の部分
void Object::UpdateVerticalVelocity(float elapsedFrame)
{
	// 重力処理
	velocity.y += gravity * elapsedFrame;
}

// 垂直移動更新処理
// 縦方向の移動の更新の計算をいている処理の部分
void Object::UpdateVerticalMove(float elapsedTime)
{
	//垂直方向の移動量
	float my = velocity.y * elapsedTime;

	//落下中
	if (my < 0.0f)
	{
		//レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		//レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x, position.y + my - 0.5f, position.z };

		//レイキャストによる地面判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//地面に接地している
			position.y = hit.position.y;

			//傾斜率の計算
			//面の法線から xz 平面の傾きを計算
			float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x
				+ hit.normal.z * hit.normal.z);
			//傾斜率（ｘｚ平面の傾きとｙの向きから傾斜率を計算し反転）
			slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

			//着地した
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			//空中に浮いている
			position.y += my;
			isGround = false;
		}
	}
	//上昇中
	else if (my > 0.0f)
	{
		position.y += my;
		isGround = false;
	}

}

void Object::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ 平面の速力を減速する

	// 速度の平面である XZ の長さを計算
	DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

	DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
	float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));

	if (length > 0.0f)
	{
		// 摩擦力
		float friction = this->friction * elapsedFrame;

		// 空中にいるときは摩擦力を減らす
		if (isGround == false) friction *= airControl;

		// 摩擦による横方向の減速処理
		if (length > friction)
		{
			// 速度を単位ベクトル化
			auto v = DirectX::XMVector2Normalize(velVel);

			// 単位ベクトルかした速度を friction 分スケーリング
			v = DirectX::XMVectorScale(v, friction);

			// スケーリングしたベクトルを速度に減算
			DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

			velocity.x = vel.x;
			velocity.z = vel.y;
		}
		// 横方向の速力が摩擦力以下になったので速力を無効化
		else
		{
			// 速度ベクトルを 0 にする
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	if (length <= maxMoveSpeed)
	{
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			float acceleration = this->acceleration * elapsedFrame;

			// 空中にいるときは加速力を減らす
			if (isGround == false) acceleration *= airControl;

			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			// 空中にいるときは加速力を減らす
			if (isGround == false) acceleration *= airControl;

			if (length > maxMoveSpeed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}

			//下り坂でガタガタしないようにする
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= length * slopeRate * elapsedFrame;
			}
		}
	}



#if false
	// XZ 平面の速力を加速の処理
	if (length <= maxMoveSpeed)
	{
		DirectX::XMFLOAT2 moveVec = { moveVecX , moveVecZ };
		DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

		// 移動方向ベクトルの長さ計算している（Length 関数を使ってもよい）
		DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&moveVec);
		float moveVecLength = DirectX::XMVectorGetX(DirectX::XMVector2Length(vec));

		//float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		// 移動ベクトルがゼロベクトルでないなら
		if (moveVecLength > 0.0f)
		{
			// 加速する
			// 加速力の計算
			float acceleration = this->acceleration * elapsedFrame;

			// 空中にいるときは加速力を減らす
			if (isGround == false) acceleration *= airControl;

			// 移動ベクトルによる加速処理
			DirectX::XMVECTOR velVec = DirectX::XMLoadFloat2(&vel);
			velVec = DirectX::XMVectorAdd(velVec, DirectX::XMVectorScale(vec, acceleration));
			// スケーリングしたベクトルが速度ベクトル

			DirectX::XMStoreFloat2(&vel, velVec);
			velocity.x = vel.x;
			velocity.z = vel.y;

			// 最大速度制限（計算した速度で再度長さを求める）
			float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVec));
			if (length > maxMoveSpeed)
			{
				// 速度を単位ベクトル化
				auto v = DirectX::XMVector2Normalize(velVel);

				// 単位ベクトルかした速度を maxMoveSpeed 分スケーリング
				v = DirectX::XMVectorScale(v, maxMoveSpeed);

				// スケーリングしたベクトルが速度ベクトル
				DirectX::XMStoreFloat2(&vel, v);

				velocity.x = vel.x;
				velocity.z = vel.y;
			}


		}
	}
#endif
	// 移動方向ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// 水平移動更新処理
void Object::UpdateHorizontalMove(float elapsedTime)
{
	//// 移動処理
	//position.x += velocity.x * elapsedTime;
	//position.z += velocity.z * elapsedTime;

#if 1
	//水平速力量計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		//水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		//レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

		//レイキャストによる壁判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			//壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			//補正位置の計算
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			position.x = collectPosition.x;
			position.z = collectPosition.z;

			//壁ずり方向へレイキャスト
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				//壁ずり方向で壁に当たらなかったら補正位置に移動
				position.x = collectPosition.x;
				position.z = collectPosition.z;
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
			}
		}
		else
		{
			//移動
			position.x += mx;
			position.z += mz;
		}
	}

#endif

}
