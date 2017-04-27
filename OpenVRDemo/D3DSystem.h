#pragma once

#include <d3d11.h>
#include "OpenVRSystem.h"

class D3DSystem
{
public:
	D3DSystem();
	~D3DSystem();

	bool Initialize(HWND hWnd);

	// Create device and swap chain;
	bool CreateDeviceAndSwapChain(HWND hWnd);
	// Create target view;
	bool CreateTargetView();
	// Create depth stencil;
	bool CreateDepthStencil();
	// Create depth stencil state
	bool CreateDepthStencilState();
	// Create the state using the device;
	bool CreateDepthStencilStateUsingDevice();
	// Create and bind viewport
	void BindViewport();

private:
	OpenVRSystem * m_vr = NULL;
	IDXGISwapChain *m_SwapChain = NULL;
	ID3D11Device *m_Device = NULL;
	ID3D11DeviceContext *m_Context = NULL;
	ID3D11Texture2D *m_BackBufferTex = NULL;
	ID3D11RenderTargetView *m_RenderTargetView = NULL;
	ID3D11DepthStencilState *m_depthStencilState = NULL;
	ID3D11DepthStencilState *m_depthDisabledStencilState = NULL;
	D3D11_VIEWPORT m_viewport;
	D3D_FEATURE_LEVEL  featureLevel;
};

