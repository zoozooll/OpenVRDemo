#pragma once

#include <openvr.h>
class OpenVRSystem
{
public:
	OpenVRSystem();
	~OpenVRSystem();

	bool Initialize(HWND hwnd);
	UINT32 GetRenderWidth();
	UINT32 GetRenderHeight();

	vr::IVRSystem *m_pHMD = NULL;
	vr::IVRRenderModels *m_pRenderModels = NULL;
	UINT32 m_nRenderWidth, m_nRenderHeight;
};

