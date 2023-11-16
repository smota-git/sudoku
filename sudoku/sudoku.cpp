// sudoku-old.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int imax = 8;

int main_it = 0;

vector<vector<vector<int>>> previous_acceptable_values;
vector<vector<vector<int>>> previous_index_order;
int previous_temp[100][imax + 1][imax + 1];
int ptemp_size = 0;

bool test1(int& qa, int& ia, int& ja, int ar[imax + 1][imax + 1]) {
    for (int m = 0; m < imax + 1; m++) {
        if ((m == ja) || (ar[ia][m] == 0)) continue;
        if (qa == ar[ia][m]) return false;
    }
    return true;
}

bool test2(int& qa, int& ia, int& ja, int ar[imax + 1][imax + 1]) {
    for (int m = 0; m < imax + 1; m++) {
        if ((m == ia) || (ar[m][ja] == 0)) continue;
        if (qa == ar[m][ja]) return false;
    }
    return true;
}

bool test3(int& qa, int& ia, int& ja, int ar[imax + 1][imax + 1]) {
    int a = int(ia / 3);
    int b = int(ja / 3);
    for (int m = 0; m < 3; m++) {
        for (int n = 0; n < 3; n++) {
            if (((3 * a + m == ia) && (3 * b + n == ja)) || (ar[3 * a + m][3 * b + n] == 0)) continue;
            if (qa == ar[3 * a + m][3 * b + n]) return false;
        }
    }
    return true;
}

void manipulate_vector(vector<vector<int>>& double_vector, int i, int j) {
    vector<int> double_vector_i = double_vector[i];
    vector<int> double_vector_j = double_vector[j];

    double_vector[i] = double_vector_j;
    double_vector[j] = double_vector_i;
}

void bubblesort(vector<vector<int>>& VectorOfVectors1, vector<vector<int>>& VectorOfVectors2) {
    bool test = true;
    int i = 0;
    int length1 = size(VectorOfVectors1);
    while ((i < length1) && test) {
        test = false;
        for (int j = 0; j < length1 - i - 1; j++) {
            if (size(VectorOfVectors1[j]) > size(VectorOfVectors1[j + 1])) {
                test = true;
                manipulate_vector(VectorOfVectors1, j, j + 1);
                manipulate_vector(VectorOfVectors2, j, j + 1);
            }
        }
        i++;
    }
}

void adjust_acceptable_values(vector<vector<int>>& VectorOfVectors1, vector<vector<int>>& VectorOfVectors2, int q, int i, int j) {
    vector<vector<int>> PomVector1 = {};
    vector<vector<int>> PomVector2 = {};

    int k = 0;

    while (k < size(VectorOfVectors1)) {
        vector<int> TempVector = {};

        for (int l = 1; l < size(VectorOfVectors1[k]); l++)
            TempVector.push_back(VectorOfVectors1[k][l]);

        if (find(TempVector.begin(), TempVector.end(), q) != TempVector.end()) {
            bool condition1 = VectorOfVectors2[k][1] - 1 == i;
            bool condition2 = VectorOfVectors2[k][2] - 1 == j;
            bool condition3 = (((VectorOfVectors2[k][1] - 1) / 3 == i / 3) && ((VectorOfVectors2[k][2] - 1) / 3 == j / 3));

            if (condition1 || condition2 || condition3) {
                bool just_filled = condition1 && condition2;

                vector<int>::iterator it = TempVector.begin();
                bool erased = false;
                while (!erased) {
                    if (*it == q) {
                        it = TempVector.erase(it);
                        erased = true;
                    }
                    else it++;
                }

                if (size(TempVector) > 0 && (!just_filled)) {
                    vector<int> rg = { VectorOfVectors1[k][0] };
                    TempVector.insert(next(TempVector.begin(), 0), rg.cbegin(), rg.cend());
                    PomVector1.push_back({ TempVector });
                    PomVector2.push_back(VectorOfVectors2[k]);
                }
            }
            else {
                vector<int> rg = { VectorOfVectors1[k][0] };
                TempVector.insert(next(TempVector.begin(), 0), rg.cbegin(), rg.cend());
                PomVector1.push_back({ TempVector });
                PomVector2.push_back(VectorOfVectors2[k]);
            }
        }
        else {
            vector<int> rg = { VectorOfVectors1[k][0] };
            TempVector.insert(next(TempVector.begin(), 0), rg.cbegin(), rg.cend());
            PomVector1.push_back({ TempVector });
            PomVector2.push_back(VectorOfVectors2[k]);
        }

        k++;
    }

    VectorOfVectors1 = PomVector1;
    VectorOfVectors2 = PomVector2;

    bubblesort(VectorOfVectors1, VectorOfVectors2);
}

void main_iteration(vector<vector<int>>& VectorOfVectors1, vector<vector<int>>& VectorOfVectors2, int ar[imax + 1][imax + 1]) {
    int newI = VectorOfVectors2[0][1] - 1;
    int newJ = VectorOfVectors2[0][2] - 1;

    int newQ = VectorOfVectors1[0][1];

    int empty_values_count = size(VectorOfVectors1);
    vector<vector<int>> new_acceptable_values = VectorOfVectors1;
    vector<vector<int>> new_index_order = VectorOfVectors2;

    adjust_acceptable_values(new_acceptable_values, new_index_order, newQ, newI, newJ);

    bool contradiction = false;

    while ((size(new_acceptable_values) < empty_values_count - 1) && (!contradiction)) {

        cout << "VALUE CHANGE 1" << endl;

        vector<int>::iterator it = VectorOfVectors1[0].begin();
        bool erased = false;
        while (!erased) {
            if (*it == newQ) {
                VectorOfVectors1[0].erase(it);
                erased = true;
            }
            else it++;
        }

        if (size(VectorOfVectors1[0]) > 1) {
            newQ = VectorOfVectors1[0][1];

            new_acceptable_values = VectorOfVectors1;
            new_index_order = VectorOfVectors2;

            adjust_acceptable_values(new_acceptable_values, new_index_order, newQ, newI, newJ);
        }
        else {

            cout << "VALUE CHANGE 2" << endl;

            ptemp_size--;

            new_acceptable_values = previous_acceptable_values[ptemp_size];
            new_index_order = previous_index_order[ptemp_size];

            previous_acceptable_values.pop_back();
            previous_index_order.pop_back();

            for (int i = 0; i < imax + 1; i++) {
                for (int j = 0; j < imax + 1; j++) {
                    ar[i][j] = previous_temp[ptemp_size][i][j];
                    previous_temp[ptemp_size][i][j] = 0;
                }
            }

            contradiction = true;
        }
    }

    if (!contradiction) {
        if (size(VectorOfVectors1[0]) > 2) {
            vector<vector<int>> previous_vector = VectorOfVectors1;
            vector<int>::iterator it = previous_vector[0].begin() + 1;
            previous_vector[0].erase(it);
            previous_acceptable_values.push_back(previous_vector);
            previous_index_order.push_back(VectorOfVectors2);

            for (int i = 0; i < imax + 1; i++) {
                for (int j = 0; j < imax + 1; j++) {
                    previous_temp[ptemp_size][i][j] = ar[i][j];
                }
            }
            ptemp_size++;
        }

        ar[newI][newJ] = newQ;
    }

    VectorOfVectors1 = new_acceptable_values;
    VectorOfVectors2 = new_index_order;
}

int main() {

    locale loc("czech");
    locale::global(loc);

    auto start = high_resolution_clock::now();

    //int init[imax + 1][imax + 1] = {};

    /*
        print("Zadej pocatecni hodnoty:\n\n")

        for i in range(imax + 1) :
            init[i, 0] = input("  M(" + str(i + 1) + ",1) = ", end = '  ')
            init[i, 1] = input("  M(" + str(i + 1) + ",2) = ", end = '  ')
            init[i, 2] = input("  M(" + str(i + 1) + ",3) = ", end = '  ')
            init[i, 3] = input("  M(" + str(i + 1) + ",4) = ", end = '  ')
            init[i, 4] = input("  M(" + str(i + 1) + ",5) = ", end = '  ')
            init[i, 5] = input("  M(" + str(i + 1) + ",6) = ", end = '  ')
            init[i, 6] = input("  M(" + str(i + 1) + ",7) = ", end = '  ')
            init[i, 7] = input("  M(" + str(i + 1) + ",8) = ", end = '  ')
            init[i, 8] = input("  M(" + str(i + 1) + ",9) = ")
            print("\n\n")

        print("\n\n")
    */

    //int init[imax + 1][imax + 1] = { {3, 0, 0, 8, 0, 9, 0, 0, 7}, {0, 0, 1, 4, 0, 3, 6, 0, 0}, {0, 5, 0, 1, 2, 6, 0, 3, 0}, {6, 9, 5, 0, 0, 0, 8, 2, 1}, {0, 0, 7, 0, 0, 0, 9, 0, 0}, {2, 1, 3, 0, 0, 0, 7, 5, 4}, {0, 4, 0, 2, 3, 5, 0, 7, 0}, {0, 0, 6, 7, 0, 4, 5, 0, 0}, {5, 0, 0, 6, 0, 1, 0, 0, 9} };
    //int init[imax + 1][imax + 1] = { {0, 0, 3, 0, 0, 0, 2, 0, 0}, { 0, 0, 0, 4, 0, 2, 0, 0, 0}, { 2, 0, 0, 3, 5, 9, 0, 0, 6}, { 0, 3, 7, 0, 0, 0, 4, 2, 0}, { 0, 0, 2, 0, 7, 0, 6, 0, 0}, { 0, 1, 8, 0, 0, 0, 3, 5, 0}, { 3, 0, 0, 9, 4, 6, 0, 0, 5}, {0, 0, 0, 7, 0, 8, 0, 0, 0}, {0, 0, 9, 0, 0, 0, 1, 0, 0} };
    //int init[imax + 1][imax + 1] = {{1, 0, 0, 6, 0, 4, 0, 0, 2}, {0, 0, 0, 0, 5, 0, 0, 0, 0}, {0, 0, 9, 3, 0, 7, 1, 0, 0}, {7, 0, 1, 0, 0, 0, 2, 0, 6}, {0, 5, 0, 0, 1, 0, 0, 8, 0}, {8, 0, 2, 0, 0, 0, 4, 0, 3}, {0, 0, 6, 5, 0, 8, 7, 0, 0}, {0, 0, 0, 0, 6, 0, 0, 0, 0}, {3, 0, 0, 4, 0, 1, 0, 0, 8} };

    //int init[imax + 1][imax + 1] = { {0, 0, 0, 8, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 4, 3, 0}, {5, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 7, 0, 8, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 2, 0, 0, 3, 0, 0, 0, 0}, {6, 0, 0, 0, 0, 0, 0, 7, 5}, {0, 0, 3, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 2, 0, 0, 6, 0, 0} };

    //int init[imax + 1][imax + 1] = { {2, 0, 0, 8, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 4, 3, 0}, {5, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 7, 0, 8, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 2, 0, 0, 3, 0, 0, 0, 0}, {6, 0, 0, 0, 0, 0, 0, 7, 5}, {0, 0, 3, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 2, 0, 0, 6, 0, 0} };

    int init[imax + 1][imax + 1] = { {0, 0, 0, 4, 0, 0, 0, 7, 1},
        { 0, 8, 0, 0, 3, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 5, 0, 0, 1, 0, 4, 0, 0, 0 },
        { 0, 0, 0, 6, 0, 0, 8, 0, 0 },
        { 0, 9, 0, 0, 0, 0, 0, 3, 0 },
        { 0, 0, 0, 0, 2, 0, 9, 0, 0 },
        { 7, 0, 4, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0 }
    }; // řeší 86 sekund pomocí 32575 iterací 

    int temp[imax + 1][imax + 1];
    memcpy(temp, init, (imax + 1) * (imax + 1) * sizeof(int));

    vector<vector<int>> index_order = {};
    vector<vector<int>> acceptable_values = {};

    int i = -1;
    int j = 8;

    for (int index = 0; index < (imax + 1) * (imax + 1); index++) {
        vector<int> acceptable_values_array = {};
        acceptable_values_array.push_back({ index + 1 });
        j++;
        if (j == imax + 1) {
            j = 0;
            i++;
        }
        if (temp[i][j] == 0) {
            int value_position = 0;
            for (int value_index = 0; value_index < imax + 1; value_index++) {
                int q = value_index + 1;
                if (test1(q, i, j, temp) && test2(q, i, j, temp) && test3(q, i, j, temp)) {
                    value_position++;
                    acceptable_values_array.push_back({ q });
                }
            }

            acceptable_values.push_back(acceptable_values_array);
            index_order.push_back({ { index + 1, i + 1, j + 1 } });
        }
    }

    bubblesort(acceptable_values, index_order);

    while (size(acceptable_values) > 0) {
        main_it++;
        cout << main_it << endl;
        main_iteration(acceptable_values, index_order, temp);
    }

    cout << "Počáteční stav: " << endl;

    for (int i = 0; i < imax + 1; i++) {
        cout << init[i][0] << " ";
        cout << init[i][1] << " ";
        cout << init[i][2] << " ";
        cout << init[i][3] << " ";
        cout << init[i][4] << " ";
        cout << init[i][5] << " ";
        cout << init[i][6] << " ";
        cout << init[i][7] << " ";
        cout << init[i][8] << endl;
    }

    cout << endl;

    cout << "Koncový stav: " << endl;

    for (int i = 0; i < imax + 1; i++) {
        cout << temp[i][0] << " ";
        cout << temp[i][1] << " ";
        cout << temp[i][2] << " ";
        cout << temp[i][3] << " ";
        cout << temp[i][4] << " ";
        cout << temp[i][5] << " ";
        cout << temp[i][6] << " ";
        cout << temp[i][7] << " ";
        cout << temp[i][8] << endl;
    }

    cout << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Celkový potřebný čas: " << duration.count() / 1000000.0f << " sekund" << endl;
}