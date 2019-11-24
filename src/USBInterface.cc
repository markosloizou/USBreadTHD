#include "USBInterface.h"

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <libexplain/read.h> // explain_read
#include <algorithm>
#include <bitset>

#define BUFFER_LENGTH 4096

USBInterface::USBInterface(std::string devPath)
{
    dev = -1;
    dev = open(devPath.c_str(), O_RDWR);
    if(dev == -1)
    {
        std::cout << "Error: Could not open device" << std::endl;
//         abort();
    }
    else
    {
        std::cout << "Dev succesfully opened with devicce descriptor: " << dev << std::endl;
    }
}

void USBInterface::openDev(std::string devPath)
{
    dev = open(devPath.c_str(), O_RDWR );
    if(dev == -1)
    {
        std::cout << "Error: Could not open device" << std::endl;
        //         abort();
    }
}

bool USBInterface::writeToDev(std::string text)
{
    write(dev, (const void *) text.c_str(), (size_t)text.size());
}

std::string USBInterface::readFromDev(void)
{
    char *buffer = new char[BUFFER_LENGTH];
    ssize_t rd = 0;
    do{
        rd = read(dev, buffer, BUFFER_LENGTH);
    } while(rd == 0);
    
    if (rd < 0)
    {
        std::cout << "Error: " <<  explain_read(dev, buffer, BUFFER_LENGTH) << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string str;
    for (int i = 0; i < rd; i++) 
    { 
        str = str + buffer[i]; 
    } 
    /*
    for(int i = 0 ; i < rd; i++)
    {
        
        std::bitset<8>  x(str[i]);
        std::cout << "byte: "<< i << "\t" << x << std::endl;
    }*/
    //std::cout << "rd: " << rd << std::endl;
    //std::cout << "str: " << str.length() << std::endl;
    if(rd > 2) 
    {
        int del = 0;
        if(str[rd-1] == '\n') del = 1;
        if(str[rd-2] == '\r') del += 2;
        if(del == 2) str.erase(str.length()-2);
    }
    
    //std::cout << "str: " << str << std::endl;
    //str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    //str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}
