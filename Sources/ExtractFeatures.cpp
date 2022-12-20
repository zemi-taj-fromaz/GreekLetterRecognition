#include "../Headers/ExtractFeatures.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cmath>

ExtractFeatures::ExtractFeatures(){};

vector<string> ExtractFeatures::split(string str, string token){
    vector<string> result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

double ExtractFeatures::distance(pair<double,double> p1, pair<double,double> p2){
    return sqrt( pow(p1.first-p2.first,2) + pow(p1.second - p2.second,2));
}

double ExtractFeatures::length(vector<pair<double,double>> gesta){
    double ret = 0;

    for(int i = 1; i < gesta.size(); i++){
        ret += distance(gesta[i],gesta[i-1]);
    }

    return ret;
}

void ExtractFeatures::standardScaler(vector<pair<double,double>>& coordinates){
    double xc = 0;
    double yc = 0;
    double maxX = 0;
    double maxY = 0;

    for(int i  = 0; i < coordinates.size() ; i++){
        xc += coordinates[i].first;
        yc += coordinates[i].second;
    }

    xc /= coordinates.size();
    yc /= coordinates.size();


   

    pair<double,double> Tc({xc,yc});

    for(int i = 0; i < coordinates.size(); i++){
        coordinates[i].first -= Tc.first;
        coordinates[i].second -= Tc.second;
        maxX = max(maxX,abs(coordinates[i].first));
        maxY = max(maxY,abs(coordinates[i].second));
    }

     double maxCord = max(maxX,maxY);

    for(int i  = 0; i < coordinates.size() ; i++){

        coordinates[i].first /= maxCord;
        coordinates[i].second /= maxCord;
    }

    

}


vector<pair<double,double>> ExtractFeatures::__extract__(vector<pair<double,double>> coordinates,int M){

    standardScaler(coordinates);

    double D = length(coordinates);
    //cout<<"curve length : "<<D<<"\n";

    vector<double> distance_from_first(coordinates.size(),0);
    distance_from_first.push_back(0);
    for(int i = 1; i < coordinates.size(); i++){
        distance_from_first[i] = distance_from_first[i-1] + distance(coordinates[i-1],coordinates[i]);
    }

    vector<double> key_distances;
    for(int i = 0; i < M; i++){
        key_distances.push_back(i*D/(M-1)); 
    }
    
    vector<pair<double,double>> key_points;
    int j = 0;
    double dist;
    double dist_2;
    for(int i = 1; i < distance_from_first.size() && j < M; i++){
        dist = distance_from_first[i];
        if(dist < key_distances[j]) continue;
        dist_2 = distance_from_first[i-1];
        if(abs(dist_2 - key_distances[j]) < abs(dist - key_distances[j])){
            key_points.push_back(coordinates[i-1]);
        } else {
            key_points.push_back(coordinates[i]);
        }
        j++;
    }

    return key_points;

}

