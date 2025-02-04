#pragma once

#include "SwapChain.h"
#include "Device.h"
#include "PhysicalDevice.h"
#include "Surface.h"

SwapChain::SwapChain(Device &device, Surface &surface) :
	pVkSwapChain(VK_NULL_HANDLE),
	pDevice(device),
	pSurface(surface),
	pArrImages(),
	pArrImageViews(),
	pFormat(VK_FORMAT_UNDEFINED),
	pExtent({ 0, 0 })
{
}

SwapChain::~SwapChain()
{
	if (pVkSwapChain != VK_NULL_HANDLE)
	{
		Destroy();
	}
}

void SwapChain::Create()
{
	// Get the swap chain support details
	const PhysicalDevice &physicalDevice = pDevice.GetPhysicalDevice();
	const Surface &surface = pSurface;
	const SwapChainSupportDetails &swapChainSupportDetails = pSurface
		.GetSwapChainSupportDetails(physicalDevice);

	// Choose the surface format
	const VkSurfaceFormatKHR &surfaceFormat = pSurface
		.ChooseSurfaceFormat(swapChainSupportDetails.formats);

	// Choose the present mode
	const VkPresentModeKHR &presentMode = pSurface
		.ChoosePresentMode(swapChainSupportDetails.presentModes);

	// Choose the swap extent
	const VkSurfaceCapabilitiesKHR &surfaceCapabilities = pSurface
		.GetCapabilities(physicalDevice);

	const VkExtent2D &swapExtent = pSurface
		.ChooseSwapExtent(surfaceCapabilities);

	// Get the number of images in the swap chain
	u32 iImageCount = surfaceCapabilities.minImageCount + 1;

	if (surfaceCapabilities.maxImageCount > 0 && iImageCount > surfaceCapabilities.maxImageCount)
	{
		iImageCount = surfaceCapabilities.maxImageCount;
	}

	// Create the swap chain
	VkSwapchainCreateInfoKHR vkSwapChainCreateInfo = {};
	vkSwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vkSwapChainCreateInfo.surface = pSurface.GetVkNative();
	vkSwapChainCreateInfo.minImageCount = iImageCount;
	vkSwapChainCreateInfo.imageFormat = surfaceFormat.format;
	vkSwapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	vkSwapChainCreateInfo.imageExtent = swapExtent;
	vkSwapChainCreateInfo.imageArrayLayers = 1;
	vkSwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const QueueFamilyIndices &queueFamilyIndices = physicalDevice.
		GetQueueFamilyIndices();

	u32 queueFamilyIndicesArr[] = { queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentFamily };

	if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily)
	{
		vkSwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		vkSwapChainCreateInfo.queueFamilyIndexCount = 2;
		vkSwapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndicesArr;
	}
	else
	{
		vkSwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkSwapChainCreateInfo.queueFamilyIndexCount = 0;
		vkSwapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	vkSwapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	vkSwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vkSwapChainCreateInfo.presentMode = presentMode;
	vkSwapChainCreateInfo.clipped = VK_TRUE;
	vkSwapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CHECK_RESULT(vkCreateSwapchainKHR(pDevice.GetVkNative(), &vkSwapChainCreateInfo, nullptr, &pVkSwapChain));

}

void SwapChain::Destroy()
{
	for (VkImageView vkImageView : pArrImageViews)
	{
		vkDestroyImageView(pDevice.GetVkNative(), vkImageView, nullptr);
	}

	if (pVkSwapChain != VK_NULL_HANDLE)
	{

		vkDestroySwapchainKHR(pDevice.GetVkNative(), pVkSwapChain, nullptr);
		pVkSwapChain = VK_NULL_HANDLE;
	}
}

bool SwapChain::IsValid() const
{
	return pVkSwapChain != VK_NULL_HANDLE;
}

VkSwapchainKHR SwapChain::GetVkNative() const
{
	return pVkSwapChain;
}

const Vec<VkImage> &SwapChain::GetImages() const
{
	Vec<VkImage> vImages = GetSwapchainImagesKHR(pDevice.GetVkNative(), pVkSwapChain);
	return vImages;
}

void SwapChain::CreateImageViews()
{
	const Vec<VkImage> &vImages = GetImages();
	pArrImageViews.resize(vImages.size());
	for (u32 i = 0; i < vImages.size(); i++)
	{
		VkImageViewCreateInfo vkImageViewCreateInfo = {};
		vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vkImageViewCreateInfo.image = vImages[i];
		vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		vkImageViewCreateInfo.format = pFormat;
		vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		vkImageViewCreateInfo.subresourceRange.levelCount = 1;
		vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		vkImageViewCreateInfo.subresourceRange.layerCount = 1;
		VK_CHECK_RESULT(vkCreateImageView(pDevice.GetVkNative(), &vkImageViewCreateInfo, nullptr, &pArrImageViews[i]));
	}
}

const Vec<VkImageView> &SwapChain::GetImageViews() const
{
	return pArrImageViews;
}

VkFormat SwapChain::GetFormat() const
{
	return VkFormat();
}

VkExtent2D SwapChain::GetExtent() const
{
	return VkExtent2D();
}


