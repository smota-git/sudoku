// sudoku.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// definice nového typu: potřebný pro záznam přípustných číslic pro každou pozici
typedef vector<vector<int>> doublevector;

// určení rozměrů tabulky (nemusí být jen 9x9, ale taky 4x4, 16x16, 25x25 atd. - vždy druhá mocnina nějakého čísla)
const int sqrt_of_max = 3;
const int maximal_value = sqrt_of_max * sqrt_of_max;

/// <summary>
/// funkce pro zadání počátečních podmínek (za předpokladu, že nejsou zadány přímo v kódu) 
/// </summary>
/// <param name="initial_layout">počáteční rozložení číslic</param>
void insert_initial_values(int initial_layout[maximal_value][maximal_value]) {

    cout << "Zadej pocatecni hodnoty:" << endl << endl;

    for (int i = 0; i < maximal_value; i++) {
        int j;
        cout << "  i = " << i << ": " << endl << endl;
        cout << "    Budete zadávat pro konkrétní hodnoty \"j\" (ano/ne)? ";
        string feedback;
        while ((feedback != "a") && (feedback != "n") && (feedback != "ano") && (feedback != "ne")) {
            cin >> feedback;
        }
        cout << endl;
        if ((feedback == "a") || (feedback == "ano")) {
            while (true) {
                cout << "    j = ";
                cin >> j;
                if ((j < 0) || (j > maximal_value - 1)) break;
                cout << "    M(" << i << ", " << j << ") = ";
                cin >> initial_layout[i][j];
                cout << endl;
            }
            cout << endl;
        }
    }

    cout << endl;
}

/// <summary>
/// funkce pro vyhledání pořadového čísla pozice s danými souřadnicemi pozice v příslušném složeném vektoru
/// </summary>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="vector2D_position_indices">vektor souřadnic pozic v uspořádaném vektoru přípustných čísel</param>
/// <returns>pořadové číslo pozice v aktuálním uspořádání</returns>
int search_position_index(int i, int j, doublevector& vector2D_position_indices) {
    int k = 0;
    while ((vector2D_position_indices[k][0] != i) || (vector2D_position_indices[k][1] != j)) k++;

    return k;
}

/// <summary>
/// test, zda s ohledem na rozložení číslic v daném ŘÁDKU může daná pozice obsahovat konkrétní číslo
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <returns>true nebo false</returns>
bool can_be_in_line(int q, int i, int j, int tested_layout[maximal_value][maximal_value]) {
    for (int m = 0; m < maximal_value; m++) {
        if ((m == j) || (tested_layout[i][m] == 0)) continue;
        if (q == tested_layout[i][m]) return false;
    }
    return true;
}

/// <summary>
/// test, zda s ohledem na rozložení číslic v daném SLOUPCI může daná pozice obsahovat konkrétní číslo
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <returns>true nebo false</returns>
bool can_be_in_column(int q, int i, int j, int tested_layout[maximal_value][maximal_value]) {
    for (int m = 0; m < maximal_value; m++) {
        if ((m == i) || (tested_layout[m][j] == 0)) continue;
        if (q == tested_layout[m][j]) return false;
    }
    return true;
}

/// <summary>
/// test, zda s ohledem na rozložení číslic v dané BUŇCE může daná pozice obsahovat konkrétní číslo
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <returns>true nebo false</returns>
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
/// zjištění počtu pozic v daném ŘÁDKU, které mohou obsahovat konkrétní  číslo
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
/// <returns>počet pozic s danou vlastností</returns>
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
/// zjištění počtu pozic v daném SLOUPCI, které mohou obsahovat konkrétní  číslo
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
/// <returns>počet pozic s danou vlastností</returns>
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
/// zjištění počtu pozic v dané BUŇCE, které mohou obsahovat konkrétní  číslo
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
/// <returns>počet pozic s danou vlastností</returns>
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
/// prohození složek 2D-vektorů v rámci bubblesortu
/// </summary>
/// <param name="a">pořadové číslo první prohazované pozice</param>
/// <param name="b">pořadové číslo druhé prohazované pozice</param>
/// <param name="double_vector">uspořádávaný 2D-vektor</param>
void interchange_vectors(int a, int b, doublevector& double_vector) {
    vector<int> double_vector_a = double_vector[a];
    vector<int> double_vector_b = double_vector[b];

    double_vector[a] = double_vector_b;
    double_vector[b] = double_vector_a;
}

/// <summary>
/// algoritmus bubblesort - uspořádává složky (tvořené vektory) daných 2D-vektorů podle velikosti (počtu prvků obsažených v jednotlivých složkách) 
/// </summary>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
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
/// při výběru čísla pro vyplnění dané pozice se zde odpovídající číslo vyškrtává ze seznamu přípustných čísel pro všechny ostatní
/// pozice nacházející se ve stejném řádku, sloupci a buňce; následně jsou složky příslušných 2D-vektorů, jejichž velikost se vynuluje,
/// vyloučeny 
/// </summary>
/// <param name="q">zkoumané číslo</param>
/// <param name="i">souřadnice řádku (od O)</param>
/// <param name="j">souřadnice sloupce (od O)</param>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
void adjust_acceptable_values(int q, int i, int j, doublevector& vector2D_possibilities, doublevector& vector2D_position_indices) {
    doublevector site_possibilities = {};
    doublevector site_indices = {};

    int k = 0;

    while (k < size(vector2D_possibilities)) {

        bool site_contribution = true;

        // nachází se číslo q v seznamu přípustných číslic pro iterovanou pozici?
        if (find(vector2D_possibilities[k].begin(), vector2D_possibilities[k].end(), q) != vector2D_possibilities[k].end()) {

            // pokud ano, ověříme, zda se zadanou pozicí sdílí řádek, sloupec nebo buňku
            bool possible_q_in_same_line = vector2D_position_indices[k][0] == i;
            bool possible_q_in_same_column = vector2D_position_indices[k][1] == j;
            bool possible_q_in_same_cell = ((vector2D_position_indices[k][0] / sqrt_of_max == i / sqrt_of_max) && (vector2D_position_indices[k][1] / sqrt_of_max == j / sqrt_of_max));

            if (possible_q_in_same_line || possible_q_in_same_column || possible_q_in_same_cell) {
                //krajní možnost: iterovaná a zadaná pozice jsou totožné
                bool just_filled = possible_q_in_same_line && possible_q_in_same_column;

                // výmaz číslice q ze seznamu přípustných možností (to se díky cyklu provede pro každou iterovanou pozici splňující
                // aspoň jednu z podmínek)
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

        // pokud upravený seznam možností pro iterovanou pozici (od původní podoby se liší smazaním čísla q) splňuje dodatečné požadavky 
        // (nesmí platit, že  se smazáním q eliminují všechny možnosti nebo jsou iterovaná a zadaná pozice totožné, tj. site_contribution = false),
        // můžeme ho přidat do nově konstruovaného 2D-vektoru možností pro jednotlivé pozice
        if (site_contribution) {
            site_possibilities.push_back({ vector2D_possibilities[k] });
            site_indices.push_back(vector2D_position_indices[k]);
        }

        k++;
    }

    // nakonec nově konstruované 2D-vektory ztotožníme s původními
    vector2D_possibilities = site_possibilities;
    vector2D_position_indices = site_indices;
}

/// <summary>
/// další zefektivnění: pro účely zjednodušení na základě zjištěných přípustných číslic hledáme případné pozice, které jako jediné
/// připouštějí umístění některých číslic v rámci daného řádku, sloupce nebo buňky (tzv. skrytý singl)
/// </summary>
/// <param name="layout">zkoumané rozložení číslic</param>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
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
/// hlavní iterace: pro zefektivnění výpočtu jsou na začátku 2D-vektory přípustných číslic seřazeny podle velikosti; funkce dále 
/// vybírá číslo pro vyplnění aktuální pozice a kontroluje, jestli pro danou volbu nedojde k vyškrtnutí některých složek odpovídajících
/// dosud nevyplněným pozicím z vektoru přípustných číslic (a tedy pro stávající volbu obsazení nevyplněných pozic úloha nemá řešení);
/// pokud ano, je vybrána jiná číslice (která projde stejným kontrolním mechanizmem), pokud ne, vyhledáme nejdřív případné skryté singly,
/// a pokud se tím příznivý případ nenaruší, je dané rozložení číslic spolu s vektorem přípustných číslic (zmenšeným o číslici aktuálně
/// vybranou) zapsáno do alternativ (pro případ, že se daná volba nakonec stejně ukáže ve výsledku nevyhovující v některé z dalších
/// iterací); jestliže se všechny přípustné číslice odpovídající dané pozici nakonec ukážou být nevyhovující, je z alternativ vyvolán
/// poslední případ, kdy byla číslice pro některou z předchozích pozic vybrána z více možností a spolu s rozložením čísel a vektorem
/// přípustných možností odpovídajících této předešlé situaci je vybrána kombinace parametrů pro další iteraci
/// </summary>
/// <param name="tested_layout">zkoumané rozložení číslic</param>
/// <param name="vector2D_possibilities">vektor přípustných čísel na jednotlivých pozicích</param>
/// <param name="vector2D_position_indices">souřadnice pozic ve vektoru přípustných čísel</param>
/// <param name="size_of_relevant_alternatives">počet pozic připouštějících alternativní kombinace parametrů pro případ, že se dostaneme do sporu</param>
/// <param name="previous_tested_layout">rozložení číslic odpovídající alternativám</param>
/// <param name="previous_acceptable_values">přípustné číslice odpovídající alternativám</param>
/// <param name="previous_indices_order">souřadnice pozic odpovídajících alternativám</param>
void main_iteration(int tested_layout[maximal_value][maximal_value], doublevector& vector2D_possibilities, doublevector& vector2D_position_indices,
    int& size_of_relevant_alternatives, int previous_tested_layout[maximal_value * maximal_value][maximal_value][maximal_value],
    vector<doublevector>& previous_acceptable_values, vector<doublevector>& previous_indices_order) {

    // uspořádání vektorů přípustných číslic podle velikosti
    bubblesort(vector2D_possibilities, vector2D_position_indices);

    // výběr pozice a číslice
    int newI = vector2D_position_indices[0][0];
    int newJ = vector2D_position_indices[0][1];
    int newQ = vector2D_possibilities[0][0];

    // počet dosud nevyplněných pozic
    int missing_values_count = (int)size(vector2D_possibilities);
 
    doublevector new_acceptable_values = vector2D_possibilities;
    doublevector new_indices_order = vector2D_position_indices;

    int new_tested_layout[maximal_value][maximal_value];
    memcpy(new_tested_layout, tested_layout, maximal_value * maximal_value * sizeof(int));
    new_tested_layout[newI][newJ] = newQ;

    // úprava nových parametrů
    adjust_acceptable_values(newQ, newI, newJ, new_acceptable_values, new_indices_order);
    if (size(new_acceptable_values) == missing_values_count - 1)
        find_hidden_singles(new_tested_layout, new_acceptable_values, new_indices_order);

    // počet nevyplněných pozic se snížil o 1 a tomu musí odpovídat také velikost vektoru s výčtem přípustných pozic pro všechny nevyplněné pozice;
    // pokud se však ukáže, že tento vektor se zmenšil o víc, znamená to, že navíc vymizely všechny přípustné číslice pro některou další, dosud
    // nevyplněnou pozici a naši poslední volbu je tedy třeba modifikovat
    while (size(new_acceptable_values) < missing_values_count - 1) {

        // pro danou pozici smažeme možnost, která se ukázala být nevyhovující
        cout << "  průchod 1.mazacím blokem" << endl;

        vector<int>::iterator it = vector2D_possibilities[0].begin();
        bool erased = false;
        while (!erased) {
            if (*it == newQ) {
                vector2D_possibilities[0].erase(it);
                erased = true;
            }
            else it++;
        }

        // pokud nebyly pro danou pozici vyčerpány všechny možnosti přípustných číslic, vybereme další z nich; v opačném případě zastavíme cyklus -
        // - je potřeba se vrátit na nejbližší pozici, připouštějící výběr jiné číslice
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

        // vyčerpání všech možností pro danou pozici: vybíráme nejbližší alternativu, kterou po překopírování parametrů ihned
        // smažeme (abychom si uvolnili přístup k případné další alternativě)
        cout << "  průchod 2.mazacím blokem" << endl;

        size_of_relevant_alternatives--;

        new_acceptable_values = previous_acceptable_values[size_of_relevant_alternatives];
        new_indices_order = previous_indices_order[size_of_relevant_alternatives];

        previous_acceptable_values.pop_back();
        previous_indices_order.pop_back();

        memcpy(tested_layout, previous_tested_layout[size_of_relevant_alternatives], maximal_value * maximal_value * sizeof(int));
        memset(previous_tested_layout[size_of_relevant_alternatives], 0, maximal_value * maximal_value * sizeof(int));
    }
    else {

        // správná konfigurace parametrů nalezena: pokud se pro danou pozici nejedná o poslední přípustnou číslici, odmažeme ze seznamu pro ni 
        // přípustných číslic aktuální hodnotu a vzniklou kombinaci parametrů uložíme do seznamu alternativ
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

    // vektor přípustných čísel na jednotlivých pozicích se v případě nalezení správné kombinace změnil, tedy do něj uložíme nově nalezené 
    // (a upravené) schéma
    vector2D_possibilities = new_acceptable_values;
    vector2D_position_indices = new_indices_order;
}

/// <summary>
/// hlavní blok: ze vstupních hodnot (zadaných ručně nebo z kódu) vytvoří z booleanovských funkcí na začátku kódu 2D-vektory
/// acceptable_values a indices_order obsahující výčet čísel, které mohou obsadit jednotlivé pozice, toto je ještě následně upraveno
/// hledáním skrytých singlů, čímž se výčet ještě o něco zjednoduší; po uspořádání spustím hlavní cyklus, který běží tak dlouho,
/// dokud není obsazena poslední pozice (v takovém případě dojde k vymazání veškerých hodnot z vektoru acceptable_values)
/// </summary>
/// <returns></returns>
int main() {

    locale loc("czech");
    locale::global(loc);

    int initial_layout[maximal_value][maximal_value];

    string manual_entry = "0";

    cout << "Budete počáteční hodnoty zadávat  ručně (ano/ne)? ";

    while ((manual_entry != "a") && (manual_entry != "n") && (manual_entry != "ano") && (manual_entry != "ne")) {
        cin >> manual_entry;
    }

    cout << endl;

    if ((manual_entry == "a") || (manual_entry == "ano")) {
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
        {   {0, 0, 0, 4, 0, 0, 0, 7, 1},
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

    // počáteční podmínky uloženy - zapínáme časovač
    auto start = high_resolution_clock::now();

    int supplied_layout[maximal_value][maximal_value];
    memcpy(supplied_layout, initial_layout, maximal_value * maximal_value * sizeof(int));

    doublevector indices_order = {};
    doublevector acceptable_values = {};

    int i = -1;
    int j = maximal_value - 1;


    // vytváříme vektor přípustných číslic (acceptable values) na základě případné kolize s číslicemi ve stejném řádku, sloupci nebo buňce, plynoucí
    // z počátečních podmínek
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

    // pro účely zjednodušení na základě zjištěných přípustných číslic hledáme případné pozice, které jako jediné připouštějí umístění některých
    // číslic v rámci daného řádku, sloupce nebo buňky
    find_hidden_singles(supplied_layout, acceptable_values, indices_order);

    vector<doublevector> previous_acceptable_values;
    vector<doublevector> previous_indices_order;

    int previous_tested_layout[maximal_value * maximal_value][maximal_value][maximal_value];
    int size_of_relevant_alternatives = 0;

    cout << "Průběh:  " << endl;

    // hlavní cyklus - pro kontrolu složitosti výpočtu počítáme počet průchodů
    while (size(acceptable_values) > 0) {
        static int iteration_order = 0;
        iteration_order++;
        cout << "  " << iteration_order << ".iterace" << endl;
        main_iteration(supplied_layout, acceptable_values, indices_order, size_of_relevant_alternatives, previous_tested_layout, previous_acceptable_values,
            previous_indices_order);
    }

    cout << endl;

    cout << "Počáteční stav: " << endl;

    for (int i = 0; i < maximal_value; i++) {
        for (int j = 0; j < maximal_value; j++) {
            cout << initial_layout[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    cout << "Koncový stav: " << endl;

    for (int i = 0; i < maximal_value; i++) {
        for (int j = 0; j < maximal_value; j++) {
            cout << supplied_layout[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    // vypnutí časovače, změření doby trvání výpočtu
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Celkový potřebný čas: " << duration.count() / 1000000.0f << " sekund" << endl;

    cin.get();

    return 0;
}