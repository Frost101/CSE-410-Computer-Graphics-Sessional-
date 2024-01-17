#ifndef HELPER_H
#define HELPER_H
#include<bits/stdc++.h>
using namespace std;

static unsigned long int g_seed = 1;
inline long int random(){
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

#endif