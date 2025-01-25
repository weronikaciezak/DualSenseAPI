#include <windows.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <iostream>

#define PS5_VENDOR_ID  0x054C  // Sony
#define PS5_PRODUCT_ID 0x0CE6  // DualSense

HANDLE OpenDualSense() {
    GUID hidGuid;
    HidD_GetHidGuid(&hidGuid);

    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&hidGuid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "No HID devices found!" << std::endl;
        return nullptr;
    }

    SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {};
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(deviceInfoSet, nullptr, &hidGuid, i, &deviceInterfaceData); i++) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, nullptr, 0, &requiredSize, nullptr);

        auto* deviceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA*)malloc(requiredSize);
        if (!deviceDetailData) continue;
        deviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceDetailData, requiredSize, nullptr, nullptr)) {
            HANDLE deviceHandle = CreateFile(deviceDetailData->DevicePath, GENERIC_READ | GENERIC_WRITE,
                                             FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

            free(deviceDetailData);

            if (deviceHandle != INVALID_HANDLE_VALUE) {
                HIDD_ATTRIBUTES attributes = {};
                attributes.Size = sizeof(HIDD_ATTRIBUTES);
                if (HidD_GetAttributes(deviceHandle, &attributes) && attributes.VendorID == PS5_VENDOR_ID && attributes.ProductID == PS5_PRODUCT_ID) {
                    SetupDiDestroyDeviceInfoList(deviceInfoSet);
                    return deviceHandle;
                }
                CloseHandle(deviceHandle);
            }
        } else {
            free(deviceDetailData);
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return nullptr;
}

void ReadDualSenseButtons(HANDLE deviceHandle) {
    unsigned char buffer[64] = {0};
    unsigned char lastState[3] = {0}; // Bytes 8, 9, and 10
    unsigned char lastLeftStick[2] = {128, 128}; // Bytes 1 and 2
    unsigned char lastRightStick[2] = {128, 128}; // Bytes 3 and 4

    while (true) {
        DWORD bytesRead;
        if (ReadFile(deviceHandle, buffer, sizeof(buffer), &bytesRead, nullptr)) {
            if (buffer[8] != lastState[0] || buffer[9] != lastState[1] || buffer[10] != lastState[2]) {
                std::cout << "Buttons: ";

                // D-Pad and main buttons
                switch (buffer[8] & 0x0F) {
                    case 0x0: std::cout << "[DPad Up] "; break;
                    case 0x1: std::cout << "[DPad Up-Right] "; break;
                    case 0x2: std::cout << "[DPad Right] "; break;
                    case 0x3: std::cout << "[DPad Down-Right] "; break;
                    case 0x4: std::cout << "[DPad Down] "; break;
                    case 0x5: std::cout << "[DPad Down-Left] "; break;
                    case 0x6: std::cout << "[DPad Left] "; break;
                    case 0x7: std::cout << "[DPad Up-Left] "; break;
                    case 0x8: std::cout << "[DPad Neutral] "; break;
                }

                if (buffer[8] & 0x10) std::cout << "[Square] ";
                if (buffer[8] & 0x20) std::cout << "[Cross] ";
                if (buffer[8] & 0x40) std::cout << "[Circle] ";
                if (buffer[8] & 0x80) std::cout << "[Triangle] ";

                // Additional buttons
                if (buffer[9] & 0x01) std::cout << "[L1] ";
                if (buffer[9] & 0x02) std::cout << "[R1] ";
                if (buffer[9] & 0x04) std::cout << "[L2] ";
                if (buffer[9] & 0x08) std::cout << "[R2] ";
                if (buffer[9] & 0x10) std::cout << "[Create] ";
                if (buffer[9] & 0x20) std::cout << "[Options] ";
                if (buffer[9] & 0x40) std::cout << "[L3] ";
                if (buffer[9] & 0x80) std::cout << "[R3] ";

                // System buttons
                if (buffer[10] & 0x01) std::cout << "[PS] ";
                if (buffer[10] & 0x02) std::cout << "[Touchpad] ";
                if (buffer[10] & 0x04) std::cout << "[Mute] ";

                std::cout << std::endl;

                // Update state
                lastState[0] = buffer[8];
                lastState[1] = buffer[9];
                lastState[2] = buffer[10];
            }
            if (buffer[1] != lastLeftStick[0] || buffer[2] != lastLeftStick[1]) {
                std::cout << "Left Stick - X: " << static_cast<int>(buffer[1]) << ", Y: " << static_cast<int>(buffer[2]) << std::endl;
                lastLeftStick[0] = buffer[1];
                lastLeftStick[1] = buffer[2];
            }
            if (buffer[3] != lastRightStick[0] || buffer[4] != lastRightStick[1]) {
                std::cout << "Right Stick - X: " << static_cast<int>(buffer[3]) << ", Y: " << static_cast<int>(buffer[4]) << std::endl;
                lastRightStick[0] = buffer[3];
                lastRightStick[1] = buffer[4];
            }
        } else {
            std::cerr << "Read error!" << std::endl;
            break;
        }
    }
}

int main() {
    HANDLE deviceHandle = OpenDualSense();
    if (!deviceHandle) {
        std::cerr << "DualSense not found!" << std::endl;
        return -1;
    }

    std::cout << "DualSense connected! Reading button inputs..." << std::endl;
    ReadDualSenseButtons(deviceHandle);

    CloseHandle(deviceHandle);
    return 0;
}