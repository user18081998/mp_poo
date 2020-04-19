#include "moteur.h"
#include "moteur.cpp"
using namespace std;
int main(int argc, char** argv){
    if(argc>3)
        return 1;
    // on extrait une liste de fichier a tester avec
    string filename;
    vector<string> fileList;
    string temp=(argv[1]);
    ifstream ff(temp+"/temp.txt");
    while( ff >> filename) fileList.push_back(string(argv[2])+"/"+filename);
    
    cout<<"On commence l'indexation"<<endl;
    Moteur<IndexUnorderedMap,AnalyseurWF> moteur;
    for(string& chemin : fileList){
        vector<string> texte=moteur.lecteur.readFile(chemin);
        vector<Token> tokens=moteur.analyseur.analyser(texte);
        moteur.index.indexer(tokens,chemin);
    }
    // for some reason this works way faster than this :
    // moteur.index.indexer(moteur.analyseur.analyser(texte),moteur.lecteur.readFile(chemin));
    cout<<"index has a total of "<<moteur.index.size()<<" words."<<endl;
    cout<<"and "<<moteur.index.getNumberOfDocuments()<<" files indexed."<<endl;

    while(true){
    vector<string> searchWords;
    cout<<"Enter a word to search [^C to quit] : ";
    string word;
    cin>>word;
    searchWords.push_back(word);
    vector<Recherche> recherche=moteur.rechercher(searchWords);
    for(auto& i : recherche) cout<<i;
    }

    return 0;
}