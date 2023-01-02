#include <bits/stdc++.h>
using namespace std ;

template<class T> 
void print_matrix(vector<vector<T>> &mat){
    for(int i=0;i<mat.size();i++){
        for(int j=0;j<mat[i].size();j++){
            cout<<mat[i][j]<<" ";
        }
        cout<<endl;
    }
}

int n; 
vector<vector<int>> row,col,matrix,solved_matrix1,solved_matrix2; 
vector<int> domain_size,degree;
vector<int> variables,values,vah5;

bool cmp_vah1(int i,int j){
    return domain_size[i]<domain_size[j];
}
bool cmp_vah2(int i,int j){
    return degree[i]<degree[j];
}
bool cmp_vah3(int i,int j){
    return ((domain_size[i]==domain_size[j]) ? (degree[i]<degree[j]) : (domain_size[i]<domain_size[j]));
}
bool cmp_vah4(int i,int j){
    return (domain_size[i]/degree[i])<(domain_size[j]/degree[j]) ;
}

multiset<int, bool(*)(int,int)> vah1(cmp_vah1),vah2(cmp_vah2),vah3(cmp_vah3),vah4(cmp_vah4);
vector<int> vah5; 

int variable_heuristic; 

enum {VAH1,VAH2,VAH3,VAH4,VAH5,VAH0};

int get_next_variable_vah1(){
   if( vah1.empty() )
        return -1; 
    int t=*vah1.begin();
    vah1.erase(vah1.begin());
    return t; 
}
int get_next_variable_vah2(){
    if( vah2.empty() )
        return -1; 
    int t=*vah2.begin();
    vah2.erase(vah2.begin());
    return t; 
}
int get_next_variable_vah3(){
    if( vah3.empty() )
        return -1; 
    int t=*vah3.begin();
    vah3.erase(vah3.begin());
    return t; 
}
int get_next_variable_vah4(){
    if( vah4.empty() )
        return -1; 
    int t=*vah4.begin();
    vah4.erase(vah4.begin());
    return t; 
}
int get_next_variable_vah5(){
    if(vah5.empty()) return -1;
    int t=*vah.rbegin();
    vah5.pop_back();
    return t;
}

int (*get_next_variable)(); 

bool backtrack(int i){
    if(i==variables.size()) return true;

    int r=variables[i]/n,c=variables[i]%n;
    
    for(int k=1;k<=n;k++){
        if(row[r][k]||col[c][k]) continue ;
        matrix[r][c]=k;
        row[r][k]++;
        col[c][k]++;
        bool res=backtrack(i+1);
        if(res) return true;
        row[r][k]--;
        col[c][k]--;

    }
    return false; 
}

bool forwardcheck(int i){
    return false; 
}


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


    for(int &y:variables){
        vah1.insert(y);
        vah2.insert(y);
        vah3.insert(y);
        vah4.insert(y);
    }
    vah5 = variables ;
    
    cout<<"variables size="<<variables.size()<<endl;
    cout<<"VAH1, size="<<vah1.size()<<endl;
    for(int x:vah1)
        cout<<x<<" " ; cout<<endl;
    cout<<"VAH2, size="<<vah2.size()<<endl; 
    for(int x:vah2)
        cout<<x<<" "; cout<<endl;
    cout<<"VAH3, size="<<vah3.size()<<endl;
    for(int x:vah3)
        cout<<x<<" "; cout<<endl;
    cout<<"VAH4, size="<<vah4.size()<<endl;
    for(int x:vah4)
        cout<<x<<" "; cout<<endl;
    
    backtrack(0);
    cout<<"after backtracking: "<<endl;
    print_matrix(matrix);
}