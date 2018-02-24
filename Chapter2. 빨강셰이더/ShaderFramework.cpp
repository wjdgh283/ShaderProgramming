//**********************************************************************
//
// ShaderFramework.cpp
// 
// ���̴� ���� ���� C��Ÿ���� �ʰ��� �����ӿ�ũ�Դϴ�.
// (���� ������ �ڵ��Ͻ� ���� ���� �̷��� �����ӿ�ũ��
// �ۼ��Ͻø� �ȵ˴ϴ�. -_-)
//
// Author: Pope Kim
//
//**********************************************************************

#include "ShaderFramework.h"
#include <stdio.h>

#define PI           3.14159265f
#define FOV          (PI/4.0f)							// �þ߰�
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT)		// ȭ���� ��Ⱦ��
#define NEAR_PLANE   1									// ���� ���
#define FAR_PLANE    10000								// ���Ÿ� ���


//----------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------

// D3D ����
LPDIRECT3D9             gpD3D = NULL;				// D3D
LPDIRECT3DDEVICE9       gpD3DDevice = NULL;				// D3D ��ġ

														// ��Ʈ
ID3DXFont*              gpFont = NULL;

// ��
LPD3DXMESH				gpSphere = NULL;

// ���̴�
LPD3DXEFFECT			gpColorShader = NULL;

// �ؽ�ó

// ���α׷� �̸�
const char*				gAppName = "�ʰ��� ���̴� ���� �����ӿ�ũ";

//-----------------------------------------------------------------------
// ���α׷� ������/�޽��� ����
//-----------------------------------------------------------------------

// ������
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// ������ Ŭ������ ����Ѵ�.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		gAppName, NULL };
	RegisterClassEx(&wc);

	// ���α׷� â�� �����Ѵ�.
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindow(gAppName, gAppName,
		style, CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	// Client Rect ũ�Ⱑ WIN_WIDTH, WIN_HEIGHT�� ������ ũ�⸦ �����Ѵ�.
	POINT ptDiff;
	RECT rcClient, rcWindow;

	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd, rcWindow.left, rcWindow.top, WIN_WIDTH + ptDiff.x, WIN_HEIGHT + ptDiff.y, TRUE);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// D3D�� ����� ��� ���� �ʱ�ȭ�Ѵ�.
	if (!InitEverything(hWnd))		//Direct3D �� ��Ÿ �ʱ�ȭ�� �����ϸ� ���� �����ִ� �� �Ұ����ϹǷ�
		PostQuitMessage(1);			//���α׷� ����

									// �޽��� ����
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // �޽����� ������ ������ ������Ʈ�ϰ� ����� �׸���
		{
			PlayDemo();
		}
	}

	UnregisterClass(gAppName, wc.hInstance);
	return 0;
}

// �޽��� ó����
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		ProcessInput(hWnd, wParam);
		break;

	case WM_DESTROY:		//â�� ���� ���� �ʱ�ȭ ���߿� �����ߴ� D3D �ڿ����� �����ϰ� ���α׷�����
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);	//���𿡼� ó������ �ʴ� ������ �޽������� �⺻�޽��� ó���Ⱑ ó��
}

// Ű���� �Է�ó��
void ProcessInput(HWND hWnd, WPARAM keyPress)
{
	switch (keyPress)
	{
		// ESC Ű�� ������ ���α׷��� �����Ѵ�.
	case VK_ESCAPE:
		PostMessage(hWnd, WM_DESTROY, 0L, 0L);
		break;
	}
}

//------------------------------------------------------------
// ���ӷ���
//------------------------------------------------------------
void PlayDemo()
{
	Update();
	RenderFrame();
}

// ���ӷ��� ������Ʈ
void Update()
{
}

//------------------------------------------------------------
// ������
//------------------------------------------------------------

void RenderFrame()
{
	D3DCOLOR bgColour = 0xFF0000FF;	// ������ - �Ķ�

	gpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), bgColour, 1.0f, 0);

	gpD3DDevice->BeginScene();
	{
		RenderScene();				// 3D ��ü���� �׸���.
		RenderInfo();				// ����� ���� ���� ����Ѵ�.
	}
	gpD3DDevice->EndScene();

	//��� �׸��⸦ ���ƴٸ� ����ۿ� ����Ǿ� �ִ� ������ ����� ȭ�鿡 �ѷ��ش�
	gpD3DDevice->Present(NULL, NULL, NULL, NULL);
}


// 3D ��ü���� �׸���.
void RenderScene()
{
	// �� ����� �����.
	D3DXMATRIXA16			matView;
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -200.0f);					//ī�޶���ġ
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);						//�ٶ󺸴� ���� ��ġ
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);						//ī�޶��� ������
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);	//����ĸ����

																// ��������� �����.
	D3DXMATRIXA16			matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);		//������� �����(�������ù�)
																								//�������ù� : D3DXMatrixOrthoLH()
																								// ��������� �����.
	D3DXMATRIXA16			matWorld;
	D3DXMatrixIdentity(&matWorld);

	// ���̴� ������������ ����
	gpColorShader->SetMatrix("gWorldMatrix", &matWorld);
	gpColorShader->SetMatrix("gViewMatrix", &matView);
	gpColorShader->SetMatrix("gProjectionMatrix", &matProjection);

	// ���̴��� �����Ѵ�.
	UINT numPasses = 0;
	gpColorShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			gpColorShader->BeginPass(i);
			{
				// ��ü�� �׸���.
				gpSphere->DrawSubset(0);
			}
			gpColorShader->EndPass();
		}
	}
	gpColorShader->End();
}

// ����� ���� ���� ���. ������ Ű ���� ������ ȭ�鿡 ������
void RenderInfo()
{
	// �ؽ�Ʈ ����
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// �ؽ�Ʈ�� ����� ��ġ
	RECT rct;
	rct.left = 5;
	rct.right = WIN_WIDTH / 3;
	rct.top = 5;
	rct.bottom = WIN_HEIGHT / 3;

	// Ű �Է� ������ ���
	gpFont->DrawText(NULL, "���� �����ӿ�ũ\n\nESC: ��������", -1, &rct, 0, fontColor);
}

//------------------------------------------------------------
// �ʱ�ȭ �ڵ�
//------------------------------------------------------------
bool InitEverything(HWND hWnd)
{
	// D3D�� �ʱ�ȭ
	if (!InitD3D(hWnd))
	{
		return false;
	}

	// ��, ���̴�, �ؽ�ó���� �ε�
	if (!LoadAssets())
	{
		return false;
	}

	// ��Ʈ�� �ε�
	if (FAILED(D3DXCreateFont(gpD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		"Arial", &gpFont)))
		//gpD3DDevice:D3D��ġ, 20:��Ʈ����, 10:��Ʈ�ʺ�, FW_BOLD:����ü, 1:�Ӹʷ���, FALSE:���Ÿ�üX 
		//DEFAULT_CHARSET:�⺻ ���ڼ��̿�,
		//OUT_DEFAULT_PRECIS:���� ȭ�鿡 ��µǴ� ��Ʈ�� ���⼭ ������ �Ӽ��� ������� ����ؾ��ϴ��� ����
		//DEFAULT_QUALITY:���⼭ �����ϴ� ��Ʈ�� ���� ��Ʈ�� ǰ���� �󸶳� ����ؾ� �ϴ��� ����
		//DEFAULT_PITCH | FF_DONTCARE:�⺻��ġ�� ����ϰ� ��Ʈ���� �������
		//"Arial":����� ��Ʈ �̸�
		//gpFont:���� ���� ��Ʈ�� ������ ������
	{
		return false;
	}

	return true;
}

// D3D ��ü �� ��ġ �ʱ�ȭ
// D3D�� �̿��ؼ� �ڿ��� �ε��ϰų� �������� �Ϸ��� D3D��ġ�� �����ؾ���
bool InitD3D(HWND hWnd)
{
	// D3D ��ü
	gpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!gpD3D)
	{
		return false;
	}

	// D3D��ġ�� �����ϴµ� �ʿ��� ����ü�� ä���ִ´�.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WIN_WIDTH;				//�����(����������)�� �ʺ�
	d3dpp.BackBufferHeight = WIN_HEIGHT;				//������ǳ���
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;			//������� ����
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//������� ������ ���� ȿ��. 
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;				//����/���ٽ� ������ ����
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//����� �ֻ����� ����۸� �����ϴ� �󵵰��� ����.
															//D3DPRESENT_INTERVAL_ONE�� ����Ͱ� ���� ����� ������ ����۸� ��������.
															//���ӿ����� ���ɻ� ������� ���� ���⸦ ��ٸ��� �ʰ� ��ٷ� �������ִ� ��찡 ����(D3DPRESENT_IMMEDIATE)

															// ä������ ������ �̿��Ͽ� D3D��ġ�� �����Ѵ�.
	if (FAILED(gpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &gpD3DDevice)))
	{
		return false;
	}

	return true;
}

bool LoadAssets()
{
	// �ؽ�ó �ε�

	// ���̴� �ε�
	gpColorShader = LoadShader("ColorShader.fx");
	if (!gpColorShader)
	{
		return false;
	}

	// �� �ε�
	gpSphere = LoadModel("sphere.x");
	if (!gpSphere)
	{
		return false;
	}

	return true;
}

// ���̴� �ε�
LPD3DXEFFECT LoadShader(const char * filename)
{
	LPD3DXEFFECT ret = NULL;

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif
	//�ε� �� ������
	D3DXCreateEffectFromFile(gpD3DDevice, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
	//gpD3DDevice:D3D��ġ, filename:�о�� ���̴� �����̸�
	//NULL:���̴��� �������� �� �߰��� ����� #define����
	//NULL:#include ���ù��� ó���� �� ����� �������̽� ������
	//dwShaderFlags:���̴��� �������� �� ����� �÷���
	//NULL:�Ű����� ������ ����� ����Ʈ Ǯ
	//ret:�ε��� ����Ʈ�� ������ ������
	//pError:�����Ϸ� ���� �޽����� ����ų ������

	// ���̴� �ε��� ������ ��� outputâ�� ���̴�
	// ������ ������ ����Ѵ�.
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

// �� �ε�
LPD3DXMESH LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, gpD3DDevice, NULL, NULL, NULL, NULL, &ret)))
		//filename:�ε��� �޽��� ���ϸ�, D3DXMESH_SYSTEMMEM:�ý��� �޸𸮿� �޽��� �ε��� ��
		//gpD3DDevice:D3D��ġ, NULL:���� �ﰢ�� �����͸� ���� ���ؿ��� ����,
		//NULL:���׸��� ������ ���� ���ؿ��� ����, NULL:����Ʈ �ν��Ͻ��� ���� ���ؿ��� ����,
		//NULL:���׸��� ���� ���� ���ؿ��� ����, ret:�ε��ؿ� �޽��� ������ ������
	{
		OutputDebugString("�� �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// �پ��� �������� ����� �̹������� �ؽ�ó�� �ε��ؿ�-�ؽ�ó �ε�
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(gpD3DDevice, filename, &ret)))
	{
		OutputDebugString("�ؽ�ó �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}
//------------------------------------------------------------
// ������ �ڵ�.
//------------------------------------------------------------

void Cleanup()
{
	// ��Ʈ�� release �Ѵ�.
	if (gpFont)
	{
		gpFont->Release();
		gpFont = NULL;
	}

	// ���� release �Ѵ�.
	if (gpSphere)
	{
		gpSphere->Release();
		gpSphere = NULL;
	}

	// ���̴��� release �Ѵ�.
	if (gpColorShader)
	{
		gpColorShader->Release();
		gpColorShader = NULL;
	}

	// �ؽ�ó�� release �Ѵ�.

	// D3D�� release �Ѵ�.
	if (gpD3DDevice)
	{
		gpD3DDevice->Release();
		gpD3DDevice = NULL;
	}

	if (gpD3D)
	{
		gpD3D->Release();
		gpD3D = NULL;
	}
}

