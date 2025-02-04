#pragma once

class Device;
class Surface;
class Shader;

// This shit is gonna make me hurt someone
class Pipeline : public IVkResource, public NonCopyable
{
private:
	VkPipeline pPipeline;
	VkPipelineLayout pLayout;

	Device &pDevice;
	Surface &pSurface;
	Vec<Shader *> vShaderStages;

	Ref<VkPipelineInputAssemblyStateCreateInfo> pPipelineInfo;

public:
	Pipeline(Device &rDevice, Surface &rSurface);
	~Pipeline();

public:

	void Create() override;
	void Destroy() override;
	bool IsValid() const override;

public:

	void AddShaderStage(Shader *stage);
};