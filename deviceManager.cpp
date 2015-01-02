#include <deviceManager.h>

DeviceManager* DeviceManager::instance = NULL;

DeviceManager::DeviceManager(void)
{

}

DeviceManager* DeviceManager::getInstance(void)
{
    if(instance == NULL)
    {
        instance = new DeviceManager();
    }
    return instance;
}
