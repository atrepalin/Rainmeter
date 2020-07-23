#include <d2d1_1.h>
#include <wrl\client.h>
#include <math.h>
#include "Context.h"
#pragma once
#ifdef LIBRARY_EXPORTS
#define EXPORT_PLUGIN
#else
#define EXPORT_PLUGIN __declspec(dllexport)
#endif
extern "C" EXPORT_PLUGIN void Init(Context context);

extern "C" EXPORT_PLUGIN void Draw(Context context);

extern "C" EXPORT_PLUGIN void Action(std::wstring arg);