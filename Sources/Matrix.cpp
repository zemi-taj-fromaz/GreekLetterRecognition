#include "../Headers/Matrix.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>

void Matrix::LU_Decompose(){
    try{
        A = matrix;
        int n = matrix.size();
        for(int k = 0; k < n-1; k++){
            if(abs(A[k][k]) < epsilon) throw std::runtime_error("Element na dijagonali blizu 0!");
            for(int i = k+1; i < n; i++){
                A[i][k] /= A[k][k];
                for(int j =k+1; j < n; j++){
                    A[i][j]-=A[i][k]*A[k][j];
                }
            }
        }
        
    } catch(const std::exception& e){
        std::cout << "Caught exception: " << e.what() << '\n';
    }
}

void Matrix::LUP_Decompose(){
    int n = matrix.size();
    A = matrix;
    
    for(int i = 0; i < n; i++){
        P.push_back(i);
    }

    int pivot;
    for (int i = 0; i < n-1; i++)
    {
        pivot = i;
        for (int j = i+1; j < n; j++)
        {
            if(abs(A[P[j]][i]) > abs(A[P[pivot]][i])) pivot = j;
        }
        
        int temp = P[i];
        P[i] = P[pivot];
        P[pivot] = temp;

        for (int j = i+1; j < n; j++)
        {
            A[P[j]][i] /= A[P[i]][i];
            for (int k = i+1; k < n; k++)
            {
                A[P[j]][k] -= A[P[j]][i] * A[P[i]][k];
            }
            
        }
    }
    vector<vector<double>> dp;
    for(int i = 0; i<P.size(); i++){
        dp.push_back(A[P[i]]);
    }
    A = dp;
    
}

vector<string> Matrix::split(string str, string token){
    vector<string>result;
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

Matrix::Matrix(int r, int c){
    rows = r;
    columns = c;
    vector<double> vd(c,0.0);
    vector<vector<double>> vvd(r,vd);
    // for(int i = 0; i < rows; i++){
    //     vvd[i][i] = 1;
    // }
    matrix = vvd;
}

Matrix::Matrix(string filename){
    filesystem::path cwd = std::filesystem::current_path() / filename;
    ifstream file(cwd.string());
    if(file){
        string row;
        while(file.good()){
            getline(file,row);
            auto vs = split(row," ");
            vector<double> vd(vs.size(),0.0);
            for (int i = 0; i < vs.size(); i++)
            {
                vd[i] = stod(vs[i]);
            }
            matrix.push_back(vd);
        }
    }
    rows = matrix.size();
    columns = matrix[0].size();
}

Matrix::Matrix(vector<vector<double>> m){
    matrix = m;
    rows = m.size();
    columns = m[0].size();
}

void Matrix::toFile(string filename){
    filesystem::path cwd = std::filesystem::current_path() / filename;
    ofstream file(cwd.string());
    if(file){
        for(auto row : matrix){
            string s;
            for(auto d : row){
                s+=to_string(d) + " ";
            }
            s+="\n";
            file << s;
        }
        file.close();
    }
}

void Matrix::Print(){
    cout<<endl;
    for(auto row : matrix){
        for(auto d : row){
            cout<<d<<" ";
        }
        cout<<"\n";
    }
    cout<<endl;
}

double Matrix::get(int rind,int cind){
    return matrix[rind][cind];
}

Matrix* Matrix::set(double d, int rind, int cind){
    matrix[rind][cind] = d;
    return this;
}

vector<vector<double>>& Matrix::getMatrix(){
    return matrix;
}
vector<vector<double>>& Matrix::getDecompMatrix(){
    return A;
}


void Matrix::T(){
    vector<vector<double>> transposed_matrix;
    for(int j = 0; j < columns; j++){
        vector<double> vt(rows,0);
        for (int i = 0; i < rows; i++){
            vt[i] = matrix[i][j];
        }
        transposed_matrix.push_back(vt);
    }
    matrix = transposed_matrix;
    int temp = rows;
    rows = columns;
    columns = temp;
}

Matrix Matrix::operator-(Matrix& operand){
    vector<double> vd(columns,0);
    vector<vector<double>> res(rows,vd);
    auto operand1 = matrix;
    auto operand2 = operand.getMatrix();
    for(int i = 0; i <rows; i++){
        for(int j = 0; j<columns; j++){
            res[i][j] = operand1[i][j] - operand2[i][j];
        }
    }
    Matrix ret(res);
    return ret;
}

Matrix Matrix::operator+(Matrix& operand){
    vector<double> vd(columns,0);
    vector<vector<double>> res(rows,vd);
    auto operand1 = matrix;
    auto operand2 = operand.getMatrix();
    for(int i = 0; i <rows; i++){
        for(int j = 0; j<columns; j++){
            res[i][j] = operand1[i][j] + operand2[i][j];
        }
    }
    Matrix ret(res);
    return ret;
}

void Matrix::operator+=(Matrix& operand){
    auto mat = operand.getMatrix();
    for(int i = 0; i <rows; i++){
        for(int j = 0; j<columns; j++){
            matrix[i][j] += mat[i][j];
        }
    }
}

void Matrix::operator-=(Matrix& operand){
    auto mat = operand.getMatrix();
    for(int i = 0; i <rows; i++){
        for(int j = 0; j<columns; j++){
            matrix[i][j] -= mat[i][j];
        }
    }
}

bool Matrix::operator==(Matrix& operand){
    auto mat = operand.getMatrix();
    for(int i = 0; i <rows; i++){
        for(int j = 0; j<columns; j++){
            if(abs(matrix[i][j] - mat[i][j]) > epsilon) return false;
           // if(matrix[i][j] == mat[i][j]) return false;
        }
    }
    return true;
}

void Matrix::operator=(Matrix& operand){
    auto mat = operand.getMatrix();
    rows = mat.size();
    columns = mat[0].size();
    matrix = mat;
}

Matrix Matrix::operator*(Matrix operand){
    operand.T();
    vector<double> vd(operand.getMatrix().size(),0);
    vector<vector<double>> res(rows,vd);
    auto operand1 = matrix;
    auto operand2 = operand.getMatrix();
    int i = 0,j;
    for(auto a : operand1){
        j = 0;
        for(auto b : operand2){
            res[i][j] = inner_product(begin(a),end(a),begin(b),0.0);
            j++;        
        }
        i++;

    }
    Matrix ret(res);
    return ret;
}

void Matrix::operator*=(double lambda){
    for(int i = 0; i < matrix.size(); i++){
        for (int j = 0; j < matrix[i].size(); j++)
        {
            matrix[i][j] *= lambda;
        }
        
    }
}

void Matrix::size(){
    cout<<rows<<" "<<columns;
}

Matrix* Matrix::forward_substitution(Matrix b){
    int n = matrix.size();
    auto dp = b.getMatrix();
    vector<vector<double>> b_vekt;
    for (int i = 0; i < n; i++){
        b_vekt.push_back(dp[P[i]]);
    }
    for(int i = 0; i < n-1; i++){
        for(int j = i+1; j < n; j++){
            b_vekt[j][0] -= A[j][i] * b_vekt[i][0];
        }
    }
    return new Matrix(b_vekt);
}

Matrix* Matrix::backward_substitution(Matrix y){
    try{
        int n = matrix.size();
        auto x_vekt = y.getMatrix();
        for(int i = n-1; i >= 0  ; i--){
            if(abs(A[i][i]) < epsilon){
                throw string("STOZERNI ELEMENT JE 0!");
            }
            x_vekt[i][0] /= A[i][i];
            for(int j = 0; j < i; j++){
                x_vekt[j][0] -= A[j][i] * x_vekt[i][0];
            }
        }
        return new Matrix(x_vekt);
    } catch(string s){
        cout<<s<<endl;
        return nullptr;
    }
}

Matrix* Matrix::inverse(){
    try{
        if (abs(det(this->matrix)) < epsilon){
            throw std::runtime_error("Inverse doesn't exist!");
        }

        this->LUP_Decompose();
        vector<vector<double>> e;
        int n = matrix.size();
        vector<vector<double>> dp(n,vector<double>(n,0));
        for(int j = 0;j<n;j++){
            e.push_back(vector<double>({0}));
        }

        for (int i = 0; i < n; i++)
        {
            e[i][0] = 1;

            Matrix* y;
            y = this->forward_substitution(e);
            Matrix* x;
            x = this->backward_substitution(*y);
            auto inv = x->getMatrix();

            for(int j = 0; j < n ; j++){
                dp[j][i] = inv[j][0];
            }

            e[i][0] = 0;
        }

        return new Matrix(dp);
        
    } catch(const std::exception& e){
        std::cout << "Caught exception: " << e.what() << '\n';
    }
}

double Matrix::det(vector<vector<double>> dp){
    int n = dp.size();
    int m = dp[0].size();
    if(m == 2 && n == 2){
        return dp[0][0]*dp[1][1]-dp[1][0]*dp[0][1];
    }
    double ret = 0;
    double ind = 1;
    vector<double> first_row = dp[0];
    vector<vector<double>> help(dp.begin()+1,dp.end());
    for(int i = 0; i < m ; i++){
        vector<vector<double>> help(dp.begin()+1,dp.end());
        for(int j = 0; j < help.size(); j++){
            help[j].erase(help[j].begin()+i);
        }
        ret += ind*first_row[i]*Matrix::det(help);
        ind*=-1;
    }
    return ret;
}

void Matrix::swap_rows(Matrix& m, int i, int j){
    auto mat = m.getMatrix();
    auto temp = mat[i];
    mat[i] = mat[j];
    mat[j] = temp;

   
    Matrix n(mat);
    m = n;
}

void Matrix::swap_columns(Matrix& m, int i, int j){
    m.T();
    Matrix::swap_rows(m,i,j);
    m.T();
}

