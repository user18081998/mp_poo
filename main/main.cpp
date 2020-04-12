#include "analyseur.h"
#include "analyseur.cpp"
using namespace std;
int main(int argc, char** argv){
    // argv[1] = $pwd
    // argv[2] = chemin corpus
    if(argc!=3)
        return 1;
    // on extrait une liste de fichier a tester avec
    string filename;
    vector<string> fileList;
    string temp=(argv[1]);
    ifstream ff(temp+"/temp.txt");
    while( ff >> filename) fileList.push_back(string(argv[2])+"/"+filename);

    cout<<"On commence l'indexation"<<endl; 
    Analyseur analyseur(fileList,7);
    cout<<"Finished analyzing!"<<endl;
    cout<<"index has a total of "<<analyseur.indexSize()<<" words."<<endl
        <<"and "<<analyseur.size()<<" files indexed."<<endl;

    string searchWord="pizza";
    // l'utilisateur peut faire des requetes
    
    while(true) {
    cout<<endl<<"Enter a word to search [^C to quit] : ";
    cin>>searchWord;
    cout<<"Search results for : "<<searchWord<<endl;
    cout<<"......"<<endl;
    Request request=analyseur.rechercher(searchWord);
    if(request.size()==0) 
        cout<<"found no results for "<<searchWord<<endl;
    else 
        cout<<request<<endl<<"displaying "<<request.size()
        <<" out of "<<request.total<<" results. "
        <<endl;
    }

    return 0;
}
