#include <gtest/gtest.h>

extern "C" {
    #include "DayWeather.h"
    #include "WeatherAggregationInfo.h"
    #include "WeatherStatus.h"
}

#define CREATE_ARRAY(lineTmp)  \
    char line[sizeof(lineTmp) + 1] = {}; \
    memcpy(line, lineTmp, sizeof(lineTmp));

TEST(DayWeatherInfo, CorrectLine) {
    CREATE_ARRAY("1;2;3");

    DayWeatherInfo dwi = {};
    int status = getValuesFromLine(line, &dwi);

    EXPECT_EQ(status, WEATHER_SUCCESS);
    EXPECT_EQ(dwi.temperature, 1);
    EXPECT_EQ(dwi.windSpeed, 2);
    EXPECT_EQ(dwi.precipitation, 3);
}

TEST(DayWeatherInfo, NegativeTemperature) {
    CREATE_ARRAY("-1;2;3");

    DayWeatherInfo dwi = {};
    int status = getValuesFromLine(line, &dwi);

    EXPECT_EQ(status, WEATHER_SUCCESS);
    EXPECT_EQ(dwi.temperature, -1);
    EXPECT_EQ(dwi.windSpeed, 2);
    EXPECT_EQ(dwi.precipitation, 3);
}

TEST(DayWeatherInfo, Empty) {
    {
        CREATE_ARRAY("");
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(line, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
        EXPECT_EQ(dwi.temperature, 0);
        EXPECT_EQ(dwi.windSpeed, 0);
        EXPECT_EQ(dwi.precipitation, 0);
    }
    {
        CREATE_ARRAY("    ");
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(line, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
        EXPECT_EQ(dwi.temperature, 0);
        EXPECT_EQ(dwi.windSpeed, 0);
        EXPECT_EQ(dwi.precipitation, 0);
    }
    {
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(NULL, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
        EXPECT_EQ(dwi.temperature, 0);
        EXPECT_EQ(dwi.windSpeed, 0);
        EXPECT_EQ(dwi.precipitation, 0);
    }
}

TEST(DayWeatherInfo, ValuesCount) {
    {
        CREATE_ARRAY("1;2;3;4");
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(line, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
        EXPECT_EQ(dwi.temperature, 0);
        EXPECT_EQ(dwi.windSpeed, 0);
        EXPECT_EQ(dwi.precipitation, 0);
    }
    {
        CREATE_ARRAY("1;2");
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(line, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
        EXPECT_EQ(dwi.temperature, 0);
        EXPECT_EQ(dwi.windSpeed, 0);
        EXPECT_EQ(dwi.precipitation, 0);
    }
}

TEST(DayWeatherInfo, IncorrectType) {
    {
        CREATE_ARRAY("A;2;3");
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(line, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
    }
    {
        CREATE_ARRAY("1;String;2");
        DayWeatherInfo dwi = {};
        int status = getValuesFromLine(line, &dwi);

        EXPECT_EQ(status, WEATHER_ERROR);
    }
}

TEST(WeatherAggregationInfo, Empty) {
    WeatherAggregationInfo wai = {};
    DayWeatherInfo dwi;
    int status = calculateAggregated(&dwi, 0, &wai);
    
    EXPECT_EQ(status, WEATHER_SUCCESS);
    EXPECT_EQ(wai.totalPrecipitation, 0);
    EXPECT_EQ(wai.minTemperature, 0);
    EXPECT_EQ(wai.maxTemperature, 0);
    EXPECT_NEAR(wai.avgTemperature, 0.0f, 0.01f);

    status = calculateAggregated(NULL, 100, &wai);
    
    EXPECT_EQ(status, WEATHER_ERROR);
    EXPECT_EQ(wai.totalPrecipitation, 0);
    EXPECT_EQ(wai.minTemperature, 0);
    EXPECT_EQ(wai.maxTemperature, 0);
    EXPECT_NEAR(wai.avgTemperature, 0.0f, 0.01f);
}

DayWeatherInfo createDwi(int t, unsigned int w, unsigned int p) {
    DayWeatherInfo dwi;
    dwi.temperature = t;
    dwi.windSpeed = w;
    dwi.precipitation = p;

    return dwi;
}

TEST(WeatherAggregationInfo, Correct) {
    WeatherAggregationInfo wai = {};
    DayWeatherInfo dwi[4];
    dwi[0] = createDwi(24, 4, 4);
    dwi[1] = createDwi(32, 2, 4);
    dwi[2] = createDwi(17, 8, 0);
    dwi[3] = createDwi(-7, 10, 1);
    int status = calculateAggregated(dwi, (sizeof(dwi) / sizeof(DayWeatherInfo)), &wai);
    
    EXPECT_EQ(status, WEATHER_SUCCESS);
    EXPECT_EQ(wai.totalPrecipitation, 9);
    EXPECT_EQ(wai.minTemperature, -7);
    EXPECT_EQ(wai.maxTemperature, 32);
    EXPECT_NEAR(wai.avgTemperature, 16.5f, 0.01f);
}