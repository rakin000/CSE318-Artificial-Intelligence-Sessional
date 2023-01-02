#include <bits/stdc++.h>
using namespace std ;


int n; 
vector<vector<int>> row,col,matrix; 
vector<int> domain_size,degree;
vector<int> variables;

// void backtrack(int r, vector<vector<int>> &matrix){
//     if( r==n ){
//         return ;
//     }

//     for()
// }


void runner(){

}

int main(){
    ios_base::sync_with_stdio(false) ;
    cin>>n;
    // cout<<n<<endl;
    matrix=vector<vector<int>>(n,vector<int>(n));
    domain_size=vector<int>(n*n,0);
    degree=vector<int>(n*n,0);
    row=vector<vector<int>>(n,vector<int>(n+1,0));
    col=vector<vector<int>>(n,vector<int>(n+1,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>matrix[i][j];
        }
    }

    

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            row[i][matrix[i][j]]++;
            col[j][matrix[i][j]]++;
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if( matrix[i][j] ) continue ;
            int d_cnt=0;
            for(int k=1;k<=n;k++)
                d_cnt+=(!row[i][k] && !col[j][k]);
                
            domain_size[i*n+j]=d_cnt;
            degree[i*n+j]=row[i][0]+col[j][0]-1;
            variables.push_back(i*n+j) ;
        }
    }

    cout<<"now n="<<n<<endl;
    cout<<"Degree: "<<endl;
    // degree ;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if( matrix[i][j]){
                cout<<0<<" ";
            }
            else {
                cout<<degree[i*n+j]<<" ";
            }
        }
        cout<<endl;
    }
    // domain_size ;
    cout<<"Now n="<<n<<endl;
    cout<<"Domain Size: "<<endl;; 
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if( matrix[i][j]){
                cout<<0<<" ";
            }
            else {
                cout<<domain_size[i*n+j]<<" ";
            }
        }
        cout<<endl;
    }
}