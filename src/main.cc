#include <iostream>
#include "USBInterface.h"
#include <fstream> 
#include <math.h>
#include <sstream>
#include <bitset>

using namespace std;

int main()
{
    unsigned int receiveCounter = 1;
    cout << "USB test program" << endl;
    ofstream ofile;
    USBInterface dev("/dev/ttyACM2");

    double max = 0.0;
    double min = 0.0;
    
    bool openFile = false;
    unsigned long sampleNumber = 0;
    
    string sval;
    while(1)
    {
        sval += dev.readFromDev();
        if(sval.find("Start") != string::npos)
        {
            if(openFile == true)
            {
                cout << "Received " << sampleNumber << " samples" << endl;
                cout << "no stop found!!!" << endl;
                ofile.close();
            }
            cout << "receiving file number:  " << receiveCounter << endl;
            stringstream filename;
            filename << "./receive/receive" << receiveCounter << ".csv";
            ofile.open(filename.str(), ios::out);
            openFile = true;
            receiveCounter++;
            sampleNumber = 0;
            sval.erase();
            continue;
        }
        else if(sval.find("Stop") != string::npos) 
        {
            cout << "Received " << sampleNumber << " samples" << endl;
            ofile.close();
            openFile = false;
            sval.erase();
            continue;
        }
        
        if(openFile && sval.size() != 0)
        {
            for(int i = 0; i < sval.size(); i+=2)
            {
                long value;
                char c1, c2; 
                
                c1 = sval[i];
                c2 = sval[i+1];
                
                if( (c1 & 0xC0)  | (c2 & 0x03) != 0)
                {
                    bitset<8> x(c1);
                    cout << "c1: " << x;
                    bitset<8> x2(c2);
                    cout << "\tc2: " << x2;
                    cout << "\t\tsample: " << sampleNumber+1 << endl;
                    i++;
                    continue;
                }
                
                value = ((( int)c1 & 0x0F)  * 256) ; // high byte 
                value += c2; //low byte
                if(value > 16000)
                {
                    cout << "Value: " << value << endl;
                    cout << "C1: " << ( int) c1 << "\t val: " << ((( int)c1 & 0x0F)  * 256) << endl;
                    cout << "C2: " << ( int) c2 << "\t val: " << ( int)c2 << endl;
                }
                ofile << sampleNumber << ", " << (double)value*3.3/pow(2,12) - 1.65 << endl;
                sampleNumber++;
            }
        }
        
    }
    /*
    for(int i = 0; i < 10000; i++)
    {
        unsigned int value;
        string sval = dev.readFromDev();
        int count = 0;
        for(int j = 0; j < sval.size(); j++)
        {
            if(sval[j] == '\n') count++;
        }
        if(count > 1)
        {
            cout << "multiple lines read: \n" << sval << endl;
        }
        char c1, c2; 
        c1 = sval[0];
        c2 = sval[1];
        value = (unsigned int) ((c1 & 0x0F)  << 8) ; // high byte 
        value += c2; //low byte
        ofile << i << ", " << (double)value*3.3/pow(2,12) - 1.6 << "\r\n";
        if(value > max) max = value;
        else if(value < min) min = value;
    }*/
    ofile.close();
    cout << "Max Value: " << max << endl;
    cout << "Min Value: " << min << endl;
    return 0;
    while(1)
    {
        int value;
        string sval = dev.readFromDev();
        char c1, c2; 
        c1 = sval[0];
        c2 = sval[1];
        value = ((unsigned int) c1 << 8) + (unsigned int)c2; 
        //cout << "Rx:  " << sval << endl;
        cout << "Val: " << (double)value*3.3/pow(2,12) - 1.6 << endl; 
        cout << flush;
    }
}
