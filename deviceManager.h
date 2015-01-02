#ifndef DEVICEMANAGER_H_INCLUDED
#define DEVICEMANAGER_H_INCLUDED

#include <vector>
#include <device.h>

#include <cstdlib>

class DeviceManager
{
    public:

        static DeviceManager* getInstance(void);

    private:

        DeviceManager(void);
        static DeviceManager* instance;

        std::vector<Device*> devices;
};

#endif // DEVICEMANAGER_H_INCLUDED
