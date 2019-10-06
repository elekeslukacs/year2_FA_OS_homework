/**
*@author Elekes Lukacs-Roland
*@group 30424
*You are required to implement correctly and efficiently the insert and search operations in a hash table
using open addressing and quadratic probing.
*It can be observed, that the time for searching is constant (O(1)), which shows that hash tables are efficient for
storing elements and finding them quickly.
*Search of elements that are not in the table needs more steps, but it is also constant (O(1)), which is efficient
to determine whether an id can be found in the table or not.
*/
#include <iostream>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "Profiler.h"
#define N 10007
using namespace std;

int C1 = 1;
int C2 = 1;
//int totalEffort = 0;
int currentEffort = 0;
double avgEffort[5][2]; //row 0 for found, 1 for not found
int maxEffort[5][2];



struct Entry{
    int id;
    char name[30];
};

Entry table[N];

int randElement (int limit1, int limit2){
    srand(time(NULL));
    int x = rand()%(limit2 + 1 - limit1) + limit1;
    return x;
}

void initTable(){
    for (int i = 0; i < N; i++){
        table[i].id = -1;
    }
}

int h (int x, int i){
    return (x + C1 * i + C2 * i*i) % N;
}



int insertInHash (int x){
    int i = 0;
    int j;
    do{
        j = h(x, i);
        if (table[j].id == -1 || table[j].id == x){
            table[j].id = x;
            //cout<<"position: "<<j<<"\n\n";
            return j;
        }
        i++;
    }
    while (i != N);
    cout << "error, hash table overflow";
    return -1;
}

int searchInHash (int x){
    int i = 0;
    int j;
    do{
        j = h(x, i);
        //totalEffort++;
        currentEffort++;
        if(table[j].id == x){
            return j;
        }
        i++;
    }
    while (table[j].id != -1 && i != N);
    return -1;
}

void averageCase(){
    initTable();
    double alfa[5] = {0.8, 0.85, 0.9, 0.95, 0.99};



    for (int fillFactor = 0; fillFactor < 5; fillFactor++){
        int n = alfa[fillFactor] * N;   //number of elements we have to put in the hash table

        avgEffort[fillFactor][0] = 0;
        avgEffort[fillFactor][1] = 0;
        //average case, 5 repetitions
        for (int cases = 0; cases < 5; cases++){
            initTable();

            //saving 1500 elements that will be found
//            int found[3000];
//            srand(time(NULL));
//            FillRandomArray(found, 3000, 10, 40000, true);


            //saving 1500 elements that will not be found
            int nfound[1500];
            srand(time(NULL));
            FillRandomArray(nfound, 1500, 40001, 80000, true);

            //generating distinct keys
            srand(time(NULL));
            int entries[N];
            FillRandomArray(entries, n, 10, 40001, true);

            //inserting entries in hash table
            for (int i = 0; i < n; i++){
                insertInHash(entries[i]);
            }


            maxEffort[fillFactor][0] = 0;
            maxEffort[fillFactor][1] = 0;

            currentEffort = 0;

            //searching elements
            for (int i =0; i < 1500; i++){
                //search elements that are in the table
                currentEffort = 0;
                if (searchInHash(table[i+4].id) >= 0 ){   //we take every fourth key from the table, I checked, in most cases the found elements
                                                          //are 1500 or it should be very close. That works because filling factor is big enough

                    if (currentEffort > maxEffort[fillFactor][0]){
                         maxEffort[fillFactor][0] = currentEffort;         //maximum effort per element
                    }
                    avgEffort[fillFactor][0] += currentEffort;              //total effort
                    currentEffort = 0;
                }

                searchInHash(nfound[i]);                                    //searching for keys that are not in the table
                    if (currentEffort > maxEffort[fillFactor][1]){
                         maxEffort[fillFactor][1] = currentEffort;
                    }
                    avgEffort[fillFactor][1] += currentEffort;
                    currentEffort = 0;

            }

            //cout<<kontor<<endl;

        }

        avgEffort[fillFactor][0] /= 5;
        avgEffort[fillFactor][1] /= 5;
        avgEffort[fillFactor][0] /= 1500;
        avgEffort[fillFactor][1] /= 1500;

    }


    FILE *out = fopen("output.txt", "w");
    fprintf(out, "Filling Factor       Avg. Effort found      Max Effort found       Avg. Effort not-found        Max Effort not-found\n");

    for(int i = 0 ; i < 5; i++){
        fprintf(out, "     %.2f                    %.2f                 %d                      %.2f                          %d\n",
                    alfa[i],            avgEffort[i][0],         maxEffort[i][0],       avgEffort[i][1],                maxEffort[i][1]);
    }
    fclose(out);
}

int main()
{
//    Entry in[5];
//    initTable();
//    for (int i = 0; i < 5; i++){
//        cin>>in[i].id;
//        scanf("%s", &in[i].name);
//        int j = insertInHash(in[i].id);
//        strcpy(table[j].name, in[i].name);
//    }
//    if (searchInHash(9)>0){
//        printf("%s\n", table[searchInHash(9)].name);
//    }
//    else printf("not found\n");
//
//    if (searchInHash(112)>0){
//        printf("%s\n", table[searchInHash(112)].name);
//    }
//    else printf("not found\n");
//
//    if (searchInHash(99)>0){
//        printf("%s\n", table[searchInHash(99)].name);
//    }
//    else printf("not found\n");
    //cout<<maxEffort[1][0]<<"   "<<maxEffort[1][1];
    averageCase();
    cout<<maxEffort[4][0]<<"   "<<maxEffort[4][1];
    return 0;
}
