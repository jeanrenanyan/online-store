
#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

vector<vector<string>>get_answer_list();

vector<int> assign_to_clusters(vector<vector<double>>&, vector<vector<double>>&);

vector<vector<double>> update_clusters(vector<vector<double>>&, vector<int>&, int);

vector<int> ind_in_poll_resp(string);

void writeinfile(string, vector<vector<double>>);

vector<vector<double>> norm_resp(vector<vector<string>>);

vector<double>create_cluster();

void denormalize(vector<vector<double>>&);

void kohonen_network(vector<vector<double>>&, vector<vector<double>>&);

double euclidean_distance(vector<double>, vector<double>);



