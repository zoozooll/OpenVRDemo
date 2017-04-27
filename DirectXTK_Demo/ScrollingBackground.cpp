#include "pch.h"
#include "ScrollingBackground.h"


ScrollingBackground::ScrollingBackground() :
	mScreenHeight(0),
	mTextureWidth(0),
	mTextureHeight(0),
	mScreenPos(0.0f, 0.0f),
	mTextureSize(0.0f, 0.0f),
	mOrigin(0.0f, 0.0f)
{
}


ScrollingBackground::~ScrollingBackground()
{
}

void ScrollingBackground::Load(ID3D11ShaderResourceView * texture)
{
	mTexture = texture;
	if (texture)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		texture->GetResource(resource.GetAddressOf());
		D3D11_RESOURCE_DIMENSION dim;
		resource->GetType(&dim);
		if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		{
			throw std::exception("ScrollingBackground expects a Texture2D");
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
		resource.As(&tex2D);
		D3D11_TEXTURE2D_DESC desc;
		tex2D->GetDesc(&desc);
		mTextureWidth = desc.Width;
		mTextureHeight = desc.Height;
		mTextureSize.x = 0.0f;
		mTextureSize.y = float(desc.Height);
		mOrigin.x = (float)desc.Width / 2.0f;
		mOrigin.y = 0.0f;
	}
}

void ScrollingBackground::SetWindow(int screenWidth, int screenHeight)
{
	mScreenHeight = screenHeight;
	mScreenPos.x = float(screenWidth) / 2.0f;
	mScreenPos.y = float(screenHeight) / 2.0f;
}

void ScrollingBackground::Update(float deltaY)
{
	mScreenPos.y += deltaY;
	mScreenPos.y = fmodf(mScreenPos.y, float(mTextureHeight));
}

void ScrollingBackground::Draw(DirectX::SpriteBatch * batch) const
{
	using namespace DirectX;
	XMVECTOR screenPos = XMLoadFloat2(&mScreenPos);
	XMVECTOR origin = XMLoadFloat2(&mOrigin);
	if (mScreenPos.y < mScreenHeight)
	{
		batch->Draw(mTexture.Get(), screenPos, NULL,
			Colors::White, 0.0f, origin, g_XMOne, SpriteEffects_None, 0.0f);
	}
	XMVECTOR textureSize = XMLoadFloat2(&mTextureSize);
	batch->Draw(mTexture.Get(), screenPos - textureSize, NULL,
		Colors::White, 0.0f, origin, g_XMOne, SpriteEffects_None, 0.0f);
	if (mTextureHeight < mScreenHeight)
	{
		batch->Draw(mTexture.Get(), screenPos + textureSize, NULL,
			Colors::White, 0.0f, origin, g_XMOne, SpriteEffects_None, 0.0f);
	}
}
