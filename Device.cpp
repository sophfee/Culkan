#pragma once

#include "PhysicalDevice.h"
#include "Device.h"
#include "Instance.h"

Device::Device(PhysicalDevice &physicalDevice) :
	pVkDevice(VK_NULL_HANDLE),
	pGraphicsQueue(VK_NULL_HANDLE),
	pPresentQueue(VK_NULL_HANDLE),
	pPhysicalDevice(physicalDevice),
	sQueueFamilyIndices(physicalDevice.GetQueueFamilyIndices())
{
}

Device::~Device()
{
	if (pVkDevice != VK_NULL_HANDLE)
	{
		vkDestroyDevice(pVkDevice, nullptr);
	}
}

void Device::Create()
{
	const QueueFamilyIndices &queueFamilyIndices = pPhysicalDevice.GetQueueFamilyIndices();

	Vec<VkDeviceQueueCreateInfo> queueCreateInfos;
	Set<u32> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentFamily };

	// The queue priority is a floating point value between 0.0 and 1.0
	f32 queuePriority = 1.0f;

	for (u32 queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = pPhysicalDevice.GetFeatures();

	Instance &instance = Singleton<Instance>::GetInstance();
	const Vec<const char *> &deviceExtensions = instance.GetDeviceExtensions();

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	u32 deviceExtensionCount = 0;
	const char **deviceExtensionNames = glfwGetRequiredInstanceExtensions(&deviceExtensionCount);

	deviceCreateInfo.enabledExtensionCount = deviceExtensionCount;
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames;

	VK_CHECK_RESULT(vkCreateDevice(pPhysicalDevice.GetVkNative(), &deviceCreateInfo, nullptr, &pVkDevice));

	vkGetDeviceQueue(pVkDevice, queueFamilyIndices.graphicsFamily, 0, &pGraphicsQueue);
	vkGetDeviceQueue(pVkDevice, queueFamilyIndices.presentFamily, 0, &pPresentQueue);

	if (pGraphicsQueue == VK_NULL_HANDLE || pPresentQueue == VK_NULL_HANDLE)
	{
		throw runtime_error("Failed to get device queue");
	}

}

void Device::Destroy()
{
}

bool Device::IsValid() const
{
	return false;
}

const VkDevice &Device::GetVkNative() const
{
	return pVkDevice;
}

const PhysicalDevice &Device::GetPhysicalDevice() const
{
	return pPhysicalDevice;
}

const VkQueue &Device::GetGraphicsQueue() const
{
	return pGraphicsQueue;
}

const VkQueue &Device::GetPresentQueue() const
{
	return pPresentQueue;
}