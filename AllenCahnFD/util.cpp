/*util.cpp*/
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>

#include "util.hpp"

using std::cout;
using std::endl;

template <typename Type>
int write2bin(std::string filename, Type * arr, unsigned int n)
{
	std::FILE * pFile;
    pFile = std::fopen(filename.c_str(), "wb");
    if (pFile==NULL)
    {
        cout << "Error opening file " << filename << "!" << endl;
        return -1;
    }
    cout << "Writing output to file " << filename << endl;
    std::fwrite(arr, sizeof(Type), n, pFile);
    std::fclose(pFile);
    cout << "Done\n";
    return 0;
}

std::string time2fname(const std::string & prefix, const unsigned int & t, const std::string & suffix)
{
	std::ostringstream current_t;
    current_t << t;
    return prefix + current_t.str() + suffix;
}

template int write2bin<int>(std::string, int *, unsigned int);
template int write2bin<float>(std::string, float *, unsigned int);
template int write2bin<double>(std::string, double *, unsigned int);

template int write2bin<const int>(std::string, const int *, unsigned int);
template int write2bin<const float>(std::string, const float *, unsigned int);
template int write2bin<const double>(std::string, const double *, unsigned int);