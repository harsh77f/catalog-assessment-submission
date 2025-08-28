#include <bits/stdc++.h>
#include "json.hpp"   // JSON library: https://github.com/nlohmann/json
using namespace std;
using json = nlohmann::json;

// Function to convert a number in base `b` to decimal
long long decodeBase(const string &val, int base) {
    long long num = 0;
    for (char c : val) {
        int digit = (c >= '0' && c <= '9') ? c - '0' : (c - 'A' + 10);
        num = num * base + digit;
    }
    return num;
}

// Function to compute secret using Lagrange interpolation
double lagrangeInterpolation(vector<pair<int,long long>> &points, int k) {
    double secret = 0.0;

    for (int i = 0; i < k; i++) {
        double term = points[i].second; // y_i
        int x_i = points[i].first;

        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            int x_j = points[j].first;
            term *= (0.0 - x_j) / (double)(x_i - x_j);  // Evaluate at x=0
        }
        secret += term;
    }
    return secret;
}

int main() {
    // Read JSON from file
    ifstream inFile("input.json");
    if (!inFile) {
        cerr << "Error opening input.json" << endl;
        return 1;
    }

    json j;
    inFile >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<int,long long>> points;

    for (auto& [key, value] : j.items()) {
        if (key == "keys") continue;
        int x = stoi(key);
        int base = stoi(value["base"].get<string>());
        string y_str = value["value"];
        long long y = decodeBase(y_str, base);
        points.push_back({x, y});
    }

    // Sort points by x just in case
    sort(points.begin(), points.end());

    // Use only first k points for interpolation
    vector<pair<int,long long>> subset(points.begin(), points.begin() + k);

    double secret = lagrangeInterpolation(subset, k);

    cout << "Secret (c) = " << round(secret) << endl;

    return 0;
}
