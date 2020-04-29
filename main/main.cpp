#include "moteur.h"
#include "moteur.cpp"

// ./mp.sh "/home/ubuntu/Desktop/corpus/petit"

using namespace std;

int main(int argc, char** argv){
    vector<string> arguments;
    for(int i=0;i<argc;i++) arguments.push_back((string) argv[i]);

    
    // declaring variables
    Index_* index=0;
    Analyseur_* analyseur=0;
    Moteur* moteur=0;
    Lecteur lecteur;
    
    string typeIndex;
    string typeAnalyseur;
    ifstream fi("/home/ubuntu/Desktop/cpp/mp/main/parser/index1.txt");
    fi>>typeIndex;
    fi>>typeAnalyseur;
    fi.close();
    if(typeIndex=="IndexUnorderedMap") index = new IndexUnorderedMap;
    else if(typeIndex=="IndexMap") index = new IndexMap;
    else if(typeIndex=="IndexSet") index = new IndexSet;
    else return 2;
    if(typeAnalyseur=="AnalyseurATF") analyseur = new AnalyseurATF;
    else if(typeAnalyseur=="AnalyseurBinary") analyseur = new AnalyseurBinary;
    else if(typeAnalyseur=="AnalyseurWF") analyseur = new AnalyseurWF;
    else return 2;
    moteur = new Moteur(index,analyseur);


    if(argc<=1){ // importer un moteur
        cout<<"importing .."<<endl;
        lecteur.importIndex(moteur->index,"/home/ubuntu/Desktop/cpp/mp/main/parser/index1.txt");
        
        //on confirme qu'on a bien importer un moteur
        cout<<"The index has "<<index->getNumberOfDocuments()<<" documents."<<endl;
    }

    if(argc==3){ // construire un index a partir des arguments fournis
    
        //construire une liste des fichiers a indexer
        string filename;
        vector<string> fileList;
        string temp=arguments[1];
        ifstream ff(temp+"/temp.txt");
        while( ff >> filename) fileList.push_back(arguments[2]+"/"+filename);

        cout<<"On commence l'indexation"<<endl;

        for(string& chemin : fileList){
            vector<string> texte=lecteur.readFile(chemin);
            vector<Token> tokens=moteur->analyseur->analyser(texte);
            moteur->index->indexer(tokens,chemin);
        }
        //on confirme qu'on a bien construit un index
        cout<<"index has a total of "<<moteur->index->size()<<" words."<<endl;
        cout<<"and "<<moteur->index->getNumberOfDocuments()<<" files indexed."<<endl;

        // on exporte le moteur pour le reutiliser
        cout<<"exporting index..."<<endl;
        lecteur.exportMoteur(moteur,"/home/ubuntu/Desktop/cpp/mp/main/parser/index1.txt");
        cout<<"successfully exported the index!"<<endl;
    }
    while(true){
    vector<string> searchWords;
    cout<<"Enter a word to search [^C to quit] : ";
    string word;
    cin>>word;
    searchWords.push_back(word);
    vector<Recherche> recherche=moteur->rechercher(searchWords);
    for(auto& i : recherche) cout<<i;
    }
}