#pragma once
#include <SpriteBatch.h>

class AnimatedTexture
{
public:
	AnimatedTexture();

	AnimatedTexture(const DirectX::XMFLOAT2& origin,
		float rotation,
		float scale,
		float depth);

	~AnimatedTexture();

	void Load(ID3D11ShaderResourceView* texture,
		int frameCount,
		int framesPerSecond);

	void Update(float elapsed);

	void Draw(DirectX::SpriteBatch* batch, const DirectX::XMFLOAT2& screenPos) const;

	void Draw(DirectX::SpriteBatch* batch, int frame, const DirectX::XMFLOAT2& screenPos) const;

	void Reset();

	void Stop();

	void Play();

	void Pause();

	bool IsPaused() const;

private:
	bool mPause;
	int mFrame;
	int mFrameCount;
	int mTextureWidth;
	int mTextureHeight;
	float mTimePerFrame;
	float mTotalElapsed;
	float mDepth;
	float mRotation;
	DirectX::XMFLOAT2 mOrigin;
	DirectX::XMFLOAT2 mScale;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
};

