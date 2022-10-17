#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
int heshDel = 13;

/*
17
Открытая адресация
Учет товара: АРТИКУЛ,
название товара,
количество в наличии(штук)
 */

struct Titem {
    unsigned int Key = 0;
    string Prod[2];
    bool open = true;
};

struct TTable {
    vector<Titem> values = vector<Titem>(15);//непосредственно таблица из элементов
    double N = 0; //количество закрытых адресов таблицы
    double Del = 0;
};

//хэш функция
int heshFunction(int articNum) {
    return articNum % heshDel;
}

void outFile(TTable table) {
    ofstream out;
    out.open("output.txt");
    for (int i = 0; i < table.values.size(); ++i) {
        table.values[i] = table.values[i];
        if (!table.values[i].open && table.values[i].Key > 0)
            out << table.values[i].Key << " " << table.values[i].Prod[0] <<
                " " << table.values[i].Prod[1] << endl;
    }
}

//  Нормальная вставка
void putProdFinal(Titem item, TTable *table) {
    int ind = heshFunction(item.Key);
//  Находим место
    while (!(table->values[ind].open && table->N != -1)) {
        ind++;
        if (ind >= table->values.size()) break;
    }
//    Проверка на рехэш
    if ((table->N >= table->values.size() * 0.75) || ind >= table->values.size()) {
        cout << "Начал рехэширование " << (table->N) << " " << (table->values.size() * 0.75) << " " << ind << " "
             << table->values.size() << " \n";
        table->values.resize(heshDel * 2 + 5); //Увеличили хэш таблицу  в 2 раза
        heshDel *= 2; //Поменяли хэш функцию
        TTable newTable;
        newTable.values.resize(heshDel + 5);
        for (int i = 0; i < table->values.size(); ++i) {
            if (!table->values[i].open && table->values[i].Key != -1) {
                putProdFinal(table->values[i], &newTable);
            }
        }
        for (int i = 0; i < heshDel + 5; ++i) {
            table->values[i] = newTable.values[i];
        }
        //table = &newTable;
        putProdFinal(item, table);
        return;
    }
//    Кладем в ячейку
    table->values[ind] = item;
    table->values[ind].open = false;
    table->N++;
    cout << table->values[ind].Key << " in " << ind << " index\n";
    outFile(*table);
}

/*  вставка 1 вариант, неправильно
void putProd(Titem item, TTable *table, int ind) {
    if (table->values[ind].open && table->N != -1) {
        table->values[ind] = item;
        table->values[ind].open = false;
        table->N++;
        cout << "in " << ind << " index\n";
        return;
    } else {
        //добавление на 1 ячейку
        ind++;
        if (table->N >= table->values.size() * 0.75) {
            table->values.resize(table->values.size() * 2);
            heshDel *= 2;
            TTable newTable;
            for (int i = 0; i < table->values.size(); ++i) {
                if (!table->values[i].open && table->values[i].Key != -1) {
                    cout << table->values[i].Key << " ";
                    putProd(table->values[i], &newTable,heshFunction(table->values[i].Key));
                }
            }
            cout << endl << "Рехеширую!" << endl;
        }
        if (ind == table->values.size()) {
            table->values.resize(table->values.size() + 1);
            cout << "!Увеличение на 1!\n";
        }
        putProd(item, table, ind);
    }
} */

void rehesh(TTable *table) {
    cout << "Рехеш после удаления ячейки" << endl;
    TTable newTable;
    newTable.values.resize(heshDel + 5);
    for (int i = 0; i < table->values.size(); ++i) {
        if (!table->values[i].open && table->values[i].Key != -1) {
            putProdFinal(table->values[i], &newTable);
        }
    }
    for (int i = 0; i < heshDel + 5; ++i) {
        table->values[i] = newTable.values[i];

    }
    return;
}

// Взятие из хэша функции на вход поступает артикул
void findProd(int artic, TTable table, int ind) {
    if (table.values[ind].Key == artic && !table.values[ind].open) {
        cout << "Нашел!";
        cout << table.values[ind].Prod[0] << " " << table.values[ind].Prod[1] << endl;
    } else {
        ind++;
        if (ind == table.values.size()) {
            cout << "!Такого товара нет!\n";
            return;
        }
        findProd(artic, table, ind);
    }
    /*int ind = heshFunction(artic);
//  Находим место
    while (table->values[ind].Key != artic) {
        ind++;
        if (ind >= table->values.size()){
            cout << "Такого нет\n";
            return;
        }
    }
    cout << "Нашел!\n";
    cout << table->values[ind].Prod[0] << " " << table->values[ind].Prod[1] << endl << endl;*/
}

// Удаление продукта
void deleteProd(int artic, TTable *table, int ind) {
    if (table->values[ind].Key == artic) {
        cout << "Удалил из " << ind << " ячейки " << endl;
        table->values[ind].Key = -1; //признак удаленного адреса
        table->Del += 1;
    } else {
        ind++;
        if (ind == table->values.size()) {
            cout << "\n!Такого товара для удаления нет!\n";
            return;
        }
        deleteProd(artic, table, ind);
    }
    if ((table->Del) > (table->N * 0.25)) {
        rehesh(table);
        table->N -= table->Del;
        table->Del = 0;
    }
}

void takeCin(TTable *table) {
    int a;
    string b, c;
    cout << "Введите артикул ";
    cin >> a;
    cout << "Введите название ";
    cin >> b;
    cout << "Введите количество ";
    cin >> c;
    Titem item;
    item.Key = a;
    item.Prod[0] = b;
    item.Prod[1] = c;
    putProdFinal(item, table);
    outFile(*table);
}

// Взятие товаров из файла
void takeInformation(string file, TTable *table) {
    Titem item;
    ifstream in;
    in.open(file);
    string s1, s2, artic;
    while (in >> artic) {
        in >> s1;
        in >> s2;
        item.Key = stoi(artic);
        item.Prod[0] = s1;
        item.Prod[1] = s2;
        putProdFinal(item, table);
    }
}

int main() {
    TTable heshTable;
    takeInformation("input.txt", &heshTable);
    cout << "------------------------------------------------" << endl;
    //deleteProd(42366, &heshTable, heshFunction(42366));
    findProd(24662, heshTable, heshFunction(24662));
    /*cout << "-------------------------------------------------" << endl;
    deleteProd(24662, &heshTable, heshFunction(24662));
    deleteProd(27782, &heshTable, heshFunction(27782));*/
    cout << "-------------------------------------------------" << endl;
    clock_t start = clock();
    findProd(66778, heshTable, heshFunction(66778));
    clock_t end = clock(); //замеры времени и памяти
    cout << "Working time - " << (double) (end - start) / CLOCKS_PER_SEC << " seconds\n\n";

    clock_t start2 = clock();
    findProd(43244, heshTable, heshFunction(43244));
    clock_t end2 = clock(); //замеры времени и памяти
    cout << "Working time - " << (double) (end2 - start2) / CLOCKS_PER_SEC << " seconds\n\n";

    clock_t start3 = clock();
    findProd(56445, heshTable, heshFunction(56445));
    clock_t end3 = clock(); //замеры времени и памяти
    cout << "Working time - " << (double) (end3 - start3) / CLOCKS_PER_SEC << " seconds\n\n";


    cout << "-------------------------------------------------" << endl;
    deleteProd(27782, &heshTable, heshFunction(27782));
    cout << "-------------------------------------------------" << endl;

    takeCin(&heshTable);

    cout << heshTable.values.size();
    cout << endl << heshTable.N;
}

/* запихивать в файл
12331 Meal 322
42445 Honey 21
42366 Popcorn 215
65436 Cola 11
26609 Water 122
24662 Tea 120
23620 Salat 122
23611 FAFF 22
23654 sljf 2028
23668 Sadsf 2024
23670 Ssf 25
 */
