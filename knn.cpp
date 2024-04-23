#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

double euclideanDistance(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0.0;
    for (long long i = 0; i < v1.size(); ++i) {
        sum += pow(v1[i] - v2[i], 2);
    }
    return sqrt(sum);
}

double manhattanDistance(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0.0;
    for (long long i = 0; i < v1.size(); ++i) {
        sum += abs(v1[i] - v2[i]);
    }
    return sum;
}

double pearsonCorrelation(const vector<double>& v1, const vector<double>& v2) {
    double sumXY = 0.0, sumX = 0.0, sumY = 0.0, sumX2 = 0.0, sumY2 = 0.0;
    long long n = v1.size();

    for (long long i = 0; i < n; ++i) {
        sumXY += v1[i] * v2[i];
        sumX += v1[i];
        sumY += v2[i];
        sumX2 += v1[i] * v1[i];
        sumY2 += v2[i] * v2[i];
    }

    double numerator = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

    if (denominator == 0) {
        return 0; 
    }

    return numerator / denominator;
}
vector<vector<double>> transpose(const vector<vector<double>>& matrix) {
    if (matrix.empty()) {
        return {};
    }
    long long rows = matrix.size();
    long long cols = matrix[0].size();
    vector<vector<double>> result(cols, vector<double>(rows));
    for (long long i = 0; i < cols; ++i) {
        for (long long j = 0; j < rows; ++j) {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}
vector<pair<long, double>> findKNearestNeighbors(const vector<vector<double>>& data, long userIndex, long k) {
    vector<pair<long, double>> neighbors; // Guardar el par (index,distancia)
    for (long i = 0; i < data.size(); ++i) {
        if (i != userIndex) {
            double dist = euclideanDistance(data[userIndex], data[i]);
            neighbors.push_back({i, dist});
        }
    }
    
    sort(neighbors.begin(), neighbors.end(), [](const pair<long, double>& a, const pair<long, double>& b) {
        return a.second < b.second;
    });
    if (neighbors.size() > k) {
        neighbors.resize(k);
    }
    return neighbors;
}

int main() {
    ifstream file("D:\\Ciencia_Datos\\Movie_Ratings.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file.\n";
        return 1;
    }

    //Read 
    vector<vector<double>> data;
    string line;
    string firstLine;
    if (!getline(file, firstLine)) {
        return 1;
    }
    istringstream issNames(firstLine);
    vector<string> personNames;
    string name;

   
        while (getline(issNames, name, ',')) {
            if(name!=""){
                personNames.push_back(name);
            }
            
        }
    for(auto i : personNames){
        cout<<i<<" ";
    }
    vector<string> movies;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<double> row;
        
        string value;
        string id;
       
        getline(iss, id, ',');
        movies.push_back(id); 
        while (getline(iss, value, ',')) {
            if (value == "") {
                row.push_back(-1);
            } else {
                row.push_back(stoi(value));
            }
        }
        data.push_back(row);
    }

    // Print the data for verification
    /*for ( auto row : data) {
        for (double value : row) {
            cout << value << " ";
        }
        cout << endl;
    }*/
    
    long long numMovies = data.size();
    vector<vector<double>> transposedData = transpose(data);
    for (const auto& row : transposedData) {
        for (double value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
    for (long long i = 0; i < numMovies; ++i) {
        for (long long j = i + 1; j < numMovies; ++j) {
            double pearsonCorr = pearsonCorrelation(data[i], data[j]);
            cout << "Movie " << movies[i] << " y Movie " << movies[j] << ":\n";
            cout << "Pearson Correlation: " << pearsonCorr << endl;
            cout << endl;
        }
    }
    long long numUsers = personNames.size();

    
    for (long long i = 0; i < numUsers; ++i) {
        for (long long j = i + 1; j < numUsers; ++j) {
            double manhattanDist = manhattanDistance(transposedData[i], transposedData[j]);
            double euclideanDist = euclideanDistance(transposedData[i], transposedData[j]);
            cout << "Usuario " << personNames[i] << " and Usuario " << personNames[j] << ":\n";
            cout << "Manhattan Distance: " << manhattanDist << endl;
            cout << "Euclidean Distance: " << euclideanDist << endl;
            cout << endl;
        }
    }
    int userIndex = 18;

   
    int k = 3; 
    vector<pair<long, double>> nearestNeighbors = findKNearestNeighbors(transposedData, userIndex, k);

    
    cout << "K Nearest Neighbors para el Usuario " << personNames[userIndex] << ":" << endl;
    for (const auto& neighbor : nearestNeighbors) {
        int mayor = 0;
        int index = 0;
        int val = 0;
        cout << "User " << personNames[neighbor.first] << " - Distance: " << neighbor.second << endl;
        for (auto value : transposedData[neighbor.first]) {
            if(value>mayor){
                mayor = value;
                val = index;
            }
            index++;
        }
        cout<<"Recomendacion "<<personNames[index]<<": "<<movies[val]<<endl;
    }
    
    file.close(); 
    return 0;
}
