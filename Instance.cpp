#pragma once

#include "PhysicalDevice.h"
#include "Instance.h"
#include <stdexcept>

Instance::Instance() :
	pVkInstance(VK_NULL_HANDLE),
	pWindow(nullptr),
	sAppName("Vulkan Application"),
	pArrLayers(),
	pArrExtensions(),
	pPhysicalDevices()
{
}

Instance::~Instance()
{
	if (pVkInstance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(pVkInstance, nullptr);
	}

	if (pWindow != nullptr)
	{
		glfwDestroyWindow(pWindow);
	}

	glfwTerminate();
}

void Instance::Create()
{
	ASSERT(glfwInit() == GLFW_TRUE, "GLFW failed to initialize");

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	pWindow = glfwCreateWindow(800, 600, sAppName.c_str(), nullptr, nullptr);

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;

	appInfo.pApplicationName = sAppName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = sAppName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo vkInstanceCreateInfo = {};
	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pNext = nullptr;
	vkInstanceCreateInfo.flags = 0;
	vkInstanceCreateInfo.pApplicationInfo = &appInfo;

#if defined(_DEBUG)

	pArrLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	pArrLayers.push_back("VK_LAYER_LUNARG_parameter_validation");
	pArrLayers.push_back("VK_LAYER_LUNARG_object_tracker");
	pArrLayers.push_back("VK_LAYER_LUNARG_core_validation");
	pArrLayers.push_back("VK_LAYER_LUNARG_image");
	pArrLayers.push_back("VK_LAYER_LUNARG_swapchain");
	pArrLayers.push_back("VK_LAYER_GOOGLE_threading");

#endif

	vkInstanceCreateInfo.enabledLayerCount = static_cast<u32>(pArrLayers.size());
	vkInstanceCreateInfo.ppEnabledLayerNames = pArrLayers.data();
	
	vkInstanceCreateInfo.enabledExtensionCount = static_cast<u32>(pArrExtensions.size());
	vkInstanceCreateInfo.ppEnabledExtensionNames = pArrExtensions.data();

	VkResult result = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &pVkInstance);
	if (result != VK_SUCCESS)
	{
		throw runtime_error("Failed to create Vulkan instance");
	}
}

void Instance::AddLayer(string layer)
{
	pArrLayers.push_back(layer.c_str());
}

void Instance::AddExtension(string extension)
{
	pArrExtensions.push_back(extension.c_str());
}

void Instance::AddAllExtensions()
{
	Vec<VkExtensionProperties> vExtensionData = EnumerateInstanceExtensionProperties();
	for (VkExtensionProperties &vkExtension : vExtensionData)
	{
		pArrExtensions.push_back(vkExtension.extensionName);
	}
}

void Instance::SetAppName(string appName)
{
	sAppName = appName;
}

vector<shared_ptr<PhysicalDevice>> Instance::GetPhysicalDevices()
{
	Vec<VkPhysicalDevice> vPhysicalDevices = EnumeratePhysicalDevices(pVkInstance);
	
	for (VkPhysicalDevice &vkPd : vPhysicalDevices)
	{
		pPhysicalDevices.push_back(make_shared<PhysicalDevice>(vkPd));
	}

	return pPhysicalDevices;
}
const Vec<const char *> &Instance::GetDeviceExtensions() const
{
	return pArrExtensions;
}

const Vec<const char *> &Instance::GetDeviceLayers() const
{
	return pArrLayers;
}

VkInstance Instance::GetVkNative() const
{
	return pVkInstance;
}
