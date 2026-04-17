
#include <iostream>
using namespace std;

#define SIZE 10
#define MAX 10000

struct State {
    int x, y;
    int fuel;
    char c1, c2, c3, c4;
    int parent;
};

bool isSame(State a, State b){
    return (a.x==b.x && a.y==b.y && a.fuel==b.fuel &&
            a.c1==b.c1 && a.c2==b.c2 && a.c3==b.c3 && a.c4==b.c4);
}

void printState(State s){
    cout<<"("<<s.x<<","<<s.y<<","<<s.fuel<<","
        <<s.c1<<","<<s.c2<<","<<s.c3<<","<<s.c4<<")\n";
}

bool isGoal(State s){
    return (s.x==1 && s.y==1 &&
            s.c1=='t' && s.c2=='t' && s.c3=='t' && s.c4=='t');
}

int grid[SIZE][SIZE] = {0};

int C1[2]={3,3};
int C2[2]={4,6};
int C3[2]={6,8};
int C4[2]={9,9};

int fuelStation[2]={3,2};

bool valid(int x,int y){
    return (x>=0 && x<SIZE && y>=0 && y<SIZE && grid[x][y]==0);
}

State move(State s,int dx,int dy){
    State n = s;
    n.x += dx;
    n.y += dy;
    n.fuel -= 1;

    if(n.x==C1[0] && n.y==C1[1]) n.c1='t';
    if(n.x==C2[0] && n.y==C2[1]) n.c2='t';
    if(n.x==C3[0] && n.y==C3[1]) n.c3='t';
    if(n.x==C4[0] && n.y==C4[1]) n.c4='t';

    if(n.x==fuelStation[0] && n.y==fuelStation[1]) n.fuel=20;

    return n;
}

void printPath(State arr[], int index){
    if(index==-1) return;
    printPath(arr, arr[index].parent);
    printState(arr[index]);
}

// check duplicate
bool exists(State arr[], int size, State s){
    for(int i=0;i<size;i++){
        if(isSame(arr[i],s)) return true;
    }
    return false;
}