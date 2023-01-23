#include <bits/stdc++.h>
using namespace std ;

int main(){
    fstream in("car-f-92.stu",ios_base::in) ;

    string line;

    while( getline(in,line)){
        cout<<line<<endl;
        stringstream inputstream(line);
        vector<int> temp ;    
        int tmp ;
        while(inputstream>>tmp){
            temp.push_back(tmp) ;
            cout<<tmp<<" ";
        }
        cout<<endl;
    }
}