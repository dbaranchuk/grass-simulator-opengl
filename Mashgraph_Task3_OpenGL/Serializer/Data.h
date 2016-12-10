#ifndef Data_H
#define Data_H

#include <iostream>
#include <vector>

#include "ISerializer.h"

class Data
{
    /* Wind */
    uint wind_step = 0;
    uint wind_mode = 0;

    /* Grass */
    std::vector<float> grassScalesData;
    std::vector<VM::vec2> grassPositionsData;
    std::vector<VM::vec2> grassRotationsData;
    std::vector<VM::vec4> grassVarianceData;
    std::vector<VM::vec4> grassColorData;

public:
    Data(){}

    /* Wind */
    void set_step(uint step) { wind_step = step; }
    void set_mode(uint mode) { wind_mode = mode; }

    uint get_step() { return wind_step; }
    uint get_mode() { return wind_mode; }
    
    /* Grass */
    std::vector<float> &getGrassScalesData() { return grassScalesData; }
    std::vector<VM::vec2> &getGrassPositionsData() { return grassPositionsData; }
    std::vector<VM::vec2> &getGrassRotationsData() { return grassRotationsData; }
    std::vector<VM::vec4> &getGrassVarianceData() { return grassVarianceData; }
    std::vector<VM::vec4> &getGrassColorData() { return grassColorData; }

    void setGrassScalesData(std::vector<float> &s) { grassScalesData = s; }
    void setGrassPositionsData(std::vector<VM::vec2> &pos) { grassPositionsData = pos; }
    void setGrassRotationsData(std::vector<VM::vec2> &angle) { grassRotationsData = angle; }
    void setGrassVarianceData(std::vector<VM::vec4> &var) { grassVarianceData = var; }
    void setGrassColorData(std::vector<VM::vec4> &col) { grassColorData = col; }

    /* Serialize */
    void serialize(ISerializer &);

};
#endif // Data_H
