#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	// Effekseer �����_������
	effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
		graphics.GetDevice(),
		graphics.GetDeviceContext(),
		2048
	);

	// Effekseer �}�l�[�W���[����
	effekseerManager = Effekseer::Manager::Create(2048);

	// Effekseer �����_���̊e��ݒ�
	effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
	effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
	effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
	effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
	effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
	// Effekseer ���̂Ń��[�_�[�̐ݒ�i���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͈ȉ��̐ݒ��OK�j
	effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
	effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
	effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

	// Effekseer ��������W�n�Ōv�Z����
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
