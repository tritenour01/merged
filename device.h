#ifndef DEVICE_H_INCLUDED
#define DEVICE_H_INCLUDED

#include <connection.h>

class Device
{
    public:

        Device(void);

    private:

        QString name;

        enum OS {WINDOWS, UNIX, MAC};
        OS os;

        int cores;
        QString status;

        Connection* connection;
};

#endif // DEVICE_H_INCLUDED
