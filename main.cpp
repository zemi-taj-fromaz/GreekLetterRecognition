//CTRL+SHIFT+P for user snippets
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "Sources/ExtractFeatures.cpp"
#include "Sources/Backpropagation.cpp"
#include "Sources/Matrix.cpp"


using namespace std;

#define iterator vector<int>::iterator
#define ll long long

vector<double> alpha({0,0,0,0,1});
vector<double> betha({0,0,0,1,0});
vector<double> gamma({0,0,1,0,0});
vector<double> delta({0,1,0,0,0});
vector<double> etha({1,0,0,0,0});
vector<vector<double>> letter_codes({alpha,betha,gamma,delta,etha});
vector<string> letters({"alpha","beta","gamma","delta","etha"});

void generateSetFromData(int M,string dirname,string filename){
    ExtractFeatures* extractFeatures = new ExtractFeatures();

    vector<string> vs;

    filesystem::path cwd = filesystem::current_path() / dirname;

    filesystem::path outputfile = filesystem::current_path() / filename;

    ofstream outfile(outputfile.string());

    for (const auto & entry : filesystem::directory_iterator(cwd.string())){
        //std::cout << entry.path() << std::endl;

        ifstream file(entry.path().string());
        if(file){
            string input;
            while(getline(file,input)){
                vs.push_back(input);
            }
        }

        vector<pair<double,double>> coordinates;

        for(int i  =1; i < vs.size() ; i++){
            auto res = ExtractFeatures::split(vs[i]," ");
            coordinates.push_back({stod(res[0]),stod(res[1])});
        }
        
        vector<pair<double,double>> features = extractFeatures->__extract__(coordinates,M);

        vector<string> letters({"alpha","beta","gamma","delta","etha"});

        string output = "";
        for(auto f : features){
            output += to_string(f.first) + " " + to_string(f.second) + " ";
        }

        bool found_letter = false;
        for(int i = 0; i < letters.size(); i++){
            if(entry.path().string().find(letters[i]) != string::npos){
                found_letter = true;
                for(int j  = 0; j < letter_codes[i].size() ; j++){
                    output += to_string(letter_codes[i][j]);
                    if(j == letter_codes[i].size() - 1){
                        output+="\n";
                    } else {
                        output+=" ";
                    }
                }   
                break;
            }
        }
        if(!found_letter) {
            output.pop_back();
            output+="\n";
        }
        outfile<<output;
    }
}

int main(int argc, char *argv[]){

    int M  = stoi(argv[1]);

    generateSetFromData(M,"training_data","training_set.txt");

    filesystem::path cwd = filesystem::current_path() / "training_set.txt";
    ifstream file(cwd.string());
    vector<string> training_data_str;
    if(file){
        string input;
        while(getline(file,input)){
            training_data_str.push_back(input);
        }
    }

    vector<vector<double>> training_data;
    vector<double> data_row_double;
    for(auto str : training_data_str){
        
        auto data_row_str = ExtractFeatures::split(str," ");
        for(auto num : data_row_str){
            data_row_double.push_back(stod(num));
        }

        training_data.push_back(data_row_double);
        data_row_double.clear();
    }



    
    
    int architecture = stoi(argv[2]);

    Backpropagation* backpropagation = new Backpropagation(architecture,vector<int>() = {2*M , 15 , 20, 15 , 5});
    backpropagation->__train__(training_data);




    //TRAINING DONE

    //TESTING

    generateSetFromData(M,"test_data","test_set.txt");
    
    //cout<<"GENERIRALI SMO DATOTEKU!"<<endl;
    

    cwd = filesystem::current_path() / "test_set.txt";
    ifstream file_in(cwd.string());
    vector<string> test_data_str;
    if(file_in){
        string input;
        while(getline(file_in,input)){
            test_data_str.push_back(input);
        }
    }
    //cout<<"TEST DATA STR"<<test_data_str.size()<<endl;
    vector<vector<double>> test_data;
    data_row_double.clear();
    for(auto str : test_data_str){
        
        auto data_row_str = ExtractFeatures::split(str," ");
        for(auto num : data_row_str){
            data_row_double.push_back(stod(num));
        }

        test_data.push_back(data_row_double);
        data_row_double.clear();
    }

    //cout<<"TEST DATA SIZE"<<test_data.size()<<endl;
    
    for(auto vd : test_data){
        auto prediction = backpropagation->__predict__(vd);
        // for(auto i : prediction){
        //     cout<<i<<" ";
        // }cout<<endl;
        auto result = max_element(prediction.begin(), prediction.end());
        int argmaxVal = distance(prediction.begin(),result);
        
        cout<<letters[letters.size()-1-argmaxVal]<<endl;
    }

    


    



        
    return 0;
}
