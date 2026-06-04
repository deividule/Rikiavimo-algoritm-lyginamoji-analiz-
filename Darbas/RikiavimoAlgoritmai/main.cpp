#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <iomanip>

using namespace std;

vector<int> generateRandomData(int n, int minVal = 0, int maxVal = 1000000) {
    vector<int> a(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(minVal, maxVal);
    for (int i = 0; i < n; ++i) a[i] = dist(gen);
    return a;
}

vector<int> generateSortedData(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = i;
    return a;
}

vector<int> generateReverseSortedData(int n) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = n - i;
    return a;
}

void selectionSort(vector<int>& a, long long& comparisons, long long& swaps) {
    int n = (int)a.size();
    comparisons = 0;
    swaps = 0;
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            ++comparisons;
            if (a[j] < a[minIndex]) minIndex = j;
        }
        if (minIndex != i) {
            swap(a[i], a[minIndex]);
            ++swaps;
        }
    }
}

void mergeArrays(vector<int>& a, int left, int mid, int right, long long& comparisons) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = a[left + i];
    for (int j = 0; j < n2; ++j) R[j] = a[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        ++comparisons;
        if (L[i] <= R[j]) a[k++] = L[i++];
        else a[k++] = R[j++];
    }
    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];
}

void mergeSortRec(vector<int>& a, int left, int right, long long& comparisons) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRec(a, left, mid, comparisons);
    mergeSortRec(a, mid + 1, right, comparisons);
    mergeArrays(a, left, mid, right, comparisons);
}

void mergeSort(vector<int>& a, long long& comparisons) {
    comparisons = 0;
    if (!a.empty()) mergeSortRec(a, 0, (int)a.size() - 1, comparisons);
}

struct Result {
    string algorithm;
    int size;
    string dataType;
    double avgTimeMicro;
    double avgComparisons;
    double avgSwaps;
};

string dataTypeToString(int type) {
    if (type == 0) return "Nesurikiuoti";
    if (type == 1) return "Atvirkščiai surikiuoti";
    return "Jau surikiuoti";
}


int main() {
    cout << "Programa startavo!" << endl;
    system("chcp 65001 > nul");


    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> sizes = {100, 500, 1000};
    const int runs = 2;

    vector<Result> results;

    cout << "Vykdomi skaičiavimai..." << endl;

    for (int n : sizes) {
        for (int dataType = 0; dataType < 3; ++dataType) {
            cout << "Skaičiuoju n = " << n << ", tipas = " << dataTypeToString(dataType) << "..." << endl;

            vector<int> baseData;
            if (dataType == 0) baseData = generateRandomData(n);
            else if (dataType == 1) baseData = generateReverseSortedData(n);
            else baseData = generateSortedData(n);


            {
                long long totalTime = 0, totalComparisons = 0, totalSwaps = 0;
                for (int r = 0; r < runs; ++r) {
                    vector<int> a = baseData;
                    long long comparisons = 0, swaps = 0;
                    auto start = chrono::high_resolution_clock::now();
                    selectionSort(a, comparisons, swaps);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                    totalTime += duration;
                    totalComparisons += comparisons;
                    totalSwaps += swaps;
                }
                Result res = {"Selection sort", n, dataTypeToString(dataType),
                              (double)totalTime / runs, (double)totalComparisons / runs, (double)totalSwaps / runs};
                results.push_back(res);
            }

            {
                long long totalTime = 0, totalComparisons = 0;
                for (int r = 0; r < runs; ++r) {
                    vector<int> a = baseData;
                    long long comparisons = 0;
                    auto start = chrono::high_resolution_clock::now();
                    mergeSort(a, comparisons);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                    totalTime += duration;
                    totalComparisons += comparisons;
                }
                Result res = {"Merge sort", n, dataTypeToString(dataType),
                              (double)totalTime / runs, (double)totalComparisons / runs, 0.0};
                results.push_back(res);
            }
        }
    }

    cout << "\nSkaičiavimai baigti! Rezultatai:\n" << endl;

    cout << left << setw(15) << "Algoritmas"
         << setw(10) << "Dydis"
         << setw(25) << "Duomenų tipas"
         << setw(20) << "Vid. laikas (µs)"
         << setw(20) << "Vid. palyginimų"
         << setw(20) << "Vid. sukeitimų" << "\n";

    cout << string(110, '-') << "\n";

    for (const auto& r : results) {
        cout << left << setw(15) << r.algorithm
             << setw(10) << r.size
             << setw(25) << r.dataType
             << setw(20) << fixed << setprecision(2) << r.avgTimeMicro
             << setw(20) << fixed << setprecision(2) << r.avgComparisons
             << setw(20) << fixed << setprecision(2) << r.avgSwaps << "\n";
    }

    cout << "\nPrograma baigė darbą sėkmingai!" << endl;
    system("pause");
    return 0;
}
