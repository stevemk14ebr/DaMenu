#pragma once

#include "Menu/Rendering/DXTKFonts.h"
#include <d3d11.h>
#include <directxmath.h>
#include "DXTK/Inc/PrimitiveBatch.h"
#include "DXTK/Inc/VertexTypes.h"
#include <Effects.h>
#include <DirectXColors.h>
#include "DXTK/Inc/SpriteFont.h"
#include "DXTK/Inc/CommonStates.h"
#include <codecvt>
#include <locale>
#include "DXTK/Inc/SimpleMath.h"
#include "DXTK/Inc/Effects.h"
#include <atlbase.h>
#pragma comment(lib,"DirectXTK")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

class DXTKRenderer :public RenderInterface
{
public:
	virtual void DrawLineBox(const Vector2f& Position, const Vector2f& Size, const Color& color) override;
	virtual void DrawLineCircle(const Vector2f& Position,const float Radius, const Color& color) override;
	virtual void DrawLine(const Vector2f& Point1, const Vector2f& P2, const Color& color) override;
	virtual void DrawFilledBox(const Vector2f& Position, const Vector2f& Size, const Color& color) override;
	virtual void DrawFilledCircle(const Vector2f& Position, float Radius, const Color& color) override;
	virtual void RenderText(const Vector2f& Position, const Color& color, const char* format, ...) override;
	virtual Vector2f MeasureString(const char* format, ...) override;
	virtual HRESULT InitWindow(const wchar_t* WindowName,const HINSTANCE hInstance, const Vector2f& Size) override;
	virtual HRESULT Init() override;
	virtual ~DXTKRenderer();
	virtual void PreFrame() override;
	virtual void Present() override;
	virtual void BeginLine() override;
	virtual void EndLine() override;
	virtual void BeginText() override;
    virtual	void EndText() override;
	HWND GetWindowHWND();
private:
	HWND m_hWnd;
	D3D_DRIVER_TYPE m_DriverType;
	CComPtr<IDXGISwapChain> m_SwapChain;
	CComPtr<ID3D11Device> m_Device;
	CComPtr<ID3D11DeviceContext> m_DeviceContext;

	D3D_FEATURE_LEVEL    m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	CComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	CComPtr<ID3D11Texture2D> m_DepthStencil;
	CComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	CComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	CComPtr<ID3D11InputLayout> m_BatchInputLayout;

	CComPtr<ID3D11BlendState> m_AlphaBlendState;
	CComPtr<ID3D11BlendState> m_NoPremultiplied;

	DirectX::SimpleMath::Matrix m_ViewMatrix;
	DirectX::SimpleMath::Matrix m_ProjectionMatrix;
	DirectX::SimpleMath::Matrix m_WorldMatrix;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>    m_Batch;
	std::unique_ptr<DirectX::BasicEffect> m_BatchEffect;
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_Font;
};

void DXTKRenderer::DrawLineBox(const Vector2f& Position, const Vector2f& Size, const Color& color)
{
	DirectX::VertexPositionColor Verts[5];
	DirectX::SimpleMath::Vector4 DXColor = DirectX::SimpleMath::Vector4(color.R, color.G, color.B, color.A);
	DirectX::VertexPositionColor TopLeft(DirectX::XMFLOAT3(Position.x, Position.y, 0.0f), DXColor);
	DirectX::VertexPositionColor TopRight(DirectX::XMFLOAT3(Position.x + Size.x, Position.y, 0.0f), DXColor);
	DirectX::VertexPositionColor BottomLeft(DirectX::XMFLOAT3(Position.x, Position.y + Size.y, 0.0f), DXColor);
	DirectX::VertexPositionColor BottomRight(DirectX::XMFLOAT3(Position.x + Size.x, Position.y + Size.y, 0.0f), DXColor);

	Verts[0] = TopLeft;
	Verts[1] = TopRight;
	Verts[2] = BottomRight;
	Verts[3] = BottomLeft;
	Verts[4] = TopLeft;
	m_Batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, Verts, 5);
}

void DXTKRenderer::DrawLineCircle(const Vector2f& Position,const float Radius, const Color& color)
{
}

void DXTKRenderer::DrawLine(const Vector2f& Point1, const Vector2f& Point2, const Color& color)
{
	DirectX::SimpleMath::Vector4 DXColor = DirectX::SimpleMath::Vector4(color.R, color.G, color.B, color.A);
	DirectX::VertexPositionColor v1(DirectX::XMFLOAT3(Point1.x,Point1.y,0.0f),DXColor);
	DirectX::VertexPositionColor v2(DirectX::XMFLOAT3(Point2.x, Point2.y, 0.0f), DXColor);
	m_Batch->DrawLine(v1, v2);
}

void DXTKRenderer::DrawFilledBox(const Vector2f& Position, const Vector2f& Size, const Color& color)
{
	DirectX::SimpleMath::Vector4 DXColor = DirectX::SimpleMath::Vector4(color.R, color.G, color.B, color.A);
	DirectX::VertexPositionColor TopLeft(DirectX::XMFLOAT3(Position.x, Position.y, 0.0f), DXColor);
	DirectX::VertexPositionColor TopRight(DirectX::XMFLOAT3(Position.x + Size.x, Position.y, 0.0f), DXColor);
	DirectX::VertexPositionColor BottomLeft(DirectX::XMFLOAT3(Position.x, Position.y + Size.y, 0.0f), DXColor);
	DirectX::VertexPositionColor BottomRight(DirectX::XMFLOAT3(Position.x + Size.x, Position.y + Size.y, 0.0f), DXColor);
	m_Batch->DrawQuad(TopLeft, TopRight, BottomRight, BottomLeft);
}

void DXTKRenderer::DrawFilledCircle(const Vector2f& Position, float Radius,const Color& FillColor)
{

}

void DXTKRenderer::RenderText(const Vector2f& Position,const Color& color,const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char Buffer[1024];
	vsnprintf(Buffer, 1024, format, args);
	va_end(args);

	DirectX::SimpleMath::Vector4 DXColor{ color.R, color.G, color.B, color.A };
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring WStr = converter.from_bytes(Buffer);
	m_Font->DrawString(m_SpriteBatch.get(), WStr.c_str(), DirectX::XMFLOAT2(Position.x, Position.y),DXColor);
}

Vector2f DXTKRenderer::MeasureString(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char Buffer[1024];
	vsnprintf(Buffer, 1024, format, args);
	va_end(args);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring WStr = converter.from_bytes(Buffer);
	DirectX::SimpleMath::Vector2 Size = m_Font->MeasureString(WStr.c_str());
	return Vector2f(Size.x, Size.y);
}

HRESULT DXTKRenderer::InitWindow(const wchar_t* WindowName, const HINSTANCE hInstance, const Vector2f& Size)
{
	WNDCLASSEX wcex{ 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc =WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_SAMPLE1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WindowName;
	//wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SAMPLE1);
	if (!RegisterClassEx(&wcex))
	{
		printf("Failed to register\n");
		return E_FAIL;
	}

	m_hWnd = CreateWindowW(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,Size.x,Size.y, nullptr, nullptr, 
		hInstance,nullptr);
	if (!m_hWnd)
	{
		printf("Failed to Create\n");
		return E_FAIL;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	return S_OK;
}

HRESULT DXTKRenderer::Init()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, m_DriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, &m_FeatureLevel, &m_DeviceContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_Device->CreateRenderTargetView(pBackBuffer, nullptr, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
	if (FAILED(hr))
		return hr;
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);

	m_Batch.reset(new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(m_DeviceContext));
	m_BatchEffect.reset(new DirectX::BasicEffect(m_Device));
	m_BatchEffect->SetVertexColorEnabled(true);

	void const* ShaderByteCode;
	size_t ByteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&ShaderByteCode, &ByteCodeLength);
	hr = m_Device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		ShaderByteCode, ByteCodeLength, &m_BatchInputLayout);

	if (FAILED(hr))
	{
		printf("CreateInputLayout Failed\n");
		return hr;
	}
	float BlendFactor[] = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::CommonStates States(m_Device);
	m_AlphaBlendState = States.AlphaBlend();
	m_NoPremultiplied = States.NonPremultiplied();
	m_DeviceContext->OMSetBlendState(m_AlphaBlendState, BlendFactor, 0xffffffff);

	m_ViewMatrix = DirectX::XMMatrixIdentity();
	m_WorldMatrix = DirectX::XMMatrixIdentity();
	m_ProjectionMatrix = DirectX::XMMatrixOrthographicOffCenterRH(0.0f, width, height,
		0.0f, 0.0f, 1.0f);
	m_BatchEffect->SetView(m_ViewMatrix);
	m_BatchEffect->SetWorld(m_WorldMatrix);
	m_BatchEffect->SetProjection(m_ProjectionMatrix);

	m_SpriteBatch.reset(new DirectX::SpriteBatch(m_DeviceContext));
	m_Font.reset(new DirectX::SpriteFont(m_Device, (uint8_t*)ConstantiaFont,sizeof(ConstantiaFont)));
}

DXTKRenderer::~DXTKRenderer()
{
	//Smart pointers release stuff automatically
}

void DXTKRenderer::BeginLine()
{
	m_BatchEffect->Apply(m_DeviceContext);
	m_DeviceContext->IASetInputLayout(m_BatchInputLayout);
	m_Batch->Begin();
}

void DXTKRenderer::EndLine()
{
	m_Batch->End();
}

void DXTKRenderer::BeginText()
{
	m_SpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_NoPremultiplied);
}

void DXTKRenderer::EndText()
{
	m_SpriteBatch->End();
}

void DXTKRenderer::PreFrame()
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, DirectX::Colors::LightSkyBlue);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXTKRenderer::Present()
{
	m_SwapChain->Present(0, 0);
}

HWND DXTKRenderer::GetWindowHWND()
{
	return m_hWnd;
}