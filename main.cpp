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

int main()
{
	Instance &instance = Singleton<Instance>::GetInstance();
    instance.AddAllExtensions();
	instance.Create();

	Vec<Ref<PhysicalDevice >> ppPhysicalDevices = instance.GetPhysicalDevices();
	for (auto &pPhysicalDevice : ppPhysicalDevices)
	{
		VkPhysicalDeviceProperties properties = pPhysicalDevice->GetProperties();
		cout << "Device Name: " << properties.deviceName << endl;
	}

    return 0;
}
