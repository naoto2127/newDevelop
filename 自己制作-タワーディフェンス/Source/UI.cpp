#include "UI.h"
#include <Input/Input.h>

UI::UI()
	: cooldownTimerSpider(0.0f), cooldownTimerGolem(0.0f), cooldownDuration(1.5f)  // �N�[���_�E������1.5�b
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

	if (uiGolemButton)
	{
		delete uiGolemButton;
		uiGolemButton = nullptr;
	}
}
void UI::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	
	// �X�p�C�_�[�{�^���̃N�[���_�E���^�C�}�[���X�V
	if (cooldownTimerSpider > 0.0f) {
		cooldownTimerSpider -= elapsedTime;
	}

	// �S�[�����{�^���̃N�[���_�E���^�C�}�[���X�V
	if (cooldownTimerGolem > 0.0f) {
		cooldownTimerGolem -= elapsedTime;
	}

	// �}�E�X�����N���b�N�����Ƃ�
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		int mouseX = mouse.GetPositionX();
		int mouseY = mouse.GetPositionY();

		// �X�p�C�_�[�{�^�����N���b�N���ꂽ���ǂ������m�F���܂�
		if (mouseX >= uiPositionSpider.x &&
			mouseX <= uiPositionSpider.x + uiSpiderButton->GetTextureWidth() &&
			mouseY >= uiPositionSpider.y &&
			mouseY <= uiPositionSpider.y + uiSpiderButton->GetTextureHeight() &&
			cooldownTimerSpider <= 0.0f // �N�[���_�E�����I�����Ă��邩
			)
		{
			if (spawnSpiderCallback) {
				spawnSpiderCallback();
			}
			cooldownTimerSpider = cooldownDuration;  // �N�[���_�E���^�C�}�[���Z�b�g
		}

		// �S�[�����{�^�����N���b�N���ꂽ���ǂ������m�F
		if (mouseX >= uiPositionGolem.x &&
			mouseX <= uiPositionGolem.x + uiGolemButton->GetTextureWidth() &&
			mouseY >= uiPositionGolem.y &&
			mouseY <= uiPositionGolem.y + uiGolemButton->GetTextureHeight() &&
			cooldownTimerGolem <= 0.0f)  // �N�[���_�E�����I�����Ă��邩
		{
			if (spawnGolemCallback) {
				spawnGolemCallback();
			}
			cooldownTimerGolem = cooldownDuration;  // �N�[���_�E���^�C�}�[���Z�b�g
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

		// �X�p�C�_�[�{�^���𔼓����ŕ`��
		float spiderAlpha = (cooldownTimerSpider > 0.0f) ? 0.5f : 1.0f; // �N�[���_�E�����͔�����
		uiSpiderButton->Render(dc,
			uiPositionSpider.x, uiPositionSpider.y,
			static_cast<float>(uiSpiderButton->GetTextureWidth()), static_cast<float>(uiSpiderButton->GetTextureHeight()),
			0, 0,
			uiSpiderButton->GetTextureWidth(), uiSpiderButton->GetTextureHeight(),
			0,
			spiderAlpha, spiderAlpha, spiderAlpha, 1.0f);

		textureWidth = static_cast<float>(uiGolemButton->GetTextureWidth());
		textureHeight = static_cast<float>(uiGolemButton->GetTextureHeight());
		
		// �S�[�����{�^���𔼓����ŕ`��
		float golemAlpha = (cooldownTimerGolem > 0.0f) ? 0.5f : 1.0f; // �N�[���_�E�����͔�����
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