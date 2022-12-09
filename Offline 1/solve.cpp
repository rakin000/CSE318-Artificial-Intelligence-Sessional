#include<bits/stdc++.h>
#include "board.cpp" 
using namespace std ;


int heuristic_to_use ; // 1 to use manhattan 
                        // else to use hamming 
const int MANHATTAN = 1;
const int HAMMING = 2; 

struct node{
    node *parent ;
    int moves ;
    board *brd ;
    int priority ;
    node() {
        parent = nullptr;
        moves = 0 ;
        brd = nullptr ;
    }
    node(board brd, node* par = nullptr , int moves = 0){
        this->brd = new board(brd) ;
        if( heuristic_to_use == 1 ) priority = brd.manhattan()+moves;
        else priority = brd.hamming()+moves; 
        this->moves = moves ;
        this->parent = par; 
    }
    ~node(){
        delete brd ;
    }
};

class comparator{
    public: 
    int less(node* n1, node *n2){
        return n1->priority < n2->priority ;
    }
    int greater(node *n1,node *n2) {
        return n1->priority > n2->priority ;
    }
    bool operator()(node *n1,node *n2){
        return greater(n1,n2);
    }
};
class Hash {
    public: 
    int64_t operator() (const node *b){
        return b->brd->hash(7,1e9+7) ;
    }
};



vector<board> solv;
int64_t explored_node_count,expanded_node_count ;

void solve(board &b) {
    if (!b.solvable()){
      //  cout<<b.getInvCount()<<endl; 
      //  cout<<b.find_blank_position()<<endl;
        cout << "Not solvable!" << endl;
        return;
    }

    priority_queue<node *, vector<node *>, comparator> pq;
    vector<node *> todelete;
    node *initial = new node(b, 0);
    pq.push(initial);
    todelete.push_back(initial);

    while (!pq.empty()){
        node *last = pq.top();
        pq.pop();
        explored_node_count++;

        if (last->brd->goal())
        {
            while (last != nullptr)
            {
                solv.push_back(*last->brd);
                last = last->parent;
            }
            break;
        }

        for (board &bb : last->brd->neighbors())
        {
            node *temp = new node(bb, last, last->moves + 1);
            if (last->parent == nullptr || !last->parent->brd->equals(bb))
            {
                ++expanded_node_count;
                pq.push(temp);
                todelete.push_back(temp);
            }
        }
    }

    // cout << "created nodes: " << todelete.size() << endl;
    for (int i = 0; i < todelete.size(); i++){
        delete todelete[i];
    }
}

int main(){
    int n;
    cin>>n; 
    vector<vector<int>> tiles(n,vector<int> (n)) ;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>tiles[i][j]; 
            // tiles[i][j] = (c=='*' ? 0: c-'0') ;
        }
    }

    board b(tiles);

    heuristic_to_use = MANHATTAN; 
    explored_node_count = expanded_node_count = 0;
    solve(b) ;

    if( !solv.empty() ){ 
		cout << "Minimum Number of Moves: " << solv.size()-1 << endl;
		for(int i=solv.size()-1;i>=0;i--){
			cout<<solv[i];
		}
	}

    cout<<"Using Manhattan distance: ";
    cout<<"Explored Nodes: "<<explored_node_count<<", Expanded Nodes: "<<expanded_node_count<<endl ;

    heuristic_to_use = HAMMING ;
    explored_node_count = expanded_node_count = 0;
    solve(b);

    cout<<"Using Hamming distance: " ;
    cout<<"Explored Nodes: "<<explored_node_count<<", Expanded Nodes: "<<expanded_node_count<<endl;
}
