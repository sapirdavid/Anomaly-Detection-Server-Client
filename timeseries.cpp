/*
 * timeseries.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */
#include "timeseries.h"

//initialize time_series_data map from csv file data
void TimeSeries::initialize(const char *CSVfileName, map<string, vector<float>> &time_series_data) {
    //vector for save the names of feature in the order they appear in the file
    vector<string> features_names;
    ifstream csv_file;
    csv_file.open(CSVfileName);
    //flag for the first line
    bool first_line = true;
    while (csv_file.good()) {
        //if it is the first line in the file
        if (first_line) {
            string line;
            getline(csv_file, line);
            //create string stream from the string of the first line
            stringstream column_names(line);
            while (column_names.good()) {
                string feature;
                //get the string delimited by comma - this is the feature name
                getline(column_names, feature, ',');
                //crate a vector values for this feature
                vector<float> feature_vector{};
                time_series_data.insert(std::make_pair(feature, feature_vector));
                //enter the name of this feature to the vector names
                features_names.push_back(feature);
            }
            //done with the first line
            first_line = false;
        }
            //if it is not the first line in the file, continue read the file line by line
        else {
            int index_column = 0;
            string line;
            char ch;
            float value_float;
            //get the next line from the file
            getline(csv_file, line);
            //create string stream from the string of the line
            stringstream data(line);
            while (data.good()) {
                //read the float values that separated with comma
                while (data >> value_float) {
                    //store the float value in his feature vector
                    string feature = features_names[index_column];
                    time_series_data[feature].push_back(value_float);
                    //read and discard the comma
                    data >> ch;
                    index_column++;
                }
            }
        }
    }
}

//return the values vector of specific feature as per name
vector<float> TimeSeries::find_vector_values(string feature_name) const {
    map<string,vector<float>> map = this->time_series_data;
    return map[feature_name];
}
//return vector of names features
vector<string> TimeSeries::features_names() const {
    //vector for the features names
    vector<string> features_names{};
    map<string,vector<float>> map = this->time_series_data;
    //go over the keys of the map - there are the names pf the features
    for (std::map<string,vector<float>>::iterator it = map.begin(); it != map.end(); it++) {
        features_names.push_back(it->first);
    }
    return features_names;
}



