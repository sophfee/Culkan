#pragma once

#include <vulkan/vulkan.h>

class Instance;
class Device;

class PhysicalDevice : public NonCopyable
{
private:
	VkPhysicalDevice pPhysicalDevice;
	QueueFamilyIndices sQueueFamilyIndices;

public:
	PhysicalDevice();
	PhysicalDevice(VkPhysicalDevice device);
	~PhysicalDevice();

public:

	VkPhysicalDevice GetVkNative() const;

	// Get the properties of the physical device
	VkPhysicalDeviceProperties GetProperties() const;

	// Get the features of the physical device
	VkPhysicalDeviceFeatures GetFeatures() const;

	// Get the memory properties of the physical device
	VkPhysicalDeviceMemoryProperties GetMemoryProperties() const;

	// Get the queue family properties of the physical device
	Vec<VkQueueFamilyProperties> GetQueueFamilyProperties() const;

	// Get the format properties of the physical device
	VkFormatProperties GetFormatProperties(VkFormat format) const;

	// Get the image format properties of the physical device
	VkImageFormatProperties GetImageFormatProperties(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags) const;

	// Get the sparse image format properties of the physical device
	Vec<VkSparseImageFormatProperties> GetSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling) const;

	void FindQueueFamilies();

	// Get the queue family indices of the physical device
	const QueueFamilyIndices &GetQueueFamilyIndices() const;

	// Rates the suitability of the physical device.
	// The higher the score, the more suitable the physical device is.
	// The score is based on the properties and features of the physical device.
	u32 RateSuitability() const;

	Ref<Device> CreateDevice();

public:

	friend class Instance;
};