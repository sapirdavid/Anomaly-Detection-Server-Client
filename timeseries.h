#include <map>
#include <fstream>
#include<sstream>
#include <vector>
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
    map<string, vector<float>> time_series_data;
public:
    //constructor of timeseries
    TimeSeries(const char *CSVfileName) {
        //initialize time_series_data map from csv file data
        initialize(CSVfileName, time_series_data);
    }
    //initialize time_series_data map from csv file data
    void initialize(const char *CSVfileName, map<string, vector<float>> &time_series_data);
    //return the values vector of specific feature as per name
    vector<float> find_vector_values (string feature_name) const;
    //return vector of names features
    vector<string> features_names () const;
};

#endif /* TIMESERIES_H_ */
