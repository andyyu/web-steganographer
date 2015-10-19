#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn.length() == 0)
        return false;
    else{
        vector<unsigned short> v;
        Compressor::compress(msg, v);
        string message = BinaryConverter::encode(v);
        vector<string> lines;
        string temp = hostIn;
        while (temp.find("\n") != string::npos)
        {
            unsigned long pos = temp.find("\r\n");
            unsigned long pos2 = temp.find("\n");
            if (pos < pos2)
            {
                lines.push_back(temp.substr(0, pos));
                temp.erase(temp.begin(), temp.begin()+pos+2);
            }
            else
            {
                lines.push_back(temp.substr(0, pos2));
                temp.erase(temp.begin(), temp.begin()+pos2+1);
            }
        }
        if (temp.length() != 0)
            lines.push_back(temp.substr(0, temp.length()));
        for (int i = 0; i < lines.size(); i++)
        {
            int j = int(lines[i].size()-1);
            while (j > 0 && (lines[i][j] == ' ' || lines[i][j] == '\t'))
            {
                lines[i] = lines[i].substr(0, j);
                j--;
            }
        }
        int k = 0;
        for (int j = 0; j < lines.size(); j++)
        {
            if (j < message.length()%lines.size())
            {
                for (int m = k; m < k + message.length()/lines.size()+1; m++)
                    lines[j] += message[m];
                k = k + message.length()/lines.size()+1;
            }
            else
            {
                for (int m = k; m < k + message.length()/lines.size(); m++)
                    lines[j] += message[m];
                k = k + message.length()/lines.size();
            }
        }
        hostOut = "";
        for (int i = 0; i < lines.size(); i++)
        {
            hostOut += lines[i] + '\n';
        }
    }
    return true;
}


bool Steg::reveal(const string& hostIn, string& msg) 
{
    vector<string> lines;
    string temp = hostIn;
    msg = "";
    while (temp.find('\n') != std::string::npos)
    {
        lines.push_back(temp.substr(0, temp.find('\n')));
        temp.erase(temp.begin(), temp.begin()+temp.find('\n')+1);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        int j = int(lines[i].size()-1);
        while (j >= 0 && (lines[i][j] == ' ' || lines[i][j] == '\t'))
        {
            j--;
        }
        msg+= lines[i].substr(j+1, lines[i].size()-j-1);
    }
    vector<unsigned short> dec;
    if (BinaryConverter::decode(msg, dec))
        {
            if (!Compressor::decompress(dec, msg))
                return false;
        }
    else
        return false;
    return true;
}
