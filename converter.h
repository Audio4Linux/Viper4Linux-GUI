#ifndef CONVERTER_H
#define CONVERTER_H
#include <iostream>
class converter
{
public:
    enum mode{
        officialV4A,
        teamDeWittV4A
    };
    converter();
    static std::string read(std::string,mode);
private:
    std::string intToString(int);
};

#endif // CONVERTER_H
