#ifndef INCLUDED_COOENGINE_CORE_COMMON_H
#define INCLUDED_COOENGINE_CORE_COMMON_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Win32 headers
#include <objbase.h>
#include <Windows.h>

// Standard headers
#include <algorithm>
#include <array>
#include <chrono>
#include <codecvt>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <variant>

// External headers
#include <RapidJSON/document.h>
#include <RapidJSON/filereadstream.h>
#include <RapidJSON/filewritestream.h>

// Utility functions to release DirectX interfaces
template <class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

template <class T>
inline void SafeDelete(T*& ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

#endif // !INCLUDED_COOENGINE_CORE_COMMON_H
