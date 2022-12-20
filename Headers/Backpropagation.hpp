#ifndef BACKPROPAGATION_HPP
#define BACKPROPAGATION_HPP

using namespace std;

#include <vector>
#include <string>
#include "Matrix.hpp"

using namespace std;

class Backpropagation {

    private:
        int type;
        double learning_rate;
        double momentum;
        vector<string> learning_algo;
        vector<int> architecture;
        vector<string> type_str;
        vector<Matrix> weights;
        vector<Matrix> delta_weights;
        vector<Matrix> gradients;
        vector<vector<double>> nodes_outputs;
        void adjust_weights();
        void compute_gradient(double);
        void initialize_weights();
        double sigmoid(double);
        double compute_error(vector<vector<double>>,vector<vector<double>>);
        
    public:
        Backpropagation(int,vector<int>);
        
        void __train__(vector<vector<double>>);
        vector<double> __predict__(vector<double>);


};

#endif