#pragma once
#include "Surface.h"
#include "Instance.h"
#include "PhysicalDevice.h"

Surface::Surface() :
	pVkSurface(VK_NULL_HANDLE),
	pInstance(Singleton<Instance>::GetInstance())
{
}

Surface::Surface(Instance &instance) :
	pVkSurface(VK_NULL_HANDLE),
	pInstance(instance)
{
}

Surface::~Surface()
{
	if (pVkSurface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(pInstance.GetVkNative(), pVkSurface, nullptr);
	}
}

void Surface::Create()
{
	if (pVkSurface == VK_NULL_HANDLE)
	{
		if (glfwCreateWindowSurface(pInstance.GetVkNative(), pInstance.GetWindow(), nullptr, &pVkSurface) != VK_SUCCESS)
		{
			throw runtime_error("Failed to create window surface");
		}
	}
}

void Surface::Destroy()
{
	if (pVkSurface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(pInstance.GetVkNative(), pVkSurface, nullptr);
		pVkSurface = VK_NULL_HANDLE;
	}
}

bool Surface::IsValid() const
{
	return pVkSurface != VK_NULL_HANDLE;
}

VkSurfaceKHR Surface::GetVkNative() const
{
	return pVkSurface;
}

const SwapChainSupportDetails &Surface::GetSwapChainSupportDetails(const PhysicalDevice &physicalDevice) const
{
	SwapChainSupportDetails swapChainSupportDetails;
	swapChainSupportDetails.capabilities = GetCapabilities(physicalDevice);
	swapChainSupportDetails.formats = GetFormats(physicalDevice);
	swapChainSupportDetails.presentModes = GetPresentModes(physicalDevice);
	return swapChainSupportDetails;
}

const VkSurfaceCapabilitiesKHR &Surface::GetCapabilities(const PhysicalDevice &physicalDevice) const
{
	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.GetVkNative(), pVkSurface, &capabilities));
	return capabilities;
}

const VkSurfaceCapabilitiesKHR &Surface::GetCapabilities(Ref<PhysicalDevice> physicalDevice) const
{
	return GetCapabilities(*physicalDevice);
}

Vec<VkSurfaceFormatKHR> &Surface::GetFormats(const PhysicalDevice &physicalDevice) const
{
	Vec<VkSurfaceFormatKHR> vSurfaceFormats = EnumerateSurfaceFormatsKHR(physicalDevice.GetVkNative(), pVkSurface);
	return vSurfaceFormats;
}

Vec<VkSurfaceFormatKHR> &Surface::GetFormats(Ref<PhysicalDevice> physicalDevice) const
{
	return GetFormats(*physicalDevice);
}

Vec<VkPresentModeKHR> &Surface::GetPresentModes(const PhysicalDevice &physicalDevice) const
{
	Vec<VkPresentModeKHR> vPresentModes = EnumerateSurfacePresentModesKHR(physicalDevice.GetVkNative(), pVkSurface);
	return vPresentModes;
}

Vec<VkPresentModeKHR> &Surface::GetPresentModes(Ref<PhysicalDevice> physicalDevice) const
{
	return GetPresentModes(*physicalDevice);
}

VkPresentModeKHR Surface::ChoosePresentMode(const Vec<VkPresentModeKHR> &vPresentModes) const
{
	for (const VkPresentModeKHR &vkPresentMode : vPresentModes)
	{
		if (vkPresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return vkPresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR Surface::ChooseSurfaceFormat(const Vec<VkSurfaceFormatKHR> &vSurfaceFormats) const
{
	for (const VkSurfaceFormatKHR &vkSurfaceFormat : vSurfaceFormats)
	{
		if (vkSurfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM && 
			vkSurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return vkSurfaceFormat;
		}
	}
	return vSurfaceFormats[0];
}

VkExtent2D Surface::ChooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities) const
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(pInstance.GetWindow(), &width, &height);
		VkExtent2D actualExtent = { static_cast<u32>(width), static_cast<u32>(height) };
		actualExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}
}