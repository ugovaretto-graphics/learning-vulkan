
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

#define P_(s, member, os, indent) \
    (os << string(indent, ' ') << #member << ": " << s.member << '\n')

//------------------------------------------------------------------------------
void PrintPhysicalDeviceMainProps(const VkPhysicalDeviceProperties& p,
                                  ostream& os, int indent = 0) {
    P_(p, apiVersion, os, indent);
    P_(p, driverVersion, os, indent);
    P_(p, vendorID, os, indent);
    P_(p, deviceID, os, indent);
    P_(p, deviceType, os, indent);
    P_(p, deviceName, os, indent);
    // PrintPhysicalDeviceLimits(p.limts, os, indent);
    // PrintPhysicalDeviceSparseProperties(p.sparseProperties);
}

//------------------------------------------------------------------------------
// queue families, device memory
void PrintPhysicalDeviceOtherProperties(const VkPhysicalDevice& d,
                                        ostream& os) {
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(d, &physicalDeviceMemoryProperties);

    uint32_t queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(d, &queueFamilyPropertyCount,
                                             nullptr);
    vector<VkQueueFamilyProperties> queueFamilyProperties(
        queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(d, &queueFamilyPropertyCount,
                                             queueFamilyProperties.data());
    throw logic_error("To be completed");
}

//------------------------------------------------------------------------------
tuple<VkResult, VkDevice> CreateLogicalDevice(const VkPhysicalDevice& dev) {
    VkResult result;
    VkPhysicalDeviceFeatures supportedFeatures;
    VkPhysicalDeviceFeatures requiredFeatures = {};
    vkGetPhysicalDeviceFeatures(dev, &supportedFeatures);
    requiredFeatures.multiDrawIndirect = supportedFeatures.multiDrawIndirect;
    requiredFeatures.tessellationShader = VK_TRUE;
    requiredFeatures.geometryShader = VK_TRUE;
    const VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,  // sType
        nullptr,                                     // pNext
        0,                                           // flags
        0,                                           // queueFamilyIndex
        1,                                           // queueCount
        nullptr                                      // pQueuePriorities
    };
    const VkDeviceCreateInfo deviceCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,  // sType
        nullptr,                               // pNext
        0,                                     // flags
        1,                                     // queueCreateInfoCount
        &deviceQueueCreateInfo,                // pQueueCreateInfos
        0,                                     // enabledLayerCount
        nullptr,                               // ppEnabledLayerNames
        0,                                     // enabledExtensionCount
        nullptr,                               // ppEnabledExtensionNames
        &requiredFeatures,                     // pEnabledFeatures
    };
    VkDevice logicalDevice;
    result = vkCreateDevice(dev, &deviceCreateInfo, nullptr, &logicalDevice);
    return {result, logicalDevice};
}

//------------------------------------------------------------------------------
tuple<VkResult, VkInstance, vector<VkPhysicalDevice>> Init() {
    VkResult result = VK_SUCCESS;

    VkApplicationInfo appInfo = {};

    VkInstanceCreateInfo instanceCreateInfo = {};

    // A generic application info structure

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    appInfo.pApplicationName = "Application";

    appInfo.applicationVersion = 1;

    appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

    // Create the instance.

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    instanceCreateInfo.pApplicationInfo = &appInfo;

    instanceCreateInfo.ppEnabledExtensionNames = nullptr;

    instanceCreateInfo.ppEnabledLayerNames = nullptr;

    VkInstance instance;
    result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    vector<VkPhysicalDevice> physicalDevices;
    if (result == VK_SUCCESS) {
        // First figure out how many devices are in the system.

        uint32_t physicalDeviceCount = 0;

        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

        if (result == VK_SUCCESS) {
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

//------------------------------------------------------------------------------
void PrintGlobalLayerProps(ostream& os, int indent = 0) {
    uint32_t count{0};
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    vector<VkLayerProperties> props(count);
    vkEnumerateInstanceLayerProperties(&count, props.data());
    string i(indent, ' ');
    for (const auto& p : props) {
        os << string(10, '=') + '\n';
        P_(p, description, os, indent);
        P_(p, implementationVersion, os, indent);
        P_(p, layerName, os, indent);
        P_(p, specVersion, os, indent);
    }
}

//------------------------------------------------------------------------------
void PrintInstanceExtensionProps(ostream& os, int indent = 0) {
    uint32_t numInstanceExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions,
                                           nullptr);
    vector<VkExtensionProperties> instanceExtensionProperties(
        numInstanceExtensions);

    vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions,
                                           instanceExtensionProperties.data());
    for (const auto& p : instanceExtensionProperties) {
        P_(p, extensionName, os, indent);
    }
}

//------------------------------------------------------------------------------
void PrintExtensionProcAddresses(VkInstance instance, ostream& os) {
    uint32_t numInstanceExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions,
                                           nullptr);
    vector<VkExtensionProperties> instanceExtensionProperties(
        numInstanceExtensions);

    vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions,
                                           instanceExtensionProperties.data());
    os << hex;
    for (const auto& p : instanceExtensionProperties) {
        PFN_vkVoidFunction pf =
            vkGetInstanceProcAddr(instance, p.extensionName);
        os << p.extensionName << ": " << pf << endl;
    }
    os << dec;
}

//------------------------------------------------------------------------------
// void PrintDeviceProcAddresses(VkDevice dev, ostream& os) {
//     uint32_t numInstanceExtensions = 0;
//     vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions,
//                                            nullptr);
//     vector<VkExtensionProperties> instanceExtensionProperties(
//         numInstanceExtensions);

//     vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions,
//                                            instanceExtensionProperties.data());
//     os << hex;

//     for (const auto& p : instanceExtensionProperties) {
//         PFN_vkVoidFunction pf = vkGetDeviceProcAddr(dev, p.extensionName);
//         os << p.extensionName << ": " << pf << endl;
//     }

//     os << dec;
// }


//------------------------------------------------------------------------------
int main(int argc, char const* argv[]) {
    auto [result, instance, physicalDevices] = Init();
    if (result != VK_SUCCESS) {
        cerr << "Error: " << result << endl;
    }
    VkPhysicalDeviceProperties props;
    cout << "Physical device properties:\n\n";
    for (const auto& d : physicalDevices) {
        cout << string(10, '=') << endl;
        vkGetPhysicalDeviceProperties(d, &props);
        PrintPhysicalDeviceMainProps(props, cout);
    }
    cout << string(10, '=') + "\n\n"
         << "Global layer properties:\n\n";
    PrintGlobalLayerProps(cout);
    PrintInstanceExtensionProps(cout);
    PrintExtensionProcAddresses(instance, cout);
    return 0;
}
