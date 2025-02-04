#pragma once

#include "PhysicalDevice.h"
#include "Instance.h"
#include <stdexcept>

Instance::Instance() :
	pVkInstance(VK_NULL_HANDLE),
	pWindow(nullptr),
	sAppName("Vulkan Application"),
	pArrLayers(),
	pArrExtensions(0),
	pPhysicalDevices()
{
}

Instance::~Instance()
{
#if defined(_DEBUG)
	if (pDebugMessenger != VK_NULL_HANDLE)
	{
		auto _vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(pVkInstance, "vkDestroyDebugUtilsMessengerEXT");
		_vkDestroyDebugUtilsMessengerEXT(pVkInstance, pDebugMessenger, nullptr);
	}
#endif

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

VkBool32 Instance::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			cout << "VERBOSE: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			cout << "INFO: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			cout << "WARNING: ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			cout << "ERROR: ";
			break;
		default:
			cout << "UNKNOWN: ";
			break;
	}
	cout << pCallbackData->pMessage << endl;
	return VK_FALSE;
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
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;


	VkInstanceCreateInfo vkInstanceCreateInfo = {};
	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pNext = nullptr;
	vkInstanceCreateInfo.flags = 0;
	vkInstanceCreateInfo.pApplicationInfo = &appInfo;

#if defined(_DEBUG)
	pArrLayers.push_back("VK_LAYER_KHRONOS_validation");
	// only khronos


	//pArrLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	//pArrLayers.push_back("VK_LAYER_LUNARG_crash_diagnostic");
	//pArrLayers.push_back("VK_LAYER_LUNARG_api_dump");
	
	//pArrExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	//pArrExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	//pArrExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);

#endif

	pArrExtensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	for (auto *ext : pArrExtensions)
	{
		cout << "Extension: " << ext << endl;
	}

	vkInstanceCreateInfo.enabledLayerCount = static_cast<u32>(pArrLayers.size());
	vkInstanceCreateInfo.ppEnabledLayerNames = pArrLayers.data();

	vkInstanceCreateInfo.enabledExtensionCount = static_cast<u32>(pArrExtensions.size());
	vkInstanceCreateInfo.ppEnabledExtensionNames = pArrExtensions.data();

	VkResult result = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &pVkInstance);
	if (result != VK_SUCCESS)
	{
		throw runtime_error("Failed to create Vulkan instance");
	}

#if defined(_DEBUG) && 1

	auto _vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(pVkInstance, "vkCreateDebugUtilsMessengerEXT");

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.pNext = nullptr;
	debugUtilsMessengerCreateInfo.flags = 0;
	debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;
	debugUtilsMessengerCreateInfo.pUserData = nullptr;
	result = _vkCreateDebugUtilsMessengerEXT(pVkInstance, &debugUtilsMessengerCreateInfo, nullptr, &pDebugMessenger);
	if (result != VK_SUCCESS)
	{
		throw runtime_error("Failed to create debug messenger");
	}

#endif
}

void Instance::AddLayer(string layer)
{
	pArrLayers.push_back(layer.c_str());
}

void Instance::AddExtension(string extension)
{
	pArrExtensions.push_back(extension.c_str());
}

void Instance::AddRequiredExtensions()
{
	u32 glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//pArrExtensions.resize(glfwExtensionCount);

	for (u32 i = 0; i < glfwExtensionCount; i++)
	{
		pArrExtensions.push_back(string(glfwExtensions[i]).c_str());
	}
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

GLFWwindow *Instance::GetWindow() const
{
	return pWindow;
}
