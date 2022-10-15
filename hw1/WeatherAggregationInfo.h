#ifndef WEATHER_AGGREGATION_INFO_H
#define WEATHER_AGGREGATION_INFO_H

#include <stddef.h>

typedef struct DayWeatherInfo DayWeatherInfo;

typedef struct WeatherAggregationInfo {
    unsigned long long totalPrecipitation;
    int minTemperature;
    int maxTemperature;
    int avgTemperature;
} WeatherAggregationInfo;

int parseWeather(const char *filename, WeatherAggregationInfo *out);
void printAggregated(const WeatherAggregationInfo *aggregated);
int calculateAggregated(const DayWeatherInfo *values, size_t valuesCount, WeatherAggregationInfo *out);

#endif // WEATHER_AGGREGATION_INFO_H