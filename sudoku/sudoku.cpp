// sudoku.cpp : This file includes function main. Execution of program is starting and terminating here.
//

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// definition of new type: needed for record of acceptable digits for each iteration
typedef vector<vector<int>> doublevector;

// determination of size of table (not necessarily only 9x9, but also 4x4, 16x16, 25x25 etc. - always second power of any number)
const int sqrt_of_max = 3;
const int maximal_value = sqrt_of_max * sqrt_of_max;

/// <summary>
/// function for entry of initial conditions (assuming that they are not entered to code directly)
/// </summary>
/// <param name="initial_layout">initial layout of digits</param>
void insert_initial_values(int initial_layout[maximal_value][maximal_value]) {

    cout << "Enter initial values:" << endl << endl;

    for (int i = 0; i < maximal_value; i++) {
        int j;
        cout << "  i = " << i << ": " << endl << endl;
        cout << "    Will you enter for concrete values of \"j\" (yes/no)? ";
        string feedback;
        while ((feedback != "y") && (feedback != "n") && (feedback != "yes") && (feedback != "no")) {
            cin >> feedback;
        }
        if ((feedback == "y") || (feedback == "yes")) {
            cout << "    \"j\" should be in interval <0,8>, choice of another value causes cancellation of this line (corresponding to given \"i\")" << endl << endl;
            while (true) {
                cout << "    j = ";
                cin >> j;
                if ((j < 0) || (j > maximal_value - 1)) break;
                cout << "    M(" << i << ", " << j << ") = ";
                while (true) {
                    cin >> initial_layout[i][j];
                    if (abs(initial_layout[i][j] - 5) <= 4) break;
                }
                cout << endl;
            }
            cout << endl;
        }
        else cout << endl;
    }

    cout << endl;
}

/// <summary>
/// function for searching of order number of position with given coordinates of position in corresponding composite vector
/// </summary>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="vector2D_position_indices">vector of coordinates of positions in ordered vector of acceptable digits</param>
/// <returns>order number of position in current ordering</returns>
int search_position_index(int i, int j, doublevector& vector2D_position_indices) {
    int k = 0;
    while ((vector2D_position_indices[k][0] != i) || (vector2D_position_indices[k][1] != j)) k++;

    return k;
}

/// <summary>
/// test if regarding the digits ordering in given LINE the given position can include a concrete digit 
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="tested_layout">investigated layout of digits</param>
/// <returns>true or false</returns>
bool can_be_in_line(int q, int i, int j, int tested_layout[maximal_value][maximal_value]) {
    for (int m = 0; m < maximal_value; m++) {
        if ((m == j) || (tested_layout[i][m] == 0)) continue;
        if (q == tested_layout[i][m]) return false;
    }
    return true;
}

/// <summary>
/// test if regarding the digits ordering in given COLUMN the given position can include a concrete digit 
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="tested_layout">investigated layout of digits</param>
/// <returns>true or false</returns>
bool can_be_in_column(int q, int i, int j, int tested_layout[maximal_value][maximal_value]) {
    for (int m = 0; m < maximal_value; m++) {
        if ((m == i) || (tested_layout[m][j] == 0)) continue;
        if (q == tested_layout[m][j]) return false;
    }
    return true;
}

/// <summary>
/// test if regarding the digits ordering in given CELL the given position can include a concrete digit 
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="tested_layout">investigated layout of digits</param>
/// <returns>true or false</returns>
bool can_be_in_cell(int q, int i, int j, int tested_layout[maximal_value][maximal_value]) {
    int line_quotient = i / sqrt_of_max * sqrt_of_max;
    int column_quotient = j / sqrt_of_max * sqrt_of_max;
    for (int m = 0; m < sqrt_of_max; m++) {
        for (int n = 0; n < sqrt_of_max; n++) {
            if (((line_quotient + m == i) && (column_quotient + n == j)) || (tested_layout[line_quotient + m][column_quotient + n] == 0)) continue;
            if (q == tested_layout[line_quotient + m][column_quotient + n]) return false;
        }
    }
    return true;
}

/// <summary>
/// counting of number of positions in given LINE which can include a concrete digit
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="tested_layout">investigated layout of digits</param>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
/// <returns>number of positions with given property</returns>
int count_in_line(int q, int i, int j, int tested_layout[maximal_value][maximal_value], doublevector& vector2D_possibilities, doublevector& vector2D_position_indices) {
    int count = 0;

    for (int m = 0; m < maximal_value; m++) {
        if (tested_layout[i][m] != 0) continue;
        int position_index = search_position_index(i, m, vector2D_position_indices);
        for (int n = 0; n < size(vector2D_possibilities[position_index]); n++) {
            if (vector2D_possibilities[position_index][n] == q) count++;
        }
    }
    return count;
}

/// <summary>
/// counting of number of positions in given COLUMN which can include a concrete digit
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="tested_layout">investigated layout of digits</param>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
/// <returns>number of positions with given property</returns>
int count_in_column(int q, int i, int j, int tested_layout[maximal_value][maximal_value], doublevector& vector2D_possibilities, doublevector& vector2D_position_indices) {
    int count = 0;

    for (int m = 0; m < maximal_value; m++) {
        if (tested_layout[m][j] != 0) continue;
        int position_index = search_position_index(m, j, vector2D_position_indices);
        for (int n = 0; n < size(vector2D_possibilities[position_index]); n++) {
            if (vector2D_possibilities[position_index][n] == q) count++;
        }
    }
    return count;
}

/// <summary>
/// counting of number of positions in given CELL which can include a concrete digit
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of line (from 0)</param>
/// <param name="j">coordinates of column (from 0)</param>
/// <param name="tested_layout">investigated layout of digits</param>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
/// <returns>number of positions with given property</returns>
int count_in_cell(int q, int i, int j, int tested_layout[maximal_value][maximal_value], doublevector& vector2D_possibilities, doublevector& vector2D_position_indices) {
    int count = 0;

    int line_quotient = i / sqrt_of_max * sqrt_of_max;
    int column_quotient = j / sqrt_of_max * sqrt_of_max;

    for (int m = 0; m < maximal_value; m++) {
        int needed_i = line_quotient + (m / sqrt_of_max);
        int needed_j = column_quotient + m % sqrt_of_max;

        if (tested_layout[needed_i][needed_j] != 0) continue;

        int position_index = search_position_index(needed_i, needed_j, vector2D_position_indices);
        for (int n = 0; n < size(vector2D_possibilities[position_index]); n++) {
            if (vector2D_possibilities[position_index][n] == q) count++;
        }
    }
    return count;
}


/// <summary>
/// interchange of components of 2D-vectors within bubblesort algorithm
/// </summary>
/// <param name="a">order number of first interchanging position</param>
/// <param name="b">order number of second interchanging position</param>
/// <param name="double_vector">sorting 2D-vector</param>
void interchange_vectors(int a, int b, doublevector& double_vector) {
    vector<int> double_vector_a = double_vector[a];
    vector<int> double_vector_b = double_vector[b];

    double_vector[a] = double_vector_b;

    double_vector[b] = double_vector_a;
}

/// <summary>
/// algorithm bubblesort - it orders components (created by vectors) of given 2D-vectors according to the size (number of digits included in particular components) 
/// </summary>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
void bubblesort(doublevector& vector2D_possibilities, doublevector& vector2D_position_indices) {
    bool unsorted = true;
    int k = 0;
    int length_of_vector = (int)size(vector2D_possibilities);
    while ((k < length_of_vector) && unsorted) {
        unsorted = false;
        for (int l = 0; l < length_of_vector - k - 1; l++) {
            if (size(vector2D_possibilities[l]) > size(vector2D_possibilities[l + 1])) {
                unsorted = true;
                interchange_vectors(l, l + 1, vector2D_possibilities);
                interchange_vectors(l, l + 1, vector2D_position_indices);
            }
        }
        k++;
    }
}

/// <summary>
/// during choice of digit for given position the corresponding number is erased here from list of acceptable digits for all other 
/// positions located in the same line, column and cell; after that the components of corresponding 2D-vectors whose size is eliminated
/// are excluded
/// </summary>
/// <param name="q">investigated digit</param>
/// <param name="i">coordinates of the line (from 0)</param>
/// <param name="j">coordinates of the column (from 0)</param>
/// <param name="tested_layout">investigated layout of numbersc</param>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
void adjust_acceptable_values(int q, int i, int j, doublevector& vector2D_possibilities, doublevector& vector2D_position_indices) {
    doublevector site_possibilities = {};
    doublevector site_indices = {};

    int k = 0;

    while (k < size(vector2D_possibilities)) {

        bool site_contribution = true;

        // is the number q located in list of acceptable digits for iterated position?
        if (find(vector2D_possibilities[k].begin(), vector2D_possibilities[k].end(), q) != vector2D_possibilities[k].end()) {

            // if yes we verify if the iterated position shares with specified position line, column or cell
            bool possible_q_in_same_line = vector2D_position_indices[k][0] == i;
            bool possible_q_in_same_column = vector2D_position_indices[k][1] == j;
            bool possible_q_in_same_cell = ((vector2D_position_indices[k][0] / sqrt_of_max == i / sqrt_of_max) && (vector2D_position_indices[k][1] / sqrt_of_max == j / sqrt_of_max));

            if (possible_q_in_same_line || possible_q_in_same_column || possible_q_in_same_cell) {
                // limiting possibility: iterated and specified position are the same
                bool just_filled = possible_q_in_same_line && possible_q_in_same_column;

                // deletion of digit q from the list of acceptable possibilities (due to cycle, it is performed for each iterated
                // position satisfying at least one condition)
                vector<int>::iterator it = vector2D_possibilities[k].begin();
                bool erased = false;
                while (!erased) {
                    if (*it == q) {
                        it = vector2D_possibilities[k].erase(it);
                        erased = true;
                    }
                    else it++;
                }

                if ((size(vector2D_possibilities[k]) == 0) || just_filled) site_contribution = false;
            }
        }

        // if the adjusted list of possibilities for iterated position (from initial state differing by deletion of digit q) satisfies
        // supplementary requirements (deletion of q must not eliminate all possibilities or iterated and specified position are not
        // identical, i.e. site_contribution = false), we can add it to newly constructed 2D-vector of possibilities for particular
        // positions
        if (site_contribution) {
            site_possibilities.push_back({ vector2D_possibilities[k] });
            site_indices.push_back(vector2D_position_indices[k]);
        }

        k++;
    }

    // finally, we equate the newly constructed 2D-vectors with the initial
    vector2D_possibilities = site_possibilities;
    vector2D_position_indices = site_indices;
}

/// <summary>
/// next improvement: for the purpose of simplification on the base of found acceptable digits we search possible positions which
/// are the only admitting location of some digits within given line, column or cell (so-called hidden single)
/// </summary>
/// <param name="layout">investigated number layout</param>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
void find_hidden_singles(int layout[maximal_value][maximal_value], doublevector& vector2D_possibilities,
    doublevector& vector2D_position_indices) {

    bool new_values = true;
    while (new_values) {
        new_values = false;
        for (int q = 1; q < maximal_value + 1; q++) {
            for (int i = 0; i < maximal_value; i++) {
                for (int j = 0; j < maximal_value; j++) {

                    if (layout[i][j] != 0) continue;

                    int position_index = search_position_index(i, j, vector2D_position_indices);
                    if (find(vector2D_possibilities[position_index].begin(), vector2D_possibilities[position_index].end(), q) ==
                        vector2D_possibilities[position_index].end()) continue;

                    bool one_in_line = count_in_line(q, i, j, layout, vector2D_possibilities, vector2D_position_indices) == 1;
                    bool one_in_column = count_in_column(q, i, j, layout, vector2D_possibilities, vector2D_position_indices) == 1;
                    bool one_in_cell = count_in_cell(q, i, j, layout, vector2D_possibilities, vector2D_position_indices) == 1;

                    if ((one_in_line || one_in_column || one_in_cell) && (size(vector2D_possibilities[position_index]) > 1)) {
                        vector2D_possibilities[position_index] = { q };
                        new_values = true;
                    }
                }
            }
        }
    }
}

/// <summary>
/// main iteration: for improvement of calculation, initially, the 2D-vectors of acceptable digits are ordered by size; next, the
/// function chooses the digit for filling the actual position and controlls if for given choice some components corresponding to 
/// not yet filled positions from the vector of acceptable digits are not erased (which means that for actual choice of occupation
///  of unfilled positions the task has no solution); if yes, we choose another digit (which undergoes the same controll mechanism),
/// if no, we first find possible hidden singles and if this does not corrupt the favorable case, the given digit layout is together 
/// with vector of acceptable digits (reduced by currently selected digit) added to alternates (this serves for the case that given
/// choice will not finally appear suitable in any of next iterations); if, finally, all acceptable digits corresponding to given 
/// position will not appear suitable we choose from alternates the last case when the digit was for any of the previous positions
/// chosen from more possibilities and we choose the combination of parameters for next iteration together with layout of digits and
/// vector of acceptable possibilities corresponding to this previous situation
/// </summary>
/// <param name="tested_layout">investigated layout of digits</param>
/// <param name="vector2D_possibilities">vector of acceptable digits at particular positions</param>
/// <param name="vector2D_position_indices">coordinates of positions in vector of acceptable digits</param>
/// <param name="size_of_relevant_alternatives">number of positions admitting alternating combinations of parameters for the case when we achieve the contradiction</param>
/// <param name="previous_tested_layout">layouts of digits corresponding to alternates</param>
/// <param name="previous_acceptable_values">acceptable digits corresponding to alternates</param>
/// <param name="previous_indices_order">coordinates to positions corresponding to alternates</param>
void main_iteration(int tested_layout[maximal_value][maximal_value], doublevector& vector2D_possibilities, doublevector& vector2D_position_indices,
    int& size_of_relevant_alternatives, int previous_tested_layout[maximal_value * maximal_value][maximal_value][maximal_value],
    vector<doublevector>& previous_acceptable_values, vector<doublevector>& previous_indices_order) {

    // sorting the vectors of acceptable digits according to the size
    bubblesort(vector2D_possibilities, vector2D_position_indices);

    // choice of position and digit
    int newI = vector2D_position_indices[0][0];
    int newJ = vector2D_position_indices[0][1];
    int newQ = vector2D_possibilities[0][0];

    // number of yet unfilled positions
    int missing_values_count = (int)size(vector2D_possibilities);

    doublevector new_acceptable_values = vector2D_possibilities;
    doublevector new_indices_order = vector2D_position_indices;

    int new_tested_layout[maximal_value][maximal_value];
    memcpy(new_tested_layout, tested_layout, maximal_value * maximal_value * sizeof(int));
    new_tested_layout[newI][newJ] = newQ;

    // adjustment of new parameters
    adjust_acceptable_values(newQ, newI, newJ, new_acceptable_values, new_indices_order);
    if (size(new_acceptable_values) == missing_values_count - 1)
        find_hidden_singles(new_tested_layout, new_acceptable_values, new_indices_order);

    // number of yet unfilled positions was reduced by 1 and this should correspond to size of vector with list of acceptable values for
    // all yet unfilled positions; but in case that this reduction was larger then, at the same time, for another of yet unfilled positions
    // all acceptable digits disappeared and, so, our last choice should be modified
    while (size(new_acceptable_values) < missing_values_count - 1) {

        // for given position, we erase the possibility which showed to be unsuitable
        cout << "  passing 1.erasing block" << endl;

        vector<int>::iterator it = vector2D_possibilities[0].begin();
        bool erased = false;
        while (!erased) {
            if (*it == newQ) {
                vector2D_possibilities[0].erase(it);
                erased = true;
            }
            else it++;
        }

        // if for given position all possibilities for acceptable digits were not exhausted we choose the next one; otherwise, we stop
        // the cycle - it is necessary to return to closest position which admits choice of another digit
        if (size(vector2D_possibilities[0]) > 0) {
            newQ = vector2D_possibilities[0][0];

            new_acceptable_values = vector2D_possibilities;
            new_indices_order = vector2D_position_indices;

            new_tested_layout[newI][newJ] = newQ;

            adjust_acceptable_values(newQ, newI, newJ, new_acceptable_values, new_indices_order);
            if (size(new_acceptable_values) == missing_values_count - 1)
                find_hidden_singles(new_tested_layout, new_acceptable_values, new_indices_order);
        }
        else break;
    }

    if (size(vector2D_possibilities[0]) == 0) {

        // exhaustion of all possibilities for given position: we choose the closes alternate which we erase just after copying
        // of all parameters (so that we release access to possible another alternate)
        cout << "  passing 2.erasing block" << endl;

        size_of_relevant_alternatives--;

        new_acceptable_values = previous_acceptable_values[size_of_relevant_alternatives];
        new_indices_order = previous_indices_order[size_of_relevant_alternatives];

        previous_acceptable_values.pop_back();
        previous_indices_order.pop_back();

        memcpy(tested_layout, previous_tested_layout[size_of_relevant_alternatives], maximal_value * maximal_value * sizeof(int));
        memset(previous_tested_layout[size_of_relevant_alternatives], 0, maximal_value * maximal_value * sizeof(int));
    }
    else {

        // suitable configuration of parameters found: if it is not the last acceptable digit for given position, we erase from the list
        // of the corresponding acceptable digits the actual value and the created combination of parameters we save into the list of
        // alternates
        if (size(vector2D_possibilities[0]) > 1) {
            doublevector previous_vector = vector2D_possibilities;
            vector<int>::iterator it = previous_vector[0].begin();
            previous_vector[0].erase(it);
            previous_acceptable_values.push_back(previous_vector);
            previous_indices_order.push_back(vector2D_position_indices);

            memcpy(previous_tested_layout[size_of_relevant_alternatives], tested_layout, maximal_value * maximal_value * sizeof(int));

            size_of_relevant_alternatives++;
        }

        memcpy(tested_layout, new_tested_layout, maximal_value * maximal_value * sizeof(int));
    }

    // in the case of finding the correct combination, the vector of acceptable digits at particular positions was changed, so, we 
    // save into it the newly found (and adjusted) scheme 
    vector2D_possibilities = new_acceptable_values;
    vector2D_position_indices = new_indices_order;
}

/// <summary>
/// main block: from the initial values (entered manually or directly from code) it creates from boolean functions at the
/// beginning of code the 2D-vectors named acceptable_values and indices_order containing the list of digits which can occupy the
/// particular positions, this will be afterwards adjusted by searching hidden singles, this will simplify the 2D-vectors even more;
/// after sorting, we execute the main cycle which is running unless the last position is occupied (in that case all values from the
/// vector acceptable_values are erased)
/// </summary>
/// <returns></returns>
int main() {

    int initial_layout[maximal_value][maximal_value];

    string manual_entry = "0";

    cout << "Will you enter the initial values manually (yes/no)? ";

    while ((manual_entry != "y") && (manual_entry != "n") && (manual_entry != "yes") && (manual_entry != "no")) {
        cin >> manual_entry;
    }

    cout << endl;

    if ((manual_entry == "y") || (manual_entry == "yes")) {
        memset(initial_layout, 0, maximal_value * maximal_value * sizeof(int));
        insert_initial_values(initial_layout);
    }
    else {

        //41 
            //int some_layout[maximal_value][maximal_value] = { {3, 0, 0, 8, 0, 9, 0, 0, 7}, {0, 0, 1, 4, 0, 3, 6, 0, 0}, {0, 5, 0, 1, 2, 6, 0, 3, 0}, {6, 9, 5, 0, 0, 0, 8, 2, 1}, {0, 0, 7, 0, 0, 0, 9, 0, 0}, {2, 1, 3, 0, 0, 0, 7, 5, 4}, {0, 4, 0, 2, 3, 5, 0, 7, 0}, {0, 0, 6, 7, 0, 4, 5, 0, 0}, {5, 0, 0, 6, 0, 1, 0, 0, 9} };
        //27 
            //int some_layout[maximal_value][maximal_value] = {{0, 0, 3, 0, 0, 0, 2, 0, 0}, { 0, 0, 0, 4, 0, 2, 0, 0, 0}, { 2, 0, 0, 3, 5, 9, 0, 0, 6}, { 0, 3, 7, 0, 0, 0, 4, 2, 0}, { 0, 0, 2, 0, 7, 0, 6, 0, 0}, { 0, 1, 8, 0, 0, 0, 3, 5, 0}, { 3, 0, 0, 9, 4, 6, 0, 0, 5}, {0, 0, 0, 7, 0, 8, 0, 0, 0}, {0, 0, 9, 0, 0, 0, 1, 0, 0}};
            //int some_layout[maximal_value][maximal_value] = {{1, 0, 0, 6, 0, 4, 0, 0, 2}, {0, 0, 0, 0, 5, 0, 0, 0, 0}, {0, 0, 9, 3, 0, 7, 1, 0, 0}, {7, 0, 1, 0, 0, 0, 2, 0, 6}, {0, 5, 0, 0, 1, 0, 0, 8, 0}, {8, 0, 2, 0, 0, 0, 4, 0, 3}, {0, 0, 6, 5, 0, 8, 7, 0, 0}, {0, 0, 0, 0, 6, 0, 0, 0, 0}, {3, 0, 0, 4, 0, 1, 0, 0, 8} };
            //int some_layout[maximal_value][maximal_value] = {{0, 0, 0, 1, 0, 4, 0, 0, 0}, {0, 3, 0, 9, 6, 5, 0, 4, 0}, {0, 0, 8, 0, 0, 0, 6, 0, 0}, {5, 8, 0, 0, 0, 0, 0, 1, 3}, {0, 7, 0, 0, 0, 0, 0, 6, 0}, {4, 1, 0, 0, 0, 0, 0, 9, 2}, {0, 0, 7, 0, 0, 0, 4, 0, 0}, {0, 6, 0, 5, 7, 2, 0, 8, 0}, {0, 0, 0, 8, 0, 3, 0, 0, 0} };
        //17
            //int some_layout[maximal_value][maximal_value] = { {0, 0, 0, 8, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 4, 3, 0}, {5, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 7, 0, 8, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 2, 0, 0, 3, 0, 0, 0, 0}, {6, 0, 0, 0, 0, 0, 0, 7, 5}, {0, 0, 3, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 2, 0, 0, 6, 0, 0} };
        int some_layout[maximal_value][maximal_value] =
        { {0, 0, 0, 4, 0, 0, 0, 7, 1},
            {0, 8, 0, 0, 3, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {5, 0, 0, 1, 0, 4, 0, 0, 0},
            {0, 0, 0, 6, 0, 0, 8, 0, 0},
            {0, 9, 0, 0, 0, 0, 0, 3, 0},
            {0, 0, 0, 0, 2, 0, 9, 0, 0},
            {7, 0, 4, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        /*
        int some_layout[maximal_value][maximal_value] =
        {   {0, 0, 4, 0, 0, 0, 7, 0, 8},
            {6, 0, 0, 0, 0, 5, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 2, 7, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 3, 0},
            {0, 0, 0, 4, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 6, 3, 0, 5, 0},
            {0, 7, 2, 0, 0, 0, 4, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0}
        };*/


        //18
            //int some_layout[maximal_value][maximal_value] = { {3, 0, 0, 8, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 4, 3, 0}, {5, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 7, 0, 8, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 2, 0, 0, 3, 0, 0, 0, 0}, {6, 0, 0, 0, 0, 0, 0, 7, 5}, {0, 0, 3, 4, 0, 0, 0, 0, 0}, {0, 0, 0, 2, 0, 0, 6, 0, 0} };
        //20 
        /*int some_layout[maximal_value][maximal_value] =
        {   {5, 0, 0, 0, 0, 0, 0, 0, 9},
            {0, 2, 0, 1, 0, 0, 0, 7, 0},
            {0, 0, 8, 0, 0, 0, 3, 0, 0},
            {0, 4, 0, 0, 0, 2, 0, 0, 0},
            {0, 0, 0, 0, 5, 0, 0, 0, 0},
            {0, 0, 0, 7, 0, 6, 0, 1, 0},
            {0, 0, 3, 0, 0, 0, 8, 0, 0},
            {0, 6, 0, 0, 0, 4, 0, 2, 0},
            {9, 0, 0, 0, 0, 0, 0, 0, 5}
        };*/
        /*
        //23
        int some_layout[maximal_value][maximal_value] =
        { {0, 9, 0, 4, 0, 0, 0, 0, 0},
            {0, 0, 0, 6, 0, 0, 0, 5, 0},
            {2, 0, 4, 0, 0, 7, 8, 0, 0},
            {0, 8, 0, 0, 0, 9, 0, 0, 0},
            {3, 0, 9, 7, 0, 0, 0, 0, 6},
            {0, 1, 0, 0, 0, 0, 3, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 2, 0, 0, 0, 8},
            {7, 0, 3, 0, 0, 4, 2, 0, 0}
        };*/ /*
            int some_layout[maximal_value][maximal_value] =
        {   {0, 2, 0, 0, 0, 5, 0, 0, 0},
            {0, 1, 5, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 8, 7, 0, 3},
            {0, 5, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 9, 7, 0, 0, 0, 1, 0},
            {0, 0, 0, 3, 0, 0, 0, 4, 6},
            {0, 0, 0, 0, 8, 0, 0, 0, 1},
            {7, 0, 0, 9, 3, 0, 0, 6, 0},
            {0, 0, 0, 0, 0, 0, 4, 0, 8}
        }; */
        //36
/*        int some_layout[maximal_value][maximal_value] =
        {   {0, 1, 7, 8, 6, 0, 9, 0, 0},
            {0, 0, 6, 0, 0, 4, 2, 0, 7},
            {0, 5, 0, 0, 7, 0, 1, 6, 8},
            {6, 2, 0, 0, 0, 8, 0, 7, 9},
            {0, 0, 8, 0, 4, 0, 6, 2, 0},
            {5, 0, 0, 6, 0, 0, 0, 8, 0},
            {0, 6, 0, 0, 8, 0, 0, 1, 0},
            {7, 0, 2, 3, 0, 0, 8, 0, 6},
            {0, 0, 0, 0, 0, 6, 0, 9, 0}
        };
        */


        /*      //0
                    int some_layout[maximal_value][maximal_value] =
                    {   {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0}
                    };
          */
          /*      // varianta 4x4
                  int some_layout[maximal_value][maximal_value] =
                  {   {0, 0, 0, 3},
                      {0, 4, 0, 0},
                      {0, 0, 3, 2},
                      {0, 0, 0, 0}
                  };
          */

        memcpy(initial_layout, some_layout, maximal_value * maximal_value * sizeof(int));
    }

    // initial conditions entered - we switch the timer
    auto start = high_resolution_clock::now();

    int supplied_layout[maximal_value][maximal_value];
    memcpy(supplied_layout, initial_layout, maximal_value * maximal_value * sizeof(int));

    doublevector indices_order = {};
    doublevector acceptable_values = {};

    int i = -1;
    int j = maximal_value - 1;


    // we create the vector of acceptable values on the base of possible collision with digits in the same line, column or cell,
    // following from initial conditions
    for (int site_index = 0; site_index < maximal_value * maximal_value; site_index++) {
        vector<int> acceptable_values_array = {};
        j++;
        if (j == maximal_value) {
            j = 0;
            i++;
        }
        if (supplied_layout[i][j] == 0) {
            for (int value_index = 0; value_index < maximal_value; value_index++) {
                int q = value_index + 1;
                if (can_be_in_line(q, i, j, supplied_layout) && can_be_in_column(q, i, j, supplied_layout) && can_be_in_cell(q, i, j, supplied_layout)) {
                    acceptable_values_array.push_back({ q });
                }
            }

            acceptable_values.push_back(acceptable_values_array);
            indices_order.push_back({ { i, j } });
        }
    }

    // for the purpose of simplification on the base of found acceptable digits we find possible positions which are the only admitting
    // placement of some digits within given line, column or cell
    find_hidden_singles(supplied_layout, acceptable_values, indices_order);

    vector<doublevector> previous_acceptable_values;
    vector<doublevector> previous_indices_order;

    int previous_tested_layout[maximal_value * maximal_value][maximal_value][maximal_value];
    int size_of_relevant_alternatives = 0;

    cout << "Progress:  " << endl;

    // main cycle - for the controll of complexity of calculation, we calculate the number of passes.
    while (size(acceptable_values) > 0) {
        static int iteration_order = 0;
        iteration_order++;
        cout << "  " << iteration_order << ".iteration" << endl;
        main_iteration(supplied_layout, acceptable_values, indices_order, size_of_relevant_alternatives, previous_tested_layout, previous_acceptable_values,
            previous_indices_order);
    }

    cout << endl;

    cout << "Initial state: " << endl;

    for (int i = 0; i < maximal_value; i++) {
        for (int j = 0; j < maximal_value; j++) {
            cout << initial_layout[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    cout << "Final state: " << endl;

    for (int i = 0; i < maximal_value; i++) {
        for (int j = 0; j < maximal_value; j++) {
            cout << supplied_layout[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    // switching off the timer, measurement of duration of calculation
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Total processing time: " << duration.count() / 1000000.0f << " seconds" << endl;

    cin.get();

    return 0;
}