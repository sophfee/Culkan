#pragma once

class Device;

class Shader : public IVkResource, public NonCopyable
{
private:
	VkShaderModule pVkShaderModule;
	VkShaderStageFlagBits eStage;
	Device &pDevice;
	Vec<i8> pArrCode;

public:

	Shader(Device &device, const string &filename);
	~Shader();

public:

	void Create() override;
	void Destroy() override;
	bool IsValid() const override;
	VkShaderModule GetVkNative() const;

public:

	void SetCode(const Vec<i8> &code);
	const Vec<i8> &GetCode() const;

	const VkPipelineShaderStageCreateInfo &GetStageCreateInfo();

private:

	static const Vec<i8> &ReadFile(const string &filename);

};