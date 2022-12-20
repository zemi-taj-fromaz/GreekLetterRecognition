#ifndef EXTRACT_FEATURES_HPP
#define EXTRACT_FEATURES_HPP
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class ExtractFeatures {
    private:
        double distance(pair<double,double> p1, pair<double,double> p2);
        double length(vector<pair<double,double>> gesta);
        void standardScaler(vector<pair<double,double>>&);
    public:
        ExtractFeatures();
        vector<pair<double,double>> __extract__(vector<pair<double,double>>, int M);
        static vector<string> split(string str, string token);

};

#endif