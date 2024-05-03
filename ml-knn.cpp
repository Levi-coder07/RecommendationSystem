#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <chrono>
using namespace std;
unordered_map<int, vector<pair<int, double>>> userRatings;
vector<int> globalVector;
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
pair<double,int> euclideanDistance(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2 ,int common) {
    double sum = 0.0;
    int commonMovies = 0; // 
    unordered_map<int, double> ratingsMapV2;
    
    //Crear un mapa de ratings 
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Distancia 
    for (const auto& rating : v1) {
        // Check common
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end() ) {
            //cout<<"Movie en comun : "<< rating.first <<" rating : "<<rating.second<<" - "<<ratingsMapV2[rating.first]<<endl;
            double diff = rating.second - ratingsMapV2[rating.first];
          
            sum += diff * diff;
            
            commonMovies++;
        }
    }
    
    //Al menos una peli
    if (commonMovies >= common) {
        
        return {sqrt(sum),commonMovies};
    } else {
        return {-1,0}; // NO hay,
    }
}
pair<double, int> manhattanDistance(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2, int common) {
    double sum = 0.0;
    int commonMovies = 0; // Contador de películas en común
    unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings para el vector v2
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular la distancia de Manhattan
    for (const auto& rating : v1) {
        // Verificar si la película en el rating del usuario v1 también está en el vector v2
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end()) {
            sum += abs(rating.second - ratingsMapV2[rating.first]);
            commonMovies++;
        }
    }
    
    // Verificar si hay suficientes películas en común
    if (commonMovies >= common) {
        return {sum, commonMovies};
    } else {
        return {-1.0, 0}; // Si no hay suficientes películas en común, devolver un valor inválido
    }
}
pair<double, int> pearsonCorrelation(const vector<pair<int, double>>& v1, const vector<pair<int, double>>& v2, int common) {
    double sumXY = 0.0, sumX = 0.0, sumY = 0.0, sumX2 = 0.0, sumY2 = 0.0;
    int commonMovies = 0; // Contador de películas en común
    unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings para el vector v2
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular la correlación de Pearson
    for (const auto& rating : v1) {
        // Verificar si la película en el rating del usuario v1 también está en el vector v2
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end()) {
            double ratingV2 = ratingsMapV2[rating.first];
            sumXY += rating.second * ratingV2;
            sumX += rating.second;
            sumY += ratingV2;
            sumX2 += rating.second * rating.second;
            sumY2 += ratingV2 * ratingV2;
            commonMovies++;
        }
    }
    
    // Verificar si hay suficientes películas en común
    if (commonMovies >= common) {
        double numerator = commonMovies * sumXY - sumX * sumY;
        double denominator = sqrt((commonMovies * sumX2 - sumX * sumX) * (commonMovies * sumY2 - sumY * sumY));
        
        if (denominator == 0) {
            return {0.0, commonMovies}; // Evitar división por cero
        }
        
        return {numerator / denominator, commonMovies};
    } else {
        return {0.0, 0}; // Si no hay suficientes películas en común, devolver un valor inválido
    }
}

std::pair<double, int> cosineSimilarity(const std::vector<std::pair<int, double>>& v1, 
                                        const std::vector<std::pair<int, double>>& v2, 
                                        int common) {
    double dotProduct = 0.0, mag1 = 0.0, mag2 = 0.0;
    int commonMovies = 0; // Contador
    std::unordered_map<int, double> ratingsMapV2;
    
    // Crear un mapa de ratings
    for (const auto& rating : v2) {
        ratingsMapV2[rating.first] = rating.second;
    }
    
    // Calcular 
    for (const auto& rating : v1) {
        // Verificar 
        if (ratingsMapV2.find(rating.first) != ratingsMapV2.end()) {
            dotProduct += rating.second * ratingsMapV2[rating.first];
            mag1 += rating.second * rating.second;
            mag2 += ratingsMapV2[rating.first] * ratingsMapV2[rating.first];
            commonMovies++;
        }
    }
    
    // Calcular 
    if (commonMovies >= common) {
        mag1 = sqrt(mag1);
        mag2 = sqrt(mag2);
        
        if (mag1 == 0 || mag2 == 0) {
            return {0.0, commonMovies}; // Evitar división por cero
        }
        
        double similarity = dotProduct / (mag1 * mag2);
        return {similarity, commonMovies};
    } else {
        return {0.0, 0}; // No hay 
    }
}
    vector<pair<int, pair<double, int>>> findKNearestNeighbors(int userId, int k , int type , int common) {
    vector<pair<int, pair<double, int>>> neighbors; // Guardar el par (userId, distancia)
    pair<double,int> distance;
    for (const auto& [otherUserId, ratings] : userRatings) {
        if (otherUserId != userId) {
            if(type == 0){
                 distance = euclideanDistance(userRatings[userId], ratings,common);
            }else if (type == 1){
                 distance = manhattanDistance(userRatings[userId], ratings,common);
            }else if(type == 2){
                 distance = cosineSimilarity(userRatings[userId], ratings,common);
            }else{
                 distance = pearsonCorrelation(userRatings[userId], ratings,common);
            }
            
            if(distance.first>=0&&distance.second>0){
                
                neighbors.push_back({otherUserId, distance});
            }
            
        }
    }
    
    if(type == 0 || type == 1){
       sort(neighbors.begin(), neighbors.end(), [](const pair<int, pair<double, int>>& a, const pair<int, pair<double, int>>& b) {
    // Ordenar primero por distancia
        
        if (a.second.first != b.second.first) {
            return a.second.first < b.second.first;
        } else if (a.second.second != b.second.second) {
            // Si las distancias son iguales, ordenar por número de películas en común
            return a.second.second > b.second.second;
        } else {
            // Si las distancias y el número de películas en común son iguales, ordenar por ID
            return a.first < b.first;
        }
        });
    }else {
        
       sort(neighbors.begin(), neighbors.end(), [](const pair<int, pair<double, int>>& a, const pair<int, pair<double, int>>& b) {
    // Ordenar primero por distancia
        
        if (a.second.first != b.second.first) {
            return a.second.first > b.second.first;
        } else if (a.second.second != b.second.second) {
            // Si las distancias son iguales, ordenar por número de películas en común
            return a.second.second > b.second.second;
        } else {
            // Si las distancias y el número de películas en común son iguales, ordenar por ID
            return a.first < b.first;
        }
        });
       
    }
    
    if (neighbors.size() > k) {
        neighbors.resize(k);
    }
    
    return neighbors;
}

bool containsMovie(const vector<pair<int, double>>& ratings, int movieId) {
    return find_if(ratings.begin(), ratings.end(), [movieId](const pair<int, double>& p) {
        return p.first == movieId; // funcion basada en el primer par de vectores para verificar si esta o no 
    }) != ratings.end();
}
pair<int, double> findMovie(const vector<pair<int, double>>& ratings, int movieId) {
    auto it = find_if(ratings.begin(), ratings.end(), [movieId](const pair<int, double>& p) {
        return p.first == movieId; // Función basada en el primer elemento del par para verificar si se encuentra
    });
    
    if (it != ratings.end()) {
        return *it; // Devuelve el par encontrado
    } else {
        // Si el par no se encuentra, devuelve un par por defecto (por ejemplo, (-1, 0.0))
        return make_pair(-1, 0.0); // Cambia estos valores según sea necesario
    }
}

int main() {
    ifstream file("D:\\Ciencia_Datos\\ml-latest\\ratings.csv");
    ifstream file2("D:\\Ciencia_Datos\\ml-latest\\movies.csv");
    if (!file.is_open()) {
        cerr << "Error: Unable to open file.\n";
        return 1;
    }
    for (int i = 0; i <= 3; ++i) {
        globalVector.push_back(i);
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
    for (auto& [userId, ratings] : userRatings) {
    sort(ratings.begin(), ratings.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        if (a.second != b.second) {
            return a.second > b.second; // Ordenar rating, mayor a menor
        } else {
            return a.first < b.first; // Si el rating es igual, ordenar por ID de película
        }
    });
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
            // Agregar rating
            movies[movieID] = MovieName;
        }
    }
    int uID = 0;
    int oID = 0;
    //cin>>uID>>oID;
    long long numMovies = data.size();
    int userId = 330976;
    int k = 10; // Vecinos

    // Calcula el tiempo para la distancia de Manhattan
    /*auto startManhattan = std::chrono::steady_clock::now();
    double manhattanDist = manhattanDistance(userRatings[uID], userRatings[oID], 0).first;
    auto endManhattan = std::chrono::steady_clock::now();
    auto diffManhattan = endManhattan - startManhattan;
    cout << "Tiempo de ejecución para la distancia de Manhattan: " << std::chrono::duration <double, std::milli> (diffManhattan).count() << " ms" << endl;

    // Calcula el tiempo para la distancia Euclidiana
    auto startEuclidean = std::chrono::steady_clock::now();
    double euclideanDist = euclideanDistance(userRatings[uID], userRatings[oID], 0).first;
    auto endEuclidean = std::chrono::steady_clock::now();
    auto diffEuclidean = endEuclidean - startEuclidean;
    cout << "Tiempo de ejecución para la distancia Euclidiana: " << std::chrono::duration <double, std::milli> (diffEuclidean).count() << " ms" << endl;

    // Calcula el tiempo para la correlación de Pearson
    auto startPearson = std::chrono::steady_clock::now();
    double pearsonCorr = pearsonCorrelation(userRatings[uID], userRatings[oID], 0).first;
    auto endPearson = std::chrono::steady_clock::now();
    auto diffPearson = endPearson - startPearson;
    cout << "Tiempo de ejecución para la correlación de Pearson: " << std::chrono::duration <double, std::milli> (diffPearson).count() << " ms" << endl;

    // Calcula el tiempo para la similitud del coseno
    auto startCosine = std::chrono::steady_clock::now();
    double cosineSim = cosineSimilarity(userRatings[uID], userRatings[oID], 0).first;
    auto endCosine = std::chrono::steady_clock::now();
    auto diffCosine = endCosine - startCosine;
    cout << "Tiempo de ejecución para la similitud del coseno: " << std::chrono::duration <double, std::milli> (diffCosine).count() << " ms" << endl;
    */
    // Calcula el tiempo para el algoritmo de los k vecinos más cercanos (KNN)
    auto startKNN = std::chrono::steady_clock::now();
    vector<pair<int, pair<double, int>>> nearestNeighbors = findKNearestNeighbors(userId, k, 0, 3);
    auto endKNN = std::chrono::steady_clock::now();
    auto diffKNN = endKNN - startKNN;
    cout << "Tiempo de ejecución para el algoritmo de los k vecinos más cercanos (KNN): " << std::chrono::duration <double, std::milli> (diffKNN).count() << " ms" << endl;
   

    /*330976,230,5.0,1111111111
330976,318,5.0,1111111111
330976,436,5.0,1111111111
330976,527,5.0,1111111111
330976,1233,5.0,1111111111
330976,858,5.0,1111111111*/
    cout << "K Nearest Neighbors para el usuario " << userId << ":" << endl;
    for (const auto& [neighborId, distance] : nearestNeighbors) {
        int count = 0;
        cout << "Usuario  " <<  userId <<" vecino : "<<neighborId<< " con distancia "<<distance.first<< " en peliculas en común " <<distance.second<<endl;
        for (auto [movie, rating] : userRatings[neighborId]) {
            
            // Si no esta calificado , recomendar2
            if (!containsMovie(userRatings[userId], movie) ) {
                if(rating>=3){
                    if (count >= 5) {
                        continue;
                    }
                    cout << "Usuario : " << neighborId << " -- movieid: " <<movie<< movies[movie] <<" -- rating : "<<rating<< endl;
                    count++;
                }      
            }else{ //En comun
                    pair<int,double> par = findMovie(userRatings[userId], movie);
                   
                    cout << "Pelicula en comun " <<  movies[movie] <<" || "<< par.second <<" rating : "<<rating<< endl;
            }

        }
    }
    
    file.close(); 
    return 0;
}
