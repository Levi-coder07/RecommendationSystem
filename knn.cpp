#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
using namespace std;
using DistanceFunction = std::function<double(const vector<pair<int, double>>&, const vector<pair<int, double>>&)>;
double euclideanDistance(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0.0;
    for (long long i = 0; i < v1.size(); ++i) {
        if(v1[i] == -1 || v2[i] == -1){
            continue;
        }
        sum += pow(v1[i] - v2[i], 2);
    }
    return sqrt(sum);
}

double manhattanDistance(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0.0;
    for (long long i = 0; i < v1.size(); ++i) {
        if(v1[i] == -1 || v2[i] == -1){
            continue;
        }
        sum += abs(v1[i] - v2[i]);
    }
    return sum;
}

double pearsonCorrelation(const vector<double>& v1, const vector<double>& v2) {
    double sumXY = 0.0, sumX = 0.0, sumY = 0.0, sumX2 = 0.0, sumY2 = 0.0;
    long long n = v1.size();

    for (long long i = 0; i < n; ++i) {
        if(v1[i] == -1 || v2[i] == -1){
            continue;
        }
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
double cosineSimilarity(const vector<double>& v1, const vector<double>& v2) {
    double dotProduct = 0.0, mag1 = 0.0, mag2 = 0.0;
    for (long long i = 0; i < v1.size(); ++i) {
        if(v1[i] == -1 || v2[i] == -1){
            continue;
        }
        dotProduct += v1[i] * v2[i];
        mag1 += v1[i] * v1[i];
        mag2 += v2[i] * v2[i];
    }
    mag1 = sqrt(mag1);
    mag2 = sqrt(mag2);

    if (mag1 == 0 || mag2 == 0) {
        return 0; // Evitar división por cero
    }

    return dotProduct / (mag1 * mag2);
}
vector<pair<long, double>> findKNearestNeighbors(const vector<vector<double>>& data, long userIndex, long k,int p) {
    vector<pair<long, double>> neighbors; // Guardar el par (index,distancia)
    for (long i = 0; i < data.size(); ++i) {
        if (i != userIndex) {
            if(p==0){
                double dist = euclideanDistance(data[userIndex], data[i]);
                neighbors.push_back({i, dist});
            }else if(p==1){
                double dist = manhattanDistance(data[userIndex], data[i]);
                neighbors.push_back({i, dist});
            }
            else if(p==2){
                double dist = pearsonCorrelation(data[userIndex], data[i]);
                neighbors.push_back({i, dist});
            }
            else{
                double dist = cosineSimilarity(data[userIndex], data[i]);
                neighbors.push_back({i, dist});
            }
            
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
    
    long long numUsers = personNames.size();
    string name1 , name2;
    int userIndex = 0;
    int otherIndex = 0;
    cin>>name1>>name2;
    auto it = std::find(personNames.begin(), personNames.end(), name1);
    if (it == personNames.end())
    {
        cout<<"Name not found";
    } else
    {
    userIndex = std::distance(personNames.begin(), it);
    }
    it = std::find(personNames.begin(), personNames.end(), name2);
    if (it == personNames.end())
    {
        cout<<"Name not found";
    } else
    {
    otherIndex = std::distance(personNames.begin(), it);
    }
    
    
        
    double manhattanDist = manhattanDistance(transposedData[userIndex], transposedData[otherIndex]);
    double euclideanDist = euclideanDistance(transposedData[userIndex], transposedData[otherIndex]);
    double pearsonCorr = pearsonCorrelation(transposedData[userIndex], data[otherIndex]);
    double cosineSim = cosineSimilarity(transposedData[userIndex], transposedData[otherIndex]);
    cout << "Usuario " << personNames[userIndex] << " and Usuario " << personNames[otherIndex] << ":\n";
    cout << "Manhattan : " << manhattanDist << endl;
    cout << "Euclidean : " << euclideanDist << endl;
    cout << "Pearson Correlation: " << pearsonCorr << endl;
    cout << "Similiradidad Coseno: " << cosineSim << endl;
    cout << endl;
    string knn;
    cin>>knn;
    int valu;
    it = std::find(personNames.begin(), personNames.end(), knn);
    if (it == personNames.end())
    {
        cout<<"Name not found";
    } else
    {
    valu= std::distance(personNames.begin(), it);
    }
    int k = 3; 
    
    vector<string> distances = {"Euclidian" , "Mahanttan" , "Pearson ", "COsine"};
    for(int i = 0;i<4;i++){
        vector<pair<long, double>> nearestNeighbors = findKNearestNeighbors(transposedData, valu,k, i);
        cout<<"Usando : "<<distances[i]<<endl;
        for (const auto& neighbor : nearestNeighbors) {
        int mayor = 2;
        int index = 0;
        int val = 0;
        for (auto value : transposedData[neighbor.first]) {
            if (value > mayor && transposedData[valu][index] == -1) { // Verifica que el usuario objetivo no haya visto la película
                mayor = value;
                val = index;
            }
            index++;
    }
        if (mayor > 2) { 
            cout << "Recomendacion " << personNames[neighbor.first] << neighbor.first <<": " << movies[val] <<"con "<<distances[i] <<" : "<<neighbor.second<< endl;
        } else {
            cout << "No hay recomendaciones de peliculas nuevas para este usuario " <<personNames[neighbor.first]  << neighbor.first<<" : "<< neighbor.second<< endl;
        }
        
    }
    }
    cout << "K Nearest Neighbors para el Usuario " << personNames[valu] << ":" << endl;
    
    
    file.close(); 
    return 0;
}
