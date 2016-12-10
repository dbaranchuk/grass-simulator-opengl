#include "Data.h"

void Data::serialize(ISerializer &serializer)
{
    /* Wind */
    serializer.InOut(wind_step);
    serializer.InOut(wind_mode);

    /* Grass */
    serializer.InOut(grassPositionsData);
    serializer.InOut(grassScalesData);
    serializer.InOut(grassRotationsData);
    serializer.InOut(grassVarianceData);
    serializer.InOut(grassColorData);
}

