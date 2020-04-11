#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include<iostream>
#include<fstream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

class Token{
    string word;
    int occ;
    public:
    Token(string w,int s=1);
    ~Token();
    void increment();
    string getWord() const;
    int getOcc() const;
};
class Occ{
    string doc;
    float stat;
    public:
    Occ(string d,float s);    
    ~Occ();
        
    float getStat() const;
    string getDoc() const;
};
class Word{
    string word;
    public:
    Word(string s);
    string getWord() const;
};
class Request{
    vector<Occ> request;
    public :
    Request(const vector<Occ>& v);
    ~Request();

    int size() const;
    friend ostream& operator<<(ostream& out,const Request& request);
};
class Index{
    unordered_map<string, vector<Occ> > dict;

    public:
    Index();
    ~Index();

    Request searc(const string& request,const int& max_) const;
    void add(const vector<Token>& tokens,const string& filename);

    friend ostream& operator<<(ostream& out,const Index& index);
};
class Analyseur{
    Index index;
    int MAX_N;
    vector<Token> analyser(string filename);
    public:
    Analyseur(int max=5);
    Analyseur(vector<string> fileList,int max=5);
    ~Analyseur();

    void indexer(const vector<Token>& tokens,const string& filename);
    Request rechercher(const string& request);
};
#endif