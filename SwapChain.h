#pragma once

class Device;
class Surface;

class SwapChain : public IVkResource, public NonCopyable
{
private:
	VkSwapchainKHR pVkSwapChain;
	Device &pDevice;
	Surface &pSurface;
	Vec<VkImage> pArrImages;
	Vec<VkImageView> pArrImageViews;
	VkFormat pFormat;
	VkExtent2D pExtent;

public:

	SwapChain(Device &device, Surface &surface);
	~SwapChain();

public:

	void Create() override;
	void Destroy() override;
	bool IsValid() const override;
	VkSwapchainKHR GetVkNative() const;

public:

	// Get the swap chain images
	const Vec<VkImage> &GetImages() const;

	void CreateImageViews();
	// Get the swap chain image views
	const Vec<VkImageView> &GetImageViews() const;
	// Get the swap chain format
	VkFormat GetFormat() const;
	// Get the swap chain extent
	VkExtent2D GetExtent() const;
};