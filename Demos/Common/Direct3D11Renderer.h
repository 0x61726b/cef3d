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
#include <d3d11.h>
#include <dxgi.h>
#include <d3d9.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

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


class Cef3DDirect3D11Renderer : public Cef3DSampleRenderer
{
public:
	Cef3DDirect3D11Renderer();

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

	bool CompileShader(ShaderType type,unsigned flags, const char* entryPoint,const std::string& source, ID3D10Blob** outBlob);

private:
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11RenderTargetView* Backbuffer;
	ID3D11InputLayout* FSTriangleIL;
	ID3D11VertexShader* FSTriangleVS;
	ID3D11PixelShader* FSTrianglePS;
	ID3D11Buffer* VertexBuffer;                // the pointer to the vertex buffer
	ID3D11Buffer* VSConstantBuffer;
	ID3D11Texture2D* OffscreenTex;
	ID3D11ShaderResourceView* OffscreenTexSRV;
	ID3D11SamplerState* OffscreenPSSampler;

	D3D11_VIEWPORT Viewport;

	bool Vsync;
	Cef3DSampleWindow* Window;
};