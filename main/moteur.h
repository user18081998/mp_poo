#ifndef MOTEUR_H
#define MOTEUR_H

#include<iostream>
#include<fstream>
#include<map>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<string>
#include<set>
#include<cmath>

using namespace std;

class Token{
    string word;
    float occ;
    public:
    Token(const string& w,const float s=1);
    ~Token();
    void increment();
    string getWord() const;
    float getOcc() const;
    void setOcc(const float occ);
};
class Occ{
    string doc;
    float stat;
    public:
    Occ(const string& d,const float& s);    
    ~Occ();
        
    float getStat() const;
    string getDoc() const;
    friend bool operator<(const Occ& O1,const Occ& O2);
    friend bool operator>(const Occ& O1,const Occ& O2);
    
};
class Recherche{
    vector<Occ> recherche;
    int MAX_N;
    public :
    Recherche(const vector<Occ>& v,const int& t=5);
    ~Recherche();
    void trier();
    void trierIDF();

    int size() const ; // nombre reel de resultats (size<=total comme on a une contrainte sur le nombre de resultats : MAX_N nombre total de resultats a afficher, voir class analyseur, voir index.searc)

    vector<Occ>& getReq();

    friend ostream& operator<<(ostream& out,const Recherche& recherche);
};

class Index_{
    public :
    virtual int size() const =0;
    virtual int getNumberOfDocuments() const=0;
    virtual void indexer(const vector<Token>& tokens,const string& filename) =0;
    virtual vector<Occ> operator[](const string& s) =0;
};
class IndexUnorderedMap : public Index_ {
    unordered_map<string, vector<Occ> > dict;
    int numberOfDocuments=0;
    public:
    IndexUnorderedMap();
    ~IndexUnorderedMap();
    int size() const ;
    int getNumberOfDocuments() const ;
    void indexer(const vector<Token>& tokens,const string& filename);
    vector<Occ> operator[](const string& s);

    friend ostream& operator<<(ostream& out,const IndexUnorderedMap& index);
};
class IndexSet : public Index_ {
    set<pair<string,vector<Occ> > > dict;
    int numberOfDocuments=0;
    public:
    IndexSet();
    ~IndexSet();
    int size() const;
    int getNumberOfDocuments() const ;
    void indexer(const vector<Token>& tokens,const string& filename);
    vector<Occ> operator[](const string& s);
};
class IndexMap : public Index_{
    map<string,vector<Occ> > dict;
    int numberOfDocuments=0;
    public:
    IndexMap();
    ~IndexMap();
    int size() const;
    int getNumberOfDocuments() const;
    void indexer(const vector<Token>& tokens,const string& filename);
    vector<Occ> operator[](const string& s);
};


class Analyseur_{
    virtual vector<Token> analyser(const vector<string>& text) =0;
};
class AnalyseurBinary : public Analyseur_{
    AnalyseurBinary();
    ~AnalyseurBinary();

    vector<Token> analyser(const vector<string>& text);    
};
class AnalyseurWF : public Analyseur_{ // word frquency
    public:
    AnalyseurWF();
    ~AnalyseurWF();

    vector<Token> analyser(const vector<string>& text);
};
class AnalyseurATF : public Analyseur_{ // augmented term frequency
    public:
    AnalyseurATF();
    ~AnalyseurATF();

    vector<Token> analyser(const vector<string>& text);
};

class Lecteur{
    public :
    vector<string> readFile(const string& chemin);
};

template<class Index,class Analyseur> class Moteur{
    int MAX_N;
    public :
    Moteur(const int m=5);
    ~Moteur();
    Index index;
    Analyseur analyseur;
    Lecteur lecteur;
    vector<Recherche> rechercher(const vector<string>& recherche);
};
#endif
