#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <string>


class USBInterface{
    
public:
    USBInterface(std::string devPath);
    void openDev(std::string devPath);
    bool writeToDev(std::string text);
    std::string readFromDev(void);
private:
    int dev;
    
};

#endif
