#include "stdafx.h"
#include "OpenVRSystem.h"
#include "stdafx.h"
#include <stdio.h>
#include <xstring>
#include "Utils.h"


OpenVRSystem::OpenVRSystem()
{
	m_pHMD = NULL;
	m_pRenderModels = NULL;
}


OpenVRSystem::~OpenVRSystem()
{
}

bool OpenVRSystem::Initialize(HWND hwnd)
{
	
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, ARRAYSIZE(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		std::string temp(buf);
		std::wstring wtemp(temp.begin(), temp.end());
		MessageBox(hwnd, wtemp.c_str(), L"VR_Init Failed", 0);
		return false;
	}
	m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);
	dprintf("RecommendedRenderTargetSize (%d, %d)", m_nRenderWidth, m_nRenderHeight);
	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, ARRAYSIZE(buf), "Unable to render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		std::string temp(buf);
		std::wstring wtemp(temp.begin(), temp.end());
		MessageBox(hwnd, wtemp.c_str(), L"VR Render model Failed", 0);
		return false;

	}
	if (!vr::VRCompositor())
	{
		dprintf("Compositor initialization failed");
		return false;
	}

	return true;
}

UINT32 OpenVRSystem::GetRenderWidth()
{
	return m_nRenderWidth;
}

UINT32 OpenVRSystem::GetRenderHeight()
{
	return m_nRenderHeight;
}
