#ifndef MOTEUR_CPP
#define MOTEUR_CPP

#include "moteur.h"
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
vector<Occ>& Request::getReq(){return request;}
void Request::trier(){
    sort(request.begin(),request.end(),[](const Occ& O1,const Occ& O2){return O1.getStat()>O2.getStat();});
}

//Index
IndexUnorderedMap::IndexUnorderedMap(){}
IndexUnorderedMap::~IndexUnorderedMap(){}
int IndexUnorderedMap::size() const {return dict.size();}
vector<Occ> IndexUnorderedMap::operator[](const string& s){return dict[s];}
void IndexUnorderedMap::indexer(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens)
    dict[token.getWord()].push_back(Occ(filename,token.getOcc()));
}

//Analyseur
Analyseur::Analyseur(){}
Analyseur::~Analyseur(){}
vector<Token> Analyseur::analyser(const vector<string>& text){
    vector<Token> tokens;
    bool exists;
    for(string word : text){
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

//Lecteur
vector<string> Lecteur::readFile(const string& chemin){
    ifstream file(chemin);
    vector<string> text;
    string word;
    while( file >> word )
        text.push_back(word);
    return text;
}


//Moteur
template<class Index> Moteur<Index>::Moteur(){}
template<class Index> Moteur<Index>::~Moteur(){}
template<class Index> vector<Request> Moteur<Index>::rechercher(const vector<string>& request){
    vector<Request> results;
    for(string searchWord : request){
        searchWord=lower(searchWord);
        results.push_back(Request(index[searchWord]));
        results.back().trier();
    }
    return results;
}



//ostream
ostream& operator<<(ostream& out,const Request& request){
    for(auto& item : request.request)
        out<<item.getStat()<<" "<<item.getDoc()<<endl;
    return out;
}
ostream& operator<<(ostream& out,const IndexUnorderedMap& index){
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
