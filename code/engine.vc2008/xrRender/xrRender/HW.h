// HW.h: interface for the CHW class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "hwcaps.h"
#include "dxgi.h"

#ifndef _MAYA_EXPORT
#include "stats_manager.h"
#endif

class  CHW
#if defined(USE_DX10) || defined(USE_DX11)
	:	public pureAppActivate, 
		public pureAppDeactivate
#endif	//	USE_DX10
{
//	Functions section
public:
	CHW();
	~CHW();

	void					CreateD3D				();
	void					DestroyD3D				();
	void					CreateDevice			(HWND hw, bool move_window);

	void					DestroyDevice			();

	void					Reset					(HWND hw);

	void					selectResolution		(u32 &dwWidth, u32 &dwHeight, BOOL bWindowed);
	D3DFORMAT				selectDepthStencil		(D3DFORMAT);
	void					ResizeWindowProc		(WORD h, WORD w);
	u32						selectPresentInterval	();
	u32						selectGPU				();
	u32						selectRefresh			(u32 dwWidth, u32 dwHeight, D3DFORMAT fmt);
	BOOL					support					(D3DFORMAT fmt, DWORD type, DWORD usage);

    //compare device through their description structures. Return true if current device is better
    bool                    CompareDevices          (DXGI_ADAPTER_DESC1& PreviousDevice, DXGI_ADAPTER_DESC1& CurrentDevice);

#ifdef DEBUG
#if defined(USE_DX10) || defined(USE_DX11)
	void	Validate(void)	{};
#else	//	USE_DX10
	void	Validate(void)	{	VERIFY(pDevice); VERIFY(pD3D); };
#endif	//	USE_DX10
#else
	void	Validate(void)	{};
#endif

//	Variables section
#if defined(USE_DX11)	//	USE_DX10
public:
	IDXGIAdapter1*			m_pAdapter;	//	pD3D equivalent
	ID3D11Device*			pDevice;	//	combine with DX9 pDevice via typedef
	ID3D11DeviceContext*    pContext;	//	combine with DX9 pDevice via typedef
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView*	pBaseRT;	//	combine with DX9 pBaseRT via typedef
	ID3D11DepthStencilView*	pBaseZB;

	CHWCaps					Caps;

	D3D_DRIVER_TYPE		m_DriverType;	//	DevT equivalent
	DXGI_SWAP_CHAIN_DESC	m_ChainDesc;	//	DevPP equivalent
	D3D_FEATURE_LEVEL		FeatureLevel;
#elif defined(USE_DX10)
public:
	IDXGIAdapter1*			m_pAdapter;	//	pD3D equivalent
	ID3D10Device1*       	pDevice1;	//	combine with DX9 pDevice via typedef
	ID3D10Device*        	pDevice;	//	combine with DX9 pDevice via typedef
	ID3D10Device1*       	pContext1;	//	combine with DX9 pDevice via typedef
	ID3D10Device*        	pContext;	//	combine with DX9 pDevice via typedef
	IDXGISwapChain*         m_pSwapChain;
	ID3D10RenderTargetView*	pBaseRT;	//	combine with DX9 pBaseRT via typedef
	ID3D10DepthStencilView*	pBaseZB;

	CHWCaps					Caps;

	D3D10_DRIVER_TYPE		m_DriverType;	//	DevT equivalent
	DXGI_SWAP_CHAIN_DESC	m_ChainDesc;	//	DevPP equivalent
	D3D_FEATURE_LEVEL		FeatureLevel;
#else

public:

	IDirect3D9* 			pD3D;		// D3D
	IDirect3DDevice9*		pDevice;	// render device

	IDirect3DSurface9*		pBaseRT;
	IDirect3DSurface9*		pBaseZB;

	CHWCaps					Caps;

	UINT					DevAdapter;
	D3DDEVTYPE				DevT;
	D3DPRESENT_PARAMETERS	DevPP;
#endif	//	USE_DX10

#ifndef _MAYA_EXPORT
	stats_manager			stats_manager;
#endif
#if defined(USE_DX10) || defined(USE_DX11)
	void			UpdateViews();
	DXGI_RATIONAL	selectRefresh(u32 dwWidth, u32 dwHeight, DXGI_FORMAT fmt);

	virtual	void	OnAppActivate();
	virtual void	OnAppDeactivate();
#endif	//	USE_DX10

private:
	bool					m_move_window;
};

extern ECORE_API CHW		HW;
