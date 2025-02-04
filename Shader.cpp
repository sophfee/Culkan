#pragma once

#include "Shader.h"
#include "Device.h"


Shader::Shader(Device &device, const string &filename) :
	pVkShaderModule(VK_NULL_HANDLE),
	pDevice(device)
{
	SetCode(ReadFile(filename));
	// Don't create the shader module here, create it when needed.
}

Shader::~Shader()
{
	if (pVkShaderModule != VK_NULL_HANDLE)
	{
		Destroy();
	}
}

void Shader::Create()
{
	VkShaderModuleCreateInfo shaderCreateInfo = {};
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo.codeSize = pArrCode.size();
	shaderCreateInfo.pCode = reinterpret_cast<const u32 *>(pArrCode.data());
	
	VK_CHECK_RESULT(
		vkCreateShaderModule(
			pDevice.GetVkNative(),
			&shaderCreateInfo,
			nullptr,
			&pVkShaderModule
		)
	);
}

void Shader::Destroy()
{
	if (pVkShaderModule != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(pDevice.GetVkNative(), pVkShaderModule, nullptr);
		pVkShaderModule = VK_NULL_HANDLE;
	}
}

bool Shader::IsValid() const
{
	return pVkShaderModule != VK_NULL_HANDLE;
}

VkShaderModule Shader::GetVkNative() const
{
	return pVkShaderModule;
}

void Shader::SetCode(const Vec<i8> &code)
{
	pArrCode = code;
}

const Vec<i8> &Shader::GetCode() const
{
	return pArrCode;
}

const VkPipelineShaderStageCreateInfo &Shader::GetStageCreateInfo()
{
	VkPipelineShaderStageCreateInfo stageCreateInfo = {};
	stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stageCreateInfo.stage = eStage;
	stageCreateInfo.module = pVkShaderModule;
	stageCreateInfo.pName = "main";
	return stageCreateInfo;
}

const Vec<i8> &Shader::ReadFile(const string &filename)
{
	ifstream file(filename, ios::ate | ios::binary);
	if (!file.is_open())
	{
		throw runtime_error("Failed to open file: " + filename);
	}
	size_t fileSize = static_cast<size_t>(file.tellg());
	Vec<i8> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}
