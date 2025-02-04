#pragma once

#include "Types.h"

class PhysicalDevice;

class Instance : public NonCopyable
{
private:

	VkInstance pVkInstance;
	GLFWwindow *pWindow;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT pDebugMessenger;
#endif

	string sAppName;

	Vec<const char *> pArrLayers;
	Vec<const char *> pArrExtensions;
	Vec<Ref<PhysicalDevice>> pPhysicalDevices;

public:
	Instance();
	~Instance();

public:

	// PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback;
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
	
	// Create the Vulkan instance
	void Create();
	void AddLayer(string layer);
	void AddExtension(string extension);
	void AddRequiredExtensions();

	Vec<VkExtensionProperties> GetAvailableExtensions()
	{
		return EnumerateInstanceExtensionProperties();
	}

	void AddAllExtensions();
	void SetAppName(string appName);

	Vec<Ref<PhysicalDevice>> GetPhysicalDevices();


	const Vec<const char *> &GetDeviceExtensions() const;
	const Vec<const char *> &GetDeviceLayers() const;

	// Get the Vulkan instance
	VkInstance GetVkNative() const;

	// Get the window
	GLFWwindow *GetWindow() const;
};