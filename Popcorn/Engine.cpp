#include "Engine.h"





// AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: Inner_Width(21), X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_Scale * 2), Width(28), Platform_Rect{},
  Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Circle_Pen(0), Platform_Inner_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(AsEngine *engine)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
	Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

	InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, AsEngine *engine, RECT &paint_area)
{// ������ ���������

	int x = X_Pos;
	int y = AsConfig::Y_Pos;
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
		return;

	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. ������ ������� ������
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size + Inner_Width) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);

	// 2. ������ ����
	SelectObject(hdc, Highlight_Pen);

	Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Circle_Size - 1) * AsConfig::Global_Scale, (y + Circle_Size - 1) * AsConfig::Global_Scale,
		(x + 1 + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 1 + 2) * AsConfig::Global_Scale);

	// 3. ������ ������� �����
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------




// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Hwnd(0), BG_Pen(0), BG_Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// ��������� ���� ��� ������

	Hwnd = hwnd;

	AsConfig::Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);

	Ball.Init();
	Level.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw_Platform(this);

	SetTimer(Hwnd, Timer_ID, 50, 0);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// ��������� ������ ����

	Level.Draw(hdc, paint_area);
	Platform.Draw(hdc, this, paint_area);

	//int i;

	//for (i = 0; i < 16; i++)
	//{
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
	//	Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, ELT_O, i);
	//}

	Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);

	Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos <= AsConfig::Border_X_Offset)
			Platform.X_Pos = AsConfig::Border_X_Offset;

		Platform.Redraw_Platform(this);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

		Platform.Redraw_Platform(this);
		break;

	case EKT_Space:
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width );

	return 0;
}
//------------------------------------------------------------------------------------------------------------
