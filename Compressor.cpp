#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


unsigned int computeHash(string key) { 
    if (key.length()!= 0)
        return key[0] + 11;
    return 0;
}

unsigned int computeHash(unsigned short key) {
    return (key+3)/(key+1);
}

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    int n = int(s.length());
    int c = 0;
    unsigned short temp;
    if (n/2 + 512 < 16384)
        c = n/2 + 512;
    else
        c = 16384;
    HashTable <string, unsigned short> *hashtable = new HashTable<string, unsigned short>(c/0.5, c);
    for (int i = 0; i < 256; i++)
        hashtable->set(*new string(1, static_cast<char>(i)), i, true);
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    vector<unsigned short> v;
    for (int i = 0; i < s.length(); i++)
    {
        string expandedRun = runSoFar + s[i];
        if (hashtable->get(expandedRun, temp))
        {
            runSoFar = expandedRun;
            continue;
        }
        else
        {
            hashtable->get(runSoFar, temp);
            v.push_back(temp);
            hashtable->touch(runSoFar);
            runSoFar = "";
            hashtable->get(*new string(1, static_cast<char>(s[i])), temp);
            v.push_back(temp);
            if (!hashtable->isFull()) {
                hashtable->set(expandedRun, nextFreeID);
                nextFreeID++;
            }
            else {
                string str = "";
                unsigned short id = 0;
                hashtable->discard(str, id);
                hashtable->set(expandedRun, id);
            }
        }
    }
    if (runSoFar.length()!= 0)
    {
        hashtable->get(runSoFar, temp);
        v.push_back(temp);
    }
    v.push_back(c);
    numbers = v;
	  // This compiles, but may not be correct
}

            
bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    unsigned short c = numbers[numbers.size()-1];
    HashTable <unsigned short, string> *hashtable = new HashTable<unsigned short, string>(c/0.5, c);
    for (int i = 0; i < 255; i++)
    {
        hashtable->set(i, *new string(1, static_cast<char>(i)));
    }
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    string output;
    for (int i = 0; i < numbers.size()-1; i++)
    {
        unsigned short us = numbers[i];
        string temp = "";
        if (us <= 255)
        {
            hashtable->get(us, temp);
            output+= temp;
            if (runSoFar.length() == 0) {
                runSoFar = temp;
                continue;
            }
            else {
                string expandedRun = runSoFar + temp;
                if (!hashtable->isFull()) {
                    hashtable->set(nextFreeID, expandedRun);
                    nextFreeID++;
                }
                else {
                    unsigned short sh;
                    string str;
                    hashtable->discard(sh, str);
                    hashtable->set(sh, expandedRun);
                }
                runSoFar = "";
                continue;
            }
        }
        else {
            string temp = "";
            if (!hashtable->get(us, temp))
                return false;
            else
            {
                hashtable->touch(us);
                output += temp;
                runSoFar = temp;
            }
        }
    }
    s = output;
	return true;  // This compiles, but may not be correct
}
