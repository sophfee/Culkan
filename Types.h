#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <functional>

#include "NonCopyable.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

using namespace std;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

class IVkResource {
public:
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual bool IsValid() const = 0;
};

template <typename T>
using Ref = shared_ptr<T>;

template <typename T>
using WeakRef = weak_ptr<T>;

template <typename T>
using Vec = vector<T>;

template <typename T>
using Set = set<T>;

template <typename T>
using Func = function<T>;

#define VK_CHECK_RESULT(f)																								\
{																														\
	VkResult res = (f);																									\
	if (res != VK_SUCCESS)																								\
	{																													\
		throw runtime_error("Vulkan error: " + to_string(res));															\
	}																													\
}

#define ASSERT(condition, message)																						\
{																														\
	if (!(condition))																									\
	{																													\
		throw runtime_error(message);																					\
	}																													\
}


template <typename T, typename R = ::VkResult>
Vec<T> Enumerate(Func<R(uint32_t *, T *)> enumerateFunction)
{
	uint32_t count = 0;
	if constexpr (is_same_v<R, VkResult>)
	{
		VK_CHECK_RESULT(enumerateFunction(&count, nullptr));
	}
	else
	{
		enumerateFunction(&count, nullptr);
	}

	Vec<T> items(count);
	if constexpr (is_same_v<R, VkResult>)
	{
		VK_CHECK_RESULT(enumerateFunction(&count, items.data()));
	}
	else
	{
		enumerateFunction(&count, items.data());
	}

	return items;
}

#define GENERATE_ENUMERATE_FUNCTION(name, type, result, func, args, call_args, ...)		\
inline Vec<type> Enumerate##name args {											\
	return Enumerate<type, result>([__VA_ARGS__](uint32_t *count, type *data)			\
	{																					\
		return func call_args;															\
	});																					\
}

// Generate enumeration functions for Vulkan
GENERATE_ENUMERATE_FUNCTION(InstanceExtensionProperties, VkExtensionProperties, VkResult, vkEnumerateInstanceExtensionProperties, (), (nullptr, count, data), );
GENERATE_ENUMERATE_FUNCTION(InstanceLayerProperties, VkLayerProperties, VkResult, vkEnumerateInstanceLayerProperties, (), (count, data), );
GENERATE_ENUMERATE_FUNCTION(PhysicalDevices, VkPhysicalDevice, VkResult, vkEnumeratePhysicalDevices, (VkInstance instance), (instance, count, data), instance);
GENERATE_ENUMERATE_FUNCTION(DeviceExtensionProperties, VkExtensionProperties, VkResult, vkEnumerateDeviceExtensionProperties, (VkPhysicalDevice device, const char *layerName), (device, layerName, count, data), device, layerName);
GENERATE_ENUMERATE_FUNCTION(DeviceLayerProperties, VkLayerProperties, VkResult, vkEnumerateDeviceLayerProperties, (VkPhysicalDevice device), (device, count, data), device);
GENERATE_ENUMERATE_FUNCTION(QueueFamilyProperties, VkQueueFamilyProperties, void, vkGetPhysicalDeviceQueueFamilyProperties, (VkPhysicalDevice device), (device, count, data), device);
GENERATE_ENUMERATE_FUNCTION(FormatProperties, VkFormatProperties, void, vkGetPhysicalDeviceFormatProperties, (VkPhysicalDevice device, VkFormat format), (device, format, data), device, format);
GENERATE_ENUMERATE_FUNCTION(SurfaceFormatsKHR, VkSurfaceFormatKHR, VkResult, vkGetPhysicalDeviceSurfaceFormatsKHR, (VkPhysicalDevice device, VkSurfaceKHR surface), (device, surface, count, data), device, surface);
GENERATE_ENUMERATE_FUNCTION(PresentModesKHR, VkPresentModeKHR, VkResult, vkGetPhysicalDeviceSurfacePresentModesKHR, (VkPhysicalDevice device, VkSurfaceKHR surface), (device, surface, count, data), device, surface);


struct QueueFamilyIndices
{
	u32 graphicsFamily = 0;
	u32 presentFamily = 0;

	bool IsComplete() const
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

template <typename T>
class Singleton
{
public:
	static T &GetInstance()
	{
		static T instance;
		return instance;
	}
};
