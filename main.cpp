#include "analyseur.h"
#include "analyseur.cpp"
using namespace std;
int main(){
    // on extrait une liste de fichier a tester avec
    string item;
    vector<string> fileList;
    ifstream ff("/home/ahmed2/Desktop/cpp/mp/fileList.txt");
    while( ff >> item) fileList.push_back("/home/ahmed2/Desktop/cpp/mp/petit_corpus/"+item);

    // on commence l'indexation 
    Analyseur analyseur(fileList,10);
    cout<<"Finished analyzing!"<<endl;
    string searchWord="pizza";
    // l'utilisateur peut rechercher
    while(true) {
    cout<<"Enter a word to search : ";
    cin>>searchWord;
    cout<<"Search results for : "<<searchWord<<endl;
    Request request=analyseur.rechercher(searchWord);
    cout<<request<<endl<<"found "<<request.size()<<" out of "<<fileList.size()<<" items lmao "<<endl;
    }
    return 0;
}