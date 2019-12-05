#include <iostream>
#include "USBInterface.h"
#include <fstream> 
#include <math.h>
#include <sstream>
#include <bitset>

using namespace std;

void processData(string s, ofstream &o, ofstream &oBinary);

int main()
{
    unsigned int receiveCounter = 1;
    cout << "USB test program" << endl;
    ofstream ofile;
    ofstream ofileBinary; 
    USBInterface dev("/dev/ttyACM1");

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
            filename.clear();
            filename.str("");
            filename << "./receive/receiveBin" << receiveCounter << ".csv";
            ofileBinary.open(filename.str(), ios::out);
                        
            openFile = true;
            receiveCounter++;
            sampleNumber = 0;
            sval.erase();
        }
        
        if(sval.find("Stop") != string::npos && openFile)
        {
            cout << "Found Stop" << endl;
            processData(sval, ofile, ofileBinary);
            ofile.close();
            ofileBinary.close();
            openFile = false;
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


void processData(string s, ofstream &o, ofstream &oBinary)
{
    s.erase(s.find("Stop"));
    unsigned int count = 0;
    int value; 
    for(int i = 0; i < s.size(); i+=2)
    {
        char c1 = s[i];
        char c2 = s[i+1];
        bitset<8> B1(c1);
        bitset<8> B2(c2);
        
        
        value = (unsigned int) ((c1 & 0x0F)  << 8) ; // high byte 
        value += c2; //low byte
        
        o << count << ", " << (double)value*3.3/pow(2,12) << endl;
        oBinary << count << ", " <<  B1 << ", " << B2 << endl;
        
        count ++;
        
    }
}
