/*util.hpp*/
#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>


template <typename Type>
int write2bin(std::string filename, Type * arr, unsigned int n);

std::string time2fname(const std::string & prefix, const unsigned int & t, const std::string & suffix = ".bin");

#endif