////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "InnerSphereModel.h"


InnerShereModel::InnerShereModel()
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_Texture = NULL;
}


InnerShereModel::InnerShereModel(const InnerShereModel& other)
{
}


InnerShereModel::~InnerShereModel()
{
}


bool InnerShereModel::Initialize(ID3D11Device* device,  ID3D11DeviceContext *deviceContext, wchar_t *textureFilename)
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


void InnerShereModel::Shutdown()
{

	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void InnerShereModel::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int InnerShereModel::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* InnerShereModel::GetTexture()
{
	return m_Texture->GetTexture();
}


bool InnerShereModel::InitializeBuffers(ID3D11Device* device)
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
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	vertices[1].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[1].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // Bottom right.
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
 
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

//static void GenSphere(int numSlices, float d)
//{
//	int i;
//	int j;
//	int index = 0;
//	int numParallels = numSlices / 2;  //numParallels : Number of rows of quadrangle; numSlices : Number of columns of quadrangle.
//	int numVertices = (numParallels + 1) * (numSlices + 1); // Number of all vertices;
//	int numIndices = numParallels * numSlices * 6; // Number of all inices for triangle. Two triangles form one quadrangle ;
//	float angleStep = (float)((2.0f * Math.PI) / ((float)numSlices)); //angle of per quadrangle in radian.
//	float[] vertices = new float[numVertices * 3];
//	float[] texCoords = new float[numVertices * 2];
//	//short[] indices = new short[numIndices];
//	List<short> indices = new List<short>();
//
//	StringBuilder buildVertices = new StringBuilder();
//	StringBuilder buildTexCoorrds = new StringBuilder();
//	buildVertices.AppendLine("Vertices: ");
//	buildTexCoorrds.AppendLine("TexCoorrds: ");
//	for (i = 0; i < numParallels + 1; i++)
//	{
//		for (j = 0; j < numSlices + 1; j++)
//		{
//			int vertex = (i * (numSlices + 1) + j) * 3;
//			vertices[vertex + 0] = (float)(d * Math.Sin(angleStep * (float)i) * Math.Sin(angleStep * (float)j));
//			vertices[vertex + 1] = (float)(d * Math.Cos(angleStep * (float)i));
//			vertices[vertex + 2] = (float)(d * Math.Sin(angleStep * (float)i) * Math.Cos(angleStep * (float)j));
//			buildVertices.AppendFormat("({0}, {1}, {2}),", vertices[vertex + 0], vertices[vertex + 1], vertices[vertex + 2]);
//			int texIndex = (i * (numSlices + 1) + j) * 2;
//			texCoords[texIndex + 0] = 1.0f - (float)j / (float)numSlices;
//			texCoords[texIndex + 1] = ((float)i / (float)numParallels);//((float)i/(float)numParallels);//
//			buildTexCoorrds.AppendFormat("({0}, {1}),", texCoords[texIndex + 0], texCoords[texIndex + 1]);
//		}
//		buildVertices.AppendLine();
//		buildTexCoorrds.AppendLine();
//	}
//	buildVertices.AppendLine();
//	buildVertices.Append(buildTexCoorrds);
//	buildVertices.AppendLine();
//
//
//	buildVertices.AppendLine("indices: ");
//	int[] divisors;
//	GetDivisors(numSlices, out divisors, 10);
//	Console.WriteLine("GetDivisors result {0}", string.Join<int>(",", divisors));
//	for (i = 0; i < numParallels; i++)
//	{
//		int slices = numSlices;
//		int interval = 1;
//		if (i < divisors.Length)
//		{
//			slices = divisors[i];
//			interval = numSlices / slices;
//		}
//		for (j = 0; j < slices; j++)
//		{
//			indices.Add((short)(i * (numSlices + 1) + j * interval));
//			indices.Add((short)((i + 1) * (numSlices + 1) + j * interval));
//			indices.Add((short)((i + 1) * (numSlices + 1) + (j + 1) * interval));
//			short[] debugBuf = new short[3];
//			debugBuf[0] = (short)(i * (numSlices + 1) + j * interval);
//			debugBuf[1] = (short)((i + 1) * (numSlices + 1) + j * interval);
//			debugBuf[2] = (short)((i + 1) * (numSlices + 1) + (j + 1) * interval);
//			buildVertices.AppendFormat("({0}, {1}, {2}),", debugBuf[0], debugBuf[1], debugBuf[2]);
//
//			indices.Add((short)(i * (numSlices + 1) + j * interval));
//			indices.Add((short)((i + 1) * (numSlices + 1) + (j + 1) * interval));
//			indices.Add((short)(i * (numSlices + 1) + (j + 1) * interval));
//			debugBuf[0] = (short)(i * (numSlices + 1) + j * interval);
//			debugBuf[1] = (short)((i + 1) * (numSlices + 1) + (j + 1) * interval);
//			debugBuf[2] = (short)(i * (numSlices + 1) + (j + 1) * interval);
//			buildVertices.AppendFormat("({0}, {1}, {2}),", debugBuf[0], debugBuf[1], debugBuf[2]);
//		}
//		buildVertices.AppendLine();
//	}
//}


void InnerShereModel::ShutdownBuffers()
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


void InnerShereModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool InnerShereModel::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, wchar_t *filename)
{
	bool result;
	m_Texture = new TextureClass();
	if (!m_Texture)
	{
		return false;
	}
	result = m_Texture->Initialize(device, deviceContext, filename);
}

void InnerShereModel::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = NULL;
	}
}