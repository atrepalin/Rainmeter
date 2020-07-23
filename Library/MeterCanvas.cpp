#include "StdAfx.h"
#include "MeterCanvas.h"
#include "Logger.h"
#include "windows.h"
#include "../Common/Gfx/Canvas.h"
#include "../Common/Gfx/Util/D2DUtil.h"

std::wstring dll;
HMODULE hlib;
void (*draw)(Gfx::Canvas::Context context);

void (*action)(std::wstring arg);

void (*init)(Gfx::Canvas::Context context);

MeterCanvas::MeterCanvas(Skin* skin, const WCHAR* name) : Meter(skin, name)
{
	dll = skin->GetParser().GetValue(name, L"Dll", L"");

	hlib = LoadLibrary(dll.c_str());

	(FARPROC&)draw = GetProcAddress(hlib, "Draw");

	(FARPROC&)action = GetProcAddress(hlib, "Action");

	(FARPROC&)init = GetProcAddress(hlib, "Init");                         

	init(skin->GetCanvas().GetContext());

	Meter::Initialize();
}

bool MeterCanvas::Update()
{
	return Meter::Update();
}

bool MeterCanvas::Draw(Gfx::Canvas& canvas)
{
	if (draw != nullptr)
		draw(canvas.GetContext());

	return Meter::Draw(canvas);
}
void MeterCanvas::Action(std::wstring arg)
{
	if (action != nullptr)
		action(arg);
}
bool MeterCanvas::HitTest(int x, int y)
{
	return Meter::HitTest(x, y);
}

MeterCanvas::~MeterCanvas()
{
	FreeLibrary(hlib);
}
