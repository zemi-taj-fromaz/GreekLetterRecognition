#include "../Headers/Backpropagation.hpp"
#include <random>
#include <algorithm>
#include <limits>
#include <cmath>

void Backpropagation::initialize_weights(){

    int matrices = this->architecture.size() - 1;
    default_random_engine generator;
    normal_distribution<double> distribution(0,1.0);

    for(int i = 0; i < architecture.size(); i++){
        nodes_outputs.push_back({0});
    }

    for (int i = 0; i < matrices; i++)
    {   
        vector<vector<double>> m;
        for (int j = 0; j < architecture[i]; j++)
        {   
            vector<double> row;
            for (int k = 0; k < architecture[i+1]; k++)
            {
                row.push_back(distribution(generator));
            }
            m.push_back(row);
            row.clear();            
        }
        Matrix* matrix = new Matrix(m);
        Matrix* gradient = new Matrix(m.size(),m[0].size());
        Matrix* delta_weight_matrix = new Matrix(m.size(),m[0].size());
        this->weights.push_back(*matrix);
        this->delta_weights.push_back(*delta_weight_matrix);
        this->gradients.push_back(*gradient);

        m.clear();
    }
}

Backpropagation::Backpropagation(int type, vector<int> architecture){
    learning_algo.push_back("Stohastic Gradient Descent");
    learning_algo.push_back("Mini-Batch Gradient Descent");
    learning_algo.push_back("Batch Gradient Descent");
    this->type = type;
    this->architecture = architecture;
    
    initialize_weights();

    learning_rate = 0.7;
    momentum = 0.3;
    
}

double Backpropagation::sigmoid(double x){
    return 1/(1+exp(-x));
}

vector<double> Backpropagation::__predict__(vector<double> input){
    Matrix* input_matrix = new Matrix(vector<vector<double>>() = {input});
    nodes_outputs[0] = input;

    int c = 1;
    for(auto matrix : weights){
        Matrix A  = (*input_matrix) * matrix;
        input_matrix = new Matrix(A.getMatrix());
        for(int i = 0; i < input_matrix->getMatrix()[0].size(); i++){
            input_matrix->getMatrix()[0][i] = sigmoid(input_matrix->getMatrix()[0][i]);
        }
        nodes_outputs[c++] = input_matrix->getMatrix()[0];
    }

    return input_matrix->getMatrix()[0];
}

double Backpropagation::compute_error(vector<vector<double>> real,vector<vector<double>> pred){

    int n = real.size();
    int m = real[0].size();

    double err = 0;

    for(int i = 0; i < n ; i++){
        for(int j = 0; j < m; j++){
            err += pow(real[i][j] - pred[i][j],2);
        }
    }

    err /= 2*n;

    return err;

}

void Backpropagation::compute_gradient(double err){
    vector<vector<double>> delta;

    vector<double> last_layer_delta;

    int last_layer_size = architecture[architecture.size()-1];
    for(int i = 0; i < last_layer_size; i++){
        double node_output = nodes_outputs[architecture.size()-1][i];
        double sigmoid_derivation = node_output*(1-node_output);
        last_layer_delta.push_back(-sigmoid_derivation*err);
    }
    delta.push_back(last_layer_delta);
    

    
    for(int i = architecture.size() - 2; i > 0; i--){
        vector<double> layer_delta(architecture[i],0);
        vector<vector<double>> layer_weights = weights[i].getMatrix();

        for(int j = 0; j < architecture[i]; j++){

            double node_output = nodes_outputs[i][j];
            double sigmoid_derivation = node_output*(1-node_output);
            layer_delta[j] = sigmoid_derivation;

            double sum = 0;
            for(int k = 0; k < last_layer_delta.size(); k++){
                sum+=layer_weights[j][k] * last_layer_delta[k];
            }

            layer_delta[j] *= sum;
        }
        delta.push_back(layer_delta);
        last_layer_delta = layer_delta;
    }
    reverse(delta.begin(),delta.end());
    // cout<<"DELTA\n";
    // for(int i = 0; i < delta.size(); i++){
    //     for(int j = 0; j< delta[i].size(); j++){
    //         cout<<delta[i][j]<<" ";
    //     }cout<<endl;
    // }


    for(int i = 0; i < gradients.size(); i++){
        // cout<<"BEFORe : ";
        // gradients[i].Print();
        vector<vector<double>> grad = gradients[i].getMatrix();
        //cout<<grad.size()<<" "<<grad[i].size()<<endl;
        for(int j = 0; j < grad.size(); j++){
            for(int k = 0; k < grad[0].size(); k++){
                grad[j][k] = delta[i][k] * nodes_outputs[i][j];
                //cout<<grad[j][k]<<" ";
            }//cout<<endl;
        }
        Matrix m(grad);
        gradients[i] = m;
        // cout<<"AFTER : ";
        // gradients[i].Print();
    }
}

void Backpropagation::adjust_weights(){
    for(int i = 0; i < gradients.size(); i++){
        vector<vector<double>> grad = gradients[i].getMatrix();
        vector<vector<double>> weight = weights[i].getMatrix();
        vector<vector<double>> delta_weight = delta_weights[i].getMatrix();
        double delta_w_jk;
        for(int j = 0; j < grad.size(); j++){
            for(int k = 0; k < grad[0].size(); k++){
                double delta_w_jk = learning_rate*grad[j][k] + momentum*delta_weight[j][k];
                delta_weight[j][k] = delta_w_jk;
                weight[j][k] += delta_w_jk;
            }
        }
        Matrix d_w(delta_weight);
        Matrix w(weight);
        weights[i] = w;
        delta_weights[i] = d_w;
    }
}

void Backpropagation::__train__(vector<vector<double>> data){
    int input_size = architecture[0];
    int output_size = architecture[architecture.size()-1];

    vector<vector<double>> input;
    vector<vector<double>> output_real;

    for(int i = 0; i < data.size(); i++){
        vector<double> input_row(data[i].begin(), data[i].begin() + input_size);
        vector<double> output_row(data[i].end()-output_size, data[i].end());
        input.push_back(input_row);
        output_real.push_back(output_row);
    }
    switch(type)
    {        
            case 1 : 
            {
                int epoch = 1;
                double error = numeric_limits<double>::max();
                while(epoch < 8)
                {   
                    
                    cout<<"epoch : "<<epoch<<"\n";
                    for(int i = 0; i < input.size(); i++){
                        auto output_predicted = this->__predict__(input[i]);
                        error = this->compute_error(vector<vector<double>>() = {output_real[i]},vector<vector<double>>() = {output_predicted});
                        compute_gradient(error);
                        adjust_weights();
                        cout<<"     input : "<<i<<" err : "<<error<<"\n";

                    }
                        epoch++;
                }
            }
            case 2:
            {
                int epoch = 1;
                double error = numeric_limits<double>::max();
                while(epoch < 8)
                {   
                    
                    cout<<"epoch : "<<epoch<<"\n";
                    for(int i = 0; i < input.size(); i++){
                        vector<vector<double>> output_predicted_mini_batch;
                        vector<vector<double>> output_real_mini_batch;
                        for(int j = 0; j < input.size()/5 && i < input.size(); j++)
                        {        
                            auto output_predicted = this->__predict__(input[i]);
                            output_predicted_mini_batch.push_back(output_predicted);
                            output_real_mini_batch.push_back(output_real[i]);
                            i++;
                        }
                        error = this->compute_error(output_real_mini_batch,output_predicted_mini_batch);
                        compute_gradient(error);
                        adjust_weights();
                        cout<<"    mini batch : "<<i*5/input.size()<<" err : "<<error<<"\n";

                    }
                        epoch++;
                }
            }
            case 3: {

                int epoch = 1;
                double error = numeric_limits<double>::max();
                while(epoch < 20)
                {   
                    cout<<"epoch : "<<epoch<<"\n";
                    vector<vector<double>> output_predicted_batch;
                    for(int i = 0; i < input.size(); i++){
                        auto output_predicted = this->__predict__(input[i]);
                        output_predicted_batch.push_back(output_predicted);
                    }
                    error = this->compute_error(output_real, output_predicted_batch);
                    compute_gradient(error);
                    adjust_weights();
                    cout<<"        err : "<<error<<"\n";
                    epoch++;
                }

            }
    }
    
}





