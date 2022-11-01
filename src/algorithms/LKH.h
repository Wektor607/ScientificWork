#include "../mainFunctions/dataProcessing.h"


#ifndef LKH_H
#define LKH_H

typedef struct Edge
{
    twtown node1;
    twtown node2;
} Edge;

bool edge_equal(Edge x, Edge y);

Edge edge_init(twtown nd1, twtown nd2);

double gain(halfmatrix *m, Edge x, Edge y);

void new_tour_create(twtown *T_old, twtown *T_new, int lenSub, Edge *X, Edge *Y, int i, twtown (*neighbours)[2], int *indexes, int *Xindex, int *Yindex);

void print_tour(twtown *T, int n);

double lkhTw(twtown *sub, int lenSub, halfmatrix *m, double *timer, const double endTime, double zeroParam1, double zeroParam2, int countTowns, int dist_param);

#endif