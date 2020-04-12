#ifndef ANALYSEUR_CPP
#define ANALYSEUR_CPP

#include "analyseur.h"
using namespace std;
inline string lower(const string& word){string word1;for(char c : word) word1.push_back(tolower(c));return word1; };

//Token
Token::Token(const string& w,const int s){ word=w; occ=s; }
Token::~Token(){}
void Token::increment() { occ++; }
string Token::getWord() const {return word; }
int Token::getOcc() const {return occ;}

//Occ
Occ::Occ(const string& d,const float& s){doc=d;stat=s;}
Occ::~Occ(){}
float Occ::getStat() const {return stat;}
string Occ::getDoc() const {return doc;}

//Word
Word::Word(const string& s){word=s;}
string Word::getWord() const {return word;}

//Request
Request::Request(const vector<Occ>& v,const int& t){request=v;total=t;}
Request::~Request(){}
int Request::size() const {return request.size();}

//Index
Index::Index(){}
Index::~Index(){}
int Index::size() const {return dict.size();}
Request Index::searc(const string& chaine,const int& max_) const {
    vector<Occ> result;
    string request=lower(chaine);
    for(pair<string,vector<Occ> > entry : dict)
        if(entry.first==request){
            result=entry.second;
            break;
        }
    sort(result.begin(),result.end(),[](const Occ& O1,const Occ& O2){return O1.getStat()>O2.getStat();});
    int total=result.size();
    while(result.size()>max_)
        result.pop_back();
    return Request(result,total);
}
void Index::add(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens)
    dict[token.getWord()].push_back(Occ(filename,token.getOcc()));
}

//Analyseur
Analyseur::Analyseur(const int& max){MAX_N=max;}
Analyseur::Analyseur(const vector<string>& fileList,const int& max){
    MAX_N=max;
    for(string file :fileList){
        vector<Token> words=analyser(file);
        indexer(words,file);
    }
}
Analyseur::~Analyseur(){}
int Analyseur::indexSize() const {return index.size();}
int Analyseur::size() const {return size_;}
void Analyseur::indexer(const vector<Token>& tokens,const string& filename) {index.add(tokens,filename); size_++;}
Request Analyseur::rechercher(const string& request){return index.searc(request,MAX_N);}
vector<Token> Analyseur::analyser(const string& filename){
    string word;
    vector<Token> tokens;
    ifstream file(filename);
    bool exists;
    while( file >> word ){
        word=lower(word);
        exists=false;
        for(int i=0;i<tokens.size();i++){
            if(tokens[i].getWord()==word){
                tokens[i].increment();
                exists=true;
                }
            }
        if(!exists)
                tokens.push_back(Token(word));
        }
    return tokens;
}


//ostream
ostream& operator<<(ostream& out,const Request& request){
    for(auto& item : request.request)
        out<<item.getStat()<<" "<<item.getDoc()<<endl;
    return out;
}
ostream& operator<<(ostream& out,const Index& index){
    for(pair<string,vector<Occ> > entry : index.dict){
        out<<entry.first<<endl;
        for(Occ word :entry.second)
        if(word.getStat()>5)
            out<<word.getStat()<<"- "<<word.getDoc()<<endl;
        out<<endl<<endl;
    }
    return out;
}
#endif