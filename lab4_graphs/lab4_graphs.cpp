//19. Имеется сеть автомобильных дорог.По  некоторым дорогам
//можно  проехать  только  в одном  направлении.Известна  длина
//каждой дороги, причем она может быть разной в  зависимости  от
//направления.С помощью алгоритма Дейкстры найти цикл минимальной
//длины из заданного города(9).
//Смирнов Александр ПС-21
//Язык: C++ 
//Visual Studio 2022

#include <iostream>
#include <vector>
#include <fstream>

const int INF = 1000000000;

struct Edge {
    int to, weight;
};

void dijkstraCycle(int start, const std::vector<std::vector<Edge>>& graph) {
    int n = graph.size();
    int minCycle = INF;

    for (int i = 0; i < n; ++i) {
        std::vector<int> dist(n, INF);
        dist[start] = 0;
        std::vector<bool> visited(n, false);

        for (int j = 0; j < n; ++j) {
            int u = -1;
            for (int k = 0; k < n; ++k) {
                if (!visited[k] && (u == -1 || dist[k] < dist[u])) {
                    u = k;
                }
            }

            if (u == -1 || dist[u] == INF) break;

            visited[u] = true;

            for (const Edge& edge : graph[u]) {
                int v = edge.to;
                int weight = edge.weight;

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                }
            }
        }

        for (const Edge& edge : graph[i]) {
            if (edge.to == start) {
                if (dist[i] + edge.weight < minCycle) {
                    minCycle = dist[i] + edge.weight;
                }

            }
        }
    }

    if (minCycle == INF) {
        std::cout << "Цикл не найден из города " << start << "\n";
    } else {
        std::cout << "Длина минимального цикла начинается от вершины " << start << " это " << minCycle << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    std::ifstream file("graph.txt");
    if (!file) {
        std::cerr << "Ошибка при открытии файла\n";
        return 1;
    }

    int n, m;
    file >> n >> m;

    std::cout << "Количество вершин: " << n << "\n";
    std::cout << "Количество рёбер: " << m << "\n";

    std::vector<std::vector<Edge>> graph(n);

    for (int i = 0; i < m; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        std::cout << u << " -> " << v << " вес: " << w << "\n";
        graph[u].push_back({ v, w });
    }

    int startCity = 1;
    dijkstraCycle(startCity, graph);

    return 0;
}
