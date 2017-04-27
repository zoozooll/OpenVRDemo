#pragma once
#include <SpriteBatch.h>
class ScrollingBackground
{
public:
	ScrollingBackground();
	~ScrollingBackground();

	void Load(ID3D11ShaderResourceView* texture);
	void SetWindow(int screenWidth, int screenHeight);
	void Update(float deltaY);
	void Draw(DirectX::SpriteBatch* batch) const;

private:
	int mScreenHeight;
	int mTextureWidth;
	int mTextureHeight;
	DirectX::XMFLOAT2 mScreenPos;
	DirectX::XMFLOAT2 mTextureSize;
	DirectX::XMFLOAT2 mOrigin;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
};

