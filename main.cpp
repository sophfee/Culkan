/*
 * Vulkan Program
 *
 * Copyright (C) 2016 Valve Corporation
 * Copyright (C) 2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
Vulkan Project Template
Create and destroy a simple Vulkan instance.
*/

#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>

#include "Instance.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "Surface.h"
#include "SwapChain.h"
#include "Pipeline.h"

int main()
{
	Instance &instance = Singleton<Instance>::GetInstance();
    //instance.AddAllExtensions();
	instance.AddRequiredExtensions();
	instance.Create();

	Vec<Ref<PhysicalDevice>> ppPhysicalDevices = instance.GetPhysicalDevices();
	Ref<PhysicalDevice> pPhysicalDevice = ppPhysicalDevices[0];
	u32 score = 0;
	for (auto &pPhysicalDevice : ppPhysicalDevices)
	{
		u32 testScore = pPhysicalDevice->RateSuitability();
		if (score > pPhysicalDevice->RateSuitability())
		{
			pPhysicalDevice = pPhysicalDevice;
			score = testScore;
		}
	}

	// Create the device
	Ref<Device> pDevice = pPhysicalDevice->CreateDevice();

	// Create the surface
	Ref<Surface> pSurface = make_shared<Surface>(instance);
	pSurface->Create();

	// Create the swap chain
	Ref<SwapChain> pSwapChain = make_shared<SwapChain>(*pDevice, *pSurface);
	pSwapChain->Create();

	// Create the pipeline
	Ref<Pipeline> pPipeline = make_shared<Pipeline>(*pDevice, *pSurface);
	pPipeline->Create();

	// Destroy the pipeline
	pPipeline->Destroy();	

	std::cout << "Physical Device: " << pPhysicalDevice->GetVkNative() << std::endl;

    return 0;
}
