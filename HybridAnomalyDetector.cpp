/*
 * HybridAnomalyDetector.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */
#include "HybridAnomalyDetector.h"

//constructor of hybrid anomaly detector
HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

//destructor of hybrid anomaly detector
HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

//the function create the correlated features
void HybridAnomalyDetector::create_cor_feature(float max_pearson_value ,const TimeSeries& ts,
                                               string max_feature_name, int size_vec, float *array_i, string f_name_i) {
    //if max_pearson_value >= 0.9 run the Simple Anomaly Detector, otherwise run the min circle anomaly
    if (abs(max_pearson_value) >= this->threshold) {
        SimpleAnomalyDetector::create_cor_feature(max_pearson_value, ts, max_feature_name, size_vec, array_i, f_name_i);
    } else if ((abs(max_pearson_value) > 0.5) && (abs(max_pearson_value) < this->threshold)) {
        //vector values of feature j
        vector<float> feature_j = ts.find_vector_values(max_feature_name);
        //convert vector to array
        float array_j[size_vec];
        std::copy(feature_j.begin(), feature_j.end(), array_j);
        //create array of point for min circle
        Point *ps[size_vec];
        for (int k = 0; k < size_vec; k++) {
            ps[k] = new Point(array_i[k], array_j[k]);
        }
        //find the min circle that comprehensive all training points
        Circle min_c = findMinCircle(ps, size_vec);
        float min_radius = min_c.radius;
        //create the correlated features
        correlatedFeatures cf;
        cf.feature1 = f_name_i;
        cf.feature2 = max_feature_name;
        cf.corrlation = max_pearson_value;
        cf.threshold = 1.1 * min_radius;
        cf.min_circle = min_c;
        this->cf.push_back(cf);
    }
}

// the function create an anomaly report
void HybridAnomalyDetector::create_report(Point p, correlatedFeatures current_cf, int i,
                                          vector<AnomalyReport> &vec_report) {
    if (current_cf.corrlation >= this->threshold) {
        SimpleAnomalyDetector::create_report(p, current_cf, i, vec_report);
    } else if ((current_cf.corrlation > 0.5) && (current_cf.corrlation < this->threshold))  {
        //if the point is outside the minimum circle it is an anomaly
        if (!(is_point_inside(current_cf.min_circle, p, 1.1))) {
            //create a report of this anomaly
            string description = current_cf.feature1 +"-"+ current_cf.feature2;
            long timeStep = i + 1;
            AnomalyReport a_report(description,timeStep);
            vec_report.push_back(a_report);
        }
    }
}


