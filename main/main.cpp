#include "moteur.h"
#include "moteur.cpp"
#include <chrono>

// ./mp.sh "/home/ahmed2/Desktop/corpus/petit"

using namespace std;

int main(int argc, char** argv){
    auto start = std::chrono::high_resolution_clock::now();
    if(argc!=4) // ./main PWD
        return 1; 
    // lire les arguments de main et decider l'action de main
    int state=-1;
    vector<string> arguments;
    for(int i=0;i<argc;i++) arguments.push_back((string) argv[i]);
    
    string PWD=arguments[1];

    if(arguments[2]=="new") // ./main PWD new corpus_directory
        state=1; 
    
    else if(arguments[2]=="import") // ./main PWD import file_to_import_from
        state=2;
    
    else if(arguments[2]=="add") // ./main PWD add fileToAdd
        state=8;
    else if(arguments[2]=="search") // ./main PWD search query
        state=9;
    
    else return 1;

    // declaring variables
    Index_* index=0;
    Analyseur_* analyseur=0;
    Moteur* moteur=0;
    Lecteur lecteur;
    
    string typeIndex;
    string typeAnalyseur;
    ifstream fi(PWD+"/moteur.txt");
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


    // state 1
    // main PWD new corpus_directory
    // construire un index a partir d'un corpus
    // puis enregistrer les changements et quitter
    if(state==1){
        //construire une liste des fichiers a indexer
        string filename;
        set<string> fileList;
        ifstream ff(PWD+"/fileList.txt");
        while( ff >> filename) fileList.insert(arguments[3]+"/"+filename);

        cout<<"indexing..."<<endl;

        for(string chemin : fileList){
            vector<string> texte=lecteur.readFile(chemin);
            vector<Token> tokens=moteur->analyseur->analyser(texte);
            moteur->index->indexer(tokens,chemin);
        }
        //on confirme qu'on a bien construit un index
        cout<<"engine has a total of "<<moteur->index->size()<<" words"<<endl;
        cout<<"and "<<moteur->index->getNumberOfDocuments()<<" files."<<endl;

        // on exporte le moteur pour le reutiliser
        cout<<"saving changes..."<<endl;
        lecteur.exportMoteur(moteur,PWD+"/moteur.txt");
        cout<<"successfully saved changes to the engine"<<endl;

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
        return 0;
    }


    // state2
    // main PWD import file_to_import_from
    // importer un moteur depuis un fichier 
    // puis enregistrer les changements et quitter
    if(state==2){ 

        cout<<"importing .."<<endl;
        lecteur.importIndex(moteur->index,arguments[3]);
        
        //on confirme qu'on a bien importer un moteur
        cout<<"The index has "<<moteur->index->getNumberOfDocuments()<<" documents."<<endl;
        
        cout<<"saving changes..."<<endl;
        lecteur.exportMoteur(moteur,PWD+"/moteur.txt");
        cout<<"successfully exported the index!"<<endl;

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
        
        return 0;
    }

    // d'ici on importe automatiquement le moteur 
    cout<<"loading engine .."<<endl;
    lecteur.importIndex(moteur->index,arguments[1]+"/moteur.txt");
    //on confirme qu'on a bien importer un moteur
    cout<<"The engine has "<<moteur->index->getNumberOfDocuments()<<" documents."<<endl;


    // state 8
    // main PWD add fileToAdd
    // ajouter un fichier a l'index
    if(state==8){
        //... a travailler
        return 0;
    }


    // state 9
    // main PWD search query
    // gerer une requete
    if(state==9){
        vector<string> searchWords=lecteur.parseQuery(arguments[3]);
        vector<Recherche> recherche=moteur->rechercher(searchWords);
        for(auto& i : recherche) cout<<"resultat pour "<<endl<<i<<endl;

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";

        return 0;
    }
}