//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 1.5.2017
//---------------------------------------------------------------------------
#include "Cef3DPCH.h"
#include "Direct3D11Renderer.h"

Cef3DDirect3D11Renderer::Cef3DDirect3D11Renderer()
	: Vsync(true)
{

}


bool Cef3DDirect3D11Renderer::Init(Cef3DSampleWindow * window)
{
	// Release old ones
	if (SwapChain)
		D3D_SAFE_RELEASE(SwapChain);
	if (Backbuffer)
		D3D_SAFE_RELEASE(Backbuffer);
	if (OffscreenTex)
		D3D_SAFE_RELEASE(OffscreenTex);
	if (OffscreenTexSRV)
		D3D_SAFE_RELEASE(OffscreenTexSRV);
	if (VSConstantBuffer)
		D3D_SAFE_RELEASE(VSConstantBuffer);

	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = window->GetWidth();
	scd.BufferDesc.Height = window->GetHeight();
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window->GetNativeHandle();
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (!Device)
	{
		hr = D3D11CreateDevice(
			0,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			D3D11_CREATE_DEVICE_DEBUG,
			0,
			0,
			D3D11_SDK_VERSION,
			&Device,
			0,
			&DeviceContext
		);

		if (FAILED(hr))
		{
			D3D_SAFE_RELEASE(Device);
			D3D_SAFE_RELEASE(DeviceContext);
			return false;
		}
	}

	IDXGIDevice* dxgiDevice = 0;
	Device->QueryInterface(IID_IDXGIDevice, (void**)&dxgiDevice);
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(IID_IDXGIAdapter, (void**)&dxgiAdapter);
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(IID_IDXGIFactory, (void**)&dxgiFactory);
	hr = dxgiFactory->CreateSwapChain(Device, &scd, &SwapChain);

	if (FAILED(hr))
	{
		D3D_SAFE_RELEASE(SwapChain);
		return false;
	}

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// Debug stuff, not actually necessary
	ID3D11InfoQueue *d3dInfoQueue = nullptr;
	Device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue);

	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
	d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
	//

	ID3D11Texture2D *pBackbuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackbuffer);

	Device->CreateRenderTargetView(pBackbuffer, NULL, &Backbuffer);
	pBackbuffer->Release();

	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = window->GetWidth();
	Viewport.Height = window->GetHeight();

	Window = window;

	return true;
}

bool Cef3DDirect3D11Renderer::InitResources()
{
	SimpleVertex v1 = { DirectX::XMFLOAT3(-1.f,-3.f,1.f) };
	SimpleVertex v2 = { DirectX::XMFLOAT3(-1.f,1.f,1.f) };
	SimpleVertex v3 = { DirectX::XMFLOAT3(3.f,1.f,1.f) };

	SimpleVertex vertices[3] = { v1,v2,v3 };

	HRESULT hr = S_OK;

	if (!VertexBuffer)
	{
		// create the vertex buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(SimpleVertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Device->CreateBuffer(&bd, NULL, &VertexBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices));
		DeviceContext->Unmap(VertexBuffer, NULL);
	}

	if (!FSTriangleVS || !FSTrianglePS)
	{
		// load and compile the two shaders
		unsigned flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;

		if (!CompileFsTriangleVertexShader(flags))
			return false;

		if (!CompileFsTrianglePixelShader(flags))
			return false;
	}

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 0, -5, 0), DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 1, 0, 0));
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), Window->GetWidth() / Window->GetHeight(), 0.1f, 10.0f);

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
	hr = Device->CreateBuffer(&cbDesc, &InitData,
		&VSConstantBuffer);

	if (FAILED(hr))
		return false;

	// Create offscreen texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.Width = Window->GetWidth();
	desc.Height = Window->GetHeight();
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	hr = Device->CreateTexture2D(&desc, NULL, &OffscreenTex);
	if (FAILED(hr))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = Device->CreateShaderResourceView(OffscreenTex, &srvDesc, &OffscreenTexSRV);

	if (FAILED(hr))
		return false;

	if (!OffscreenPSSampler)
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
		hr = Device->CreateSamplerState(&sampDesc, &OffscreenPSSampler);

		if (FAILED(hr))
			return false;
	}

	return true;
}

void Cef3DDirect3D11Renderer::Render()
{
	/* 
		Note that this is a sample framework, ideally you shouldn't set these states every frame.
		Only thing that is constantly changing in our examples are the offscreen texture so everything else could be not called at all throughout the application.
	*/
	// Clear
	float clearColor[4] = { 0,0,0,0 };
	DeviceContext->ClearRenderTargetView(Backbuffer, clearColor);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	// Set render target and viewport
	DeviceContext->OMSetRenderTargets(1, &Backbuffer, NULL);
	DeviceContext->RSSetViewports(1, &Viewport);

	// Set input layout
	DeviceContext->IASetInputLayout(FSTriangleIL);

	// Set vertex buffers and constant buffers (uniforms)
	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	DeviceContext->VSSetConstantBuffers(0, 1, &VSConstantBuffer);

	// Set primitive topology
	DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shaders
	DeviceContext->VSSetShader(FSTriangleVS, 0, 0);
	DeviceContext->PSSetShader(FSTrianglePS, 0, 0);

	// Bind offscreen texture
	DeviceContext->PSSetShaderResources(0, 1, &OffscreenTexSRV);
	// Bind the sampler
	DeviceContext->PSSetSamplers(0, 1, &OffscreenPSSampler);

	// Invoke the draw
	DeviceContext->Draw(3, 0);

	Present();
}

void Cef3DDirect3D11Renderer::Present()
{
	SwapChain->Present(Vsync ? 1 : 0, 0);
}

void Cef3DDirect3D11Renderer::Shutdown()
{
	SwapChain->SetFullscreenState(FALSE, NULL);

	D3D_SAFE_RELEASE(OffscreenPSSampler);
	D3D_SAFE_RELEASE(OffscreenTex);
	D3D_SAFE_RELEASE(OffscreenTexSRV);
	D3D_SAFE_RELEASE(VSConstantBuffer);
	D3D_SAFE_RELEASE(FSTriangleIL);
	D3D_SAFE_RELEASE(FSTriangleVS);
	D3D_SAFE_RELEASE(FSTrianglePS);
	D3D_SAFE_RELEASE(VertexBuffer);
	D3D_SAFE_RELEASE(SwapChain);
	D3D_SAFE_RELEASE(Backbuffer);
	D3D_SAFE_RELEASE(DeviceContext);
	D3D_SAFE_RELEASE(Device);
}

void Cef3DDirect3D11Renderer::UpdateOffscreenTexture(const void * buffer, int width, int height)
{
	unsigned rowSize = width * 4;
	unsigned rowStart = 0;
	unsigned char* src = (unsigned char*)buffer;

	// Update the d3d11 texture
	D3D11_MAPPED_SUBRESOURCE mappedData;
	mappedData.pData = 0;

	HRESULT hr = DeviceContext->Map(OffscreenTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	if (FAILED(hr) || !mappedData.pData)
		return;

	for (int row = 0; row < height; ++row)
		memcpy((unsigned char*)mappedData.pData + row * mappedData.RowPitch + rowStart, src + row * rowSize, rowSize);

	DeviceContext->Unmap(OffscreenTex, 0);
}

bool Cef3DDirect3D11Renderer::Resize(int width, int height)
{
	Window->Resize(width, height);
	return Init(Window);
}

bool Cef3DDirect3D11Renderer::CompileFsTriangleVertexShader(unsigned flags)
{
	std::string shaderSource = Cef3D::Cef3DFileSystem::Get().ReadFile(Cef3D::Cef3DPaths::Shaders() + "/FullscreenTriangle.hlsl");

	ID3DBlob* vsBlob = 0;
	if (!CompileShader(Vertex, flags, "fs_triangle_vs", shaderSource, &vsBlob))
		return false;

	HRESULT hr = Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &FSTriangleVS);
	if (FAILED(hr))
		return false;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = Device->CreateInputLayout(ied, 1, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &FSTriangleIL);
	if (FAILED(hr))
		return false;

	return true;
}

bool Cef3DDirect3D11Renderer::CompileFsTrianglePixelShader(unsigned flags)
{
	std::string shaderSource = Cef3D::Cef3DFileSystem::Get().ReadFile(Cef3D::Cef3DPaths::Shaders() + "/FullscreenTriangle.hlsl");

	ID3DBlob* psBlob = 0;
	if (!CompileShader(Pixel, flags, "fs_triangle_ps", shaderSource, &psBlob))
		return false;

	HRESULT hr = Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &FSTrianglePS);

	if (FAILED(hr))
		return false;
	return true;
}

bool Cef3DDirect3D11Renderer::CompileShader(ShaderType type, unsigned flags, const char* entryPoint, const std::string& source, ID3D10Blob** outBlob)
{
	ID3DBlob* errorMsgs = 0;
	HRESULT hr = D3DCompile(source.c_str(), source.length(), "", 0, 0,
		entryPoint, type == Vertex ? "vs_5_0" : "ps_5_0", flags, 0, outBlob, &errorMsgs);

	if (FAILED(hr))
		return false;
	return true;
}
