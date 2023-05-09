#include "packet.h"

QString NetworkInfo::getIp()
{
    return QString("%1.%2.%3.%4")
            .arg((ip >>  0) & 0xFF)
            .arg((ip >>  8) & 0xFF)
            .arg((ip >> 16) & 0xFF)
            .arg((ip >> 24) & 0xFF);
}

QString NetworkInfo::getMac()
{
    return QString::asprintf("%02X%02X%02X%02X%02X%02X",
                             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

bool NetworkInfo::getStaticIP()
{
    return static_ip;
}

bool NetworkInfo::getValid()
{
    return valid;
}
