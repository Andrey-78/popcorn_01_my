#pragma once

#include <Windows.h>

//------------------------------------------------------------------------------------------------------------

class AsBorder
{
public:
	void Init();
	void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);

	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;

private:
	void Draw_Element(HDC hdc, int x, int y, bool top_boder, AsEngine *engine);

	HPEN Border_Blue_Pen, Border_White_Pen;
	HBRUSH Border_Blue_Brush, Border_White_Brush;	
};
AsBorder border;