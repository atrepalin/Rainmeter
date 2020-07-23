#include "pch.h"
#include "Draw.h"
#include "Utils.h"

#pragma comment(lib,"d2d1.lib")

D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::White);

D2D1_COLOR_F colorG = D2D1::ColorF(D2D1::ColorF::Green);

D2D1_COLOR_F colorR = D2D1::ColorF(D2D1::ColorF::Red);

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidBrush;

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidBrushG;

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidBrushR;

Microsoft::WRL::ComPtr<IDWriteTextFormat> format;

static float size = 10;

static float y = 80;

static float sy = 0;

static float index = -1;

static int score = 0;

static float sx = 2;

static float x = 500;

static bool l = true;

static float objects[1024];

void Restart()
{
	for (int i = 0; i < 1024; i++)
	{
		float pos = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 50);

		pos = i * 100 + pos;

		objects[i] = pos;
	}

	size = 10;
	y = 80;
	sy = 0;
	index = -1;
	score = 0;
	sx = 2;
	x = 500;

	l = false;
}

EXPORT_PLUGIN void Init(Context context)
{
	Restart();

	context.Target->CreateSolidColorBrush(color, solidBrush.GetAddressOf());

	context.Target->CreateSolidColorBrush(colorG, solidBrushG.GetAddressOf());

	context.Target->CreateSolidColorBrush(colorR, solidBrushR.GetAddressOf());

	context.DWFactory->CreateTextFormat(L"CONSOLAS", nullptr, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 20, L"", format.GetAddressOf());
}

EXPORT_PLUGIN void Draw(Context context)
{
	auto rect = D2D1::RectF(0, 0, 500, 100);

	context.Target->FillRectangle(rect, solidBrush.Get());

	rect = D2D1::RectF(250 - size / 2, y, 250 + size / 2, y + size);

	context.Target->FillRectangle(rect, solidBrushG.Get());

	for (float o : objects)
	{
		rect = D2D1::RectF(x + o - size / 2, 80, x + o + size / 2, 80 + size);

		context.Target->FillRectangle(rect, solidBrushR.Get());
	}

	for (int i = 0; i < 1024; i++)
	{
		if (Utils::f(250 - size / 2, y, 250 + size / 2, y + size, x + objects[i] - size / 2, 80, x + objects[i] + size / 2, 80 + size) > 0)
		{
			context.Target->DrawTextW(L"Вы проиграли", 12, format.Get(), D2D1::RectF(0, 20, 500, 50), solidBrushR.Get());

			l = true;
		}
		else if (abs((int)(250 - size / 2) - (int)(x + objects[i] - size / 2)) < 10 && i > index)
		{
			index = i;

			score++;
		}
	}

	wchar_t m_str[13];

	swprintf_s(m_str, L"Счет:%d   ", score);

	context.Target->DrawTextW(m_str, 10, format.Get(), D2D1::RectF(0, 0, 500, 50), solidBrushG.Get());

	if (!l)
	{
		y += sy;

		x -= sx; 

		sx += 0.001;

		if (y < 80)
			sy++;
		else
		{
			sy = 0;
			y = 80;
		}
	}
}
EXPORT_PLUGIN void Action(std::wstring arg)
{
	if (sy == 0)
		if (arg == L"up")
			sy = -8;
	if (arg == L"restart")
		Restart();
}
