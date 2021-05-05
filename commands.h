/*
 * commands.h
 * Author: Sapir David.
 * ID: 318574712.
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
	// you may add additional methods here
};

class Data {
private:
    int sample_size;
    vector<AnomalyReport> vec_reports;
    HybridAnomalyDetector hybrid_anomaly;
public:
    Data() {};

    //the function return the current threshold of hybrid anomaly
    float get_anomaly_threshold() {
        return this->hybrid_anomaly.get_threshold();
    }
    //the function set new threshold to hybrid anomaly
    void set_anomaly_threshold(float new_threshold) {
        this->hybrid_anomaly.set_threshold(new_threshold);
    }
    //the function do learn normal for the hybrid anomaly
    void learn_normal_hybrid(TimeSeries train) {
        this->hybrid_anomaly.learnNormal(train);
    }

    //the function do detect for the hybrid anomaly
    vector<AnomalyReport> detect_hybrid(TimeSeries test) {
        vector<AnomalyReport> reports = this->hybrid_anomaly.detect(test);
        return reports;
    }

    //the function return vector of anomaly reports
    vector<AnomalyReport> get_anomaly_reports() {
        return this->vec_reports;
    }
    //the function set vector of anomaly reports
    void set_anomaly_reports(vector<AnomalyReport> reports) {
        this->vec_reports.clear();
        int amount_reports = reports.size();
        for (int i = 0; i < amount_reports ; i++) {
            this->vec_reports.push_back(reports[i]);
        }
    }
    //the function set the number of line in the input test csv file - this is the sample size
    void set_amount_samples(int amount) {
        this->sample_size = amount;
    }
    //the function get the number of line in the input test csv file - this is the sample size
    int get_amount_samples() {
        return this->sample_size;
    }
    ~Data(){}
};

class Command {
protected:
    string description;
    DefaultIO* dio;
    Data* data;
public:
	Command(DefaultIO* dio, Data* data):dio(dio), data(data){}
	virtual void execute()=0;
	virtual ~Command(){}
	virtual string getDescription() {
        return this->description;
	}
};

//command class for upload csv file
class UploadCsv:public Command {
public:
    //constructor of Upload Csv command
    UploadCsv(DefaultIO *dio, Data *data) : Command(dio, data) {
        this->description = "1.upload a time series csv file\n";
        this->data = data;
    }
    virtual void execute() {
        int amount_lines = 0;
        ofstream train_csv_file, test_csv_file;
        train_csv_file.open("anomalyTrain.csv");
        test_csv_file.open("anomalyTest.csv");

        //request's server for train CSV file
        this->dio->write("Please upload your local train CSV file.\n");
        //the client read the first line - titles
        string titles_train = this->dio->read();
        //the server write the first line(titles) to the train CSV file
        train_csv_file << titles_train << endl;
        bool continue_read = true;
        //the client continue read and the server continue write until a line appears with the word "done"
        while (continue_read) {
            string values = this->dio->read();
            if (values.compare("done") == 0) {
                continue_read = false;
            } else {
                train_csv_file << values << endl;
            }
        }
        //the server notifies the end of the input from the client
        this->dio->write("Upload complete.\n");

        //request's server for test CSV file
        this->dio->write("Please upload your local test CSV file.\n");
        //the client read the first line - titles
        string titles_test = this->dio->read();
        //the server write the first line(titles) to the train CSV file
        test_csv_file << titles_test << endl;
        continue_read = true;
        //the client continue read and the server continue write until a line appears with the word "done"
        while (continue_read) {
            amount_lines++;
            string values = this->dio->read();
            if (values.compare("done") == 0) {
                continue_read = false;
            } else {
                test_csv_file << values << endl;
            }
        }
        this->data->set_amount_samples(amount_lines - 1);
        //the server notifies the end of the input from the client
        this->dio->write("Upload complete.\n");
        //close the csv files
        train_csv_file.close();
        test_csv_file.close();
    }
    virtual ~UploadCsv(){}
};

//command class for algorithm settings
class AlgoSettings:public Command {
public:
    //constructor of Algorithm Settings command
    AlgoSettings(DefaultIO *dio, Data *data) : Command(dio, data) {
            this->description = "2.algorithm settings\n";
    }

    virtual void execute() {
        bool continue_read = true;
        float *new_threshold = new float;
        float threshold;
        float current_threshold = this->data->get_anomaly_threshold();
        //the server will show the client the current correlation threshold
        this->dio->write("The current correlation threshold is ");
        this->dio->write(current_threshold);
        this->dio->write("\n");
        while (continue_read) {
            //the server will give the client the option to change the correlation threshold
            this->dio->write("Type a new threshold\n");
            //the new threshold from the client
            this->dio->read(new_threshold);
            threshold = *(new_threshold);
            //check if the client selected a valid value
            if (threshold < 0 || threshold > 1) {
                this->dio->write("please choose a value between 0 and 1.\n");
            } else {
                continue_read = false;
                this->data->set_anomaly_threshold(threshold);
            }
        }
        delete (new_threshold);
    }
    virtual ~AlgoSettings(){}
};

//command class for algorithm settings
class DetectAnomalies:public Command {
public:
    //constructor of Detect Anomalies command
    DetectAnomalies(DefaultIO *dio, Data *data) : Command(dio, data) {
        this->description = "3.detect anomalies\n";
    }
    virtual void execute() {
        //create time series from the csv files
        TimeSeries ts_train("anomalyTrain.csv");
        TimeSeries ts_test("anomalyTest.csv");
        //run the learn normal function
        this->data->learn_normal_hybrid(ts_train);
        //run the detect function
        vector<AnomalyReport> reports = this->data->detect_hybrid(ts_test);
        //save the reports vector of
        this->data->set_anomaly_reports(reports);

        this->dio->write("anomaly detection complete.\n");
    }
    virtual ~DetectAnomalies(){}
};

//command class for display results
class DisplayResults:public Command {
public:
    //constructor of Display Results command
    DisplayResults(DefaultIO *dio, Data *data) : Command(dio, data) {
        this->description = "4.display results\n";
    }
    virtual void execute() {
        //get the reports vector
        vector<AnomalyReport> reports =this->data->get_anomaly_reports();
        int amount_reports = reports.size();
        //sort(reports.begin(), reports.end());
        //the server will print the anomalies reports
        for (int i = 0; i < amount_reports ; i++) {
            this->dio->write(reports[i].timeStep);
            this->dio->write("\t");
            this->dio->write(reports[i].description);
            this->dio->write("\n");
        }
        this->dio->write("Done.\n");
    }
    virtual ~DisplayResults(){}
};

//command class for upload anomalies and analyze results
class Analyze:public Command {
public:
    //constructor of upload anomalies and analyze results command
    Analyze(DefaultIO *dio, Data *data) : Command(dio, data) {
        this->description = "5.upload anomalies and analyze results\n";
    }
    virtual void execute() {
        int N = this->data->get_amount_samples();
        vector<string> range_anomalies;
        vector<Point> union_anomalies_vec;
        vector<int> help_vec, cut_vec, cut_range_vec;
        int start, end, TP = 0, FP = 0, P;

        //save the ranges times from the local anomalies file in range_anomalies vector
        upload_local_anomalies(range_anomalies);
        P = range_anomalies.size();

        //create union anomalies vector
        union_anomalies(union_anomalies_vec);

        //init vector for count intersection with anomaly reporting
        for (int i = 0; i < union_anomalies_vec.size(); ++i) {
            cut_vec.push_back(0);
        }

        //Compare anomalies time ranges to detector reporting times
        for (int i = 0; i < range_anomalies.size(); i++) {
            //vector for count intersection with ranges times
            cut_range_vec.push_back(0);
            //init the start and end of the range of anomalies time[i]
            help_vec = get_range(range_anomalies, i);
            start = help_vec[0];
            end = help_vec[1];
            N = N - (end - start) - 1;
            for (int j = 0; j < union_anomalies_vec.size(); j++) {
                //check if the reporting times is in the range(intersection)
                if (((start >= union_anomalies_vec[j].x) && (start <= union_anomalies_vec[j].y))
                    || ((end >= union_anomalies_vec[j].x) && (end <= union_anomalies_vec[j].y))
                    || ((end >= union_anomalies_vec[j].x) && (start <= union_anomalies_vec[j].y))) {
                    cut_vec[j]++;
                    cut_range_vec[i]++;
                }
            }
            //calculate the number of true positive
            if (cut_range_vec[i] != 0) {
                TP++;
            }
        }

        //calculate the number of false positive
        for (int k = 0; k < cut_vec.size(); ++k) {
            if (cut_vec[k] == 0) {
                FP++;
            }
        }

        //calculate of true positive rate and false alarm rate
        float true_positive_rate = static_cast<float>(TP) / static_cast<float>(P);
        float false_alarm_rate = static_cast<float>(FP) / static_cast<float>(N);

        //the results with a 3-digit cut after the dot
        true_positive_rate = floor(true_positive_rate * 1000.0) / 1000.0;
        false_alarm_rate = floor(false_alarm_rate * 1000.0) / 1000.0;

        //the server print the results
        this->dio->write("True Positive Rate: ");
        this->dio->write(true_positive_rate);
        this->dio->write("\n");
        this->dio->write("False Positive Rate: ");
        this->dio->write(false_alarm_rate);
        this->dio->write("\n");
    }

    /* the function save the ranges times from the local anomalies file that
    get from the client in range_anomalies vector */
    void upload_local_anomalies (vector<string> &range_anomalies) {
        //request's server for local anomalies file
        this->dio->write("Please upload your local anomalies file.\n");
        bool continue_read = true;
        //the client read and the server continue write until a line appears with the word "done"
        while (continue_read) {
            string range_times = this->dio->read();
            if (range_times.compare("done") == 0) {
                continue_read = false;
            } else {
                range_anomalies.push_back(range_times);
            }
        }
        //the server notifies the end of the input from the client
        this->dio->write("Upload complete.\n");
    }

    //the function create vector that contain the range (start and end)
    vector<int> get_range(vector<string> range_anomalies, int i) {
        vector<int> range_vec;
        stringstream ss(range_anomalies[i]);
        //get the numbers for the range delimited by comma
        for (int k; ss >> k;) {
            range_vec.push_back(k);
            if (ss.peek() == ',')
                ss.ignore();
        }
        return range_vec;
    }

    //the function union the anomalies reports
    void union_anomalies(vector<Point> &union_anomalies_vec) {
        int start, cur_time_step, old_time_step;
        string cur_description, old_description;
        int amount_reports = this->data->get_anomaly_reports().size();
        //go over the anomalies reports
        for (int i = 0; i < amount_reports; i++) {
            if (i == 0) {
                start =  this->data->get_anomaly_reports()[0].timeStep;
                continue;
            }
            //time step and description of the current anomaly
            cur_time_step = this->data->get_anomaly_reports()[i].timeStep;
            cur_description = this->data->get_anomaly_reports()[i].description;
            //time step and description of the last anomaly
            old_time_step = this->data->get_anomaly_reports()[i - 1].timeStep;
            old_description = this->data->get_anomaly_reports()[i - 1].description;
            //checks that they have the same description and continuity in time step
            if ((cur_description.compare(old_description) == 0) && (cur_time_step == 1 + old_time_step)) {
                if (i == amount_reports - 1) {
                    union_anomalies_vec.push_back(Point(start, cur_time_step));
                }
                continue;
            }
            else {
                union_anomalies_vec.push_back(Point(start, old_time_step));
                start = cur_time_step;
                if (i == amount_reports - 1) {
                    union_anomalies_vec.push_back(Point(start, cur_time_step));
                }
            }
        }
    }

    virtual ~Analyze(){}
};

//command class for exit
class Exit:public Command {
public:
        //constructor of exit command
    Exit(DefaultIO *dio, Data *data) : Command(dio, data) {
        this->description = "6.exit\n";
    }
    virtual void execute() {}
    virtual ~Exit(){}
};
#endif /* COMMANDS_H_ */
