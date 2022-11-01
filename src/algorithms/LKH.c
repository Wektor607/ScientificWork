#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "LKH.h"

bool edge_equal(Edge x, Edge y)
{
    return (x.node1.t.name == y.node1.t.name && x.node2.t.name == y.node2.t.name) ||
           (x.node1.t.name == y.node2.t.name && x.node2.t.name == y.node1.t.name);
}

Edge edge_init(twtown nd1, twtown nd2)
{
    Edge edge;
    edge.node1 = nd1;
    edge.node2 = nd2;
    return edge;
}
double gain(halfmatrix *m, Edge x, Edge y)
{
    return getByTown(m, x.node1.t.name, x.node2.t.name) - getByTown(m, y.node1.t.name, y.node2.t.name);
}

void print_tour(twtown *T, int n)
{
    for(int e = 0; e < n-1; ++e)
    {
        printf("%d---%d, ", T[e].t.name, T[e+1].t.name);
    }
    printf("\n");
}

void print_edges(Edge *E, int n)
{
    for(int e = 0; e < n+1; ++e)
    {
        printf("%d---%d, ", E[e].node1.t.name, E[e].node2.t.name);
    }
    printf("\n");
}

bool compair_idx(int idx1, int idx2, int lenSub)
{
    return idx1 == 0 && idx2 == 1 || idx1 == lenSub - 1 && idx2 == 0 || idx1 > 0 && idx1 < lenSub - 1 && idx1 + 1 == idx2;
}

void new_tour_create(twtown *T_old, twtown *T_new, int lenSub, Edge *X, Edge *Y, int i, twtown (*neighbours)[2], int *indexes, int *Xindex, int *Yindex)
{
    // printf("START\n");
    print_edges(X, i);
    print_edges(Y, i);
    bool direction = 1;
    for (int e_new = 0, e_old = 0; e_new < lenSub; ++e_new)
    {
        bool change_edge = 0;
        /* printf("%d\n", e_old);
        printf("%d\n", Xindex[e_old]);
        //printf("X[x]: %d\n", X[Xindex[e_old]].node1); */
       /*  if (Xindex[e_old] != -1)
        {
            change_edge = 1;
        } */
        /* printf("ne upal\n"); */
        for (int x = 0; x <= i; ++x)
        {
            if (X[x].node1.t.name == T_old[e_old].t.name || X[x].node2.t.name == T_old[e_old].t.name)
            {
                change_edge = 1;
                break;
            }
        }
        //printf("CHANGE\n");
        
        if (!change_edge) /* не меняем ребро */
        {
            T_new[e_new] = T_old[e_old];
            e_new++;
            if (direction)
            {
                e_old = e_old == lenSub - 1 ? 0 : e_old + 1;
                T_new[e_new] = T_old[e_old];
                e_old = e_old == lenSub - 1 ? 0 : e_old + 1;
            }
            else
            {
                e_old = e_old == 0 ? lenSub - 1 : e_old - 1;
                T_new[e_new] = T_old[e_old];
                //printf("%d %d %d\n", T_old[e_old].t.name, e_old, e_new);
                e_old = e_old == 0 ? lenSub - 1 : e_old - 1;
            }
            if (e_new >= lenSub-1)
                break;
        }
        else /* меняем ребро */
        {
            printf("here\n");
            if (e_new == 0) /* не начинаем с измененённого ребра */
            {
                e_old++;
                e_new--;
                continue;
            }
            int y = 0;
            twtown cur_town;
            /* Ищем Y, который будет следующим ребром в T_new и запоминаем второй его конец*/
            for (; y <= i; ++y)
            {
                if (Y[y].node1.t.name == T_new[e_new - 1].t.name)
                {
                    cur_town = T_new[e_new] = Y[y].node2;
                    break;
                }
                if (Y[y].node2.t.name == T_new[e_new - 1].t.name)
                {
                    cur_town = T_new[e_new] = Y[y].node1;
                    break;
                }
            }
            printf("YES\n");
            /* Цикл работающий до тех пор пока в T_new записываем подряд Y или пока не закончится тур */
            while(e_new < lenSub-1)
            {
                int flag_neighb = 0;
                for (int neighb = 0; neighb < 2; ++neighb)
                {
                    /* edge_tmp - два вариант ребер на старом туре, которые соединены с Y */
                    Edge edge_tmp = edge_init(T_new[e_new], neighbours[indexes[T_new[e_new].t.name]][neighb]);
                    int flag = 1;
                    /* Проверяем ялвяется ли edge_tmp теми ребрами, которые необходимо убрать из тура */
                    //int flag_old = 1;
                    for (int n = 0; n <= i; ++n)
                    {
                        if (edge_equal(edge_tmp, X[n]))
                        {
                            // flag = 0;
                            flag = 0;
                            break;
                        }
                    }
                    // int idx1 = indexes[edge_tmp.node1], idx2 = indexes[edge_tmp.node2]; 
                    // if (compair_idx(idx1, idx2, lenSub))
                    // {
                    //     if(edge_equal(edge_tmp, X[Xindex[idx1]]))
                    //         flag = 0;
                    // }
                    // else if (edge_equal(edge_tmp, X[Xindex[idx2]]))
                    // {
                    //     flag = 0;
                    // }
                    // if(flag_old != flag)
                    // {
                    //     for(int x_idx = 0; x_idx < lenSub; ++x_idx)
                    //     {
                    //         printf("Xindex[%d]: %d\n", x_idx, Xindex[x_idx]);
                    //     }
                    //     for(int j = 0; j <= i; ++j)
                    //     {
                    //         printf("X[%d]: %d %d\n", j, X[j].node1, X[j].node2);
                    //     }
                    //     printf("edge_tmp: %d %d X[Xindex[idx1]]: %d %d X[Xindex[idx2]]: %d %d\n", edge_tmp.node1, edge_tmp.node2, X[Xindex[idx1]].node1, X[Xindex[idx1]].node2, X[Xindex[idx2]].node1, X[Xindex[idx2]].node2);
                    // }
                    // Поиск номера следующего ребра в старом туре и проверка направления относительно старого тура
                    if (flag)
                    {
                        int edge = 0;
                        bool miniflag = 0;
                        for(; edge < lenSub; ++edge)
                        {
                            if(edge_tmp.node2.t.name == T_old[edge].t.name)
                            {
                                e_old = edge;
                                miniflag = 1;
                                break;
                            }
                        }
                        if (miniflag)
                        {
                            // direction = indexes[edge_tmp.node2] > indexes[edge_tmp.node1]; /* ошибка */
                            int idx1 = indexes[edge_tmp.node1.t.name], idx2 = indexes[edge_tmp.node2.t.name];  
                            direction = compair_idx(idx1, idx2, lenSub);
                        }
                        flag_neighb = 1;
                        break;
                    }
                }
                printf("stop neighb\n");
                /* Оба соседа оказались X */
                if(!flag_neighb)
                {
                    // printf("I am hear!\n");
                    for(int r = 0; r <= i; ++r)
                    {
                        if(Y[r].node1.t.name == cur_town.t.name && y != r)
                        {
                            e_new++;
                            cur_town = T_new[e_new] = Y[y].node2;
                            // printf("FIRST IF: %d %d %d %d\n", Y[r].node1, Y[r].node2, Y[y].node1, Y[y].node2);
                            y = r;
                            break;
                        }
                        if(Y[r].node2.t.name == cur_town.t.name && y != r)
                        {
                            e_new++;
                            cur_town = T_new[e_new] = Y[y].node1;
                            // printf("SECOND IF: %d %d %d %d\n", Y[r].node1, Y[r].node2, Y[y].node1, Y[y].node2);
                            y = r;
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
                // printf("iter end %d\n", e_new);
            }
            // printf("go out\n");
        }
    }
    print_tour(T_new, lenSub);
    // printf("end\n");
}
double lkhTw(twtown *sub, int lenSub, halfmatrix *m, double *timer, const double endTime, double zeroParam1, double zeroParam2, int countTowns, int dist_param)
{
    depoShift(lenSub, sub);
    printf("%d\n", lenSub);
    /* 1 Create starting tour T */
    //Edge T[lenSub];
    twtown T[lenSub];
    Edge X[lenSub];
    Edge Y[lenSub];
    int useflag8 = 0, useflag9 = 0, ynoty = 0;
    int Xindex[lenSub];
    int Yindex[lenSub];
    int indexes[countTowns];/* индексы городов, поменять 1-ый параметр на число всех городов !!!*/
    double FT = 0, FT1 = 0;
    twtown neighbours[lenSub][2]; /* neighbours[i] - массив соседних городов для города i по текущему туру */
    Xindex[0] = Yindex[0] = -1;
    for (int town = 0; town < lenSub - 1; ++town)
    {
        T[town] = sub[town];
        neighbours[town][1] = sub[town + 1];
        neighbours[town + 1][0] = sub[town];
        Xindex[town + 1] = Yindex[town + 1] = -1;
        if(dist_param == 0)
        {
            FT += getByTown(m, sub[town].t.name, sub[town + 1].t.name);
        }
        indexes[sub[town].t.name] = town;
    }
    T[lenSub - 1] = sub[lenSub - 1];
    neighbours[0][0] = sub[lenSub - 1];
    neighbours[lenSub - 1][1] = sub[0];
    indexes[sub[lenSub-1].t.name] = lenSub - 1;
    print_tour(T, lenSub);
    /* for (int i = 0; i < lenSub; ++i)
    {
        printf("%d %d %d\n", sub[i].t.name, neighbours[i][0], neighbours[i][1]);
    } */
    if(dist_param == 0)
    {
        FT += getByTown(m, sub[lenSub - 1].t.name, sub[0].t.name);
    }
    else
    {
        FT = subtourdistanceTw(sub, lenSub, m, *timer, endTime);
    }
    // printf("FT start %lf\n", FT);
    bool new_tour = 1;
    while(new_tour)
    {
        new_tour = 0;
        /* 2 Choose t1*/
        for (int t1_idx = 0; t1_idx < lenSub; ++t1_idx)
        {
            twtown t1_var = T[t1_idx];
            /* 3 choose X[0] */
            twtown t2[2] = {neighbours[t1_idx][0], neighbours[t1_idx][1]};
            for (int t2_var = 0; t2_var < 2; t2_var++)
            {
                int t2_idx = indexes[t2[t2_var].t.name];
                if(compair_idx(t1_idx, t2_idx, lenSub))
                {
                    Xindex[t1_idx] = 0;
                    X[0] = edge_init(t1_var, t2[t2_var]);
                }
                else
                {
                    Xindex[t2_idx] = 0;
                    X[0] = edge_init(t2[t2_var], t1_var);
                }
                //Xindex[t1] = 0;
                /* 4  Choose Y[0] */
                for (int t3_idx = 0; t3_idx < lenSub; ++t3_idx)
                {
                    twtown t3 = T[t3_idx];
                    int prev_idx = t3.t.name; /* t_(2i-1) */
                    if(t3.t.name == neighbours[t2_idx][0].t.name || t3.t.name == neighbours[t2_idx][1].t.name || t3.t.name == t2[t2_var].t.name)
                    {
                        continue;
                    }
                    Y[0] = edge_init(t2[t2_var], t3);
                    if(compair_idx(t2_idx, t3_idx, lenSub))
                    {
                        Yindex[t2_idx] = 0;
                    }
                    else
                    {
                        Yindex[t3_idx] = 0;
                    }
                    double g0 = gain(m, X[0], Y[0]); /* g0 = |X[0]| - |Y[0]| */
                    if (g0 <= 0)
                    {
                        continue;
                    }
                    
                    bool flag8 = 0, flag9 = 0;
                    int y2_alt = -1, x2_alt = -1;
                    /* 5 После выбора X[0] и Y[0] делаем i = i + 1*/
                    printf("5 START\n");
                    for (int i = 1; i < lenSub; ++i)
                    {
                        if(flag8 || flag9)
                        {
                            i = 1;
                        }
                        if(!flag8) /* Если Y[i] существует переходим к шагу 5 в противном случае откатываемся к Y[1]*/
                        {
                            //printf("\n%d %d %d %d %d\n\n", i, X[i-1].node1, X[i-1].node2, Y[i-1].node1, Y[i-1].node2);
                            /* form var for step 6 */
                            prev_idx = indexes[Y[i-1].node2.t.name];
                            twtown t2i[2] = {neighbours[prev_idx][0], neighbours[prev_idx][1]};
                            twtown xi_node1 = Y[i - 1].node2; /* t_(2i-1) */
                            /* 6 Choose X[i] */
                            int t2i_var = 0;
                            if(i == 1 && flag9) {
                                t2i_var = (x2_alt != -1) ? 1 : 2;
                                x2_alt = -1;
                                // printf("FLAG9 USE 2\n");
                            }
                            bool flag6 = 0;
                            printf("IN FLAG6\n");
                            for (; t2i_var < 2; ++t2i_var)
                            {
                                // printf("START X[i]\n");
                                int t2i_idx = indexes[t2i[t2i_var].t.name];
                                if(compair_idx(prev_idx, t2i_idx, lenSub))
                                {
                                    Xindex[prev_idx] = i;
                                    X[i] = edge_init(xi_node1, t2i[t2i_var]);
                                }
                                else
                                {
                                    Xindex[t2i_idx] = i;
                                    X[i] = edge_init(t2i[t2i_var], xi_node1);
                                }
                                Y[i] = edge_init(t2i[t2i_var], t1_var); /* join t2i t1 */
                                if(compair_idx(t1_idx, t2i_idx, lenSub))
                                {
                                    Yindex[t1_idx] = 0;
                                }
                                else
                                {
                                    Yindex[t2i_idx] = 0;
                                }
                                
                                /* 6a create T1 and check T1 is tour */
                                twtown T1[lenSub];
                                printf("New tour create\n");
                                new_tour_create(T, T1, lenSub, X, Y, i, neighbours, indexes, Xindex, Yindex);
                                // Edge *T1 = new_tour_create(T, T1, lenSub, X, Y, i, neighbours, indexes);
                                printf("here 6a\n");
                                
                                bool flag6a = 1;
                                /* Проверяем, что всех городов по два */
                                printf("START CHECK 2\n");
                                for (int cur_town = 0; cur_town < lenSub; ++cur_town)
                                {
                                    int cntr = 0;
                                    for (int e = 0; e < lenSub; ++e)
                                    {
                                        if(T1[e].t.name == T[cur_town].t.name)
                                            cntr++;
                                        
                                        if (cntr > 1)
                                        {
                                            flag6a = 0;
                                            break;
                                        }
                                    }
                                    if(flag6a == 0)
                                        break;
                                }
                                printf("END CHECK 2\n");
                                if(!flag6a)
                                {
                                    /* bad tour T1 */
                                    // if(T1 != NULL)
                                    //     free(T1);
                                    // printf("bad tour T1\n");
                                    continue;
                                }
                                // printf("6a good\n");
                                //printf("\nNEW %d %d %d %d %d\n\n", i, X[i].node1, X[i].node2, Y[i].node1, Y[i].node2);
                            
                                FT1 = 0;
                                // twtown* sub_temp = calloc(lenSub, sizeof(twtown));
                                /* print_tour(T1, lenSub); */
                                int len_sub = 0;
                                for (int e = 0; e < lenSub-1; ++e)
                                {
                                    if(dist_param == 0)
                                    {
                                        FT1 += getByTown(m, T1[e].t.name, T1[e].t.name);
                                    }
                                    len_sub++;
                                }
                                // printf("SUB TEMP\n");
                                if(dist_param == 0)
                                {
                                    FT1 += getByTown(m, T1[lenSub-1].t.name, T1[0].t.name);
                                }
                                else
                                {
                                    // printf("START FT1\n");
                                    FT1 = subtourdistanceTw(T1, lenSub, m, *timer, endTime);   
                                    // printf("END FT1\n");
                                }
                                //printf("%lf %lf\n", FT1, FT);
                                // printf("SEARCH BEST\n");
                                if ((FT1 < FT || FT == -1) && FT1 != -1) /* Если найденный тур лучше лучшего,то заменяем*/
                                {
                                    indexes[T1[0].t.name] = 0;
                                    // printf("START CYCLE\n");
                                    for (int e = 0; e < lenSub; ++e)
                                    {
                                        Xindex[e] = Yindex[e] = -1;
                                        T[e] = T1[e];
                                        if (e < lenSub - 1)
                                        {
                                            // printf("%d %d\n",T1[e].node2, T1[e].node1);
                                            // printf("%d\n", indexes[sub_temp[e + 1].t.name]);
                                            indexes[T1[e + 1].t.name] = e + 1;
                                            // printf("indexes[e]: %d indexes[e+1]: %d\n", indexes[e],indexes[e+1]);
                                            // printf("NEW: %d\n", indexes[sub_temp[e + 1].t.name]);
                                            neighbours[e][1] = T1[e];
                                            // printf("NEW NEIGHBOUR 1: %d\n", neighbours[indexes[e]][1]);
                                            neighbours[e + 1][0] = T1[e];
                                            // printf("NEW NEIGHBOUR 2: %d\n", neighbours[indexes[e + 1]][0]);
                                            // printf("e: %d\n", e);
                                        }
                                    }
                                    // printf("END CYCLE\n");
                                    neighbours[lenSub - 1][1] = T1[lenSub-1];
                                    // printf("FIRST NEIGHBOUR\n");
                                    neighbours[0][0] = T1[lenSub - 1];
                                    // printf("SECOND NEIGHBOUR\n");
                                    FT = FT1;
                                    new_tour = 1;
                                    // print_tour(T1, lenSub);
                                    break; /* 6to2 break */
                                }
                                new_tour = 0;
                                flag6 = 1;
                                break;
                            } /* 6 end */
                            printf("FLAG 6 END\n");
                            if(!flag6)
                            {
                                break;
                            }
                            flag9 = 0;
                            if (new_tour)
                            {
                                break;
                            }
                            /* подготовка к шагу 9 */                 
                            if (i == 1)
                            {
                                x2_alt = -1;
                                // printf("FLAG9 PREV\n");
                                if(t2i_var == 0)
                                {
                                    x2_alt = t2i_var + 1;
                                }
                            }
                        }
                        /* 7 Choose Y[i] */
                        bool flag7 = 0;
                        int next_idx = 0;
                        if(i == 1 && flag8) {
                            next_idx = (y2_alt != -1) ? y2_alt : lenSub;
                            y2_alt = -1;
                            flag8 = 0;
                        }
                        for (; next_idx < lenSub; ++next_idx)
                        {
                            //printf("%d %d\n", sub[j].t.name, X[i].node2);
                            twtown yinode2 = T[next_idx]; /* t_(2i+1) */
                            if(yinode2.t.name == neighbours[next_idx][0].t.name || yinode2.t.name == neighbours[next_idx][1].t.name || yinode2.t.name == X[i].node2.t.name)
                            {
                                continue; /* 8 */ 
                            }
                            // printf("NICE\n");
                            Y[i] = edge_init(X[i].node2, yinode2);
                            if(compair_idx(t2_idx, next_idx, lenSub))
                            {
                                Yindex[t2_idx] = i;
                            }
                            else
                            {
                                Yindex[next_idx] = i;
                            }
                            bool flag7_equal = 0;
                            for (int s = 0; s < i; ++s)
                            {
                                if (edge_equal(Y[i], Y[s]))
                                {
                                    flag7_equal = 1;
                                    ynoty++;
                                    break;
                                }
                            }
                            if(flag7_equal)
                            {
                                continue;
                            }
                            // printf("SAD\n");
                            /* 7a */
                            double Gi = 0;
                            for (int r = 0; r <= i; ++r)
                            {
                                // printf("%d %d %d %d\n", X[r].node1, X[r].node2, Y[r].node1, Y[r].node2);
                                Gi += gain(m, X[r], Y[r]);
                            }
                            // if(i == 4){
                            // printf("I am hear!!!\n");
                            // }
                            if (Gi <= 0)
                                continue; /* 8 */
                            /* 7c */
                            int flag7c = 0;
                            for(int neigh = 0; neigh < 2; ++neigh)
                            {
                                X[i + 1] = edge_init(yinode2, neighbours[next_idx][neigh]);
                                for (int s = 0; s <= i; ++s)
                                {
                                    if (edge_equal(X[s], X[i+1]))
                                    {
                                        flag7c++;
                                    }
                                    if(flag7c == 2)
                                    {
                                        break;
                                    }    
                                }
                                if(flag7c == 2)
                                {
                                    break;
                                }
                            }
                            
                            if (Gi > 0 && !flag7c )//&& flag7b && !flag7c)
                            {
                                flag7 = 1;
                                break;
                            }
                        } /* 7 end */
                        /* подготовка к 8 шагу заранее */
                        if(i == 1)
                        {
                            y2_alt = -1;
                            if(next_idx < lenSub - 1 && flag7)
                            {
                                y2_alt = next_idx + 1;
                            }
                        }
                        /* 8 */
                        if(!flag7)
                        {
                            if (y2_alt != -1)
                            {
                                flag8 = 1;
                                useflag8++;
                                // printf("FLAG8\n");
                                for(int x_idx = 0; x_idx < lenSub; ++x_idx) 
                                { 
                                    if(Xindex[x_idx] != 0 && Xindex[x_idx] != 1) 
                                        Xindex[x_idx] = -1;
                                }
                                continue;
                            }
                            /* 9 */
                            if (x2_alt != -1)// && counter_flag < 100)
                            {
                                flag9 = 1;
                                useflag9++;
                                y2_alt = -1;
                                for(int x_idx = 0; x_idx < lenSub; ++x_idx) 
                                { 
                                    if(Xindex[x_idx] != 0) 
                                        Xindex[x_idx] = -1;  
                                }
                                // printf("FLAG9 UP\n");
                                continue; 
                            }
                            /* 10 */
                            break;
                        }
                        // printf("PRE 5 END\n");
                    } /* 5 end */
                    printf("5 END\n");
                    if (new_tour)
                        break;
                    for(int x_idx = 0; x_idx < lenSub; ++x_idx) 
                    { 
                        if(Xindex[x_idx] != 0) 
                            Xindex[x_idx] = -1;  
                    }
                } /* 4 end */
                printf("4 END\n");
                if (new_tour)
                    break;
                for(int x_idx = 0; x_idx < lenSub; ++x_idx) 
                { 
                    Xindex[x_idx] = -1;  
                }
                    
            } /* 3 end */
            printf("3 END\n");
            if (new_tour)
            {
                printf("change tour\n");
                print_tour(T, lenSub);
                break;
            }
            for(int x_idx = 0; x_idx < lenSub; ++x_idx) 
            { 
                Xindex[x_idx] = -1;  
            }
            printf("%d %d %d %d\n", t1_idx, useflag8, useflag9, ynoty);
        } /* 2 end */
        printf("2 END\n");
        for(int x_idx = 0; x_idx < lenSub; ++x_idx) 
        { 
            Xindex[x_idx] = -1;  
        }
    }
    // print_tour(T, lenSub);
    double best = subtourdistanceTw(T, lenSub, m, *timer, endTime);
    if(best != -1)
    {
        for(int p = 0; p < lenSub; ++p)
        {
            sub[p] = T[p];
        }
    }
    
    if(best != -1)
    {
        *timer += best;
    }

    return best;
}