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

//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3dcompiler.lib")

namespace D3D12Private
{
	std::vector<UINT8> GenerateTextureData(int TextureWidth, int TextureHeight, int TexturePixelSize)
	{
		const UINT rowPitch = TextureWidth * TexturePixelSize;
		const UINT cellPitch = rowPitch >> 3;		// The width of a cell in the checkboard texture.
		const UINT cellHeight = TextureWidth >> 3;	// The height of a cell in the checkerboard texture.
		const UINT textureSize = rowPitch * TextureHeight;

		std::vector<UINT8> data(textureSize);
		UINT8* pData = &data[0];

		for (UINT n = 0; n < textureSize; n += TexturePixelSize)
		{
			UINT x = n % rowPitch;
			UINT y = n / rowPitch;
			UINT i = x / cellPitch;
			UINT j = y / cellHeight;

			if (i % 2 == j % 2)
			{
				pData[n] = 0x00;		// R
				pData[n + 1] = 0x00;	// G
				pData[n + 2] = 0x00;	// B
				pData[n + 3] = 0xff;	// A
			}
			else
			{
				pData[n] = 0xff;		// R
				pData[n + 1] = 0xff;	// G
				pData[n + 2] = 0xff;	// B
				pData[n + 3] = 0xff;	// A
			}
		}

		return data;
	}
}

Cef3DDirect3D12Renderer::Cef3DDirect3D12Renderer()
	: Cef3DSampleRenderer(RendererType::Direct3D12), Vsync(true),
	PipelineState(0)
{

}



bool Cef3DDirect3D12Renderer::Init(Cef3DSampleWindow * window)
{
	Window = window;

	//HRESULT hr = S_OK;

	//UINT dxgiFactoryFlags = 0;

	//ID3D12Debug* debugController;
	//if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	//{
	//	debugController->EnableDebugLayer();

	//	// Enable additional debug layers.
	//	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	//}

	//D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	//hr = D3D12CreateDevice(NULL, featureLevel, __uuidof(ID3D12Device), (void**)&Device);
	//if (FAILED(hr))
	//	return false;

	//D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	//ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	//commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	//hr = Device->CreateCommandQueue(&commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue);
	//if (FAILED(hr))
	//	return false;

	//IDXGIFactory4* factory;
	//hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
	//if (FAILED(hr))
	//	return false;

	//IDXGIAdapter* adapter;
	//hr = factory->EnumAdapters(0, &adapter);
	//if (FAILED(hr))
	//	return false;

	//IDXGIOutput* adapterOutput;
	//hr = adapter->EnumOutputs(0, &adapterOutput);
	//if (FAILED(hr))
	//	return false;

	//DXGI_SWAP_CHAIN_DESC swapChainDesc;
	//ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//swapChainDesc.BufferCount = 2;
	//swapChainDesc.BufferDesc.Width = window->GetWidth();
	//swapChainDesc.BufferDesc.Height = window->GetHeight();
	//swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//swapChainDesc.OutputWindow = window->GetNativeHandle();
	//swapChainDesc.Windowed = true;
	//swapChainDesc.SampleDesc.Count = 1;
	//swapChainDesc.SampleDesc.Quality = 0;
	//swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	//swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	//IDXGISwapChain* swapChain;
	//hr = factory->CreateSwapChain(CommandQueue, &swapChainDesc, &swapChain);
	//if (FAILED(hr))
	//	return false;

	//hr = swapChain->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&SwapChain);
	//if (FAILED(hr))
	//	return false;

	//swapChain = 0;
	//factory->Release();
	//factory = 0;

	//D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc;
	//ZeroMemory(&renderTargetViewHeapDesc, sizeof(renderTargetViewHeapDesc));

	//renderTargetViewHeapDesc.NumDescriptors = 2;
	//renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//renderTargetViewHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//hr = Device->CreateDescriptorHeap(&renderTargetViewHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&RenderTargetViewHeap);
	//if (FAILED(hr))
	//	return false;

	//// Constant Buffer Heap
	//D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	//cbvHeapDesc.NumDescriptors = 1;
	//cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//Device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&ConstantBufferHeap));

	//D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	//renderTargetViewHandle = RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	//UINT renderTargetViewDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//hr = SwapChain->GetBuffer(0, __uuidof(ID3D12Resource), (void**)&BackBufferRenderTarget[0]);
	//if (FAILED(hr))
	//	return false;

	//Device->CreateRenderTargetView(BackBufferRenderTarget[0], NULL, renderTargetViewHandle);

	//renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;

	//hr = SwapChain->GetBuffer(1, __uuidof(ID3D12Resource), (void**)&BackBufferRenderTarget[1]);
	//if (FAILED(hr))
	//	return false;

	//Device->CreateRenderTargetView(BackBufferRenderTarget[1], NULL, renderTargetViewHandle);

	//BufferIndex = SwapChain->GetCurrentBackBufferIndex();

	//hr = Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator);
	//if (FAILED(hr))
	//	return false;

	//if (!InitResources())
	//	return false;

	return true;
}


bool Cef3DDirect3D12Renderer::InitResources()
{
	//Viewport.Width = Window->GetWidth();
	//Viewport.Height = Window->GetHeight();
	//Viewport.TopLeftX = 0;
	//Viewport.TopLeftY = 0;

	//ScissorsRect.top = 0;
	//ScissorsRect.bottom = Window->GetHeight();

	//ScissorsRect.left = 0;
	//ScissorsRect.right = Window->GetWidth();


	//// Root Signature
	//D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

	//// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
	//featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	//if (FAILED(Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
	//{
	//	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	//}

	//D3D12_DESCRIPTOR_RANGE1 ranges[2];
	//D3D12_ROOT_PARAMETER1 rootParameters[2];

	//// Contant Buffer
	//ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	//ranges[0].NumDescriptors = 1;
	//ranges[0].BaseShaderRegister = 0;
	//ranges[0].RegisterSpace = 0;
	//ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
	//ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//// Offscreen SRV
	//ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//ranges[1].NumDescriptors = 1;
	//ranges[1].BaseShaderRegister = 0;
	//ranges[1].RegisterSpace = 0;
	//ranges[1].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
	//ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	//rootParameters[0].DescriptorTable.pDescriptorRanges = &ranges[0];

	//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	//rootParameters[1].DescriptorTable.pDescriptorRanges = &ranges[0];

	//// Allow input layout and deny uneccessary access to certain pipeline stages.
	//D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
	//	D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
	//	D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
	//	D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
	//	D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
	//	D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	//D3D12_STATIC_SAMPLER_DESC sampler = {};
	//sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	//sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	//sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	//sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	//sampler.MipLODBias = 0;
	//sampler.MaxAnisotropy = 0;
	//sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	//sampler.MinLOD = 0.0f;
	//sampler.MaxLOD = D3D12_FLOAT32_MAX;
	//sampler.ShaderRegister = 0;
	//sampler.RegisterSpace = 0;
	//sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	//rootSignatureDesc.Desc_1_1.NumParameters = _countof(rootParameters);
	//rootSignatureDesc.Desc_1_1.pParameters = rootParameters;
	//rootSignatureDesc.Desc_1_1.NumStaticSamplers = 1;
	//rootSignatureDesc.Desc_1_1.pStaticSamplers = &sampler;
	//rootSignatureDesc.Desc_1_1.Flags = rootSignatureFlags;

	//ID3DBlob* signature;
	//ID3DBlob* error;

	//HRESULT hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

	//if (FAILED(hr))
	//	return false;
	//hr = Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&RootSig));

	//if (FAILED(hr))
	//	return false;

	//UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	//ID3DBlob* vsBlob = CompileFsTriangleVertexShader(compileFlags);
	//if (!vsBlob)
	//	return false;

	//ID3DBlob* psBlob = CompileFsTrianglePixelShader(compileFlags);
	//if (!psBlob)
	//	return false;

	//D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	//};

	//D3D12_SHADER_BYTECODE vsByteCode;
	//vsByteCode.BytecodeLength = vsBlob->GetBufferSize();
	//vsByteCode.pShaderBytecode = vsBlob->GetBufferPointer();

	//D3D12_SHADER_BYTECODE psByteCode;
	//psByteCode.BytecodeLength = psBlob->GetBufferSize();
	//psByteCode.pShaderBytecode = psBlob->GetBufferPointer();

	//D3D12_RASTERIZER_DESC rasterizerDesc;
	//rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//rasterizerDesc.FrontCounterClockwise = FALSE;
	//rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	//rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	//rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	//rasterizerDesc.DepthClipEnable = TRUE;
	//rasterizerDesc.MultisampleEnable = FALSE;
	//rasterizerDesc.AntialiasedLineEnable = FALSE;
	//rasterizerDesc.ForcedSampleCount = 0;
	//rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//D3D12_BLEND_DESC blendDesc;
	//blendDesc.AlphaToCoverageEnable = false;
	//blendDesc.IndependentBlendEnable = false;

	//const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	//{
	//	FALSE,FALSE,
	//	D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
	//	D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
	//	D3D12_LOGIC_OP_NOOP,
	//	D3D12_COLOR_WRITE_ENABLE_ALL,
	//};
	//for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	//	blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

	//// Describe and create the graphics pipeline state object (PSO).
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	//psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	//psoDesc.pRootSignature = RootSig;
	//psoDesc.VS = vsByteCode;
	//psoDesc.PS = psByteCode;
	//psoDesc.RasterizerState = rasterizerDesc;
	//psoDesc.BlendState = blendDesc;
	//psoDesc.DepthStencilState.DepthEnable = FALSE;
	//psoDesc.DepthStencilState.StencilEnable = FALSE;
	//psoDesc.SampleMask = UINT_MAX;
	//psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//psoDesc.NumRenderTargets = 1;
	//psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//psoDesc.SampleDesc.Count = 1;
	//hr = (Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&PipelineState)));
	//if (FAILED(hr))
	//	return false;

	//hr = Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList);
	//if (FAILED(hr))
	//	return false;

	//hr = CommandList->Close();
	//if (FAILED(hr))
	//	return false;

	//hr = Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&Fence);

	//FenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);

	//FenceValue = 1;

	//SimpleVertex v1 = { DirectX::XMFLOAT3(-1.f,-3.f,1.f) };
	//SimpleVertex v2 = { DirectX::XMFLOAT3(-1.f,1.f,1.f) };
	//SimpleVertex v3 = { DirectX::XMFLOAT3(3.f,1.f,1.f) };

	//SimpleVertex vertices[3] = { v1,v2,v3 };

	//D3D12_HEAP_PROPERTIES heapProps;
	//heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	//heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//heapProps.CreationNodeMask = 1;
	//heapProps.VisibleNodeMask = 1;

	//D3D12_RESOURCE_DESC vBufferResDesc;
	//vBufferResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//vBufferResDesc.Alignment = 0;
	//vBufferResDesc.Width = sizeof(vertices);
	//vBufferResDesc.Height = 1;
	//vBufferResDesc.DepthOrArraySize = 1;
	//vBufferResDesc.MipLevels = 1;
	//vBufferResDesc.Format = DXGI_FORMAT_UNKNOWN;
	//vBufferResDesc.SampleDesc.Count = 1;
	//vBufferResDesc.SampleDesc.Quality = 0;
	//vBufferResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//vBufferResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//hr = Device->CreateCommittedResource(
	//	&heapProps,
	//	D3D12_HEAP_FLAG_NONE,
	//	&vBufferResDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&VertexBuffer));

	//if (FAILED(hr))
	//	return false;

	//UINT8* pVertexDataBegin;
	//D3D12_RANGE readRange;
	//readRange.Begin = 0;
	//readRange.End = 0;

	//hr = VertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
	//memcpy(pVertexDataBegin, vertices, sizeof(vertices));
	//VertexBuffer->Unmap(0, nullptr);

	//// Initialize the vertex buffer view.
	//VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	//VertexBufferView.StrideInBytes = sizeof(SimpleVertex);
	//VertexBufferView.SizeInBytes = sizeof(vertices);

	//D3D12_RESOURCE_DESC cBufferResDesc;
	//cBufferResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//cBufferResDesc.Alignment = 0;
	//cBufferResDesc.Width = 1024 * 64;
	//cBufferResDesc.Height = 1;
	//cBufferResDesc.DepthOrArraySize = 1;
	//cBufferResDesc.MipLevels = 1;
	//cBufferResDesc.Format = DXGI_FORMAT_UNKNOWN;
	//cBufferResDesc.SampleDesc.Count = 1;
	//cBufferResDesc.SampleDesc.Quality = 0;
	//cBufferResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//cBufferResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//// Create constant buffer
	//Device->CreateCommittedResource(
	//	&heapProps,
	//	D3D12_HEAP_FLAG_NONE,
	//	&cBufferResDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&ConstantBuffer));

	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	//cbvDesc.BufferLocation = ConstantBuffer->GetGPUVirtualAddress();
	//cbvDesc.SizeInBytes = (sizeof(VsConstantBuffer) + 255) & ~255;	// CB size is required to be 256-byte aligned.
	//Device->CreateConstantBufferView(&cbvDesc, ConstantBufferHeap->GetCPUDescriptorHandleForHeapStart());

	//// Create SRV heap
	//D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	//srvHeapDesc.NumDescriptors = 1;
	//srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&OffscreenSrvHeap));


	//DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 0, -5, 0), DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 1, 0, 0));
	//DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), Window->GetWidth() / Window->GetHeight(), 0.1f, 10.0f);

	//DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(V, P);

	//DirectX::XMVECTOR Det;
	//DirectX::XMMATRIX VPInv = DirectX::XMMatrixInverse(&Det, VP);

	//VsConstantBuffer VsConstData;
	//VsConstData.CamPos = DirectX::XMFLOAT3(0, 0, 0);
	//DirectX::XMStoreFloat4x4(&VsConstData.ViewProj, VP);
	//DirectX::XMStoreFloat4x4(&VsConstData.ViewProjInv, VPInv);
	//
	//ConstantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&VsConstData));
	//memcpy(&VsConstData, &ConstantBufferData, sizeof(ConstantBufferData));
	//ConstantBuffer->Unmap(0, nullptr);

	//// Create the texture.
	//{
	//	// Describe and create a Texture2D.
	//	D3D12_RESOURCE_DESC textureDesc = {};
	//	textureDesc.MipLevels = 1;
	//	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	textureDesc.Width = Window->GetWidth();
	//	textureDesc.Height = Window->GetHeight();
	//	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	//	textureDesc.DepthOrArraySize = 1;
	//	textureDesc.SampleDesc.Count = 1;
	//	textureDesc.SampleDesc.Quality = 0;
	//	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	//	D3D12_HEAP_PROPERTIES textureHeapProps;
	//	textureHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	//	textureHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//	textureHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//	textureHeapProps.CreationNodeMask = 1;
	//	textureHeapProps.VisibleNodeMask = 1;

	//	hr = (Device->CreateCommittedResource(
	//		&textureHeapProps,
	//		D3D12_HEAP_FLAG_NONE,
	//		&textureDesc,
	//		D3D12_RESOURCE_STATE_COPY_DEST,
	//		nullptr,
	//		IID_PPV_ARGS(&OffscreenTexture)));
	//	
	//	if (FAILED(hr))
	//		return false;

	//	// Copy data to texture
	//	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(OffscreenTexture, 0, 1);

	//	D3D12_RESOURCE_DESC texBufferResDesc;
	//	texBufferResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//	texBufferResDesc.Alignment = 0;
	//	texBufferResDesc.Width = uploadBufferSize;
	//	texBufferResDesc.Height = 1;
	//	texBufferResDesc.DepthOrArraySize = 1;
	//	texBufferResDesc.MipLevels = 1;
	//	texBufferResDesc.Format = DXGI_FORMAT_UNKNOWN;
	//	texBufferResDesc.SampleDesc.Count = 1;
	//	texBufferResDesc.SampleDesc.Quality = 0;
	//	texBufferResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//	texBufferResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//	ID3D12Resource* textureUploadHeap;
	//	// Create the GPU upload buffer.
	//	Device->CreateCommittedResource(
	//		&heapProps, // upload heap
	//		D3D12_HEAP_FLAG_NONE,
	//		&texBufferResDesc,
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&textureUploadHeap));

	//	// Copy data to the intermediate upload heap and then schedule a copy 
	//	// from the upload heap to the Texture2D.
	//	std::vector<UINT8> texture = D3D12Private::GenerateTextureData(256,256,4);

	//	D3D12_SUBRESOURCE_DATA textureData = {};
	//	textureData.pData = &texture[0];
	//	textureData.RowPitch = 256 * 4; // TextureWidth * TextureSize
	//	textureData.SlicePitch = textureData.RowPitch * 256;

	//	// Texture barrier
	//	D3D12_RESOURCE_BARRIER texBarrier;
	//	texBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	texBarrier.Transition.pResource = BackBufferRenderTarget[BufferIndex];
	//	texBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	//	texBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//	texBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//	texBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	UpdateSubresources(CommandList, OffscreenTexture, textureUploadHeap, 0, 0, 1, &textureData);
	//	CommandList->ResourceBarrier(1, &texBarrier);

	//	// Describe and create a SRV for the texture.
	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.Format = textureDesc.Format;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDesc.Texture2D.MipLevels = 1;
	//	Device->CreateShaderResourceView(OffscreenTexture, &srvDesc, OffscreenSrvHeap->GetCPUDescriptorHandleForHeapStart());
	//}

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
	barrier.Transition.pResource = BackBufferRenderTarget[BufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	CommandList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	renderTargetViewHandle = RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	unsigned renderTargetViewDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (BufferIndex == 1)
	{
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}

	CommandList->SetGraphicsRootSignature(RootSig);

	ID3D12DescriptorHeap* ppHeaps[] = { ConstantBufferHeap };
	CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	CommandList->SetGraphicsRootDescriptorTable(0, ConstantBufferHeap->GetGPUDescriptorHandleForHeapStart());

	CommandList->RSSetViewports(1, &Viewport);
	CommandList->RSSetScissorRects(1, &ScissorsRect);

	CommandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, NULL);

	float clearColor[4] = { 0,1,0,0 };
	CommandList->ClearRenderTargetView(renderTargetViewHandle, clearColor, 0, NULL);

	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	CommandList->DrawInstanced(3, 1, 0, 0);

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

	BufferIndex == 0 ? BufferIndex = 1 : BufferIndex = 0;
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
	D3D_SAFE_RELEASE(BackBufferRenderTarget[0]);
	D3D_SAFE_RELEASE(BackBufferRenderTarget[1]);
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


ID3DBlob* Cef3DDirect3D12Renderer::CompileFsTriangleVertexShader(unsigned flags)
{
	std::string shaderSource = Cef3D::Cef3DFileSystem::Get().ReadFile(Cef3D::Cef3DPaths::Shaders() + "/FullscreenTriangle.hlsl");

	ID3DBlob* vsBlob = 0;
	if (!CompileShader(Vertex, flags, "fs_triangle_vs", shaderSource, &vsBlob))
		return 0;
	return vsBlob;
}

ID3DBlob* Cef3DDirect3D12Renderer::CompileFsTrianglePixelShader(unsigned flags)
{
	std::string shaderSource = Cef3D::Cef3DFileSystem::Get().ReadFile(Cef3D::Cef3DPaths::Shaders() + "/FullscreenTriangle.hlsl");

	ID3DBlob* psBlob = 0;
	if (!CompileShader(Pixel, flags, "fs_triangle_ps", shaderSource, &psBlob))
		return 0;
	return psBlob;
}


bool Cef3DDirect3D12Renderer::CompileShader(ShaderType type, unsigned flags, const char* entryPoint, const std::string& source, ID3D10Blob** outBlob)
{
	ID3DBlob* errorMsgs = 0;
	HRESULT hr = D3DCompile(source.c_str(), source.length(), "", 0, 0,
		entryPoint, type == Vertex ? "vs_5_0" : "ps_5_0", flags, 0, outBlob, &errorMsgs);

	if (FAILED(hr))
		return false;
	return true;
}