
#include "koh_ntwrk.h"
#include <iostream>


int main() {

    setlocale(LC_ALL, "Russian");

    string directory_path = "C:\\dada";

    vector<vector<string>> data{};
    for (const auto& entry : filesystem::directory_iterator(directory_path)) {
        ifstream file(entry.path());
        vector<string> row{};
        string value{};
        while (getline(file, value, ';')) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    vector<vector<double>> normalized_data = norm_resp(data);


    int clusters{};
    cout << "Введите количество кластеров: ";
    cin >> clusters;

    if (clusters < 1 || clusters > normalized_data.size()) {
        cerr << "Некорректное количество кластеров" << endl;
        return 1;
    }

    vector<vector<double>>clusters_arr(clusters);
    for (auto& cluster : clusters_arr) {
        cluster = create_cluster();
    }

    writeinfile("D:/pythonProject7/clusters.txt", clusters_arr);

    kohonen_network(normalized_data, clusters_arr);

    denormalize(clusters_arr);

    writeinfile("D:/pythonProject7/changed_clusters.txt", clusters_arr);
    writeinfile("D:/pythonProject7/dataset.txt", normalized_data);

    return 0;
}