#include <bits/stdc++.h>
using namespace std ;

template<class T> 
void print_matrix(vector<vector<T>> &mat){
   
    for(int i=0;i<mat.size();i++){
        for(int j=0;j<mat[i].size();j++){
            // cout<<mat[i][j]<<" ";
            printf("%4d ",mat[i][j]);
        }
        cout<<"\n";
    }
    
}

int n; 
vector<vector<int>> row,col,matrix,solved_matrix1,solved_matrix2; 
vector<int> domain_size,degree;
vector<int> variables,values;

bool goalcheck(){
    vector<vector<int>> row(n,vector<int>(n+1,0));
    vector<vector<int>> col(n,vector<int>(n+1,0));
    
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            row[i][matrix[i][j]]++;
            col[j][matrix[i][j]]++;
        }
    }

    for(int i=0;i<n;i++){
        for(int k=1;k<=n;k++){
            if( row[i][k] != 1 || col[i][k] != 1 )
                return false;
        }
    }
    return true;
}

bool cmp_vah1(int i,int j){
    return (domain_size[i]==domain_size[j])?i<j:domain_size[i]<domain_size[j];
}
bool cmp_vah2(int i,int j){
    return (degree[i]==degree[j])?i<j:degree[i]<degree[j];
}
bool cmp_vah3(int i,int j){
    return (domain_size[i]==domain_size[j] && degree[i]==degree[j]) 
            ? i<j :
            ((domain_size[i]==domain_size[j]) ? (degree[i]<degree[j]) : (domain_size[i]<domain_size[j])) ;
}
bool cmp_vah4(int i,int j){
    return (domain_size[i]/degree[i]==domain_size[j]/degree[j]) ? i<j: (domain_size[i]/degree[i])<(domain_size[j]/degree[j]) ;
}

set<int, bool(*)(int,int)> vah1(cmp_vah1);
multiset<int, bool(*)(int,int)> vah2(cmp_vah2),vah3(cmp_vah3),vah4(cmp_vah4);
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
void insert_variable_vah1(int v){
    vah1.insert(v) ;
}
void erase_variable_vah1(int v){
    auto it=vah1.find(v); 
    if(it!=vah1.end())
        vah1.erase(it);
}

int get_next_variable_vah2(){
    if( vah2.empty() )
        return -1; 
    int t=*vah2.begin();
    vah2.erase(vah2.begin());
    return t; 
}
void insert_variable_vah2(int v){
    vah2.insert(v) ;
}
void erase_variable_vah2(int v){
    auto it=vah2.find(v);
    if(it!=vah2.end())
        vah2.erase(it);
}

int get_next_variable_vah3(){
    if( vah3.empty() )
        return -1; 
    int t=*vah3.begin();
    vah3.erase(vah3.begin());
    return t; 
}
void insert_variable_vah3(int v){
    vah3.insert(v);
}
void erase_variable_vah3(int v){
    auto it=vah3.find(v);
    if(it!=vah3.end())
        vah3.erase(it) ;
}

int get_next_variable_vah4(){
    if( vah4.empty() )
        return -1; 
    int t=*vah4.begin();
    vah4.erase(vah4.begin());
    return t; 
}
void insert_variable_vah4(int v){
    vah4.insert(v);
}
void erase_variable_vah4(int v){
    auto it=vah4.find(v);
    if(it!=vah4.end())
        vah4.erase(it) ;
}

int get_next_variable_vah5(){
    if(vah5.empty()) return -1;
    int t=*vah5.rbegin();
    vah5.pop_back();
    return t;
}
void restore_variable_vah5(int v){
    vah5.push_back(v);
}
void erase_variable_vah5(int v){
}
void insert_variable_vah5(int v){
}

int (*get_next_variable)();
void (*insert_variable)(int);
void (*erase_variable)(int);
void (*restore_variable)(int);
bool (*solver)(); 

bool backtrack(){
    int var=get_next_variable();
    if(var==-1) return goalcheck();

    int r=var/n,c=var%n;
    
    for(int k=1;k<=n;k++){
        if( (row[r][k]) || (col[c][k]) ) continue ;
        matrix[r][c]=k;
        row[r][k]++;
        col[c][k]++;
        bool res=backtrack();
        if(res) return true;
        row[r][k]--;
        col[c][k]--;

    }
    restore_variable(var);
    return false; 
}

bool forwardcheck(){
    int var=get_next_variable();
    if(var==-1 ) return goalcheck();

    int r=var/n,c=var%n;
    
    // cout<<"var: "<<var<<endl;
    // cout<<"Start Backtracking: "<<endl;
    // cout<<"size: "<<vah1.size()<<endl;
    // for(int x: vah1)
    //     cout<<x<<" "; cout<<endl;

    for(int i=0;i<n;i++){
        if( i!=c && !matrix[r][i] ){
            erase_variable(r*n+i);
            domain_size[r*n+i]--;
            degree[r*n+i]--;
        }
        if( i!=r && !matrix[i][c] ){
            erase_variable(i*n+c);
            domain_size[i*n+c]--;
            degree[i*n+c]--;
        }
    }
    for(int i=0;i<n;i++){
        if( i!=c && !matrix[r][i]  ){
            insert_variable(r*n+i);
        }
        if( i!=r && !matrix[i][c]){
            insert_variable(i*n+c);
        }
    }
    
    // cout<<"Before Backtracking: "<<endl;
    // cout<<"size: "<<vah1.size()<<endl;
    // for(int x:vah1)
    //     cout<<x<<" "; cout<<endl;


    for(int k=1;k<=n;k++){
        if( (row[r][k]) || (col[c][k]) ) continue ;
        matrix[r][c]=k;
        row[r][k]++;
        col[c][k]++;
        bool res=forwardcheck();
        if(res) return true;
        row[r][k]--;
        col[c][k]--;

    }
    matrix[r][c]=0;

    // cout<<"After Done Backtracking: "<<endl;
    // cout<<"size: "<<vah1.size()<<endl;
    // for(int x:vah1)
    //     cout<<x<<" "; cout<<endl;

    for(int i=0;i<n;i++){
        if( i!=c && !matrix[r][i] ){
            erase_variable(r*n+i);
            domain_size[r*n+i]++;
            degree[r*n+i]++;
        }
        if( i!=r && !matrix[i][c] ){
            erase_variable(i*n+c);
            domain_size[i*n+c]++;
            degree[i*n+c]++;
        }
    }
    for(int i=0;i<n;i++){
        if( i!=c && !matrix[r][i] && domain_size[r*n+i]>0 ){
            insert_variable(r*n+i);
        }
        if( i!=r && !matrix[i][c] && domain_size[i*n+c]>0 ){
            insert_variable(i*n+c);
        }
    }
    // cout<<"After Restoring: "<<endl;
    // cout<<"size: "<<vah1.size()<<endl;
    // for(int x:vah1)
    //     cout<<x<<" "; cout<<endl;
    
    restore_variable(var); 
    return false; 
}


void take_input(string filename){
    fstream fs(filename,ios_base::in);
    stringstream inputstream;
    inputstream<<fs.rdbuf();
    char c;
    string input; 
    while(inputstream>>c){
        if(isdigit(c)){
            input+=c;
        }
        else if(c==',')
            input+=' ';
        else if(c=='|')
            input+='\n';
    }
    cout<<input<<endl;

    inputstream=stringstream(input);

    inputstream>>n;
    // cout<<n<<endl;
    matrix=vector<vector<int>>(n,vector<int>(n));
    domain_size=vector<int>(n*n,0);
    degree=vector<int>(n*n,0);
    row=vector<vector<int>>(n,vector<int>(n+1,0));
    col=vector<vector<int>>(n,vector<int>(n+1,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            inputstream>>matrix[i][j];
        }
    }
}

int main(int argc,char **argv){
    if(argc<4)
        throw exception();
    
    if( string(argv[1]) == "bt" ){
        solver=backtrack;
    }
    else if( string(argv[1]) == "fc")
        solver=forwardcheck;
    else {

    }
    if( string(argv[2]) == "vah1" ){
        insert_variable=insert_variable_vah1;
        erase_variable=erase_variable_vah1;
        restore_variable=insert_variable_vah1;
        get_next_variable=get_next_variable_vah1;
    }
    else if( string(argv[2]) == "vah2" ){
        insert_variable=insert_variable_vah2;
        erase_variable=erase_variable_vah2;
        restore_variable=insert_variable_vah2;
        get_next_variable=get_next_variable_vah2;
    }
    else if( string(argv[2]) == "vah3" ){
        insert_variable=insert_variable_vah3;
        erase_variable=erase_variable_vah3;
        restore_variable=insert_variable_vah3;
        get_next_variable=get_next_variable_vah3;
    }
    else if( string(argv[2]) == "vah4" ){
        insert_variable=insert_variable_vah4;
        erase_variable=erase_variable_vah4;
        restore_variable=insert_variable_vah4;
        get_next_variable=get_next_variable_vah4;
    }
    else if( string(argv[2]) == "vah5" ){
        insert_variable=insert_variable_vah5;
        erase_variable=erase_variable_vah5;
        restore_variable=restore_variable_vah5;
        get_next_variable=get_next_variable_vah5;
    }
    else {
        throw exception() ;
    }

    take_input(argv[3]); 

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
    shuffle(vah5.begin(),vah5.end(),default_random_engine(3));
    
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
    cout<<"VAH5, size="<<vah5.size()<<endl;
    for(int x:vah5)
        cout<<x<<" "; cout<<endl;
    
    // get_next_variable=get_next_variable_vah1;
    // restore_variable=insert_variable_vah1;
    // insert_variable=insert_variable_vah1;
    // erase_variable=erase_variable_vah1;
    // bool found=backtrack();
    // cout<<"after forwardchecking: "<<endl;
    // cout<<(found?"result: true":"result: false")<<endl;
    // cout<<(goalcheck()?"goal: true":"goal: false")<<endl;
    // print_matrix(matrix);

    bool found=solver();
    cout<<(goalcheck()?"goal: true":"goal: false")<<endl;
    print_matrix(matrix);


}
