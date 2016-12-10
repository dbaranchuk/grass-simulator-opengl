#include "Serializer.h"

/* BinarySerializerWriter */

BinarySerializerWriter::BinarySerializerWriter(const char *path)
{
    fout.open(path, std::ios::binary | std::ios::out);
}

BinarySerializerWriter::~BinarySerializerWriter()
{
    fout.close();
}

bool BinarySerializerWriter::InOut(float &x)
{
    fout.write((char *)(&x), sizeof(x));
    return true;
}

bool BinarySerializerWriter::InOut(uint &x)
{
    fout.write((char *)(&x), sizeof(x));
    return true;
}

bool BinarySerializerWriter::InOut(std::vector<float> &vec)
{
    size_t size = vec.size();
    fout.write((char *)(&size), sizeof(size));

    for (uint i = 0; i < size; i++) {
        fout.write((char *)(&vec[i]), sizeof(vec[i]));
    }
    return true;
}

bool BinarySerializerWriter::InOut(std::vector<VM::vec2> &vec)
{
    size_t size = vec.size();
    fout.write((char *)(&size), sizeof(size));

    for (uint i = 0; i < size; i++) {
        fout.write((char *)(&vec[i].x), sizeof(vec[i].x));
        fout.write((char *)(&vec[i].y), sizeof(vec[i].y));
    }
    return true;
}

bool BinarySerializerWriter::InOut(std::vector<VM::vec3> &vec)
{
    size_t size = vec.size();
    fout.write((char *)(&size), sizeof(size));

    for (uint i = 0; i < size; i++) {
        fout.write((char *)(&vec[i].x), sizeof(vec[i].x));
        fout.write((char *)(&vec[i].y), sizeof(vec[i].y));
        fout.write((char *)(&vec[i].z), sizeof(vec[i].z));
    }
    return true;
}

bool BinarySerializerWriter::InOut(std::vector<VM::vec4> &vec)
{
    size_t size = vec.size();
    fout.write((char *)(&size), sizeof(size));

    for (uint i = 0; i < size; i++) {
        fout.write((char *)(&vec[i].x), sizeof(vec[i].x));
        fout.write((char *)(&vec[i].y), sizeof(vec[i].y));
        fout.write((char *)(&vec[i].z), sizeof(vec[i].z));
        fout.write((char *)(&vec[i].w), sizeof(vec[i].w));
    }
    return true;
}


/* BinarySerializerReader */

BinarySerializerReader::BinarySerializerReader(const char *path)
{
    fin.open(path, std::ios::binary | std::ios::in);
}

BinarySerializerReader::~BinarySerializerReader()
{
    fin.close();
}

bool BinarySerializerReader::InOut(float &x)
{
    fin.read((char *)(&x), sizeof(x));
    return true;
}

bool BinarySerializerReader::InOut(uint &x)
{
    fin.read((char *)(&x), sizeof(x));
    return true;
}

bool BinarySerializerReader::InOut(std::vector<float> &vec)
{
    size_t size;
    fin.read((char *)(&size), sizeof(size));

    vec = std::vector<float>(size);

    for (uint i = 0; i < size; i++) {
        fin.read((char *)(&vec[i]), sizeof(vec[i]));
    }
    return true;
}

bool BinarySerializerReader::InOut(std::vector<VM::vec2> &vec)
{
    size_t size;
    fin.read((char *)(&size), sizeof(size));

    vec = std::vector<VM::vec2>(size);

    for (uint i = 0; i < size; i++) {
        fin.read((char *)(&vec[i].x), sizeof(vec[i].x));
        fin.read((char *)(&vec[i].y), sizeof(vec[i].y));
    }
    return true;
}

bool BinarySerializerReader::InOut(std::vector<VM::vec3> &vec)
{
    size_t size;
    fin.read((char *)(&size), sizeof(size));

    vec = std::vector<VM::vec3>(size);

    for (uint i = 0; i < size; i++) {
        fin.read((char *)(&vec[i].x), sizeof(vec[i].x));
        fin.read((char *)(&vec[i].y), sizeof(vec[i].y));
        fin.read((char *)(&vec[i].z), sizeof(vec[i].z));
    }
    return true;
}

bool BinarySerializerReader::InOut(std::vector<VM::vec4> &vec)
{
    size_t size;
    fin.read((char *)(&size), sizeof(size));

    vec = std::vector<VM::vec4>(size);

    for (uint i = 0; i < size; i++) {
        fin.read((char *)(&vec[i].x), sizeof(vec[i].x));
        fin.read((char *)(&vec[i].y), sizeof(vec[i].y));
        fin.read((char *)(&vec[i].z), sizeof(vec[i].z));
        fin.read((char *)(&vec[i].w), sizeof(vec[i].w));
    }
    return true;
}

/* TextSerializerWriter */

TextSerializerWriter::TextSerializerWriter(const char *path)
{
    fout.open(path);
}

TextSerializerWriter::~TextSerializerWriter()
{
    fout.close();
}

bool TextSerializerWriter::InOut(float &x)
{
    fout << x << std::endl;
    return true;
}

bool TextSerializerWriter::InOut(uint &x)
{
    fout << x << std::endl;
    return true;
}

bool TextSerializerWriter::InOut(std::vector<VM::vec4> &vec)
{
    size_t size = vec.size();
    fout << size << std::endl;

    for (uint i = 0; i < size; i++) {
        fout << vec[i].x << std::endl;
        fout << vec[i].y << std::endl;
        fout << vec[i].z << std::endl;
        fout << vec[i].w << std::endl;
    }
    return true;
}

bool TextSerializerWriter::InOut(std::vector<VM::vec3> &vec)
{
    size_t size = vec.size();
    fout << size << std::endl;

    for (uint i = 0; i < size; i++) {
        fout << vec[i].x << std::endl;
        fout << vec[i].y << std::endl;
        fout << vec[i].z << std::endl;
    }
    return true;
}

bool TextSerializerWriter::InOut(std::vector<VM::vec2> &vec)
{
    size_t size = vec.size();
    fout << size << std::endl;

    for (uint i = 0; i < size; i++) {
        fout << vec[i].x << std::endl;
        fout << vec[i].y << std::endl;
    }
    return true;
}

bool TextSerializerWriter::InOut(std::vector<float> &vec)
{
    size_t size = vec.size();
    fout << size << std::endl;

    for (uint i = 0; i < size; i++) {
        fout << vec[i] << std::endl;
    }
    return true;
}


/* TextSerializerReader */

TextSerializerReader::TextSerializerReader(const char *path)
{
    fin.open(path);
}

TextSerializerReader::~TextSerializerReader()
{
    fin.close();
}

bool TextSerializerReader::InOut(float &x)
{
    fin >> x;
    return true;
}

bool TextSerializerReader::InOut(uint &x)
{
    fin >> x;
    return true;
}

bool TextSerializerReader::InOut(std::vector<VM::vec4> &vec)
{
    size_t size;
    fin >> size;

    //vec = std::vector<VM::vec4>(size);

    for (uint i = 0; i < size; i++) {
        fin >> vec[i].x;
        fin >> vec[i].y;
        fin >> vec[i].z;
        fin >> vec[i].w;
    }
    return true;
}

bool TextSerializerReader::InOut(std::vector<VM::vec3> &vec)
{
    size_t size;
    fin >> size;

    //vec = std::vector<VM::vec3>(size);

    for (uint i = 0; i < size; i++) {
        fin >> vec[i].x;
        fin >> vec[i].y;
        fin >> vec[i].z;
    }
    return true;
}

bool TextSerializerReader::InOut(std::vector<VM::vec2> &vec)
{
    size_t size;
    fin >> size;

    for (uint i = 0; i < size; i++) {
        fin >> vec[i].x;
        fin >> vec[i].y;
    }
    return true;
}

bool TextSerializerReader::InOut(std::vector<float> &vec)
{
    size_t size;
    fin >> size;
    
    //vec = std::vector<float>(size);

    for (uint i = 0; i < size; i++) {
        fin >> vec[i];
    }
    return true;
}
