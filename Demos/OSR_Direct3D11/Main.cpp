//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3DGlobals.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#include <Cef3D.h>
#include <Cef3DPCH.h>


#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3d9.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "windowscodecs.lib")

#define PROFILE 1

#define D3D_SAFE_RELEASE(p) if (p) { ((IUnknown*)p)->Release();  p = 0; }

class Cef3DDelegate;

bool InitWin32(int Width, int Height, int X, int Y, HINSTANCE hInstance);
bool InitD3D(int Width, int Height, HWND window);
bool CompileVertexShader(unsigned flags);
bool CompilePixelShader(unsigned flags);
int GetMouseModifiers(WPARAM wparam);
int GetKeyboardModifiers(WPARAM wparam, LPARAM lparam);
bool IsKeyDown(WPARAM wparam);
void Cleanup();
void PumpMessageLoop();

IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
ID3D11Buffer* pVsConstantBuffer;
ID3D11Texture2D* pOffscreenTex;
ID3D11ShaderResourceView* pOffscreenTexSRV;
ID3D11SamplerState* pOffScreenSampler;
HWND TopWindow = 0;
Cef3D::Cef3DBrowser* OverlayBrowser = 0;
bool MouseTracking = false;
struct SimpleVertex;
struct VsConstantBuffer;

class Cef3DDelegate
{
public:
	void OnPaint(Cef3D::Cef3DBrowser* browser,
		Cef3D::Cef3DOsrRenderType type,
		const std::vector<Cef3D::Cef3DRect>& dirtyRects,
		const void* buffer,
		int width,
		int height)
	{
		unsigned rowSize = width * 4;
		unsigned rowStart = 0;
		unsigned char* src = (unsigned char*)buffer;

		if (!devcon)
			return;

		// Update the d3d11 texture
		D3D11_MAPPED_SUBRESOURCE mappedData;
		mappedData.pData = 0;

		HRESULT hr = devcon->Map(pOffscreenTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

		if (FAILED(hr) || !mappedData.pData)
			return;

		for (int row = 0; row < height; ++row)
			memcpy((unsigned char*)mappedData.pData + row * mappedData.RowPitch + rowStart, src + row * rowSize, rowSize);

		devcon->Unmap(pOffscreenTex, 0);
	}

	void OnBrowserCreated(Cef3D::Cef3DBrowser* browser)
	{
		if(!OverlayBrowser)
			OverlayBrowser = browser;
	}

};

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	{
		int WinWidth = 1400;
		int WinHeight = 900;

		if (!InitWin32(WinWidth, WinHeight, 300, 300, hInInstance))
			return -1;

		if (!InitD3D(WinWidth, WinHeight, TopWindow))
			return -1;

		bool isSubProcessed = true;

		Cef3D::Cef3DDefinition definition;
		definition.UseChildProcess = isSubProcessed;
		definition.OffscreenRendering = true;

		bool init = Cef3D_Init(definition);

		if (!init)
			return -1;

		scoped_ptr<Cef3DDelegate> Cef3DListener(new Cef3DDelegate);

		Cef3D::Cef3DBrowserDefinition def;
		def.Rect = Cef3D::Cef3DRect(WinWidth, WinHeight);
		def.ParentHandle = TopWindow;

		scoped_ptr<Cef3D::Cef3DBrowser> browser2;
		browser2.reset(Cef3D_CreateBrowser(def));

		Cef3D::Cef3DDelegates::OnPaint.Add(CEF3D_BIND(&Cef3DDelegate::OnPaint, Cef3DListener.get()));
		Cef3D::Cef3DDelegates::OnAfterCreated.Add(CEF3D_BIND(&Cef3DDelegate::OnBrowserCreated, Cef3DListener.get()));

		PumpMessageLoop();

		browser2->Close(true);

		Cleanup();

		// Currently, if we close like this, Chromium might not have enough time to cleanup everything, so wait for everything to shut down before exiting our message loop
		Cef3D_Shutdown();
	}

	return 0;
}

struct SimpleVertex
{
	DirectX::XMFLOAT3 Point;
};

struct VsConstantBuffer
{
	DirectX::XMFLOAT4X4 ViewProj;
	DirectX::XMFLOAT4X4 ViewProjInv;
	DirectX::XMFLOAT3 CamPos;
	float padding;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;

	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	{
		if (message == WM_KEYDOWN)
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
		}

		bool isSystemKey = message == WM_SYSCHAR ||
			message == WM_SYSKEYDOWN ||
			message == WM_SYSKEYUP;

		if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
			OverlayBrowser->SendKeyEvent(Cef3D::CEF3D_KEY_RAWKEYDOWN, lParam, wParam, isSystemKey, GetKeyboardModifiers(wParam, lParam));
		else if (message == WM_KEYUP || message == WM_SYSKEYUP)
			OverlayBrowser->SendKeyEvent(Cef3D::CEF3D_KEY_UP, lParam, wParam, isSystemKey, GetKeyboardModifiers(wParam, lParam));
		else
			OverlayBrowser->SendKeyEvent(Cef3D::CEF3D_KEY_CHAR, lParam, wParam, isSystemKey, GetKeyboardModifiers(wParam, lParam));

		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		Cef3D::Cef3DMouseEventType btnType =
			(message == WM_LBUTTONDOWN ? Cef3D::CEF3D_LBUTTON_DOWN : (
				message == WM_RBUTTONDOWN ? Cef3D::CEF3D_RBUTTON_DOWN : Cef3D::CEF3D_MBUTTON_DOWN));

		if (OverlayBrowser)
		{
			OverlayBrowser->SendMouseClickEvent(btnType, x, y, GetMouseModifiers(wParam));
		}
		break;
	}

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		Cef3D::Cef3DMouseEventType btnType =
			(message == WM_LBUTTONUP ? Cef3D::CEF3D_LBUTTON_UP : (
				message == WM_RBUTTONUP ? Cef3D::CEF3D_RBUTTON_UP : Cef3D::CEF3D_MBUTTON_UP));

		if (OverlayBrowser)
		{
			OverlayBrowser->SendMouseClickEvent(btnType, x, y, GetMouseModifiers(wParam));
		}
		break;
	}

	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		if (!MouseTracking)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
		}

		if (OverlayBrowser)
		{
			OverlayBrowser->SendMouseClickEvent(Cef3D::CEF3D_MOUSE_MOVE, x, y, GetMouseModifiers(wParam));
		}
		break;
	}
	case WM_MOUSELEAVE:
	{
		if (MouseTracking)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE & TME_CANCEL;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
			MouseTracking = false;
		}

		if (OverlayBrowser)
		{
			POINT p;
			::GetCursorPos(&p);
			::ScreenToClient(hWnd, &p);

			OverlayBrowser->SendMouseClickEvent(Cef3D::CEF3D_MOUSE_LEAVE, p.x, p.y, GetMouseModifiers(wParam));
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		POINT screen_point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		HWND scrolled_wnd = ::WindowFromPoint(screen_point);
		if (scrolled_wnd != hWnd)
			break;

		ScreenToClient(hWnd, &screen_point);
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (OverlayBrowser)
		{
			OverlayBrowser->SendMouseWheelEvent(screen_point.x, screen_point.y, IsKeyDown(VK_SHIFT) ? delta : 0, !IsKeyDown(VK_SHIFT) ? delta : 0, GetMouseModifiers(wParam));
		}
		break;
	}
	case WM_SIZE:
	{
		if (OverlayBrowser)
		{
			RECT clientRect;
			::GetClientRect(hWnd, &clientRect);

			int w = clientRect.right - clientRect.left;
			int h = clientRect.bottom - clientRect.top;

			// Re init Direct3D
			if (!InitD3D(w, h, hWnd))
			{
				PostQuitMessage(0);
				break;
			}
			OverlayBrowser->SendResize(Cef3D::Cef3DRect(clientRect.left,clientRect.top,clientRect.right - clientRect.left,clientRect.bottom - clientRect.top));
		}
		break;
	}
	break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool InitWin32(int Width, int Height, int X, int Y, HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	LPCWSTR className = L"Cef3D.OSR.Direct3D 11";

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = className;

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, Width, Height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	TopWindow = CreateWindowEx(NULL,
		className,
		L"Cef3D Rendering Window",
		WS_OVERLAPPEDWINDOW,
		X,
		Y,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (TopWindow == 0)
		return false;

	ShowWindow(TopWindow, SW_SHOW);

	return true;
}

bool InitD3D(int Width, int Height, HWND window)
{
	// Release old ones
	if (swapchain)
		D3D_SAFE_RELEASE(swapchain);
	if (backbuffer)
		D3D_SAFE_RELEASE(backbuffer);
	if (pOffscreenTex)
		D3D_SAFE_RELEASE(pOffscreenTex);
	if (pOffscreenTexSRV)
		D3D_SAFE_RELEASE(pOffscreenTexSRV);
	if (pVsConstantBuffer)
		D3D_SAFE_RELEASE(pVsConstantBuffer);

	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = Width;
	scd.BufferDesc.Height = Height;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (!dev)
	{
		hr = D3D11CreateDevice(
			0,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			D3D11_CREATE_DEVICE_DEBUG,
			0,
			0,
			D3D11_SDK_VERSION,
			&dev,
			0,
			&devcon
		);

		if (FAILED(hr))
		{
			D3D_SAFE_RELEASE(dev);
			D3D_SAFE_RELEASE(devcon);
			return false;
		}
	}

	IDXGIDevice* dxgiDevice = 0;
	dev->QueryInterface(IID_IDXGIDevice, (void**)&dxgiDevice);
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(IID_IDXGIAdapter, (void**)&dxgiAdapter);
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(IID_IDXGIFactory, (void**)&dxgiFactory);
	hr = dxgiFactory->CreateSwapChain(dev, &scd, &swapchain);

	if (FAILED(hr))
	{
		D3D_SAFE_RELEASE(swapchain);
		return false;
	}

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// Debug stuff, not actually necessary
	ID3D11InfoQueue *d3dInfoQueue = nullptr;
	dev->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue);

	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
	//

	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;

	devcon->RSSetViewports(1, &viewport);

	SimpleVertex v1 = { DirectX::XMFLOAT3(-1.f,-3.f,1.f) };
	SimpleVertex v2 = { DirectX::XMFLOAT3(-1.f,1.f,1.f) };
	SimpleVertex v3 = { DirectX::XMFLOAT3(3.f,1.f,1.f) };

	SimpleVertex vertices[3] = { v1,v2,v3 };

	if (!pVBuffer)
	{
		// create the vertex buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SimpleVertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bd, NULL, &pVBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices));
		devcon->Unmap(pVBuffer, NULL);
	}
	
	if (!pVS || !pPS)
	{
		// load and compile the two shaders
		unsigned flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;

		if (!CompileVertexShader(flags))
			return false;

		if (!CompilePixelShader(flags))
			return false;


		// set the shader objects
		devcon->VSSetShader(pVS, 0, 0);
		devcon->PSSetShader(pPS, 0, 0);
	}

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 0, -5, 0), DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 1, 0, 0));
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), Width / Height, 0.1f, 10.0f);

	DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(V, P);

	DirectX::XMVECTOR Det;
	DirectX::XMMATRIX VPInv = DirectX::XMMatrixInverse(&Det, VP);


	VsConstantBuffer VsConstData;
	VsConstData.CamPos = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMStoreFloat4x4(&VsConstData.ViewProj, VP);
	DirectX::XMStoreFloat4x4(&VsConstData.ViewProjInv, VPInv);

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(VsConstantBuffer);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &VsConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	hr = dev->CreateBuffer(&cbDesc, &InitData,
		&pVsConstantBuffer);

	if (FAILED(hr))
		return false;

	// Create offscreen texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	hr = dev->CreateTexture2D(&desc, NULL, &pOffscreenTex);
	if (FAILED(hr))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = dev->CreateShaderResourceView(pOffscreenTex, &srvDesc, &pOffscreenTexSRV);

	if (FAILED(hr))
		return false;

	if (!pOffScreenSampler)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = dev->CreateSamplerState(&sampDesc, &pOffScreenSampler);

		if (FAILED(hr))
			return false;
	}

	return true;
}

bool CompileVertexShader(unsigned flags)
{
	std::string shaderSource = Cef3D::Cef3DFileSystem::Get().ReadFile(Cef3D::Cef3DPaths::Shaders() + "/FullscreenTriangle.hlsl");
	ID3D10Blob *VS;
	ID3DBlob* errorMsgs = 0;

	//(const char*)errorMsgs->GetBufferPointer(),(unsigned)errorMsgs->GetBufferSize() - 1

	HRESULT hr = D3DCompile(shaderSource.c_str(), shaderSource.length(), "", 0, 0,
		"fs_triangle_vs", "vs_5_0", flags, 0, &VS, &errorMsgs);

	if (FAILED(hr))
		return false;

	hr = dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	if (FAILED(hr))
		return false;

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = dev->CreateInputLayout(ied, 1, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	if (FAILED(hr))
		return false;
	devcon->IASetInputLayout(pLayout);

	return true;
}

bool CompilePixelShader(unsigned flags)
{
	std::string shaderSource = Cef3D::Cef3DFileSystem::Get().ReadFile(Cef3D::Cef3DPaths::Shaders() + "/FullscreenTriangle.hlsl");
	ID3D10Blob *VS;
	ID3DBlob* errorMsgs = 0;

	HRESULT hr = D3DCompile(shaderSource.c_str(), shaderSource.length(), "", 0, 0,
		"fs_triangle_ps", "ps_5_0", flags, 0, &VS, &errorMsgs);

	if (FAILED(hr))
		return false;

	// encapsulate both shaders into shader objects
	hr = dev->CreatePixelShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pPS);

	if (FAILED(hr))
		return false;

	return true;
}

void Frame()
{
	if (!devcon)
		return;

	float clearColor[4] = { 0,0,0,0 };
	devcon->ClearRenderTargetView(backbuffer, clearColor);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;


	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->VSSetConstantBuffers(0, 1, &pVsConstantBuffer);

	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind offscreen texture
	devcon->PSSetShaderResources(0, 1, &pOffscreenTexSRV);
	// Bind the sampler
	devcon->PSSetSamplers(0, 1, &pOffScreenSampler);

	devcon->Draw(3, 0);

	swapchain->Present(1, 0);
}

void Cleanup()
{
	if (!swapchain)
		return;

	swapchain->SetFullscreenState(FALSE, NULL);

	D3D_SAFE_RELEASE(pOffScreenSampler);
	D3D_SAFE_RELEASE(pOffscreenTex);
	D3D_SAFE_RELEASE(pOffscreenTexSRV);
	D3D_SAFE_RELEASE(pVsConstantBuffer);
	D3D_SAFE_RELEASE(pLayout);
	D3D_SAFE_RELEASE(pVS);
	D3D_SAFE_RELEASE(pPS);
	D3D_SAFE_RELEASE(pVBuffer);
	D3D_SAFE_RELEASE(swapchain);
	D3D_SAFE_RELEASE(backbuffer);
	D3D_SAFE_RELEASE(dev);
	D3D_SAFE_RELEASE(devcon);
}

void PumpMessageLoop()
{
	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		Cef3D_PumpMessageLoop(true);
		Frame();
	}
}

/* Input helpers for Windows */
int GetMouseModifiers(WPARAM wparam)
{
	int modifiers = 0;
	if (wparam & MK_CONTROL)
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (wparam & MK_SHIFT)
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (IsKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;
	if (wparam & MK_LBUTTON)
		modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
	if (wparam & MK_MBUTTON)
		modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
	if (wparam & MK_RBUTTON)
		modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	return modifiers;
}

int GetKeyboardModifiers(WPARAM wparam, LPARAM lparam)
{
	int modifiers = 0;
	if (IsKeyDown(VK_SHIFT))
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (IsKeyDown(VK_CONTROL))
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (IsKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;

	switch (wparam) {
	case VK_RETURN:
		if ((lparam >> 16) & KF_EXTENDED)
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		if (!((lparam >> 16) & KF_EXTENDED))
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_NUMLOCK:
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
	case VK_DIVIDE:
	case VK_MULTIPLY:
	case VK_SUBTRACT:
	case VK_ADD:
	case VK_DECIMAL:
	case VK_CLEAR:
		modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_SHIFT:
		if (IsKeyDown(VK_LSHIFT))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RSHIFT))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_CONTROL:
		if (IsKeyDown(VK_LCONTROL))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RCONTROL))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_MENU:
		if (IsKeyDown(VK_LMENU))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (IsKeyDown(VK_RMENU))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_LWIN:
		modifiers |= EVENTFLAG_IS_LEFT;
		break;
	case VK_RWIN:
		modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	}
	return modifiers;
}

bool IsKeyDown(WPARAM wparam) {
	return (GetKeyState(wparam) & 0x8000) != 0;
}

