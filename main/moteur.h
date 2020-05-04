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
    void setStat(const float& value);
    friend bool operator<(const Occ& O1,const Occ& O2);
    friend bool operator>(const Occ& O1,const Occ& O2);
    
};
class Recherche{
    vector<Occ> recherche;
    int MAX_N;
    public :
    Recherche(const vector<Occ>& v,const int& t=5);
    ~Recherche();
    void trier(); // sort vector terms by coefficient from greatest to smallest
    void updateWithIDF(const int& numberOfDocuments); // multiply the coefficients in the vector with a significant constant

    int size() const ; // nombre reel de resultats (size<=total comme on a une contrainte sur le nombre de resultats : MAX_N nombre total de resultats a afficher, voir class analyseur, voir index.searc)

    vector<Occ>& getReq();

    friend ostream& operator<<(ostream& out,const Recherche& recherche);
};

class Index_{
    protected :
    virtual int setNumberOfDocuments(const int& i)=0;
    public :
    virtual ~Index_()=0;
    string type;
    virtual int size() const =0;
    virtual int getNumberOfDocuments() const=0;
    virtual void setDict(const unordered_map<string, vector<Occ> >& map)=0;
    virtual void indexer(const vector<Token>& tokens,const string& filename) =0;
    virtual vector<Occ> operator[](const string& s) =0;

    friend class Lecteur;
};

class IndexUnorderedMap : public Index_ {
    unordered_map<string, vector<Occ> > dict;
    int numberOfDocuments=0;
    protected :
    int setNumberOfDocuments(const int& i) override;
    void setDict(const unordered_map<string, vector<Occ> >& d) override;
    public:
    IndexUnorderedMap();
    IndexUnorderedMap(const unordered_map<string, vector<Occ> >& s,const int& i=0);
    ~IndexUnorderedMap() override;
    IndexUnorderedMap& operator=(const IndexUnorderedMap& index);
    int size() const override;
    int getNumberOfDocuments() const override;
    void indexer(const vector<Token>& tokens,const string& filename) override;
    vector<Occ> operator[](const string& s) override;

    friend class Lecteur;
    friend ofstream& operator<<(ofstream& out,const IndexUnorderedMap& index);
    friend ostream& operator<<(ostream& out,const IndexUnorderedMap& index);
};

class IndexSet : public Index_ {
    set<pair<string,vector<Occ> > > dict;
    int numberOfDocuments=0;
    protected :
    int setNumberOfDocuments(const int& i) override;
    void setDict(const unordered_map<string, vector<Occ> >& d) override;
    public:
    IndexSet();
    IndexSet(const set<pair<string,vector<Occ> > >& s, const int& n);
    ~IndexSet();
    int size() const override;
    int getNumberOfDocuments() const override;
    void indexer(const vector<Token>& tokens,const string& filename) override;
    vector<Occ> operator[](const string& s) override;

    friend class Lecteur;
    friend ofstream& operator<<(ofstream& out,const IndexSet& index);
};
class IndexMap : public Index_{
    map<string,vector<Occ> > dict;
    int numberOfDocuments=0;
    protected :
    int setNumberOfDocuments(const int& i) override;
    void setDict(const unordered_map<string, vector<Occ> >& d) override;
    public:
    IndexMap();
    IndexMap(const map<string,vector<Occ> > s, const int& n);
    ~IndexMap();
    int size() const override;
    int getNumberOfDocuments() const override;
    void indexer(const vector<Token>& tokens,const string& filename) override;
    vector<Occ> operator[](const string& s) override;

    friend class Lecteur;
    friend ofstream& operator<<(ofstream& out,const IndexMap& index);
};


class Analyseur_{
    public :
    string type;
    virtual vector<Token> analyser(const vector<string>& text) =0;
};
class AnalyseurBinary : public Analyseur_{
    public :
    AnalyseurBinary();
    ~AnalyseurBinary();

    vector<Token> analyser(const vector<string>& text) override;    
};
class AnalyseurWF : public Analyseur_{ // word frquency
    public:
    AnalyseurWF();
    ~AnalyseurWF();

    vector<Token> analyser(const vector<string>& text) override;
};
class AnalyseurATF : public Analyseur_{ // augmented term frequency
    public:
    AnalyseurATF();
    ~AnalyseurATF();

    vector<Token> analyser(const vector<string>& text) override;
};
class Moteur{
    int MAX_N;
    public :
    Index_* index;
    Analyseur_* analyseur;
    Moteur(Index_* index, Analyseur_* analyseur, const int m=5);
    Moteur();
    ~Moteur();
    vector<Recherche> rechercher(const vector<string>& recherche);
};

class Lecteur{
    public :
    vector<string> readFile(const string& chemin);
    void importIndex(Index_* index,const string& chemin);
    // void exportIndex(Index_* index,const string& chemin);
    void exportIndex(Index_* index,const string& chemin);
    void exportMoteur(Moteur* moteur,const string& chemin);
    vector<string> parseQuery(const string& query);
};
/*
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
*/
#endif