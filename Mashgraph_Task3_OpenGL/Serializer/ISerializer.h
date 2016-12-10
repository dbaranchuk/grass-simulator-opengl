#ifndef ISERIALIZER_H
#define ISERIALIZER_H

#include <vector>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

typedef unsigned int uint;

class ISerializer
{
public:
    ISerializer() {}
    virtual ~ISerializer() {}

    virtual bool InOut(uint &) = 0;
    virtual bool InOut(float &) = 0;
    virtual bool InOut(std::vector<float> &) = 0;
    virtual bool InOut(std::vector<VM::vec2> &) = 0;
    virtual bool InOut(std::vector<VM::vec3> &) = 0;
    virtual bool InOut(std::vector<VM::vec4> &) = 0;
};

#endif // ISERIALIZER_H
