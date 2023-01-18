#pragma once

#include <d3d9.h>

enum eTextAlignment {
	LEFT,
	CENTER,
	RIGHT
};

/**
 * @brief
*/
class D3DHelper
{
public:
	D3DHelper(IDirect3DDevice9 *pdevice)
	{
		device = pdevice;
	}

	void DrawLine(int x, int y, int x2, int y2, float thickness, D3DCOLOR LineColor, D3DCOLOR LineColor2 = 0);
	void DrawLine(RECT rc, float thickness, D3DCOLOR LineColor, D3DCOLOR LineColor2 = 0);
	void DrawRectangle(int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BoxColor2); 
	void DrawOutlinedRectangle(int x, int y, int w, int h, float thickness, D3DCOLOR BoxColor, D3DCOLOR BorderColor, bool topline, bool bottomline, bool leftline, bool rightline, D3DCOLOR BoxColor2);
	IDirect3DDevice9* device;
};

/**
 * @brief
*/
struct Vertex
{
	float x, y, z, ht;
	D3DCOLOR Color;
};

/**
 * @brief
 * @param x
 * @param y
 * @param x2
 * @param y2
 * @param thickness
 * @param LineColor
 * @param LineColor2
*/
void D3DHelper::DrawLine(int x, int y, int x2, int y2, float thickness, D3DCOLOR LineColor, D3DCOLOR LineColor2) {
	
	Vertex V[2] =
	{
		(float)x, (float)y, 0.0f, 1.0f, LineColor,
		(float)x2, (float)y2, 0.0f, 1.0f, LineColor,
	};

	device->SetTexture(0, NULL);
	device->SetPixelShader(0);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 2, V, sizeof(Vertex));
	if (thickness && thickness > 1.f)
	{
		int thick = (int)thickness;
		for (int i = 1; i < thick; i++)
		{
			DrawLine(x, y + i, x2, y2 + i, 0, LineColor, LineColor2);
		}
	}
}

void D3DHelper::DrawLine(RECT rc, float thickness, D3DCOLOR LineCol, D3DCOLOR LineCol2)
{
	DrawLine(rc.left, rc.top, rc.right, rc.bottom, thickness, LineCol, LineCol2);
}

/**
 * @brief
 * @param x
 * @param y
 * @param w
 * @param h
 * @param BoxColor
*/
void D3DHelper::DrawRectangle(int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BoxColor2 = 0)
{
	Vertex V[4] =
	{
		{ (float)x, (float)y + (float)h, 0.0f, 0.0f, BoxColor },{ (float)x, (float)y, 0.0f, 0.01, BoxColor },
		{ (float)x + (float)w, (float)y + (float)h, 0.0f, 0.0f, BoxColor },{ (float)x + (float)w, (float)y, 0.0f, 0.0f, BoxColor }
	};

	device->SetTexture(0, NULL);
	device->SetPixelShader(0);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
}

/**
 * @brief
 * @param x
 * @param y
 * @param w
 * @param h
 * @param thickness
 * @param BoxColor
 * @param BorderColor
*/
void D3DHelper::DrawOutlinedRectangle(int x, int y, int w, int h, float thickness, D3DCOLOR BoxColor, D3DCOLOR BorderColor, bool topline = true, bool bottomline = true, bool leftline = true, bool rightline = true, D3DCOLOR BoxColor2 = 0)
{
	if (BoxColor)
		DrawRectangle(x, y, w, h, BoxColor, BoxColor2);
	//Top horiz line
	if (topline)
		DrawLine(x, y, x + w, y, thickness, BorderColor);
	//Left vertical line
	if (leftline)
		DrawLine(x, y, x, y + h, thickness, BorderColor);
	//right vertical line
	if (rightline)
		DrawLine(x + w, y, x + w, y + h, thickness,BorderColor);
	//bottom horiz line
	if (bottomline)
		DrawLine(x, y + h, x + w, y + h, thickness, BorderColor);
}