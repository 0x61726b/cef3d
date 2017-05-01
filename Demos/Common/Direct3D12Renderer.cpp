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
#include "Direct3D12Renderer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Cef3DDirect3D12Renderer::Cef3DDirect3D12Renderer()
	: Cef3DSampleRenderer(RendererType::Direct3D12), Vsync(true)
{

}



bool Cef3DDirect3D12Renderer::Init(Cef3DSampleWindow * window)
{
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;

	hr = D3D12CreateDevice(NULL, featureLevel, __uuidof(ID3D12Device), (void**)&Device);
	if (FAILED(hr))
		return false;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	hr = Device->CreateCommandQueue(&commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue);
	if (FAILED(hr))
		return false;

	IDXGIFactory4* factory;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&factory);
	if (FAILED(hr))
		return false;

	IDXGIAdapter* adapter;
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return false;

	IDXGIOutput* adapterOutput;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
		return false;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Height = window->GetWidth();
	swapChainDesc.BufferDesc.Width = window->GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = window->GetNativeHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	IDXGISwapChain* swapChain;
	hr = factory->CreateSwapChain(CommandQueue, &swapChainDesc, &swapChain);
	if (FAILED(hr))
		return false;

	hr = swapChain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&SwapChain);
	if (FAILED(hr))
		return false;

	swapChain = 0;
	factory->Release();
	factory = 0;

	D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc;
	ZeroMemory(&renderTargetViewHeapDesc, sizeof(renderTargetViewHeapDesc));

	renderTargetViewHeapDesc.NumDescriptors = 1;
	renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	renderTargetViewHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = Device->CreateDescriptorHeap(&renderTargetViewHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&RenderTargetViewHeap);
	if (FAILED(hr))
		return false;

	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	renderTargetViewHandle = RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	hr = SwapChain->GetBuffer(0, __uuidof(ID3D12Resource), (void**)&BackBufferRenderTarget);
	if (FAILED(hr))
		return false;

	Device->CreateRenderTargetView(BackBufferRenderTarget, NULL, renderTargetViewHandle);

	hr = Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator);
	if (FAILED(hr))
		return false;

	hr = Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList);
	if (FAILED(hr))
		return false;

	hr = CommandList->Close();
	if (FAILED(hr))
		return false;

	hr = Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Fence);

	FenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);

	FenceValue = 1;

	return true;
}


bool Cef3DDirect3D12Renderer::InitResources()
{
	return true;
}


void Cef3DDirect3D12Renderer::Render()
{
	HRESULT hr;
	ID3D12CommandList* ppCommandLists[1];

	hr = CommandAllocator->Reset();
	if (FAILED(hr))
		return;

	hr = CommandList->Reset(CommandAllocator, PipelineState);
	if (FAILED(hr))
		return;

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = BackBufferRenderTarget;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	CommandList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	renderTargetViewHandle = RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	CommandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, NULL);

	float clearColor[4] = { 0,0,0,0 };
	CommandList->ClearRenderTargetView(renderTargetViewHandle, clearColor, 0, NULL);

	// 
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	CommandList->ResourceBarrier(1, &barrier);

	hr = CommandList->Close();
	if (FAILED(hr))
		return;

	ppCommandLists[0] = CommandList;

	CommandQueue->ExecuteCommandLists(1, ppCommandLists);
}


void Cef3DDirect3D12Renderer::Present()
{
	SwapChain->Present(Vsync ? 1 : 0, 0);

	unsigned long long fenceToWaitFor;
	fenceToWaitFor = FenceValue;
	CommandQueue->Signal(Fence, fenceToWaitFor);

	FenceValue++;

	if (Fence->GetCompletedValue() < fenceToWaitFor)
	{
		Fence->SetEventOnCompletion(fenceToWaitFor, FenceEvent);
		WaitForSingleObject(FenceEvent, INFINITE);
	}
}

void Cef3DDirect3D12Renderer::Shutdown()
{
	if (SwapChain)
	{
		SwapChain->SetFullscreenState(false, NULL);
	}

	// Close the object handle to the fence event.
	CloseHandle(FenceEvent);

	D3D_SAFE_RELEASE(Fence);
	D3D_SAFE_RELEASE(PipelineState);
	D3D_SAFE_RELEASE(CommandList);
	D3D_SAFE_RELEASE(CommandAllocator);
	D3D_SAFE_RELEASE(BackBufferRenderTarget);
	D3D_SAFE_RELEASE(RenderTargetViewHeap);
	D3D_SAFE_RELEASE(SwapChain);
	D3D_SAFE_RELEASE(CommandQueue);
	D3D_SAFE_RELEASE(Device);
}


void Cef3DDirect3D12Renderer::UpdateOffscreenTexture(const void * buffer, int width, int height)
{
}


bool Cef3DDirect3D12Renderer::Resize(int width, int height)
{
	return true;
}


bool Cef3DDirect3D12Renderer::CompileFsTriangleVertexShader(unsigned flags)
{
	return true;
}

bool Cef3DDirect3D12Renderer::CompileFsTrianglePixelShader(unsigned flags)
{
	return true;
}


bool Cef3DDirect3D12Renderer::CompileShader(ShaderType type, unsigned flags, const char* entryPoint, const std::string& source, ID3D10Blob** outBlob)
{
	return true;
}