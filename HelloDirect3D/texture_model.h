
#ifndef _TEXTURE_MODEL_H_
#define _TEXTURE_MODEL_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

#include "TextureClass.h"

using namespace DirectX;

class TextureModel
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	TextureModel();
	TextureModel(const TextureModel&);
	~TextureModel();

	bool Initialize(ID3D11Device *, ID3D11DeviceContext *, wchar_t *);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device *, ID3D11DeviceContext *, wchar_t *);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass *m_Texture;
};

#endif