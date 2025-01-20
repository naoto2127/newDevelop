#include "UI.h"
#include <Input/Input.h>

UI::UI()
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

	// �{�^���̈ʒu��ݒ�i���[�J���ϐ����g�p�j
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
}
void UI::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	// �}�E�X�����N���b�N�����Ƃ�
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		int mouseX = mouse.GetPositionX();
		int mouseY = mouse.GetPositionY();

		// �X�p�C�_�[�{�^�����N���b�N���ꂽ���ǂ������m�F���܂�
		if (mouseX >= uiPositionSpider.x &&
			mouseX <= uiPositionSpider.x + uiSpiderButton->GetTextureWidth() &&
			mouseY >= uiPositionSpider.y &&
			mouseY <= uiPositionSpider.y + uiSpiderButton->GetTextureHeight())
		{
			if (spawnSpiderCallback) {
				spawnSpiderCallback();
			}
		}

		// �S�[�����{�^�����N���b�N���ꂽ���ǂ������m�F���܂�
		if (mouseX >= uiPositionGolem.x &&
			mouseX <= uiPositionGolem.x + uiGolemButton->GetTextureWidth() &&
			mouseY >= uiPositionGolem.y &&
			mouseY <= uiPositionGolem.y + uiGolemButton->GetTextureHeight())
		{
			if (spawnGolemCallback) {
				spawnGolemCallback();
			}
		}
	}
}


void UI::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 2D �X�v���C�g�`��
	{
		Graphics& graphics = Graphics::Instance();
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(uiSpiderButton->GetTextureWidth());
		float textureHeight = static_cast<float>(uiSpiderButton->GetTextureHeight());

		// �����X�^�[�{�^���i�w偁j�̕`��
		uiSpiderButton->Render(dc,
			uiPositionSpider.x, uiPositionSpider.y,
			textureWidth, textureHeight,
			0, 0, 
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1);

		textureWidth = static_cast<float>(uiGolemButton->GetTextureWidth());
		textureHeight = static_cast<float>(uiGolemButton->GetTextureHeight());
		// �����X�^�[�{�^���i�S�[�����j�`��
		uiGolemButton->Render(dc,
			uiPositionGolem.x, uiPositionGolem.y,
			textureWidth, textureHeight,
			0, 0,
			textureWidth, textureHeight,
			0,
			1, 1, 1, 1);
	}
}

void UI::SetSpawnSpiderCallback(std::function<void()> callback) {
	spawnSpiderCallback = callback;
}

void UI::SetSpawnGolemCallback(std::function<void()> callback) {
	spawnGolemCallback = callback;
}