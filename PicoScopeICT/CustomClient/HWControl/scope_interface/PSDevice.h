#pragma once

class PSDevice {
public:
    PSDevice();
    ~PSDevice();

    [[nodiscard]] bool IsDeviceOpen() const;
    void OpenDevice();
    void CloseDevice();

private:
    bool test_device_open;
};
