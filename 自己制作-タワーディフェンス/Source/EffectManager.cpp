#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	// Effekseer レンダラ生成
	effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
		graphics.GetDevice(),
		graphics.GetDeviceContext(),
		2048
	);

	// Effekseer マネージャー生成
	effekseerManager = Effekseer::Manager::Create(2048);

	// Effekseer レンダラの各種設定
	effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
	effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
	effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
	effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
	effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
	// Effekseer 内のでローダーの設定（特別なカスタマイズをしない場合は以下の設定でOK）
	effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
	effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
	effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

	// Effekseer を左手座標系で計算する
	effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

void EffectManager::Finalize()
{
	
}

void EffectManager::Update(float elapsedTime)
{
	effekseerManager->Update(elapsedTime * 60.0f);
}

void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
	effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

	effekseerRenderer->BeginRendering();

	effekseerManager->Draw();

	effekseerRenderer->EndRendering();
}
