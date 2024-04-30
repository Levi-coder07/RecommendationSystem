#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;
unordered_map<int, vector<pair<int, double>>> userRatings;
/*double euclideanDistance(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0.0;
    for (long long i = 0; i < v1.size(); ++i) {
        if(v1[i] == -1 || v2[i] == -1){
            continue;
        }
        sum += pow(v1[i] - v2[i], 2);
    }
    return sqrt(sum);
}*/
double euclideanDistance(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2) {
    double sum = 0.0;
    int commonMovies = 0; // Contador de películas en común
    unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings 
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular la distancia 
    for (const auto& rating : v1) {
        // Verificar si hay pelicula en comun
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end() ) {
            //cout<<"Movie en comun : "<< rating.first <<" rating : "<<rating.second<<" - "<<ratingsMapV2[rating.first]<<endl;
            double diff = rating.second - ratingsMapV2[rating.first];
          
            sum += diff * diff;
            
            commonMovies++;
        }
    }
    
    // Solo calcular la distancia si hay al menos una película
    if (commonMovies > 0) {
        cout<<commonMovies<<endl;
        return sqrt(sum);
    } else {
        return -1.0; // NO hay
    }
}
double manhattanDistance(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2) {
    double sum = 0.0;
    int commonMovies = 0; // Contador de películas en común
    unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings para el vector v2
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular l distancia
    for (const auto& rating : v1) {
        // Verificar si la película en el rating del usuario v1 también está en el vector v2
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end() ) {
            sum += abs(rating.second - ratingsMapV2[rating.first]);
            commonMovies++;
        }
    }
    return sum;
}

double pearsonCorrelation(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2) {
    double sumXY = 0.0, sumX = 0.0, sumY = 0.0, sumX2 = 0.0, sumY2 = 0.0;
    int commonMovies = 0; // Contador de película
    unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings 
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular la correlación de Pearson usando solo co,mun
    for (const auto& rating : v1) {
        // Verificar los similares
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end() ) {
            double ratingV2 = ratingsMapV2[rating.first];
            sumXY += rating.second * ratingV2;
            sumX += rating.second;
            sumY += ratingV2;
            sumX2 += rating.second * rating.second;
            sumY2 += ratingV2 * ratingV2;
            commonMovies++;
        }
    }
    
    // Calcular la correlación de Pearson si hay al menos una película en común
    if (commonMovies > 0) {
        double numerator = commonMovies * sumXY - sumX * sumY;
        
        double denominator = sqrt((commonMovies * sumX2 - sumX * sumX) * (commonMovies * sumY2 - sumY * sumY));
        
        if (denominator == 0) {
            return 0; // Evitar división por cero
        }
        
        return numerator / denominator;
    } else {
        return 0; // NO hay
    }
}

double cosineSimilarity(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2) {
    double dotProduct = 0.0, mag1 = 0.0, mag2 = 0.0;
    int commonMovies = 0; // Contador de películas en común
    unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings para el vector v2
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular la similitud de coseno usando solo las películas en común
    for (const auto& rating : v1) {
        // Verificar si la película en el rating del usuario v1 también está en el vector v2
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end() ) {
            dotProduct += rating.second * ratingsMapV2[rating.first];
            mag1 += rating.second * rating.second;
            mag2 += ratingsMapV2[rating.first] * ratingsMapV2[rating.first];
            commonMovies++;
        }
    }
    
    // Calcular la similitud de coseno si hay al menos una película en común
    if (commonMovies > 0) {
        mag1 = sqrt(mag1);
        mag2 = sqrt(mag2);
        
        if (mag1 == 0 || mag2 == 0) {
            return 0; // Evitar división por cero
        }
        
        return dotProduct / (mag1 * mag2);
    } else {
        return 0; //  NO hay
    }
}

vector<pair<int, double>> findKNearestNeighbors(int userId, int k) {
    vector<pair<int, double>> neighbors; // Guardar el par (userId, distancia)
  
    for (const auto& [otherUserId, ratings] : userRatings) {
        if (otherUserId != userId) {
           
            double distance = pearsonCorrelation(userRatings[userId], ratings);
            if(distance>0){
                neighbors.push_back({otherUserId, distance});
            }
            
        }
    }
    
    sort(neighbors.begin(), neighbors.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second < b.second; // Ordenar en orden ascendente por distancia
    });

    if (neighbors.size() > k) {
        neighbors.resize(k);
    }
    return neighbors;
}

bool containsMovie(const std::vector<std::pair<int, double>>& ratings, int movieId) {
    return std::find_if(ratings.begin(), ratings.end(), [movieId](const std::pair<int, double>& p) {
        return p.first == movieId; // funcion basada en el primer par de vectores para verificar si esta o no 
    }) != ratings.end();
}
int main() {
    ifstream file("D:\\Ciencia_Datos\\ml-latest-small\\ratings.csv");
    ifstream file2("D:\\Ciencia_Datos\\ml-latest-small\\movies.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file.\n";
        return 1;
    }

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
   
    
    while (getline(file, line)) {
        stringstream ss(line);
        string userIdStr, movieIdStr, ratingStr;
        if (getline(ss, userIdStr, ',') && getline(ss, movieIdStr, ',') && getline(ss, ratingStr, ',')) {
            int userId = stoi(userIdStr);
            int movieId = stoi(movieIdStr);
            double rating = stod(ratingStr);
            
            // Agregar la calificación al usuario correspondiente
            userRatings[userId].push_back({movieId, rating});
        }
    }
   
    string line2;
    string firstLine2;
    if (!getline(file2, firstLine2)) {
        return 1;
    }
    istringstream issNames2(firstLine2);
    
    string name_2;
    while (getline(issNames2, name_2, ',')) {
        if(name_2!=""){
                personNames.push_back(name_2);
        }
            
    }
    unordered_map<int, string> movies;
    while (getline(file2, line2)) {
        stringstream ss(line2);
        string MovieID, MovieStr;
        if (getline(ss, MovieID, ',') && getline(ss, MovieStr)) {
            int movieID = stoi(MovieID);
            string MovieName = MovieStr;
            // Agregar la calificación al usuario correspondiente
            movies[movieID] = MovieName;
        }
    }
    int uID = 0;
    int oID = 0;
    cin>>uID>>oID;
    long long numMovies = data.size();
    double manhattanDist = manhattanDistance(userRatings[uID], userRatings[oID]);
    double euclideanDist = euclideanDistance(userRatings[uID], userRatings[oID]);
    double pearsonCorr = pearsonCorrelation(userRatings[uID], userRatings[oID]);
    double cosineSim = cosineSimilarity(userRatings[uID], userRatings[oID]);
    cout << "Manhattan : " << manhattanDist << endl;
    cout << "Euclidean : " << euclideanDist << endl;
    cout << "Pearson Correlation: " << pearsonCorr << endl;
    cout << "Similiradidad Coseno: " << cosineSim << endl;
    int userId = 3;
    int k = 5; // Número de vecinos más cercanos a encontrar

    vector<pair<int, double>> nearestNeighbors = findKNearestNeighbors(userId, k);
   


    cout << "K Nearest Neighbors para el usuario " << userId << ":" << endl;
    for (const auto& [neighborId, distance] : nearestNeighbors) {
        int count = 0;
        for (auto [movie, rating] : userRatings[neighborId]) {
            // Verificar si el usuario actual ha calificado la película recomendada
            if (!containsMovie(userRatings[userId], movie) && rating > 4) {
                std::cout << "User " << neighborId << " movie: " << movies[movie] <<" rating : "<<rating<< std::endl;
                count++;
                // Salir del bucle si se han hecho suficientes recomendaciones
                if (count >= 1) {
                    break;
                }
            }
        }
    }
    
    file.close(); 
    return 0;
}
