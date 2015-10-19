#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    string result = "";
    for (int i = 0; i < numbers.size(); i++)
    {
        string temp = convertNumberToBitString(numbers[i]);
        for (int j = 0; j < temp.length(); j++)
        {
            if (temp[j] == '1')
                result += "\t";
            else
                result += " ";
        }
    }
    return result;
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
    
    int counter = 0;
    string stemp = "";
    numbers.clear();
    if(bitString.size()%16 != 0) return false;
    for (int i = 0; i < bitString.length(); i++)
    {
        if (bitString[i] == ' ')
        {
            counter++;
            stemp += '0';
        }
        else if (bitString[i] == '\t')
        {
            counter++;
            stemp += '1';
        }
        else
            return false;
        unsigned short temp = 0;
        if (counter == 16)
        {
            if (convertBitStringToNumber(stemp, temp))
                numbers.push_back(temp);
            counter = 0;
            stemp = "";
        }
    }
    return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
