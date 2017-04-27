////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texture_model.h"


TextureModel::TextureModel()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_Texture = NULL;
}


TextureModel::TextureModel(const TextureModel& other)
{
}


TextureModel::~TextureModel()
{
}


bool TextureModel::Initialize(ID3D11Device* device,  ID3D11DeviceContext *deviceContext, wchar_t *textureFilename)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}
	return true;
}


void TextureModel::Shutdown()
{

	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void TextureModel::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int TextureModel::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* TextureModel::GetTexture()
{
	return m_Texture->GetTexture();
}


bool TextureModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.5f);  // Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.5f);  // Top left.
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.5f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.5f);  // Top right.
	vertices[3].texture = XMFLOAT2(1.f, 0.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // Bottom right.
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 1;

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
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
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void TextureModel::ShutdownBuffers()
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


void TextureModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool TextureModel::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, wchar_t *filename)
{
	bool result;
	m_Texture = new TextureClass();
	if (!m_Texture)
	{
		return false;
	}
	result = m_Texture->Initialize(device, deviceContext, filename);
}

void TextureModel::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = NULL;
	}
}

//private int esGenSphere(int numSlices, float d) {
//	int i;
//	int j;
//	int iidex = 0;
//	int numParallels = numSlices / 2;
//	numVertices = (numParallels + 1) * (numSlices + 1);
//	numIndices = numParallels * numSlices * 6;
//	float angleStep = (float)((2.0f * Math.PI) / ((float)numSlices));
//	float vertices[] = new float[numVertices * 3];
//	float texCoords[] = new float[numVertices * 2];
//	short indices[] = new short[numIndices];
//	for (i = 0; i < numParallels + 1; i++) {
//		for (j = 0; j < numSlices + 1; j++) {
//			int vertex = (i * (numSlices + 1) + j) * 3;
//			vertices[vertex + 0] = (float)(d * Math.sin(angleStep * (float)i) * Math.sin(angleStep * (float)j));
//			vertices[vertex + 1] = (float)(d * Math.cos(angleStep * (float)i));
//			vertices[vertex + 2] = (float)(d * Math.sin(angleStep * (float)i) * Math.cos(angleStep * (float)j));
//
//			int texIndex = (i * (numSlices + 1) + j) * 2;
//			texCoords[texIndex + 0] = 1.0f - (float)j / (float)numSlices;
//			texCoords[texIndex + 1] = ((float)i / (float)numParallels);//((float)i/(float)numParallels);//
//		}
//	}
//
//	for (i = 0; i < numParallels; i++) {
//		for (j = 0; j < numSlices; j++) {
//			indices[iidex++] = (short)(i * (numSlices + 1) + j);
//			indices[iidex++] = (short)((i + 1) * (numSlices + 1) + j);
//			indices[iidex++] = (short)((i + 1) * (numSlices + 1) + (j + 1));
//
//			indices[iidex++] = (short)(i * (numSlices + 1) + j);
//			indices[iidex++] = (short)((i + 1) * (numSlices + 1) + (j + 1));
//			indices[iidex++] = (short)(i * (numSlices + 1) + (j + 1));
//		}
//	}
//	//writeToSdcard(mainActivity, vertices, texCoords, indices);
//	ByteBuffer bb = ByteBuffer.allocateDirect(
//		vertices.length * 4);
//	bb.order(ByteOrder.nativeOrder());
//
//	vertexBuffer = bb.asFloatBuffer();
//	vertexBuffer.put(vertices);
//	vertexBuffer.position(0);
//
//	ByteBuffer cc = ByteBuffer.allocateDirect(
//		texCoords.length * 4);
//	cc.order(ByteOrder.nativeOrder());
//
//	textureBuffer = cc.asFloatBuffer();
//	textureBuffer.put(texCoords);
//	textureBuffer.position(0);
//
//	ByteBuffer dd = ByteBuffer.allocateDirect(
//		indices.length * 2);
//	dd.order(ByteOrder.nativeOrder());
//
//	IndicesBuffer = dd.asShortBuffer();
//	IndicesBuffer.put(indices);
//	IndicesBuffer.position(0);
//
//	GLES20.glEnableVertexAttribArray(attribPosition);
//	GLES20.glEnableVertexAttribArray(attribTexCoord);
//	//		GLES20.glEnable(GLES20.GL_BLEND);
//	//		GLES20.glBlendFunc(GLES20.GL_ONE,GLES20.GL_ONE_MINUS_SRC_ALPHA);
//	//		GLES20.glDrawElements(GLES20.GL_TRIANGLES,6,GLES20.GL_UNSIGNED_SHORT,1);
//	//		GLES20.glEnable(GLES20.GL_BLEND);
//	//   GLES20.glEnableVertexAttribArray(maPositionHandle);
//
//	return numIndices;
//}
