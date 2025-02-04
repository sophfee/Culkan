#pragma once

#include "PhysicalDevice.h"
#include "Device.h"

PhysicalDevice::PhysicalDevice()
	:pPhysicalDevice(VK_NULL_HANDLE),
	sQueueFamilyIndices(QueueFamilyIndices{})
{
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice device):
	pPhysicalDevice(device),
	sQueueFamilyIndices(QueueFamilyIndices{})
{
}

PhysicalDevice::~PhysicalDevice()
{
}

VkPhysicalDevice PhysicalDevice::GetVkNative() const
{
	return VkPhysicalDevice();
}

VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(pPhysicalDevice, &properties);
	return properties;
}

VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const
{
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(pPhysicalDevice, &features);
	return features;
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::GetMemoryProperties() const
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(pPhysicalDevice, &memoryProperties);
	return memoryProperties;
}

Vec<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const
{
	u32 iArrQueueFamilyPropertyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &iArrQueueFamilyPropertyCount, nullptr);
	
	Vec<VkQueueFamilyProperties> queueFamilyProperties(iArrQueueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &iArrQueueFamilyPropertyCount, queueFamilyProperties.data());

	return queueFamilyProperties;
}

VkFormatProperties PhysicalDevice::GetFormatProperties(VkFormat format) const
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(pPhysicalDevice, format, &formatProperties);
	return formatProperties;
}

VkImageFormatProperties PhysicalDevice::GetImageFormatProperties(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags) const
{
	VkImageFormatProperties imageFormatProperties;
	vkGetPhysicalDeviceImageFormatProperties(pPhysicalDevice, format, type, tiling, usage, flags, &imageFormatProperties);
	return imageFormatProperties;
}

Vec<VkSparseImageFormatProperties> PhysicalDevice::GetSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling) const
{
	u32 iArrSparseImageFormatPropertyCount = 0;
	vkGetPhysicalDeviceSparseImageFormatProperties(pPhysicalDevice, format, type, samples, usage, tiling, &iArrSparseImageFormatPropertyCount, nullptr);
	Vec<VkSparseImageFormatProperties> sparseImageFormatProperties(iArrSparseImageFormatPropertyCount);
	vkGetPhysicalDeviceSparseImageFormatProperties(pPhysicalDevice, format, type, samples, usage, tiling, &iArrSparseImageFormatPropertyCount, sparseImageFormatProperties.data());
	return sparseImageFormatProperties;
}

void PhysicalDevice::FindQueueFamilies()
{
	sQueueFamilyIndices = QueueFamilyIndices{};

	u32 iArrQueueFamilyPropertyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &iArrQueueFamilyPropertyCount, nullptr);

	Vec<VkQueueFamilyProperties> queueFamilyProperties(iArrQueueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &iArrQueueFamilyPropertyCount, queueFamilyProperties.data());

	for (auto &queueFamilyProperty : queueFamilyProperties)
	{
		if (queueFamilyProperty.queueCount > 0 && queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			sQueueFamilyIndices.graphicsFamily = &queueFamilyProperty - queueFamilyProperties.data();
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(pPhysicalDevice, sQueueFamilyIndices.graphicsFamily, nullptr, &presentSupport);
		if (queueFamilyProperty.queueCount > 0 && presentSupport)
		{
			sQueueFamilyIndices.presentFamily = &queueFamilyProperty - queueFamilyProperties.data();
		}
		if (sQueueFamilyIndices.IsComplete())
		{
			break;
		}
	}
}

const QueueFamilyIndices &PhysicalDevice::GetQueueFamilyIndices() const
{
	if (!sQueueFamilyIndices.IsComplete())
	{
		throw runtime_error("Queue family indices are not complete");
	}

	return sQueueFamilyIndices;
}

u32 PhysicalDevice::RateSuitability() const
{
	auto vkProperties = GetProperties();
	auto vkFeatures = GetFeatures();
	
	u32 score = 0;

	// Discrete GPUs have a significant performance advantage
	if (vkProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 2048;
	}

	// Maximum possible size of textures affects graphics quality
	score += vkProperties.limits.maxImageDimension3D;

	// Application can't function without geometry shaders
	if (!vkFeatures.geometryShader)
	{
		return 0;
	}

	return score;
}

Ref<Device> PhysicalDevice::CreateDevice()
{
	Device *pDevice = new Device(*this);
	pDevice->Create();
	return Ref<Device>(pDevice);
}

// Get the supported depth formats
