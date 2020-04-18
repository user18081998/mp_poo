#ifndef MOTEUR_H
#define MOTEUR_H

#include<iostream>
#include<fstream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

class Token{
    /* les tokens servent a traiter les donnees brutes
       et les convertir en des structures avec lesquelles
       on peut travailler */
    string word;
    int occ;
    public:
    Token(const string& w,const int s=1);
    ~Token();
    void increment();
    string getWord() const;
    int getOcc() const;
};
class Occ{
    string doc;
    float stat;
    public:
    Occ(const string& d,const float& s);    
    ~Occ();
        
    float getStat() const;
    string getDoc() const;
};
class Word{
    string word;
    public:
    Word(const string& s);
    string getWord() const;
};
class Request{
    vector<Occ> request;
    public :
    Request(const vector<Occ>& v,const int& t=0);
    ~Request();

    int total; // nombre total de resultats
    int size() const ; // nombre reel de resultats (size<=total comme on a une contrainte sur le nombre de resultats : MAX_N nombre total de resultats a afficher, voir class analyseur, voir index.searc)

    friend ostream& operator<<(ostream& out,const Request& request);
};
class Index{
    unordered_map<string, vector<Occ> > dict;

    public:
    Index();
    ~Index();

    int size() const ; //nombre de mots indexes

    void indexer(const vector<Token>& tokens,const string& filename); // ajout dans l'index

    friend ostream& operator<<(ostream& out,const Index& index);
};
class Analyseur{
    Index index;
    int MAX_N; // nombre maximum de resultats pour les requetes que l'analyseur affiche 
    int size_; // nombre de fichiers dans l'analyseur
    vector<Token> analyser(const string& filename);
        /* analyser est une fonction qui accepte le nom d'un fichier
       elle doit retourner un vecteur de tokens qu'on degage du fichier
       un token est le pair {mot,n} 
       avec n le nombre d'occurences du mot dans le fichier */
    
    public:
    Analyseur(const int& max=5);
    Analyseur(const vector<string>& fileList,const int& max=5);
    ~Analyseur();
    
    // la seule "raison d'etre" de ces deux methodes est d'afficher des statistiques concernant l'analyseur
    int size() const ; //nombre de fichiers 
    int indexSize() const ; //nombre de mots

    
};
class Lecteur{
    public :
    vector<string> readFile(const string& chemin);
};
class Ordonnanceur{
    public:
    void trier(const Request& request);
};
class Moteur{
    Index index;
    Analyseur analyseur;
    Lecteur lecteur;
    Ordonnanceur ordonnanceur;
    public :
    Request rechercher(const vector<string>& request);
    void indexer(const vector<Token>& tokens,const string& chemin); 
    // == index.indexer(const vector<Token>& tokens,const string& chemin)
};
#endif
