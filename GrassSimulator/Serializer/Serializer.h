#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <fstream>
#include <queue>
#include "ISerializer.h"

class BinarySerializerWriter: public ISerializer
{
    std::ofstream fout;

public:
    BinarySerializerWriter(const char *);
    ~BinarySerializerWriter();

    bool InOut(uint &);
    bool InOut(float &);
    bool InOut(std::vector<float> &);
    bool InOut(std::vector<VM::vec2> &);
    bool InOut(std::vector<VM::vec3> &);
    bool InOut(std::vector<VM::vec4> &);
};

class BinarySerializerReader: public ISerializer
{
    std::ifstream fin;

public:
    BinarySerializerReader(const char *);
    ~BinarySerializerReader();

    bool InOut(uint &);
    bool InOut(float &);
    bool InOut(std::vector<float> &);
    bool InOut(std::vector<VM::vec2> &);
    bool InOut(std::vector<VM::vec3> &);
    bool InOut(std::vector<VM::vec4> &);
};


class TextSerializerWriter: public ISerializer
{
    std::ofstream fout;

public:
    TextSerializerWriter(const char *);
    ~TextSerializerWriter();

    bool InOut(uint &);
    bool InOut(float &);
    bool InOut(std::vector<float> &);
    bool InOut(std::vector<VM::vec2> &);
    bool InOut(std::vector<VM::vec3> &);
    bool InOut(std::vector<VM::vec4> &);
};


class TextSerializerReader: public ISerializer
{
    std::ifstream fin;

public:
    TextSerializerReader(const char *);
    ~TextSerializerReader();

    bool InOut(uint &);
    bool InOut(float &);
    bool InOut(std::vector<float> &);
    bool InOut(std::vector<VM::vec2> &);
    bool InOut(std::vector<VM::vec3> &);
    bool InOut(std::vector<VM::vec4> &);
};


#endif // SERIALIZER_H
