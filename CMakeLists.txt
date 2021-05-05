cmake_minimum_required(VERSION 3.17)
project(ass7)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass6 anomaly_detection_util.h anomaly_detection_util.cpp AnomalyDetector.h CLI.h CLI.cpp commands.h HybridAnomalyDetector.h HybridAnomalyDetector.cpp MainTrain.cpp minCircle.h minCircle.cpp Server.cpp Server.h SimpleAnomalyDetector.h SimpleAnomalyDetector.cpp timeseries.h timeseries.cpp)