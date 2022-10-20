#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weather/include/WeatherStatus.h"
#include "weather/include/WeatherAggregationInfo.h"

void printHelp() {
        printf("Usage:\n");
        printf("main <input>\n");
        printf("where input is file with weather records for every day with semicolon-separated values: T;W;P, "
               "T - temperature, W - wind, P - precipitation\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2 || !strcmp(argv[1], "-h")) {
        printHelp();
        return WEATHER_ERROR;
    }

    WeatherAggregationInfo aggregated;
    memset(&aggregated, 0, sizeof(aggregated));
    if (WEATHER_FAILED(parseWeather(argv[1], &aggregated))) {
        return WEATHER_ERROR;
    }

    printAggregated(&aggregated);

    return WEATHER_SUCCESS;
}