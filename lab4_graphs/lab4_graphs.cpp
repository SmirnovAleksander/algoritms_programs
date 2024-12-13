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

const int maxInt = 1000000000;

struct Graph {
    int to;
    int weight;
};

void mainCycle(int start, const std::vector<std::vector<Graph>>& graph) {
    int n = graph.size();
    int minCycle = maxInt;

    for (int i = 0; i < n; i++) {
        std::vector<int> length(n, maxInt);
        length[start] = 0;
        std::vector<bool> visited(n, false);

        for (int j = 0; j < n; j++) {
            int u = -1;
            for (int k = 0; k < n; k++) {
                if (!visited[k] && (u == -1 || length[k] < length[u])) {
                    u = k;
                }
            }

            if (u == -1 || length[u] == maxInt) break;

            visited[u] = true;

            for (const Graph& Graph : graph[u]) {
                int v = Graph.to;
                int weight = Graph.weight;

                if (length[v] > length[u] + weight) {
                    length[v] = length[u] + weight;
                }
            }
        }

        for (const Graph& Graph : graph[i]) {
            if (Graph.to == start) {
                if (length[i] + Graph.weight < minCycle) {
                    minCycle = length[i] + Graph.weight;
                }

            }
        }
    }

    if (minCycle == maxInt) {
        std::cout << "Цикл не найден из города " << start + 1 << "\n";
    }
    else {
        std::cout << "Длина минимального цикла начинается от города " << start + 1 << " это " << minCycle << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    std::ifstream file("graph.txt");
    if (!file) {
        std::cerr << "Ошибка при открытии файла\n";
    }

    int n, m;
    file >> n >> m;

    std::cout << "Количество городов: " << n << "\n";
    std::cout << "Количество дорог: " << m << "\n";

    std::vector<std::vector<Graph>> graph(n);

    for (int i = 0; i < m; ++i) {
        int u, v, w;
        file >> u >> v >> w;

        graph[u - 1].push_back({ v - 1, w });

        std::cout << u << " -> " << v << " вес: " << w << "\n";
    }

    int startCity;
    std::cout << "Введите начальный город (от 1 до " << n << "): ";
    std::cin >> startCity;

    if (startCity < 1 || startCity > n) {
        std::cerr << "Ошибка: начальный город вне диапазона!\n";
    }
    else {
        mainCycle(startCity - 1, graph);
    }

    return 0;
}
