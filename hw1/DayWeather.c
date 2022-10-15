#include "DayWeather.h"
#include "WeatherStatus.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TEMPERATURE = 0,
    WIND_SPEED,
    PRECIPITATION,
    QUANTITY_COUNT,
    MAX_QUANTITY = QUANTITY_COUNT - 1
} WeatherQuantity;

int getValuesFromLine(char *line, DayWeatherInfo *dwi) {
    if (line == NULL) {
            printf("Line is null\n");
            return WEATHER_ERROR;
    }
    
    char *startPtr = strtok(line, ";");
    int values[QUANTITY_COUNT] = {};

    int quantity = TEMPERATURE;
    while (startPtr != NULL) {
        if (quantity > MAX_QUANTITY) {
            printf("Too many values in one line\n");
            return WEATHER_ERROR;
        }

        char *endPtr = NULL;
        long parsedValue = strtol(startPtr, &endPtr, 10);
        if (startPtr != endPtr) {
            // TODO: check overflow?
            values[quantity] = parsedValue;
        } else {
            printf("Cannot get value %s\n", startPtr);
            return WEATHER_ERROR;
        }
        ++quantity;
        startPtr = strtok(NULL, ";");
    }
    if (quantity != MAX_QUANTITY + 1) {
        printf("Missed value\n");
        return WEATHER_ERROR;
    }

    dwi->temperature = values[TEMPERATURE];
    if (values[WIND_SPEED] < 0) {
        printf("WARNING: Wind speed has suspicious value: %d\n", values[WIND_SPEED]);
    }
    if (values[PRECIPITATION] < 0) {
        printf("WARNING: Precipitation has suspicious value: %d\n", values[PRECIPITATION]);
    }
    dwi->windSpeed = values[WIND_SPEED];
    dwi->precipitation = values[PRECIPITATION];

    return WEATHER_SUCCESS;
}