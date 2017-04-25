////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "SphereModelClass.h"
#include <cmath>

SphereModelClass::SphereModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;
}


SphereModelClass::SphereModelClass(const SphereModelClass& other)
{
}


SphereModelClass::~SphereModelClass()
{
}


bool SphereModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}
	result = InitializeWorldMatrix();
	if (!result)
	{
		return false;
	}
	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void SphereModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void SphereModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int SphereModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* SphereModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void SphereModelClass::AddVertex(float x, float y, float z, float tx, float ty, std::vector<VertexType> &vertdata, D3DXVECTOR3 normal)
{
	VertexType temp;
	temp.position.set(x, y, z);
	temp.texture.set(tx, ty);
	temp.normal = normal;
	vertdata.push_back(temp);
}

void SphereModelClass::AddCubeToScene(std::vector<VertexType> &vertdata, std::vector<unsigned long> &indices, int numSlices, float radius)
{
	int i;
	int j;
	int index = 0;
	int numParallels = numSlices / 2;  //numParallels : Number of rows of quadrangle; numSlices : Number of columns of quadrangle.
	int numVertices = (numParallels + 1) * (numSlices + 1); // Number of all vertices;
	int numIndices = numParallels * numSlices * 6; // Number of all inices for triangle. Two triangles form one quadrangle ;
	float angleStep = (float)((2.0f * DirectX::XM_2PI) / ((float)numSlices)); //angle of per quadrangle in radian.
	//float[] vertices = new float[numVertices * 3];
	//float[] texCoords = new float[numVertices * 2];
	//short[] indices = new short[numIndices];
	//List<short> indices = new List<short>();
	D3DXVECTOR3 normal;
	normal.set(0, 0, 1);
	for (i = 0; i < numParallels + 1; i++)
	{
		for (j = 0; j < numSlices + 1; j++)
		{
			float x = (float)(radius * std::sin(angleStep * (float)i) * std::sin(angleStep * (float)j));
			float y = (float)(radius * std::cos(angleStep * (float)i));
			float z = (float)(radius * std::sin(angleStep * (float)i) * std::cos(angleStep * (float)j));
			float u = 1.0f - (float)j / (float)numSlices;
			float v = ((float)i / (float)numParallels);//((float)i/(float)numParallels);//
			AddVertex(x, y, z, u, v, vertdata, normal);
		}
	}
	for (i = 0; i < numParallels; i++)
	{
		for (j = 0; j < numSlices; j++)
		{
			indices.push_back((short)(i * (numSlices + 1) + j));
			indices.push_back((short)((i + 1) * (numSlices + 1) + j));
			indices.push_back((short)((i + 1) * (numSlices + 1) + (j + 1)));

			indices.push_back((short)(i * (numSlices + 1) + j));
			indices.push_back((short)((i + 1) * (numSlices + 1) + (j + 1)));
			indices.push_back((short)(i * (numSlices + 1) + (j + 1)));
		}
	}
}


bool SphereModelClass::InitializeBuffers(ID3D11Device* device)
{
	std::vector<VertexType> vertices;
	std::vector<unsigned long> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//float testScale = 1.0f;
	//// Load the vertex array with data.
	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f) * testScale;  // Bottom left.
	//vertices[0].texture = D3DXVECTOR2(0.0, 1.0f);

	//vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f) * testScale;  // Top left.
	//vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	//vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f) * testScale;  // top right.
	//vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);

	//vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f) * testScale;  // bottom right.
	//vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);

	//// Load the index array with data.
	//indices[0] = 0;  
	//indices[1] = 1;  
	//indices[2] = 2;  
	//indices[3] = 2;
	//indices[4] = 3;
	//indices[5] = 0;


	//Matrix4 mat;
	//AddCubeToScene(mat, vertices, indices);

	
	AddCubeToScene(vertices, indices, 64, 1.0f);
			

	//for (int i = 0; i < vertices.size() / 2; i++)
	//{
	//	std::swap(vertices[i], vertices[vertices.size() - i - 1]);
	//}
	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	m_vertexCount = vertices.size();
	m_indexCount = indices.size();

	return true;
}

bool SphereModelClass::InitializeWorldMatrix()
{
	float m_fScale = 10.0f;
	float m_iSceneVolumeWidth = 20,
		m_iSceneVolumeHeight = 20,
		m_iSceneVolumeDepth = 20;
	float m_fScaleSpacing = 4.0f;
	Matrix4 matScale;
	matScale.scale(m_fScale, m_fScale, m_fScale);
	Matrix4 matTransform;
	matTransform.translate(-0.5f, 0.0f, -0.5f);

	m_worldMatrix = matScale * matTransform;
	return true;
}

void SphereModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void SphereModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool SphereModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void SphereModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

Matrix4 SphereModelClass::GetWorldMatrix()
{
	return m_worldMatrix;
}

