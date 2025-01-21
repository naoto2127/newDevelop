#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Collision.h"
#include "Camera.h"
#include "StageManager.h"
#include "StageMain.h"
#include "EnemySpider.h"
#include "EnemyGolem.h"
#include "EnemyManager.h"
#include <EffectManager.h>
#include "TowerManager.h"
#include "Tower.h"
#include "Input/Input.h"


// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	// カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		{ 0,10,-10 },
		{ 0, 0, 0 },
		{ 0, 1, 0 }
	);
	camera.SetPspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// カメラコントローラー初期化
	cameraController = new CameraController();
	cameraController->SetRange(40);
	cameraController->SetAngle({ DirectX::XMConvertToRadians(60), 0, 0 });
	

	//タワー初期化
	playerTower = new Tower(); // タワー生成
	playerTower->SetPosition({ 0, 0, -25 }); // 位置設定
	playerTower->SetRotation({ 0, DirectX::XMConvertToRadians(90), 0 }); // 回転設定

	// タワーを管理する仕組みに登録
	TowerManager::Instance().Register(playerTower);

	enemyTower = new Tower(); // タワー生成
	enemyTower->SetPosition({ 0, 0, 25 }); // 位置設定
	enemyTower->SetRotation({ 0, DirectX::XMConvertToRadians(-90), 0 }); // 回転設定

	// タワーを管理する仕組みに登録
	TowerManager::Instance().Register(enemyTower);

	//砦の初期設定
	for (int i = 0; i < numFort; i++)
	{
		fort[i] = new Tower();
		fort[i]->SetPosition({ static_cast<float>(i) * 15 - 15,0,0 });
		TowerManager::Instance().Register(fort[i]);
	}

	{
		//ゲージスプライト
		gauge = new Sprite();
		
		//UIボタン表示
		ui = new UI();

		// スパイダースポーンコールバックを設定する
		ui->SetSpawnSpiderCallback([this]() {
			if (ui->GetButtonSpider())
			{
				EnemySpider* spider = new EnemySpider();
				spider->SetPosition(playerTower->GetPosition());
				spider->SetTarget(fort[0]);
				EnemyManager::Instance().Register(spider);

				// 最後にスポーンした時間をリセット
				ui->SetCooldownTimerSpider({});

				// ボタンを無効化してクールダウン中
				ui->SetButtonSpider(false);

			}
		});

		// ゴーレムスポーンコールバックを設定する
		ui->SetSpawnGolemCallback([this]() {
			if (ui->GetButtonGolem())
			{
				EnemyGolem* golem = new EnemyGolem();
				golem->SetPosition(playerTower->GetPosition());
				golem->SetTarget(fort[2]);
				EnemyManager::Instance().Register(golem);

				// 最後にスポーンした時間をリセット
				ui->SetCooldownTimerGolem({});

				// ボタンを無効化してクールダウン中
				ui->SetButtonGolem(false);

			}
		});
	}

}

// 終了化
void SceneGame::Finalize()
{
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	// エネミー終了化
	EnemyManager::Instance().Clear();

	//ステージ初期化
	StageManager::Instance().Clear();

	// カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//タワー終了化
	TowerManager::Instance().Clear();

	if (ui)
	{
		delete ui;
		ui = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = playerTower->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//UI表示
	ui->Update(elapsedTime);
	
	//砦の更新処理
	TowerManager::Instance().Update(elapsedTime);

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);
	
	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//当たり判定処理
	//player->CollisionPlayerVsEnemies();

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		// シェーダの開始
		shader->Begin(dc, rc);

		// エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//タワー描画
		TowerManager::Instance().Render(dc, shader);

		// シェーダの終了
		shader->End(dc);
	}

	// 3D エフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		// エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//タワーデバック描画
		TowerManager::Instance().DrawDebugPrimitive();
	}

	// 2Dスプライト描画
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);

		if (ui)
		{
			//UI表示
			ui->Render();
		}

	}

	// 2DデバッグGUI描画
	{
		if (ImGui::TreeNode("Object"))
		{
			if (ImGui::TreeNode("TowerManager"))
			{
				//砦のデバック描画　
				TowerManager::Instance().DrawDebugGUI();

				ImGui::TreePop();
			}

			//UIのデバック描画
			ui->DrawDebugGUI();

			ImGui::TreePop();
		}
	}
}

void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//エネミーの頭上のワールド座標を計算
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();

		//XMVECTORに変換
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		//ワールド座標からスクリーン座標へ変換
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//スクリーン座標を　XMFLOAT2　に変換
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		//ここからはゲージの表示処理
		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		//現在　HP　/　最大HPからゲージの長さの比率を計算
		float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

		//ゲージを描画
		gauge->Render(dc,
			screenPosition.x - gaugeWidth * 0.5f,//計算した２D位置を中心に表示
			screenPosition.y - gaugeHeight * 0.5f,//計算した２D位置を中心に表示
			gaugeWidth * healthRate,//ゲージ幅　＊ゲージの長さの比率
			gaugeHeight,//ゲージの高さ
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f
		);
	}
	//エネミー配置処理
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//マウスカーソル座標を取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());

		DirectX::XMVECTOR ScreenPosition, WorldPosition;

		//レイの始点を計算（ｚが0.0f）
		screenPosition.z = 0.0f;
		//レイの始点をXMVECTORに変換
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		//レイの始点を２Ｄ座標から３Ｄ座標に変換
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//始点の位置をrayStartに入れなおし、XMFLOAT3に変換
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//レイの終点を算出（zが1.0f）
		screenPosition.z = 1.0f;
		//レイの終点をXMVECTORに変換
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		//レイの終点を２D座標から３D座標に変換
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//終点の位置をrayEndに入れなおしXMFLOAT3に変換
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		// タワーの位置を取得
		Tower* targetTower = TowerManager::Instance().GetTower(numEnemyTower); // 0 は最初のタワーを指定
		
		//レイキャスト
		HitResult hit;
		if (StageManager::Instance().RayCast(rayStart, rayEnd, hit))
		{

			DirectX::XMFLOAT3 towerPosition = targetTower->GetPosition();
			//敵を生成し
			EnemySpider* spider = new EnemySpider();
			//敵の位置をhit.positionに登録する
			spider->SetPosition(hit.position);
			//この敵をplayerに追跡させる
			spider->SetTarget(enemyTower);
			//EnemyManagerに登録する
			EnemyManager::Instance().Register(spider);
		}
	}
}