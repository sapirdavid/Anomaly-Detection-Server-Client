#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <cmath>

struct correlatedFeatures {
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Circle min_circle = Circle(Point(0,0), 0);
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:vector<correlatedFeatures> cf;
protected:float threshold;

public:
    //constructor of simple anomaly detector
    SimpleAnomalyDetector();
    //destructor of simple anomaly detector
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    int amount_values(const TimeSeries& ts) const;
    bool check_duplicate(string i_name, string j_name);
    virtual void create_cor_feature(float max_pearson_value, const TimeSeries& ts,
                                    string max_feature_name, int size_vec, float *array_i, string f_name_i);
    virtual void create_report(Point p, correlatedFeatures current_cf, int i, vector<AnomalyReport> &vec_report);
    virtual float get_threshold();
    virtual void set_threshold(float new_threshold);
    };

#endif /* SIMPLEANOMALYDETECTOR_H_ */
