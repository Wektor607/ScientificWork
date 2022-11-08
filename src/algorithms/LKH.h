#include "../mainFunctions/dataProcessing.h"


#ifndef LKH_H
#define LKH_H

typedef struct Edge
{
    double len;
    int    idx1;
    int    idx2;
} Edge;

bool edge_equal(Edge x, Edge y);

Edge edge_init(halfmatrix *m, twtown t1, twtown t2, int idx1, int idx2);

double gain(Edge x, Edge y);

void new_tour_create(halfmatrix *m, twtown *T_old, twtown *T_new, int lenSub, Edge *X, Edge *Y, int i, twtown (*neighbours)[2], int *indexes, int *Xindex, int *Yindex);

void print_tour(twtown *T, int n);

double lkhTw(twtown *sub, int lenSub, halfmatrix *m, double *timer, const double endTime, double zeroParam1, double zeroParam2, int countTowns, int dist_param);

#endif