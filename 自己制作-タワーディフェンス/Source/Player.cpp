#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include <Graphics/DebugRenderer.h>
#include <Graphics/Graphics.h>
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "Tower.h"
#include "TowerManager.h"

Player::Player()
{
	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	// ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");
}

Player::~Player()
{
	delete hitEffect;

	delete model;
}

void Player::Update(float elapsedTime)
{
	// 移動入力処理
	InputMove(elapsedTime);

	// ジャンプ入力処理
	//InputJump();

	// 弾丸入力処理
	//InputProjectile();

	// 速力処理更新
	UpdateVelocity(elapsedTime);

	// 弾丸更新処理
	//projectileManager.Update(elapsedTime);

	// プレイヤーと敵との衝突処理
	CollisionPlayerVsEnemies();

	// 弾丸と敵の衝突処理
	CollisionProjectilesVsEnemies();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列を更新
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	// 弾丸描画処理
	projectileManager.Render(dc, shader);
}

// デバッグ用GUI描画
void Player::DrawDebugGUI()
{
	//ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	//{
	//}
	//ImGui::End();
		// トランスフォーム
		if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			// スケール
			ImGui::InputFloat3("Scale", &scale.x);
		}

}

// デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	projectileManager.DrawDebugPrimitive();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラの方向を取得
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルは　XZ 平面に水平なベクトルになるようにする

	// カメラ右方向ベクトルを XZ の単位ベクトルに変換
	float cameraRightX = cameraRight.x;	// 右方向ベクトルの X
	float cameraRightZ = cameraRight.z;	// 右方向ベクトルの Z
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルを XZ 単位ベクトルに変換
	float cameraFrontX = cameraFront.x;	// 前方向ベクトルの X
	float cameraFrontZ = cameraFront.z;	// 前方向ベクトルの Z
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);// XZ の平方根
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し
	// スティックの垂直入力値をカメラ前方向に反映
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);	// 水平移動
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);	// 垂直移動													// 垂直移動
	vec.y = 0.0f;

	return vec;
}



void Player::InputMove(float elapsedTime)
{
	// 進行方向ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// 移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);

	// 旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}



void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// すべての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i) 
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		// 衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylinderVsCylinder(
			GetPosition(),// プレイヤーの位置
			GetRadius(),// プレイヤーの半径
			GetHeight(), // プレイヤーの高さ
			enemy->GetPosition(),// 敵の位置
			enemy->GetRadius(),// 敵の半径
			enemy->GetHeight(), // 敵の高さ
			outPosition
		))
		{
			// 押し出し後の位置設定（outPosition を敵の位置に設定）
			enemy->SetPosition(outPosition);
		}
	}
}

// 弾丸と敵の衝突処理
void Player::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// すべての弾丸とすべての敵を総当たりで衝突処理
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);

		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			// 衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition
			))
			{
				// ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					// 吹き飛ばす
					{
						// 吹き飛ばす移動方向の速度ベクトル
						DirectX::XMFLOAT3 impulse;
						// 吹き飛ばす力
						const float power = 10.0f;

						// 敵の位置
						DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
						// 弾の位置
						DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
						// 弾から敵への方向ベクトルを計算（敵の位置 - 弾の位置）
						auto v = DirectX::XMVectorSubtract(eVec, pVec);
						// 方向ベクトルを正規化
						v = DirectX::XMVector3Normalize(v);

						DirectX::XMFLOAT3 vec;
						DirectX::XMStoreFloat3(&vec, v);

						// 吹き飛ばす移動方向の速度ベクトルに power を掛けて設定
						impulse.x = power * vec.x;
						impulse.y = power * 0.5;
						impulse.z = power * vec.z;

						enemy->AddImpulse(impulse);
					}

					// ヒットエフェクト再生
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					// 弾丸破棄
					projectile->Destroy();
				}
			}
		}
	}
}

void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		// ジャンプカウントがジャンプリミットより小さければ
		if (jumpCount < jumpLimit)
		{
			// ジャンプカウントを1増やして
			jumpCount++;
			// ジャンプ
			Jump(jumpSpeed);
		}
	}
}

// 弾丸入力処理
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// 直進弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		// 前方向（モデルを 0.01f しているので行列の拡縮が 0.01 倍されている。なので前方方向を取得する際、
		// 100 倍に戻してから取得してもよいよ）
		DirectX::XMFLOAT3 dir;
		dir.x = transform._31 * 100.0f;
		dir.y = 0.0f;
		dir.z = transform._33 * 100.0f;
		// 100 倍するのが気持ち悪いというのであれば下の y 軸を基準にして平面の向きを計算しても良い
		//dir.x = sinf(angle.y);
		//dir.y = 0.0f;
		//dir.z = cosf(angle.y);
		
		// 発射位置（プレイヤーの腰当たり）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;
		// 発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
	}

	// 追尾弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		// 前方向
		DirectX::XMFLOAT3 dir;
		dir.x = transform._31 * 100.0f;
		dir.y = 0.0f;
		dir.z = transform._33 * 100.0f;

		// 発射位置（プレイヤーの腰当たり）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;

		// ターゲット（デフォルトではプレイヤーの前方）
		DirectX::XMFLOAT3 target;
		target.x = pos.x + dir.x * 1000.0f;
		target.y = pos.y + dir.y * 1000.0f;
		target.z = pos.z + dir.z * 1000.0f;

		// 一番近くの敵をターゲットにする
		float dist = FLT_MAX;
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)
		{
			// 敵との距離判定

			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			// 自分の位置（position を XMVECTOR に変換）
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			// 敵の位置（enemy->GetPosition() を XMVECTOR に変換）
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			// 自分から敵までのベクトルを計算（nemy->GetPosition() - position）
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
			// そのベクトルの長さを計算
			DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
			float d;
			DirectX::XMStoreFloat(&d, D);

			// すべての敵の中で一番近い敵をターゲットとして設定
			if (d < dist)
			{
				dist = d;
				target = enemy->GetPosition();
				target.y += enemy->GetHeight() * 0.5f;
			}
		}

		// 発射
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos, target);
	}
}

void Player::OnLanding()
{
	// ジャンプ回数のリセット
	jumpCount = 0;
}
