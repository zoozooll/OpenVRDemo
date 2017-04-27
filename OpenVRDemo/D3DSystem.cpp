#include "stdafx.h"

#include "D3DSystem.h"
#include "Utils.h"

D3DSystem::D3DSystem()
{
	m_vr = NULL;
}


D3DSystem::~D3DSystem()
{
}

bool D3DSystem::Initialize(HWND hWnd)
{
	
	bool result;
	m_vr = new OpenVRSystem;
	if (!m_vr)
	{
		return false;
	}
	result = m_vr->Initialize(hWnd);
	if (!result)
	{
		return false;
	}
	result = CreateDeviceAndSwapChain(hWnd);
	if (!result)
	{
		return false;
	}
	result = CreateTargetView();
	if (!result)
	{
		return false;
	}
	result = CreateDepthStencil();
	if (!result)
	{
		return false;
	}
	result = CreateDepthStencilState();
	if (!result)
	{
		return false;
	}
	// Bind depth stencil state;
	m_Context->OMSetDepthStencilState(m_depthStencilState, 1);
	result = CreateDepthStencilStateUsingDevice();
	if (!result)
	{
		return false;
	}
	BindViewport();
	return true;
}

bool D3DSystem::CreateDeviceAndSwapChain(HWND hWnd)
{

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// Create device and swap chain;
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE ,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_NULL	 ,
		D3D_DRIVER_TYPE_SOFTWARE ,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_UNKNOWN
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = 1920;
	swapDesc.BufferDesc.Height = 1080;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // unsigned normal
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1; // multisampling, which antialiasing for geometry. Turn it off
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt-enter fullscreen
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	HRESULT errorCode;
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		errorCode = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc,
			&m_SwapChain, &m_Device, &featureLevel, &m_Context);
		if (SUCCEEDED(errorCode))
		{
			dprintf("D3D11CreateDeviceAndSwapChain success %d", driverTypes[i]);
			break;
		}
	}
	if (FAILED(errorCode))
	{
		dprintf("failed to create D3D device and swapchain");
		return false;
	}
	errorCode = m_SwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_BackBufferTex));
	if (FAILED(errorCode))
	{
		dprintf("failed create back buffer");
		return false;
	}
	return true;
}

bool D3DSystem::CreateTargetView()
{
	D3D11_TEXTURE2D_DESC bbDesc;
	ZeroMemory(&bbDesc, sizeof(D3D11_TEXTURE2D_DESC));
	m_BackBufferTex->GetDesc(&bbDesc);
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = bbDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	// Create target view;
	HRESULT errorCode = m_Device->CreateRenderTargetView(m_BackBufferTex, &rtvDesc, &m_RenderTargetView);
	if (FAILED(errorCode))
	{
		dprintf("D3D11 create target view error");
		return false;
	}
	Memory::SafeRelease(m_BackBufferTex);
	return true;
}

bool D3DSystem::CreateDepthStencil()
{
	// Create depth stencil;
	ID3D11Texture2D *pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = m_vr->GetRenderWidth();// swapDesc.BufferDesc.Width;
	descDepth.Height = m_vr->GetRenderHeight();// swapDesc.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;// DXGI_FORMAT_D32_FLOAT;//DXGI_FORMAT_D24_UNORM_S8_UINT;;//pDeviceSettings->d3d11.AutoDepthStencilFormat;
														// DXGI_FORMAT_D32_FLOAT_S8X24_UINT
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT errorCode = m_Device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(errorCode))
	{
		dprintf("create depth stencil texture failed");
		return false;
	}
	return true;
}

bool D3DSystem::CreateDepthStencilState()
{
	// Creteate depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HRESULT errorCode = m_Device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
	if (FAILED(errorCode))
	{
		dprintf("Create depth stencil state failed");
		return false;
	}
	return true;
}

bool D3DSystem::CreateDepthStencilStateUsingDevice()
{
	// Create the state using the device;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HRESULT errorCode = m_Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(errorCode))
	{
		dprintf("Create the state using the device failed");
		return false;
	}
	return true;
}

void D3DSystem::BindViewport()
{
	// Create and bind viewport
	m_viewport.Width = static_cast<float>(m_vr->GetRenderWidth());
	m_viewport.Height = static_cast<float>(m_vr->GetRenderHeight());
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_Context->RSSetViewports(1, &m_viewport);
}
