#ifndef CONVERTER_H
#define CONVERTER_H
#include <iostream>
class converter
{
public:
    converter();
    static std::string read(std::string);
private:
    std::string intToString(int);
};


#endif // CONVERTER_H
