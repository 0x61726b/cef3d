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

#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_4.h>

#include "Win32Common.h"

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


class Cef3DDirect3D12Renderer : public Cef3DSampleRenderer
{
public:
	Cef3DDirect3D12Renderer();

	// Cef3DSampleRenderer interface methods
	virtual bool Init(Cef3DSampleWindow* window) override;
	virtual bool InitResources() override;
	virtual void Render() override;
	virtual void Present() override;
	virtual void Shutdown() override;
	virtual void UpdateOffscreenTexture(const void* buffer,
		int width,
		int height) override;
	bool Resize(int width, int height) override;



	void SetVsync(bool vsync) { Vsync = vsync; }

private:
	bool CompileFsTriangleVertexShader(unsigned flags);
	bool CompileFsTrianglePixelShader(unsigned flags);

	bool CompileShader(ShaderType type, unsigned flags, const char* entryPoint, const std::string& source, ID3D10Blob** outBlob);

private:
	IDXGISwapChain3* SwapChain;
	ID3D12Device* Device;
	ID3D12CommandQueue* CommandQueue;
	ID3D12DescriptorHeap* RenderTargetViewHeap;
	ID3D12Resource* BackBufferRenderTarget;
	ID3D12CommandAllocator* CommandAllocator;
	ID3D12GraphicsCommandList* CommandList;
	ID3D12PipelineState* PipelineState;
	ID3D12Fence* Fence;
	HANDLE FenceEvent;
	unsigned long long FenceValue;

	//D3D11_VIEWPORT Viewport;

	bool Vsync;
	Cef3DSampleWindow* Window;
};