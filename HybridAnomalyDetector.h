
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual void create_cor_feature(float max_pearson_value, const TimeSeries& ts,
                                    string max_feature_name, int size_vec, float *array_i, string f_name_i);
    virtual void create_report(Point p, correlatedFeatures current_cf, int i, vector<AnomalyReport> &vec_report);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
