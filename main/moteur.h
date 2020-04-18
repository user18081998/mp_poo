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
    void trier();

    int total; // nombre total de resultats
    int size() const ; // nombre reel de resultats (size<=total comme on a une contrainte sur le nombre de resultats : MAX_N nombre total de resultats a afficher, voir class analyseur, voir index.searc)

    vector<Occ>& getReq();

    friend ostream& operator<<(ostream& out,const Request& request);
};
class IndexUnorderedMap{
    unordered_map<string, vector<Occ> > dict;

    public:
    IndexUnorderedMap();
    ~IndexUnorderedMap();

    int size() const ; //nombre de mots indexes

    void indexer(const vector<Token>& tokens,const string& filename); // ajout dans l'index
    vector<Occ> operator[](const string& s);
    friend ostream& operator<<(ostream& out,const IndexUnorderedMap& index);
};
class IndexSet{
    //...
};
class Analyseur{
    public:
    Analyseur();
    ~Analyseur();

    vector<Token> analyser(const vector<string>& text);

};
class Lecteur{
    public :
    vector<string> readFile(const string& chemin);
};
template<class Index> class Moteur{
    public :
    Moteur();
    ~Moteur();
    Index index;
    Analyseur analyseur;
    Lecteur lecteur;
    vector<Request> rechercher(const vector<string>& request);

};
#endif
