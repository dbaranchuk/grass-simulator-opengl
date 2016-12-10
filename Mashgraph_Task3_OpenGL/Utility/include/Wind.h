#ifndef WIND_H
#define WIND_H

#include "vec4.h"
#include "mat4.h"

#include <cmath>

typedef unsigned int uint;

const VM::vec4 EAST = VM::vec4(1, 0, 0, 0);
const VM::vec4 NORTH = VM::vec4(0, 0, 1, 0);
const size_t MAX_STEP = 16000,
            MIN_STEP = 12000;

enum { OFF, UP, DOWN };

class Wind
{
    float strength = 0.05;
    uint step = 0;
    VM::vec4 direction = EAST;

    uint mode = OFF;

public:
    VM::vec4 compute_variance()
    {
        VM::vec4 variance = VM::vec4(0, 0, 0, 0);
        switch (mode){
            case OFF:
                if (step > 0){
                    variance = direction * strength * --step / MAX_STEP;
                }
                break;
            case UP:
                mode = (++step == MAX_STEP) ? DOWN : UP;
                variance = direction * strength * step / MAX_STEP;
                break;
            case DOWN:
                mode = (--step == MIN_STEP) ? UP : DOWN;
                variance = direction * strength * step / MAX_STEP;
                break;
        }
        return rotation(19 * M_PI/10) * variance;
    }

    void turn_on() { mode = UP; }
    void turn_off() { mode = OFF; }

    /* Getters */
    float get_strength() { return strength; }
    uint get_step() { return step; }
    VM::vec4 get_direction() { return direction; }
    uint get_mode() { return mode; }
    
    /* Setters */
    void set_strength(float s) { strength = s; }
    void set_step(uint s) { step = s; }
    void set_direction(VM::vec4 d) { direction = d; }
    void set_mode(uint m) { mode = m; }

private:
    VM::mat4 rotation(float angle)
    {
        VM::mat4 matrix(1.);

        matrix[0][0] = std::cos(angle);
        matrix[0][1] = -std::sin(angle);
        matrix[1][0] = std::sin(angle);
        matrix[1][1] = std::cos(angle);
        return matrix;
    }
};

#endif // WIND_H 
