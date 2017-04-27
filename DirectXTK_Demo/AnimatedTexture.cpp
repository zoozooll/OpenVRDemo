#include "pch.h"
#include "AnimatedTexture.h"


AnimatedTexture::AnimatedTexture() :
	mPause(false),
	mFrame(0),
	mFrameCount(0),
	mTextureWidth(0),
	mTextureHeight(0),
	mTimePerFrame(0.f),
	mTotalElapsed(0.f),
	mRotation(0.f),
	mScale(1.f, 1.0f),
	mDepth(0.0f),
	mOrigin(0.0f, 0.0f)
{
}

AnimatedTexture::AnimatedTexture(const DirectX::XMFLOAT2& origin,
	float rotation,
	float scale,
	float depth) :
	mPause(false),
	mFrame(0),
	mFrameCount(0),
	mTextureWidth(0),
	mTextureHeight(0),
	mTimePerFrame(0.f),
	mTotalElapsed(0.f),
	mRotation(rotation),
	mScale(scale, scale),
	mDepth(depth),
	mOrigin(origin)
{

}

AnimatedTexture::~AnimatedTexture()
{
}

void AnimatedTexture::Load(ID3D11ShaderResourceView * texture, int frameCount, int framesPerSecond)
{
	if (frameCount < 0 || framesPerSecond <= 0)
	{
		throw std::invalid_argument("AnimatedTexture");
	}

	mPause = false;
	mFrame = 0;
	mFrameCount = frameCount;
	mTimePerFrame = 1.0f / float(framesPerSecond);
	mTotalElapsed = 0.0f;
	mTexture = texture;
	if (texture)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		texture->GetResource(resource.GetAddressOf());
		D3D11_RESOURCE_DIMENSION dim;
		resource->GetType(&dim);
		if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		{
			throw std::exception("AnimatedTexture excepts a Texture2D");
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
		resource.As(&tex2D);
		D3D11_TEXTURE2D_DESC desc;
		tex2D->GetDesc(&desc);
		mTextureWidth = int(desc.Width);
		mTextureHeight = int(desc.Height);
	}
}

void AnimatedTexture::Update(float elapsed)
{
	if (mPause)
	{
		return;
	}
	mTotalElapsed += elapsed;
	if (mTotalElapsed > mTimePerFrame)
	{
		mFrame++;
		mFrame = mFrame % mFrameCount;
		mTotalElapsed -= mTimePerFrame;
	}
}

void AnimatedTexture::Draw(DirectX::SpriteBatch* batch, const DirectX::XMFLOAT2 & screenPos) const
{
	Draw(batch, mFrame, screenPos);
}

void AnimatedTexture::Draw(DirectX::SpriteBatch* batch, int frame, const DirectX::XMFLOAT2 & screenPos) const
{
	int frameWidth = mTextureWidth / mFrameCount;
	RECT sourceRect;
	sourceRect.left = frameWidth * frame;
	sourceRect.top = 0;
	sourceRect.right = sourceRect.left + frameWidth;
	sourceRect.bottom = mTextureHeight;
	batch->Draw(mTexture.Get(), screenPos, &sourceRect, DirectX::Colors::White,
		mRotation, mOrigin, mScale, DirectX::SpriteEffects_None, mDepth);
}

void AnimatedTexture::Reset()
{
	mFrame = 0;
	mTotalElapsed = 0.0f;
}

void AnimatedTexture::Stop()
{
	mPause = true;
	mFrame = 0;
	mTotalElapsed = 0.0f;
}

void AnimatedTexture::Play()
{
	mPause = false;
}

void AnimatedTexture::Pause()
{
	mPause = true;
}

bool AnimatedTexture::IsPaused() const
{
	return  mPause;
}
