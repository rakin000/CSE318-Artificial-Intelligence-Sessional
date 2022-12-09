#include <bits/stdc++.h>
using namespace std ;


class board {
    vector<vector<int>> tiles ;
    int n,hamming_distance, manhattan_distance, zero; 
    const int blank_tile= 0 ;
    public: 

    board(vector<vector<int>> tiles ){
        n = tiles.size() ;
        if( n != tiles[0].size() ){
            throw exception();
        }

        this->tiles = vector<vector<int>> (n,vector<int> (n)) ;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                this->tiles[i][j] = tiles[i][j] ;
            }
        }

        hamming_distance = manhattan_distance = 0 ;
        int goalboard_value = 1; 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if( i+j != 2*(n-1) && this->tiles[i][j] != goalboard_value )
                    hamming_distance++ ;
                if( this->tiles[i][j] == blank_tile ){
                    zero = n*i+j ;
                }
                else {
                    manhattan_distance += abs( (this->tiles[i][j]-1)/n-i ) + abs( ((this->tiles[i][j]-1)%n) - j )  ;
                }
                goalboard_value++;
            }
        }
    }
    board( const board &brd ){
        this->n = brd.n ;
        this->hamming_distance = brd.hamming_distance ;
        this->manhattan_distance = brd.manhattan_distance ;
        this->zero = brd.zero ;

        this->tiles = vector<vector<int>> (n,vector<int> (n)) ;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++)
                tiles[i][j] = brd.tiles[i][j] ;
        }
    }

    vector<board> neighbors(){
        vector<board> temp;

        vector<vector<int>> brd = tiles ;
        int x = zero/n; int y = zero%n ;
        // cout<<x<<" "<<y<<endl;
        if (x + 1 < n){
            swap(brd[x][y],brd[x+1][y]) ;
            temp.push_back(board(brd));
            swap(brd[x][y],brd[x+1][y]) ;
        }
        if (x - 1 >= 0){
            swap(brd[x][y],brd[x-1][y]);
            temp.push_back(board(brd));
            swap(brd[x][y],brd[x-1][y]);
        }
        if (y + 1 < n){
            swap(brd[x][y],brd[x][y+1]);
            temp.push_back(board(brd));
            swap(brd[x][y],brd[x][y+1]);

        }
        if (y - 1 >= 0){
            swap(brd[x][y],brd[x][y-1]);
            temp.push_back(board(brd)) ;
            swap(brd[x][y],brd[x][y-1]);
        }
        // cout<<temp.size()<<endl ;
        
        return temp ;
    }
    vector<vector<int>>& get_tiles(){
        return tiles; 
    }
    int getInvCount()
    {
        int px,py,qx,qy;
        int inv_count = 0;

        // for(int i=0;i<n;i++){
        //     for(int j=0;j<n;j++){
        //         cout<<tiles[i][j]<<" ";
        //     }
        //     cout<<endl;
        // }
        for (int i = 0; i < n*n - 1; i++){
            for (int j = i + 1; j < n*n; j++){
                px =i/n;
                py =i%n;
                qx =j/n;
                qy =j%n;
                if ( tiles[qx][qy] && tiles[px][py] && (tiles[px][py] > tiles[qx][qy]) ){
                    // cout<<"("<<px<<","<<py<<")  and ("<<qx<<","<<qy<<")"<<endl;
                    inv_count++;
                }
            }
        }
        return inv_count;
    }

    // find Position of blank from bottom
    int find_blank_position()
    {
        // start from bottom-right corner of matrix
        for (int i = n - 1; i >= 0; i--)
            for (int j = n - 1; j >= 0; j--)
                if (tiles[i][j] == 0)
                    return n - i;
    }

    // This function returns true if given
    // instance of N*N - 1 puzzle is solvable
    bool solvable()
    {
        int invCount = getInvCount();
        if (n & 1)
            return !(invCount & 1);

        else // grid is even
        {
            int pos = find_blank_position();
            if (pos & 1)
                return !(invCount & 1);
            else
                return invCount & 1;
        }
    }

    int64_t hash(int64_t base, int64_t p) const {
        int64_t val = 0 ;
        int64_t b = 1ll; 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                val = (val + (b*tiles[i][j])%p )%p;
                b = b*base ;
            }
        }
        return val ;
    }
    bool equals(board &brd){
        if( this->n != brd.n ) return 0 ;
        bool res = 1; 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                res &= (tiles[i][j] == brd.tiles[i][j] ) ;
            }
        }
        return res ;
    }
    int manhattan(){
        return manhattan_distance ;
    }
    int hamming(){
        return hamming_distance; 
    }
    bool goal() {
        return !hamming_distance ;
    }

    friend ostream& operator<<(ostream& out, board &b){
        // out<<"manhattan = "<<b.manhattan_distance<<endl;
        // out<<"hamming = "<<b.hamming_distance<<endl;
        out<<b.n<<endl ;
        for(int i=0;i<b.n;i++){
            for(int j=0;j<b.n;j++){
                out<<b.tiles[i][j]<<" ";
            }
            out<<endl;
        }
        return out ;
    }
};

void test() {
    board brd(vector<vector<int>> ({{1,3,2},{4,0,5},{7,8,6}}) ) ;
    cout<<brd<<endl;
 
    vector<board> nn = brd.neighbors() ;
    // cout<<nn.size() ;
    for(auto b: nn ){
        cout<<b<<endl;
    }
}