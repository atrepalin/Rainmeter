#ifndef __METERCANVAS_H__
#define __METERCANVAS_H__

#include "Meter.h"

class MeterCanvas : public Meter
{
public:
	MeterCanvas(Skin* skin, const WCHAR* name);

	UINT GetTypeID() override { return TypeID<MeterCanvas>(); }

	virtual bool Update();
	virtual bool Draw(Gfx::Canvas& canvas);

	virtual void Action(std::wstring arg);

	bool HitTest(int x, int y);

	~MeterCanvas();
};
#endif