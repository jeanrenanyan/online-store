#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());

double euclidean_distance(vector<double>data1, vector<double>data2) {
    double res = 0;
    for (int i = 0;i < data1.size();i++) {
        res += pow(data1[i] - data2[i], 2);
    }
    return sqrt(res);
}

vector<vector<string>>get_answer_list() {

    vector<vector<string>>answers{};
    string line{};

    fstream file("answerlist.txt");

    if (file.is_open()) {
        vector<string>temp_answers{};
        while (getline(file, line)) {
            if (line.size() != 0) temp_answers.push_back(line);
            else { answers.push_back(temp_answers); temp_answers = {}; }
        }
    }

    file.close();

    return answers;
}

vector<int> ind_in_poll_resp(string response) {
    
    vector<vector<string>>answer_list = get_answer_list();
    vector<int>elem_and_max{};
    for (int i = 0; i < answer_list.size(); i++) {
        for (int j = 0; j < answer_list[i].size(); j++) {
            if (answer_list[i][j] == response) {
                elem_and_max.push_back(j);
                elem_and_max.push_back(answer_list[i].size() - 1);
                return elem_and_max;
            }
        }
    }
}

vector<vector<double>> norm_resp(vector<vector<string>>data) {

    vector<vector<double>>fin_norm_resp{};
    for (auto i : data) {
        vector<double>temp_norm{};
        for (string j : i) {

            if (j.find('\n') != string::npos) {
                j = j.substr(0, j.find('\n'));
            }

            vector<int> charcs_pos = ind_in_poll_resp(j);

            double norm_charch = static_cast<double>(charcs_pos[0]) / (charcs_pos[1] - 0);
            temp_norm.push_back(norm_charch);
        }

        fin_norm_resp.push_back(temp_norm);

    }

    return fin_norm_resp;
}

vector<double> create_cluster() {
    vector<double>cluster{};
    uniform_real_distribution<>distribution(0.1, 0.3);

    for (int i = 0; i < 12; i++) {
        double elem = distribution(gen);
        elem = round(elem * 100) / 100;
        cluster.push_back(elem);
    }
    return cluster;
}

vector<int>dispersion(vector<vector<tuple<int, double>>>group_cluster, vector<int>& assignment, vector<int>restr = {}) {
    if (restr.size() == assignment.size()) return assignment;
    bool flag = false;
    for (int i = 0;i < group_cluster.size();i++) {
        double disp = 0;
        int group_size = 0;
        for (int j = 0;j < group_cluster[i].size();j++) {
            auto [v, d] = group_cluster[i][j];
            if (find(restr.begin(), restr.end(), v) == end(restr)) {
                disp += d;
                group_size += 1;
            }
        }

        disp = disp * 1.0 / group_size;

        for (int j = 0;j < group_cluster[i].size();j++) {
            auto [v, d] = group_cluster[i][j];

            if (find(restr.begin(), restr.end(), v) == end(restr)) {

                if (d <= 2 * disp) {
                    assignment[v] = i;
                    restr.push_back(v);
                }
                else flag = true;
            }
            
        }
    }

    if (flag) {
        dispersion(group_cluster, assignment, restr);

    }

    else {
        return assignment;
    };
}

vector<int> assign_to_clusters(vector<vector<double>>& data, vector<vector<double>>& clusters_arr) {
    
    vector<int> assignments(data.size());
    vector<vector<tuple<int, double>>>group_claster(clusters_arr.size());

    for (int i = 0; i < data.size(); i++) {
        double min_dist = numeric_limits<double>::max();
        int closest_cluster = 0;

        for (int j = 0; j < clusters_arr.size(); ++j) {
            double dist = euclidean_distance(data[i], clusters_arr[j]);
            if (dist < min_dist) {
                min_dist = dist;
                closest_cluster = j;
            }
        }
        assignments[i] = closest_cluster;
        group_claster[closest_cluster].push_back(tuple(i, min_dist));
    }

    dispersion(group_claster, assignments);
    return assignments;
}

vector<vector<double>> update_clusters(vector<vector<double>>& data, vector<int>& assignments, vector<vector<double>>clusters_arr, int iter) {

    vector<vector<double>> new_clusters(clusters_arr.size());
    vector<int> counts(clusters_arr.size(), 0);

    for (auto& centroid : new_clusters) {
        centroid.resize(data[0].size(), 0.0);
    }

    for (int i = 0; i < data.size();i++) {
        int cluster = assignments[i];
        for (int j = 0; j < data[i].size();j++) {
            new_clusters[cluster][j] = clusters_arr[cluster][j] + (0.3 * exp(-iter / 100.0)) * (data[i][j] - clusters_arr[cluster][j]);
        }
        counts[cluster]++;
    }

    for (int i = 0; i < clusters_arr.size(); i++) {
        if (counts[i] == 0) new_clusters[i] = create_cluster();
    }
    
    return new_clusters;
}

void kohonen_network(vector<vector<double>>& data, vector<vector<double>>& clusters_arr) {
    for (int iter = 0; iter < 100; ++iter) {

        vector<int> assignments = assign_to_clusters(data, clusters_arr);

        vector<vector<double>> new_clusters = update_clusters(data, assignments, clusters_arr, iter);

        bool converged = true;
        for (int i = 0; i < clusters_arr.size(); i++) {
            if (euclidean_distance(clusters_arr[i], new_clusters[i]) > 0.001) {
                converged = false;
                break;
            }
        }

        clusters_arr = new_clusters;

        if (converged) {
            cout << "\nСходимость достигнута на итерации " << iter << endl;
            break;
        }
    }
}

void writeinfile(string destination, vector<vector<double>>data) {

    ofstream file;

    file.open(destination, ios::ate);

    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            file << data[i][j] << " ";
        }

        file << endl;
    }
}

void denormalize(vector<vector<double>>& clusters) {
    vector<vector<string>> answer_list = get_answer_list();

    cout << "\nХарактеристики кластеров:\n";
    for (int i = 0; i < clusters.size(); ++i) {
        cout << "\nКластер " << i << ":\n";
        for (int j = 0; j < clusters[i].size(); ++j) {
            double resp_ind = round(clusters[i][j] * (answer_list[j].size() - 1));
            int index = resp_ind;
            if (resp_ind > answer_list[j].size() - 1) index = answer_list[j].size() - 1;
            cout << answer_list[j][index] << endl;
        }
        cout << endl;
    }
}