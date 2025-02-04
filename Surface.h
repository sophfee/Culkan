#pragma once

class Instance;
class PhysicalDevice;

class Surface : public IVkResource, public NonCopyable
{
private:
	VkSurfaceKHR pVkSurface;
	Instance &pInstance;

public:

	Surface();
	Surface(Instance &instance);
	~Surface();

public:

	void Create() override;
	void Destroy() override;
	bool IsValid() const override;
	VkSurfaceKHR GetVkNative() const;

public:

	// Get swap chain support details
	const SwapChainSupportDetails &GetSwapChainSupportDetails(const PhysicalDevice &physicalDevice) const;

	// Get the surface capabilities
	const VkSurfaceCapabilitiesKHR &GetCapabilities(const PhysicalDevice &physicalDevice) const;
	const VkSurfaceCapabilitiesKHR &GetCapabilities(Ref<PhysicalDevice> physicalDevice) const;
	// Get the surface formats
	Vec<VkSurfaceFormatKHR> &GetFormats(const PhysicalDevice &physicalDevice) const;
	Vec<VkSurfaceFormatKHR> &GetFormats(Ref<PhysicalDevice> physicalDevice) const;
	// Get the present modes
	Vec<VkPresentModeKHR> &GetPresentModes(const PhysicalDevice &physicalDevice) const;
	Vec<VkPresentModeKHR> &GetPresentModes(Ref<PhysicalDevice> physicalDevice) const;

	VkPresentModeKHR ChoosePresentMode(const Vec<VkPresentModeKHR> &vPresentModes) const;
	VkSurfaceFormatKHR ChooseSurfaceFormat(const Vec<VkSurfaceFormatKHR> &vSurfaceFormats) const;
	VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities) const;
};