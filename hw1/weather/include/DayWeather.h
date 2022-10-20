#ifndef DAY_WEATHER_H
#define DAY_WEATHER_H

typedef struct DayWeatherInfo {
    int temperature;
    unsigned int windSpeed;
    unsigned int precipitation;
} DayWeatherInfo;

int getValuesFromLine(char *line, DayWeatherInfo *dwi);

#endif // DAY_WEATHER_H