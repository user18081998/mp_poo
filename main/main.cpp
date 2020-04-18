#include "moteur.h"
#include "moteur.cpp"
using namespace std;
int main(){
    string item;
    vector<string> fileList;
    ifstream ff("/home/ahmed2/Desktop/cpp/mp/fileList.txt");
    while( ff >> item) fileList.push_back("/home/ahmed2/Desktop/cpp/mp/petit_corpus/"+item);

    Moteur<IndexUnorderedMap> moteur;

    for(string& chemin : fileList)
        moteur.index.indexer(moteur.analyseur.analyser(moteur.lecteur.readFile(chemin)),chemin);
    
    while(true){
    vector<string> searchWords;
    cout<<"entrer mot a rechercher : "<<endl;
    string word;
    cin>>word;
    searchWords.push_back(word);
    vector<Request> recherche=moteur.rechercher(searchWords);
    for(auto& i : recherche) cout<<i;
    }

    return 0;
}
