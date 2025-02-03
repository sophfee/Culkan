#pragma once

class PhysicalDevice;

class Device : public IVkResource, public NonCopyable
{
private:
	VkDevice pVkDevice;
	VkQueue pGraphicsQueue;
	VkQueue pPresentQueue;

	PhysicalDevice &pPhysicalDevice;
	const QueueFamilyIndices &sQueueFamilyIndices;

public:

	Device(PhysicalDevice &physicalDevice);
	~Device();

public:
	void Create() override;
	void Destroy() override;
	bool IsValid() const override;
	const VkDevice &GetVkNative() const;
	const PhysicalDevice &GetPhysicalDevice() const;
	const VkQueue &GetGraphicsQueue() const;
	const VkQueue &GetPresentQueue() const;
};