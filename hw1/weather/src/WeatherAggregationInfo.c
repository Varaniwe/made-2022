#include "WeatherAggregationInfo.h"
#include "WeatherStatus.h"
#include "DayWeather.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE (1024)

void printAggregated(const WeatherAggregationInfo *aggregated) {
    if (aggregated == NULL) {
        return;
    }

    printf("Tmin=%d, Tmax=%d, Tavg=%f, Psum=%llu\n", aggregated->minTemperature, aggregated->maxTemperature,
     aggregated->avgTemperature, aggregated->totalPrecipitation);
}

int parseWeather(const char *filename, WeatherAggregationInfo *out) {
    FILE *fileHandle = fopen(filename, "r");
    if (fileHandle == NULL) {
        printf("Cannot open file %s\n", filename);
        return WEATHER_ERROR;
    }

    DayWeatherInfo *days = malloc(sizeof(DayWeatherInfo));
    if (!days) {
        fclose(fileHandle);
        return WEATHER_ERROR;
    }

    size_t linesCount = 0;
    char line[MAX_LINE_SIZE];
    int haveErrors = 0;
    while (fgets(line, sizeof(line), fileHandle)) {
        if (linesCount != 0) {
            DayWeatherInfo *daysTmp = days;
            days = realloc(days, sizeof(DayWeatherInfo) * (linesCount + 1));
            if (days == NULL) {
                free(daysTmp);
                daysTmp = NULL;
                haveErrors = 1;
                break;
            }
        }

        if (WEATHER_FAILED(getValuesFromLine(line, days + linesCount))) {
            printf("Some error on line %zu\n", linesCount + 1);
            haveErrors = 1;
            break;
        }
        ++linesCount;
    }

    fclose(fileHandle);

    if (!haveErrors) {
        if (WEATHER_FAILED(calculateAggregated(days, linesCount, out))) {
            printf("Failed\n");
        }
    }

    free(days);
    days = NULL;

    return haveErrors ? WEATHER_ERROR : WEATHER_SUCCESS;
}

int calculateAggregated(const DayWeatherInfo *values, size_t valuesCount, WeatherAggregationInfo *out) {
    if (values == NULL || out == NULL)  {
        return WEATHER_ERROR;
    }

    memset(out, 0, sizeof(WeatherAggregationInfo));

    if (valuesCount == 0) {
        printf("WARNING: Values count is zero\n");
        return WEATHER_SUCCESS;
    }
    int initialValue = values[0].temperature;
    int minTemperature = initialValue;
    int maxTemperature = initialValue;
    long long sumTemperature = 0;
    long long sumPrecipitation = 0;
    for (size_t i = 0; i < valuesCount; ++i) {
        if (values[i].temperature < minTemperature) {
            minTemperature = values[i].temperature;
        } else if (values[i].temperature > maxTemperature) {
            maxTemperature = values[i].temperature;
        }
        sumTemperature += values[i].temperature;

        sumPrecipitation += values[i].precipitation;
    }

    out->minTemperature = minTemperature;
    out->maxTemperature = maxTemperature;
    out->avgTemperature = (float)(sumTemperature / (float)valuesCount);
    out->totalPrecipitation = sumPrecipitation;

    return WEATHER_SUCCESS;
}
