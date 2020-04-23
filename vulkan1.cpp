
#include <vulkan.h>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// #define P_(s, e, os) (os << #e << s.##e)
#define P_(a, b, c, i) (os << i << #b << ": " << a.b << '\n')

void PrintPhysicalDevicePros(const VkPhysicalDeviceProperties& p, ostream& os,
                             int indent = 0) {
    const string i(indent, ' ');
    P_(p, apiVersion, os, i);
    P_(p, driverVersion, os, i);
    P_(p, vendorID, os, i);
    P_(p, deviceID, os, i);
    P_(p, deviceType, os, i);
    P_(p, deviceName, os, i);
    // PrintPhysicalDeviceLimits(p.limts, os, indent);
    // PrintPhysicalDeviceSparseProperties(p.sparseProperties);
}

tuple<VkResult, VkInstance, vector<VkPhysicalDevice>> Init() {
    VkResult result = VK_SUCCESS;

    VkApplicationInfo appInfo = {};

    VkInstanceCreateInfo instanceCreateInfo = {};
    ;

    // A generic application info structure

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    appInfo.pApplicationName = "Application";

    appInfo.applicationVersion = 1;

    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    // Create the instance.

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    instanceCreateInfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    vector<VkPhysicalDevice> physicalDevices;
    if (result == VK_SUCCESS)

    {
        // First figure out how many devices are in the system.

        uint32_t physicalDeviceCount = 0;

        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

        if (result == VK_SUCCESS)

        {
            // Size the device array appropriately and get the physical

            // device handles.

            physicalDevices.resize(physicalDeviceCount);

            vkEnumeratePhysicalDevices(instance,

                                       &physicalDeviceCount,

                                       &physicalDevices[0]);
        }
    }

    return {result, instance, physicalDevices};
}

int main(int argc, char const* argv[]) {
    auto [result, instance, physicalDevices] = Init();
    if (result != VK_SUCCESS) {
        cerr << "Error: " << result << endl;
    }
    VkPhysicalDeviceProperties props;
    for (const auto& d : physicalDevices) {
        cout << string(10, '=') << endl;
        vkGetPhysicalDeviceProperties(d, &props);
        PrintPhysicalDevicePros(props, cout);
    }
    return 0;
}
