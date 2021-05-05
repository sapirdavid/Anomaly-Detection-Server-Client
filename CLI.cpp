/*
 * CLI.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */
#include "CLI.h"
using namespace std;

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    vector<Command*> options{};
    this->menu = options;
}

//the function init menu
void CLI::init_menu(Data *data) {
    Command *upload_csv = new UploadCsv(this->dio, data);
    menu.push_back(upload_csv);
    Command *algo_settings = new AlgoSettings(this->dio, data);
    menu.push_back(algo_settings);
    Command *detect_anomalies = new DetectAnomalies(this->dio, data);
    menu.push_back(detect_anomalies);
    Command *display_results = new DisplayResults(this->dio, data);
    menu.push_back(display_results);
    Command *analyze = new Analyze(this->dio, data);
    menu.push_back(analyze);
    Command *exit = new Exit(this->dio, data);
    menu.push_back(exit);
}

void CLI::start() {
    Data *data = new Data();
    init_menu(data);
    bool continue_display_menu = true;
    while (continue_display_menu) {
        display_menu();
        string option = this->dio->read();
        int choice = stoi(option);
        //if the client selects 6, the server-client interaction will end
        if (choice == 6) {
            continue_display_menu = false;
        } else {
            this->menu[choice - 1]->execute();
        }
    }
    clear_menu();
    delete (data);
}

//the function display the menu
void CLI::display_menu() {
    this->dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
    int amount_options = this->menu.size();
    for (int i = 0; i < amount_options; i++) {
        this->dio->write(menu[i]->getDescription());
    }
}

void CLI::clear_menu() {
    this->menu.clear();
}

CLI::~CLI() {
    this->menu.clear();
}

