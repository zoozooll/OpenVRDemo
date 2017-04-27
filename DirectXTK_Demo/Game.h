//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <SimpleMath.h>
#include <CommonStates.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "AnimatedTexture.h"
#include "ScrollingBackground.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

	// Create Device and ContextDevice
    void CreateDevice();
	//
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_DDSTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_AnimTexture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<AnimatedTexture> m_ship;
	std::unique_ptr<ScrollingBackground> m_stars;
	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::unique_ptr<DirectX::CommonStates> m_effectStates;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTex;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;
	DirectX::SimpleMath::Vector2 m_shipPos;
	DirectX::SimpleMath::Vector2 m_fontPos;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
};