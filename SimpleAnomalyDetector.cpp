/*
 * SimpleAnomalyDetector.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */
#include "SimpleAnomalyDetector.h"

//constructor of simple anomaly detector
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub
    vector<correlatedFeatures> c_features{};
    this->cf = c_features;
    this->threshold = 0.9;
}
//destructor of simple anomaly detector
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
    cf.clear();
}
//this function is the offline learning - learn the normal values
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    // TODO Auto-generated destructor stub
    //vector of the feature names
    vector<string> f_names = ts.features_names();
    //the amount of features
    int amount_features = f_names.size();
    //go over all features and check who are correlative
    for (int i = 0; i < amount_features; i++) {
        //compare every feature to other
        vector<float> feature_i = ts.find_vector_values(f_names[i]);
        float max_pearson_value = 0;
        string max_feature_name;
        // convert vector to array
        int size_vec = feature_i.size();
        float array_i[size_vec];
        std::copy(feature_i.begin(), feature_i.end(), array_i);
        //check the correlation of feature i and feature j
        for (int j = i + 1; j < amount_features; j++) {
            if (i != j) {
                // convert vector to array
                vector<float> feature_j = ts.find_vector_values(f_names[j]);
                float array_j[size_vec];
                std::copy(feature_j.begin(), feature_j.end(), array_j);
                //calculate the correlation between feature_i and feature_j
                float pearson_res = pearson(array_i, array_j, size_vec);
                //update the max correlation of feature i until now
                if (abs(pearson_res) > max_pearson_value) {
                    //save the data of the max correlative feature
                    max_pearson_value = abs(pearson_res);
                    max_feature_name = f_names[j];
                }
            }
        }
        string f_name_i = f_names[i];
        //check if this correlative features i and j are already exist - for prevent duplicates
        bool exist = check_duplicate(f_names[i], max_feature_name);
        // create correlated features
        if (!exist) {
            create_cor_feature(max_pearson_value, ts, max_feature_name, size_vec, array_i, f_name_i);
        }
    }
}
//create the correlated features
void SimpleAnomalyDetector::create_cor_feature(float max_pearson_value, const TimeSeries& ts,
                                               string max_feature_name, int size_vec, float *array_i, string f_name_i) {
    if (abs(max_pearson_value) >= this->threshold) {
        //vector values of feature j
        vector<float> feature_j = ts.find_vector_values(max_feature_name);
        //convert vector to array
        float array_j[size_vec];
        std::copy(feature_j.begin(), feature_j.end(), array_j);
        //create array of point for line reg
        Point *ps[size_vec];
        for (int k = 0; k < size_vec; k++) {
            ps[k] = new Point(array_i[k], array_j[k]);
        }
        Line line_reg = linear_reg(ps, size_vec);
        float max_offset = 0, offset;
        //find the max offset of this correlated feature i,j
        for (int t = 0; t < size_vec; t++) {
            Point p(array_i[t], array_j[t]);
            offset = dev(p, line_reg);
            if (offset > max_offset) {
                max_offset = offset;
            }
        }
        //create the correlated features
        correlatedFeatures cf;
        cf.feature1 = f_name_i;
        cf.feature2 = max_feature_name;
        cf.corrlation = max_pearson_value;
        cf.lin_reg = line_reg;
        cf.threshold = 1.1 * max_offset;
        this->cf.push_back(cf);
    }
}

//check if features are exist - to prevent duplicate
bool SimpleAnomalyDetector::check_duplicate(string i_name, string j_name) {
    int size = this->cf.size();
    for(int i = 0; i < size; i++) {
        //compare the name of the correlative feature in reverse order
        if((this->cf[i].feature1.compare(j_name) == 0) && (this->cf[i].feature2.compare(i_name) == 0)) {
            return true;
        }
    }
    return false;
}
//this function detect the anomalies
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    // TODO Auto-generated destructor stub
    //vector of the anomaly reports
    vector<AnomalyReport> vec_report{};
    //vector of the correlated features
    vector<correlatedFeatures> c_features = this->cf;
    // the amount of correlated feature
    int amount_c_features = c_features.size();
    //the amount of values in each features
    int amount_of_values = amount_values(ts);
    //go over the values line by line
    for (int i = 0; i < amount_c_features; i++) {
        //go over the correlated feature
        for (int j = 0; j < amount_of_values; j++) {
            //current correlated feature
            correlatedFeatures current_cf = c_features[i];
            //values vectors of each one from the correlated features of current_cf
            vector<float> feature_i = ts.find_vector_values(current_cf.feature1);
            vector<float> feature_j = ts.find_vector_values(current_cf.feature2);
            //create a point of the values from the i line
            Point p(feature_i[j], feature_j[j]);
            //create report in case that was anomaly
            create_report(p, current_cf, j, vec_report);
        }
    }
    return vec_report;
}

//the function create report in case that was anomaly
void SimpleAnomalyDetector::create_report(Point p, correlatedFeatures current_cf, int i, vector<AnomalyReport> &vec_report) {
    //find the offset between the point to the line reg
    float offset = dev(p, current_cf.lin_reg);
    //if the offset is bigger then the max offset of this correlated features
    if (offset > current_cf.threshold) {
        //create a report of this anomaly
        string description = current_cf.feature1 +"-"+ current_cf.feature2;
        long timeStep = i + 1;
        AnomalyReport a_report(description,timeStep);
        vec_report.push_back(a_report);
    }
}

//find the amount values of each feature
int SimpleAnomalyDetector::amount_values(const TimeSeries& ts) const {
    vector<correlatedFeatures> c_features = this->cf;
    //if there are correlated features
    if(c_features.size() > 0) {
        correlatedFeatures cf = c_features[0];
        vector<float> feature_i = ts.find_vector_values(cf.feature1);
        int size_vector = feature_i.size();
        return size_vector;
    }
    return 0;
}

//the function get the threshold
float SimpleAnomalyDetector::get_threshold() {
    return this->threshold;
}
//the function set new threshold
void SimpleAnomalyDetector::set_threshold(float new_threshold) {
    this->threshold = new_threshold;
}







