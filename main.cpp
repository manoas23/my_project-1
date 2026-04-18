#include <iostream>
using namespace std;

const int SIZE = 10;
const int MAX = 10000;

struct State {
    int x, y;
    int fuel;
    char c1, c2, c3, c4;
    int parent;
};

class Game {
public:

    int grid[SIZE][SIZE];

    int C1[2]={3,4};
    int C2[2]={4,6};
    int C3[2]={6,8};
    int C4[2]={9,9};

    int fuelStation[2]={3,2};

    Game(){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                grid[i][j]=0;
            }
        }

        grid[2][2]=1;
        grid[3][3]=1;
    }

    bool valid(int x,int y){
        if(x>=0 && x<SIZE && y>=0 && y<SIZE && grid[x][y]==0)
            return true;
        return false;
    }

    bool isGoal(State s){
        if(s.x==1 && s.y==1 &&
           s.c1=='t' && s.c2=='t' &&
           s.c3=='t' && s.c4=='t')
            return true;

        return false;
    }

    State move(State s,int dx,int dy){
        State n = s;

        n.x += dx;
        n.y += dy;
        n.fuel--;

        if(n.x==C1[0] && n.y==C1[1]) n.c1='t';
        if(n.x==C2[0] && n.y==C2[1]) n.c2='t';
        if(n.x==C3[0] && n.y==C3[1]) n.c3='t';
        if(n.x==C4[0] && n.y==C4[1]) n.c4='t';

        if(n.x==fuelStation[0] && n.y==fuelStation[1])
            n.fuel=20;

        return n;
    }

    void printState(State s){
        cout<<"("<<s.x<<","<<s.y<<","<<s.fuel<<","
            <<s.c1<<","<<s.c2<<","<<s.c3<<","<<s.c4<<")\n";
    }
};

class Algorithms {
public:

    bool isSame(State a, State b){
        return (a.x==b.x && a.y==b.y && a.fuel==b.fuel &&
                a.c1==b.c1 && a.c2==b.c2 &&
                a.c3==b.c3 && a.c4==b.c4);
    }

    bool exists(State arr[], int size, State s){
        for(int i=0;i<size;i++){
            if(isSame(arr[i],s))
                return true;
        }
        return false;
    }

    void printPath(State arr[], int index, Game &g){
        if(index==-1) return;

        printPath(arr, arr[index].parent, g);
        g.printState(arr[index]);
    }

    // ===== BFS =====
    void BFS(Game &g, State start){

        State queue[MAX];
        State visited[MAX];

        int front=0, rear=0, visitedCount=0;

        start.parent = -1;
        queue[rear++] = start;

        cout<<"\nBFS:\n";

        while(front < rear){

            State cur = queue[front++];

            if(exists(visited,visitedCount,cur))
                continue;

            visited[visitedCount]=cur;
            int idx=visitedCount;

            cout<<"Visited: ";
            g.printState(cur);

            if(g.isGoal(cur)){
                cout<<"\nGoal reached\nPath:\n";
                printPath(visited,idx,g);

                int cost=0, temp=idx;
                while(temp!=-1){
                    cost++;
                    temp=visited[temp].parent;
                }
                cout<<"Cost = "<<cost-1<<"\n";
                return;
            }

            visitedCount++;

            if(cur.fuel<=0) continue;

            int dx[4]={1,-1,0,0};
            int dy[4]={0,0,1,-1};

            for(int i=0;i<4;i++){

                int nx=cur.x+dx[i];
                int ny=cur.y+dy[i];

                if(g.valid(nx,ny)){

                    State next = g.move(cur,dx[i],dy[i]);

                    if(!exists(visited,visitedCount,next) &&
                       !exists(queue,rear,next)){

                        next.parent=idx;
                        queue[rear++]=next;
                    }
                }
            }
        }
    }

    // ===== DFS =====
    bool DFS_util(Game &g, State cur, State visited[], int &visitedCount){

        if(exists(visited,visitedCount,cur)) return false;

        visited[visitedCount]=cur;
        int idx=visitedCount;
        visitedCount++;

        cout<<"Visited: ";
        g.printState(cur);

        if(g.isGoal(cur)){
            cout<<"\nGoal reached\nPath:\n";
            printPath(visited,idx,g);

            int cost=0, temp=idx;
            while(temp!=-1){
                cost++;
                temp=visited[temp].parent;
            }
            cout<<"Cost = "<<cost-1<<"\n";
            return true;
        }

        if(cur.fuel<=0){
            visitedCount--;
            return false;
        }

        int dx[4]={1,-1,0,0};
        int dy[4]={0,0,1,-1};

        for(int i=0;i<4;i++){

            int nx=cur.x+dx[i];
            int ny=cur.y+dy[i];

            if(g.valid(nx,ny)){

                State next = g.move(cur,dx[i],dy[i]);
                next.parent=idx;

                if(DFS_util(g,next,visited,visitedCount))
                    return true;
            }
        }

        visitedCount--;
        return false;
    }

    void DFS(Game &g, State start){
        State visited[MAX];
        int visitedCount=0;

        start.parent=-1;

        cout<<"\nDFS:\n";
        DFS_util(g,start,visited,visitedCount);
    }

    // ===== DLS (جزء من IDS) =====
bool DLS(Game &g, State cur, State visited[], int &visitedCount, int limit){

    if(exists(visited,visitedCount,cur)) return false;

    visited[visitedCount]=cur;
    int idx=visitedCount;
    visitedCount++;

    cout<<"Visited: ";
    g.printState(cur);

    if(g.isGoal(cur)){
        cout<<"\nGoal reached\nPath:\n";
        printPath(visited,idx,g);

        int cost=0, temp=idx;
        while(temp!=-1){
            cost++;
            temp=visited[temp].parent;
        }
        cout<<"Cost = "<<cost-1<<"\n";
        return true;
    }

    if(limit<=0 || cur.fuel<=0){
        visitedCount--; // backtrack
        return false;
    }

    int dx[4]={1,-1,0,0};
    int dy[4]={0,0,1,-1};

    for(int i=0;i<4;i++){

        int nx=cur.x+dx[i];
        int ny=cur.y+dy[i];

        if(g.valid(nx,ny)){

            State next = g.move(cur,dx[i],dy[i]);
            next.parent=idx;

            if(DLS(g,next,visited,visitedCount,limit-1))
                return true;
        }
    }

    visitedCount--; // مهم
    return false;
}

// ===== IDS =====
void IDS(Game &g, State start){

    cout<<"\nIDS:\n";

    for(int depth=0; depth<20; depth++){

        State visited[MAX];
        int visitedCount=0;

        start.parent=-1;

        if(DLS(g,start,visited,visitedCount,depth))
            return;
    }

    cout<<"No solution found\n";
}
};

int main(){

    Game g;
    Algorithms algo;

    State start;

    cout<<"Enter start x y fuel:\n";
    cin>>start.x>>start.y>>start.fuel;

    start.c1='f';
    start.c2='f';
    start.c3='f';
    start.c4='f';

    algo.BFS(g,start);
    algo.DFS(g,start);
    algo.IDS(g,start);
}
