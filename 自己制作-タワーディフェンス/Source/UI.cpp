#include "UI.h"
#include <Input/Input.h>

UI::UI()
	: cooldownTimerSpider(0.0f), cooldownTimerGolem(0.0f), 
	SPAWN_COOLDOWN_TIME(3.0f)  // クールダウン時間3.0秒
{
	uiSpiderButton = new Sprite("Data/Sprite/UIButtonSpider.png");
	uiGolemButton = new Sprite("Data/Sprite/UIButtonGolem.png");

	float textureWidth[] =
	{
		static_cast<float>(uiSpiderButton->GetTextureWidth()),
		static_cast<float>(uiGolemButton->GetTextureWidth()),
	};
	float textureHeight[] =
	{
		static_cast<float>(uiSpiderButton->GetTextureHeight()),
		static_cast<float>(uiGolemButton->GetTextureHeight()),
	};

	// ボタンの位置を設定（ローカル変数を使用）
	uiPositionSpider = { screenWidth / 2 - textureWidth[0] / 2, screenHeight - textureHeight[0] };
	uiPositionGolem = { screenWidth / 2 + textureWidth[1] / 2, screenHeight - textureHeight[1] };
}


UI::~UI()
{
	if (uiSpiderButton)
	{
		delete uiSpiderButton;
		uiSpiderButton = nullptr;
	}

	if (uiGolemButton)
	{
		delete uiGolemButton;
		uiGolemButton = nullptr;
	}
}
void UI::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	
	//ボタンのタイマー更新
	{
		// スパイダーボタンのクールダウンタイマーを更新
		if (cooldownTimerSpider > 0.0f) {
			cooldownTimerSpider -= elapsedTime;
		}

		// ゴーレムボタンのクールダウンタイマーを更新
		if (cooldownTimerGolem > 0.0f) {
			cooldownTimerGolem -= elapsedTime;
		}
		// クールダウンが終了したらボタンを再有効化
		if (cooldownTimerSpider <= 0.0f) {
			isSpiderButtonEnabled = true;
		}
	
		if (cooldownTimerGolem <= 0.0f) {
			isGolemButtonEnabled = true;

	}
	}


	// マウスを左クリックしたとき
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		int mouseX = mouse.GetPositionX();
		int mouseY = mouse.GetPositionY();

		// スパイダーボタンがクリックされたかどうかを確認します
		if (mouseX >= uiPositionSpider.x &&
			mouseX <= uiPositionSpider.x + uiSpiderButton->GetTextureWidth() &&
			mouseY >= uiPositionSpider.y &&
			mouseY <= uiPositionSpider.y + uiSpiderButton->GetTextureHeight() &&
			cooldownTimerSpider <= 0.0f // クールダウンが終了しているか
			)
		{
			if (spawnSpiderCallback) {
				spawnSpiderCallback();
			}
			cooldownTimerSpider = SPAWN_COOLDOWN_TIME;  // クールダウンタイマーをセット
		}

		// ゴーレムボタンがクリックされたかどうかを確認
		if (mouseX >= uiPositionGolem.x &&
			mouseX <= uiPositionGolem.x + uiGolemButton->GetTextureWidth() &&
			mouseY >= uiPositionGolem.y &&
			mouseY <= uiPositionGolem.y + uiGolemButton->GetTextureHeight() &&
			cooldownTimerGolem <= 0.0f)  // クールダウンが終了しているか
		{
			if (spawnGolemCallback) {
				spawnGolemCallback();
			}
			cooldownTimerGolem = SPAWN_COOLDOWN_TIME;  // クールダウンタイマーをセット
		}
	}
}


void UI::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 2D スプライト描画
	{
		Graphics& graphics = Graphics::Instance();
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(uiSpiderButton->GetTextureWidth());
		float textureHeight = static_cast<float>(uiSpiderButton->GetTextureHeight());

		// スパイダーボタンを半透明で描画
		float spiderAlpha = (cooldownTimerSpider > 0.0f) ? 0.5f : 1.0f; // クールダウン中は半透明
		uiSpiderButton->Render(dc,
			uiPositionSpider.x, uiPositionSpider.y,
			static_cast<float>(uiSpiderButton->GetTextureWidth()), static_cast<float>(uiSpiderButton->GetTextureHeight()),
			0, 0,
			uiSpiderButton->GetTextureWidth(), uiSpiderButton->GetTextureHeight(),
			0,
			spiderAlpha, spiderAlpha, spiderAlpha, 1.0f);

		textureWidth = static_cast<float>(uiGolemButton->GetTextureWidth());
		textureHeight = static_cast<float>(uiGolemButton->GetTextureHeight());
		
		// ゴーレムボタンを半透明で描画
		float golemAlpha = (cooldownTimerGolem > 0.0f) ? 0.5f : 1.0f; // クールダウン中は半透明
		uiGolemButton->Render(dc,
			uiPositionGolem.x, uiPositionGolem.y,
			static_cast<float>(uiGolemButton->GetTextureWidth()), static_cast<float>(uiGolemButton->GetTextureHeight()),
			0, 0,
			uiGolemButton->GetTextureWidth(), uiGolemButton->GetTextureHeight(),
			0,
			golemAlpha, golemAlpha, golemAlpha, 1.0f);
	}
}

void UI::SetSpawnSpiderCallback(std::function<void()> callback) {
	spawnSpiderCallback = callback;
}

void UI::SetSpawnGolemCallback(std::function<void()> callback) {
	spawnGolemCallback = callback;
}

void UI::DrawDebugGUI()
{
	// デバッグ情報表示
	if (ImGui::CollapsingHeader("Spawn Time & Button States", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 最後にスポーンした時間
		ImGui::Text("Last Spawn Time Spider: %.2f", cooldownTimerSpider);
		ImGui::Text("Last Spawn Time Golem: %.2f", cooldownTimerGolem);

		// ボタンが有効かどうかを管理するフラグ
		ImGui::Checkbox("Spider Button Enabled", &isSpiderButtonEnabled);
		ImGui::Checkbox("Golem Button Enabled", &isGolemButtonEnabled);
	}
}