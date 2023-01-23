#include <bits/stdc++.h> 
using namespace std; 

// #define DEBUG 1
#define pb push_back
#define all(x) begin(x),end(x) 
mt19937 mt_rand(time(nullptr)) ;


int n_courses;
int n_student; 
vector<int> course_student_count ;
vector<vector<int>> course_adj_matrix, course_adj_list; 
vector<int> degree,sat_degree;
vector<int> color; 
vector<bool> marked ;

bool cmp_dsat(int i,int j){
    return (sat_degree[i]==sat_degree[j]) ? (degree[i]==degree[j]?i<j:degree[i]>degree[j]) : (sat_degree[i]>sat_degree[j]) ;
}
bool cmp_largest_degree(int i,int j){
    return (degree[i]==degree[j])?i<j:(degree[i]>degree[j]) ;
}
bool cmp_largest_enrollment(int i,int j){
    return (course_student_count[i]==course_student_count[j])?i<j:(course_student_count[i]>course_student_count[j]) ;
}


vector<int> va_largest_degree, va_random, va_largest_enrollment ; 
set<int, bool(*)(int,int)> va_dsat(cmp_dsat) ;

int get_next_var_largest_degree(){
    if( va_largest_degree.empty() ) return -1; 
    int var=*va_largest_degree.rbegin() ;
    va_largest_degree.pop_back() ;
    return var; 
}
void erase_var_largest_degree(int x){
}

int get_next_var_random() {
    if( va_random.empty() ) return -1; 
    int var=*va_random.rbegin(); 
    va_random.pop_back();
    return var; 
}
void erase_var_random(int x){}

int get_next_var_largest_enrollment(){
    if( va_random.empty() ) return -1; 
    int var=*va_random.rbegin(); 
    va_random.pop_back();
    return var; 
}
void erase_var_largest_enrollment(int x){}

int get_next_var_dsat(){
    if(va_dsat.empty() ) return -1; 
    int var=*va_dsat.begin() ;
    va_dsat.erase(va_dsat.begin()) ;
    return var; 
}
void erase_var_dsat(int x){
    if( va_dsat.empty()) throw exception() ;
    auto it=va_dsat.find(x) ;
    if( it != va_dsat.end() )
        va_dsat.erase(it) ;
}

bool dfs_check_consistency(int s){
    marked[s]=1;
    bool res=1;
    for(int v: course_adj_list[s] ){
        if( !marked[v] ){
            res&=dfs_check_consistency(v) ;
            res&=(color[v]!=color[s]) ;
        }
    }
    return res; 
}
bool check(){
    marked=vector<bool>(n_courses+1,0) ;
    bool res=1; 
    for(int u=1;u<=n_courses&&res;u++){
        if( !marked[u] ){
            res &= dfs_check_consistency(u) ;
        }
    }
    return res; 
}

int64_t get_penalty_linear(int diff) {
    if (diff <= 5) return 2*(5-diff);
    return 0 ;
}
int64_t get_penalty_exponential(int diff){
    if( diff > 5 ) return 0; 
    return 1<<(5-diff); 
}
int64_t (*get_penalty)(int); 

void take_input(string stu_file,string crs_file){
#ifdef DEBUG 
    cout<<stu_file<<endl; 
    cout<<crs_file<<endl;
#endif

    fstream stu_fs(stu_file,ios_base::in),crs_fs(crs_file,ios_base::in);

    int course_id, student_count ; 
    while(!crs_fs.eof()){
        crs_fs>>course_id;
        crs_fs>>student_count;
        while(course_id>=course_student_count.size()){
            course_student_count.push_back(0);
        }
        course_student_count[course_id]=student_count; 
    }
    crs_fs.close() ;

    n_courses=course_student_count.size()-1;
#ifdef DEBUG 
    cout<<"course_count: "<<course_student_count.size()<<endl;
#endif
    course_adj_matrix=vector<vector<int>> (course_student_count.size(),vector<int>(course_student_count.size(),0) ) ;
    course_adj_list=vector<vector<int>> (course_student_count.size()) ;
    degree=vector<int>(course_student_count.size(),0);
    sat_degree=vector<int>(course_student_count.size(),0);
    color=vector<int>(course_student_count.size(),-1);
    marked=vector<bool>(course_student_count.size(),0);
    
    string line;
    n_student=0;
    while( getline(stu_fs,line) ){
        n_student++;
        stringstream inputstream(line);
        vector<int> temp ;    
        int tmp ;
        while(inputstream>>tmp){
            temp.pb(tmp) ;
        }
        for(int i=0;i<temp.size();i++){
            for(int j=i+1;j<temp.size();j++){
                course_adj_matrix[temp[i]][temp[j]]++ ;
                course_adj_matrix[temp[j]][temp[i]]++ ;
                if( course_adj_matrix[temp[i]][temp[j]] == 1 )  course_adj_list[temp[i]].pb(temp[j]);
                if( course_adj_matrix[temp[j]][temp[i]] == 1 )  course_adj_list[temp[j]].pb(temp[i]) ;
                degree[temp[i]]++;
                degree[temp[j]]++;
            }
        }
    } 
    stu_fs.close() ;    

#ifdef DEBUG
    cout<<"Course Student Count: \n"; 
    for(int i=0;i<course_student_count.size();i++){
        cout<<i<<" "<<course_student_count[i]<<endl;
    }
    cout<<"Adjacancy Matrix: \n" ;
    for(int i=0;i<course_adj_matrix.size();i++) {
        for(int j=0;j<course_adj_matrix[i].size();j++)
            cout<<course_adj_matrix[i][j]<<" " ;
        cout<<endl;
    }
#endif 
}

void init() {
    for(int i=1;i<=n_courses;i++){
        va_dsat.insert(i) ;
        va_random.pb(i);
        va_largest_degree.pb(i);
        va_largest_enrollment.pb(i) ;
    }

    shuffle(va_random.begin(),va_random.end(),default_random_engine(5)) ;
    sort(va_largest_degree.begin(),va_largest_degree.end(), cmp_largest_degree) ;
    sort(va_largest_enrollment.begin(),va_largest_enrollment.end(), cmp_largest_enrollment) ; 
    reverse(all(va_largest_degree));
    reverse(all(va_largest_enrollment));

}

int total_colors=0;
enum {LARGEST_DEGREE, LARGEST_ENROLLMENT, RANDOM, DSAT } ;
int heuristic ;
int (*get_next_var)(void) ;

void local_search(){    
    int var ;
    while( (var=get_next_var()) != -1 ){
        marked[var]=1;
        set<int> color_set ;

        for(int v: course_adj_list[var]){
            if( color[v] == -1 ) continue ;

            color_set.insert(color[v]) ;
        }
        int col=0; 

        while( !color_set.empty() && *color_set.begin() == col ){
            col++ ;
            color_set.erase(color_set.begin());
        }
        color[var]=col ;
        total_colors=max(total_colors,col);

        if( heuristic == DSAT ){ // reorder 
            for(int v:course_adj_list[var]){
                if( !marked[v] ){
                    erase_var_dsat(v) ;
                    sat_degree[v]++ ;
                }
            }
            for(int v:course_adj_list[var]) {
                if( !marked[v] )    va_dsat.insert(v) ;
            }
        }
    }
}


int64_t dfs_calculate_penalty(int s){
    marked[s] = 1;
    int64_t pen=0;
    for(int v: course_adj_list[s] ){
        pen+= course_adj_matrix[s][v]*get_penalty(abs(color[s]-color[v])) ;
    }
    for(int v: course_adj_list[s] ){
        if( !marked[v] ){
            // pen+= course_adj_matrix[s][v]*get_penalty(abs(color[s]-color[v])) ;
            pen+=dfs_calculate_penalty(v) ;
        }
    }
    return pen; 
}
int64_t calculate_penalty(){
    marked=vector<bool>(n_courses+1,0) ;
    int64_t tot_pen = 0;
    for(int u=1;u<=n_courses;u++){
        if( !marked[u] ){
            tot_pen+=dfs_calculate_penalty(u) ;
        }
    }
    return tot_pen/2ll;
}

void dfs_get_kempe_chain(int s, int color1, int color2, vector<int> &chain){
    if( color1==color2 ) return;  
    if( color[s] != color1 && color[s] != color2 ) return ;
    chain.pb(s) ;
    marked[s] = 1; 

    for(int v: course_adj_list[s]){
        if(!marked[v] && color[v] != color[s] ){
            dfs_get_kempe_chain(v,color1,color2,chain) ;
        }
    } 
}
vector<int> get_kempe_chain(int s,int color1, int color2 ){
    vector<int> chain; 
    marked=vector<bool>(n_courses+1,0) ;
    dfs_get_kempe_chain(s,color1,color2,chain);
    return chain ;
}

void swap_colors(int u,int color1, int color2 ){
    if( color[u] == color1 ) color[u] =color2; 
    else if( color[u] == color2 ) color[u] =color1; 
}
int64_t kempe_chain_penalty_reduction(vector<int> &chain, int color1, int color2 ){
    if( chain.empty() ) return 0 ;

    marked=vector<bool>(n_courses+1,0) ;
    int64_t initial_penalty=0; 
    int64_t after_penalty=0 ; 
    for(int v: chain){
        for(int u: course_adj_list[v] ){
            if( !marked[u] ){
                initial_penalty+= get_penalty(abs(color[u]-color[v]))*course_adj_matrix[u][v] ;
                swap_colors(u,color1,color2) ;
                swap_colors(v,color1,color2) ;
                after_penalty+= get_penalty(abs(color[u]-color[v]))*course_adj_matrix[u][v] ;
                swap_colors(u,color1,color2) ;
                swap_colors(v,color1,color2) ;
            }
        }
        marked[v]=1;
    }
    return initial_penalty-after_penalty;
}
void kempe_chain_swap_colors(vector<int> &chain, int color1, int color2 ){
    for(int v: chain){
        swap_colors(v,color1,color2);
    } 
}
void kempe_chain_interchange(int max_iteration=1000){
    int u=-1,v=-1; 
    int color1,color2; 
    int64_t possible_reduction ;
    do{
        do{
            u=mt_rand()%n_courses+1;
        } while( !course_adj_list[u].size() ) ;
        color2=mt_rand()%total_colors; 

        vector<int> kempe_chain=get_kempe_chain(u,color[u],color2); 
        
        possible_reduction=kempe_chain_penalty_reduction(kempe_chain,color[u],color2);
        if( possible_reduction > 0 ) 
            kempe_chain_swap_colors(kempe_chain,color[u],color2) ;
        max_iteration--;
    } while( possible_reduction > 0 || max_iteration>0 ) ;
}

void pair_swap_operator(int max_iteration=1000){
    int u=-1,v=-1; 
    int64_t possible_reduction ;
    do{
        do{
            u=mt_rand()%n_courses+1;
            v=mt_rand()%n_courses+1;
        } while( u==v || course_adj_matrix[u][v] || color[u] == color[v] )  ;

        vector<int> chain1,chain2 ;
        chain1=get_kempe_chain(u,color[u],color[v]) ;
        if( marked[v] ) continue ;
        chain2=get_kempe_chain(v,color[v],color[u]) ; 

        possible_reduction=kempe_chain_penalty_reduction(chain1,color[u],color[v])
                          +kempe_chain_penalty_reduction(chain2,color[v],color[u]) ;
        if( possible_reduction>0) {
            int col1=color[u];
            int col2=color[v];
            kempe_chain_swap_colors(chain1,col1,col2) ;
            kempe_chain_swap_colors(chain2,col1,col2) ;
        }
        max_iteration--;
    } while( possible_reduction>0 || max_iteration>0 ) ;
}


int main(int argc,char **argv) {
    if(argc < 2 ) {
        cout<<"usage: solver [heuristic] [penalty_strategy] [max_iteration] [filename]\n" ;
        cout<<" heuristic: \t ld=largest_degree \n"; 
        cout<<"            \t le=largest_enrollment \n"; 
        cout<<"            \t dsat=dsatur \n"; 
        cout<<"            \t rnd=random \n"; 
        cout<<" penalty strategy : \t lin=linear strategy \n"; 
        cout<<"                    \t exp=exponential strategy  \n"; 
        return -1; 
    }
    int argid=1;
    int mx_iteration; 
    if( string(argv[argid]) == "ld" ){
        get_next_var=get_next_var_largest_degree;
        heuristic=LARGEST_DEGREE;
        argid++ ;
    }
    else if( string(argv[argid]) == "le" ){
        get_next_var=get_next_var_largest_enrollment;
        heuristic=LARGEST_ENROLLMENT; 
        argid++ ;
    }
    else if( string(argv[argid]) == "rnd" ){
        get_next_var=get_next_var_random;
        heuristic=RANDOM;
        argid++ ;
    }
    else {   
        get_next_var=get_next_var_dsat ;
        heuristic=DSAT ;
        if( argc > 2 ) argid++ ;
    }

    if( string(argv[argid]) == "lin"){
        get_penalty=get_penalty_linear;
        argid++;
    }
    else if( string(argv[argid])== "exp" ){
        get_penalty=get_penalty_exponential;
        argid++;
    }
    else {
        get_penalty=get_penalty_exponential;
    }

    if( isdigit(argv[argid][0]) ){
        mx_iteration=atoi(argv[argid++]);
    }
    else mx_iteration=1000 ;

    string filename=string(argv[argid]) ;
    take_input(filename+".stu",filename+".crs")  ;

    init() ;
#ifdef DEBUG 
    cout<<"dsat("<<va_dsat.size()<<")"<<": "; 
    for(int x: va_dsat)
        cout<<x<<" "; cout<<endl;

    cout<<"random("<<va_random.size()<<")"<<": "; 
    for(int x: va_random)
        cout<<x<<" "; cout<<endl;

    cout<<"largest_degree("<<va_largest_degree.size()<<")"<<": " ;
    for(int x: va_largest_degree)
        cout<<x<<" " ; cout<<endl; 
    
    cout<<"largest_enrollment("<<va_largest_enrollment.size()<<")"<<": " ;
    for(int x: va_largest_enrollment) 
        cout<<x<<" " ; cout<<endl ;

#endif

    // local_search_dsat() ;
    // local_search_largest_degree();
    // local_search_largest_enrollment() ;
    local_search() ;
    int64_t tot_penalty;
    double avg_penalty;

    cout<<"coloring: "<<(check()?"true":"false")<<endl;
    cout<<"total timeslots: "<<total_colors+1<<endl;
    // for(int i=1;i<=n_courses;i++)   
    //     cout<<i<<" "<<color[i]<<endl; 

    tot_penalty=calculate_penalty() ;
    cout<<"tot_penalty: "<<tot_penalty<<endl;
    avg_penalty=double(tot_penalty)/double(n_student) ;
    cout<<"student_count: " <<n_student<<endl;
    cout<<"avg_penalty : "<<avg_penalty<<endl;

    kempe_chain_interchange(mx_iteration);
    cout<<"coloring: "<<(check()?"true":"false")<<endl;
    cout<<"After kempe chain interchange: \n";
    tot_penalty=calculate_penalty() ;
    cout<<"tot_penalty: "<<tot_penalty<<endl;
    avg_penalty=double(tot_penalty)/double(n_student) ;
    cout<<"student_count: " <<n_student<<endl;
    cout<<"avg_penalty : "<<avg_penalty<<endl;

    pair_swap_operator(mx_iteration);
    cout<<"coloring: "<<(check()?"true":"false")<<endl;
    cout<<"After pair swap operator: \n";
    tot_penalty=calculate_penalty() ;
    cout<<"tot_penalty: "<<tot_penalty<<endl;
    avg_penalty=double(tot_penalty)/double(n_student) ;
    cout<<"student_count: " <<n_student<<endl;
    cout<<"avg_penalty : "<<avg_penalty<<endl;
}