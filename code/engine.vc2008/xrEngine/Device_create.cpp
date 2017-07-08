#include "stdafx.h"

#include "../Include/xrRender/DrawUtils.h"
#include "render.h"
#include "dedicated_server_only.h"
#include "../xrcdb/xrxrc.h"

extern XRCDB_API bool *cdb_bDebug;

void	SetupGPU(IRenderDeviceRender *pRender)
{
	// Command line
	char *lpCmdLine		= Core.Params;

	BOOL bForceGPU_SW;
	BOOL bForceGPU_NonPure;
	BOOL bForceGPU_REF;

	if (strstr(lpCmdLine,"-gpu_sw")!=NULL)		bForceGPU_SW		= TRUE;
	else										bForceGPU_SW		= FALSE;
	if (strstr(lpCmdLine,"-gpu_nopure")!=NULL)	bForceGPU_NonPure	= TRUE;
	else										bForceGPU_NonPure	= FALSE;
	if (strstr(lpCmdLine,"-gpu_ref")!=NULL)		bForceGPU_REF		= TRUE;
	else										bForceGPU_REF		= FALSE;

	pRender->SetupGPU(bForceGPU_SW, bForceGPU_NonPure, bForceGPU_REF);
}

void CRenderDevice::_SetupStates	()
{
	// General Render States
	mView.identity			();
	mProject.identity		();
	mFullTransform.identity	();
	vCameraPosition.set		(0,0,0);
	vCameraDirection.set	(0,0,1);
	vCameraTop.set			(0,1,0);
	vCameraRight.set		(1,0,0);

	m_pRender->SetupStates();
}

void CRenderDevice::_Create	(LPCSTR shName)
{
	Memory.mem_compact			();

	// after creation
	b_is_Ready					= TRUE;
	_SetupStates				();

	m_pRender->OnDeviceCreate(shName);

	dwFrame						= 0;
}

/*
void CRenderDevice::Create	() 
{
	if (b_is_Ready)		return;		// prevent double call
	Statistic			= xr_new<CStats>();
	Log					("Starting RENDER device...");

#ifdef _EDITOR
	psCurrentVidMode[0]	= dwWidth;
	psCurrentVidMode[1] = dwHeight;
#endif

	HW.CreateDevice		(m_hWnd);
	dwWidth				= HW.DevPP.BackBufferWidth	;
	dwHeight			= HW.DevPP.BackBufferHeight	;
	fWidth_2			= float(dwWidth/2)			;
	fHeight_2			= float(dwHeight/2)			;
	fFOV				= 90.f;
	fASPECT				= 1.f;

	string_path			fname; 
	FS.update_path		(fname,"$game_data$","shaders.xr");

	//////////////////////////////////////////////////////////////////////////
	Resources			= xr_new<CResourceManager>		();
	_Create				(fname);

	PreCache			(0);
}
*/

void CRenderDevice::ConnectToRender()
{
	if (!m_pRender)
		m_pRender			= RenderFactory->CreateRenderDeviceRender();
}

PROTECT_API void CRenderDevice::Create	() 
{
	if (b_is_Ready)		return;		// prevent double call
	Statistic			= xr_new<CStats>();

#ifdef	DEBUG
cdb_clRAY		= &Statistic->clRAY;				// total: ray-testing
cdb_clBOX		= &Statistic->clBOX;				// total: box query
cdb_clFRUSTUM	= &Statistic->clFRUSTUM;			// total: frustum query
cdb_bDebug		= &bDebug;
#endif

	if (!m_pRender)
		m_pRender			= RenderFactory->CreateRenderDeviceRender();
	SetupGPU(m_pRender);
	Log					("Starting RENDER device...");

#ifdef _EDITOR
	psCurrentVidMode[0]	= dwWidth;
	psCurrentVidMode[1] = dwHeight;
#endif // #ifdef _EDITOR

	fFOV				= 90.f;
	fASPECT				= 1.f;
	m_pRender->Create	(
		m_hWnd,
		dwWidth,
		dwHeight,
		fWidth_2,
		fHeight_2,
#ifdef INGAME_EDITOR
		editor() ? false :
#endif // #ifdef INGAME_EDITOR
		true
	);

	string_path			fname; 
	FS.update_path		(fname,"$game_data$","shaders.xr");

	//////////////////////////////////////////////////////////////////////////
	_Create				(fname);

	PreCache			(0, false, false);
}