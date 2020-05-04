#ifndef MOTEUR_CPP
#define MOTEUR_CPP

#include "moteur.h"

using namespace std;
inline string lower(const string& word){string word1;for(char c : word) word1.push_back(tolower(c));return word1; };

//Token
Token::Token(const string& w,const float s){ word=w; occ=s; }
Token::~Token(){}
void Token::increment() { occ++; }
string Token::getWord() const {return word; }
float Token::getOcc() const {return occ;}
void Token::setOcc(const float value){occ=value;}

//Occ
Occ::Occ(const string& d,const float& s){doc=d;stat=s;}
Occ::~Occ(){}
float Occ::getStat() const {return stat;}
string Occ::getDoc() const {return doc;}
void Occ::setStat(const float& value) {stat=value;}
bool operator<(const Occ& O1,const Occ& O2){return O1.getStat()<O2.getStat();}
bool operator>(const Occ& O1,const Occ& O2){return O1.getStat()>O2.getStat();}

//Recherche
Recherche::Recherche(const vector<Occ>& v,const int& t){recherche=v;MAX_N=t;}
Recherche::~Recherche(){}
int Recherche::size() const {return recherche.size();}
vector<Occ>& Recherche::getReq(){return recherche;}
void Recherche::trier(){
    sort(recherche.begin(),recherche.end(),greater<Occ>());
    while(recherche.size()>MAX_N)
        recherche.pop_back();
}
void Recherche::updateWithIDF(const int& numberOfDocuments){ 
    // im going to adapt the standard notations the ones i found on wikipedia
    int N=numberOfDocuments;
    float nt=size();
    float idf=log((N-nt)/nt); // probabilistic inverse document frequency
    // float idf=log(N/(1+nt))+1; // inverse document frequency
    for(auto& entry : recherche)
        entry.setStat(entry.getStat()*idf);
}

//Index
Index_::~Index_(){}
// unordered_map
IndexUnorderedMap::IndexUnorderedMap(){
    numberOfDocuments=0;
    type="IndexUnorderedMap";
}
IndexUnorderedMap::IndexUnorderedMap(const unordered_map<string, vector<Occ> >& s,const int& n){
    dict=s;
    type="IndexUnorderedMap";
    numberOfDocuments=n;
}
IndexUnorderedMap::~IndexUnorderedMap(){}
IndexUnorderedMap& IndexUnorderedMap::operator=(const IndexUnorderedMap& index){
    dict=index.dict;
    numberOfDocuments=index.numberOfDocuments;
    return *this;
}
int IndexUnorderedMap::size() const {return dict.size();}
int IndexUnorderedMap::getNumberOfDocuments() const {return numberOfDocuments;}
int IndexUnorderedMap::setNumberOfDocuments(const int& i){numberOfDocuments=i;}
void IndexUnorderedMap::setDict(const unordered_map<string, vector<Occ> >& d){dict=d;}
vector<Occ> IndexUnorderedMap::operator[](const string& s){return dict[s];}
void IndexUnorderedMap::indexer(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens){
        dict[token.getWord()].push_back(Occ(filename,token.getOcc()));
    }
    numberOfDocuments++;
}

//unordered_map improved



//map
IndexMap::IndexMap(){type="IndexMap";numberOfDocuments=0;}
IndexMap::IndexMap(const map<string,vector<Occ> > s, const int& n){
    dict=s;
    numberOfDocuments=n;
    type="IndexMap";
}
IndexMap::~IndexMap(){}
int IndexMap::size() const {return dict.size();}
int IndexMap::getNumberOfDocuments() const {return numberOfDocuments;}
int IndexMap::setNumberOfDocuments(const int& i){numberOfDocuments=i;}
void IndexMap::setDict(const unordered_map<string, vector<Occ> >& d){
    map<string,vector<Occ> > newDict;
    for(auto& entry : d)
        newDict[entry.first]=entry.second;
    dict=newDict;
}
vector<Occ> IndexMap::operator[](const string& s){return dict[s];}
void IndexMap::indexer(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens)
        dict[token.getWord()].push_back(Occ(filename,token.getOcc()));
    numberOfDocuments++;
}

//set
IndexSet::IndexSet(){type="IndexSet";numberOfDocuments=0;}
IndexSet::IndexSet(const set<pair<string,vector<Occ> > >& s, const int& n){
    dict=s;
    numberOfDocuments=n;
    type="IndexSet";
}
IndexSet::~IndexSet(){}
int IndexSet::size() const {return dict.size();}
int IndexSet::getNumberOfDocuments() const {return numberOfDocuments;}
int IndexSet::setNumberOfDocuments(const int& i){numberOfDocuments=i;}
void IndexSet::setDict(const unordered_map<string, vector<Occ> >& d){
    set<pair<string,vector<Occ> > > newDict;
    for(auto& entry: d) 
        newDict.insert(entry);
    dict=newDict;
}
vector<Occ> IndexSet::operator[](const string& s){
    for(auto& element : dict){
        if(element.first==s)
            return element.second;
    }
    vector<Occ> v;
    dict.insert(make_pair(s,v));
    return v;
}
void IndexSet::indexer(const vector<Token>& tokens,const string& filename){
    for(auto& token :tokens){
        vector<Occ> element=this->operator[](token.getWord());
        dict.erase(dict.find(make_pair(token.getWord(),element)));
        element.push_back(Occ(filename,token.getOcc()));
        dict.insert(make_pair(token.getWord(),element));
    }
    numberOfDocuments++;
}


//Analyseur
// word frquency
AnalyseurWF::AnalyseurWF(){type="AnalyseurWF";}
AnalyseurWF::~AnalyseurWF(){}
vector<Token> AnalyseurWF::analyser(const vector<string>& text){
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
// binary
AnalyseurBinary::AnalyseurBinary(){type="AnalyseurBinary";}
AnalyseurBinary::~AnalyseurBinary(){}
vector<Token> AnalyseurBinary::analyser(const vector<string>& text){
    vector<Token> tokens;
    map<string,int> mapTokens;
    for(string word : text)
        mapTokens[word]=1;
    for(auto& word : mapTokens)
        tokens.push_back(Token(word.first,word.second));
}
// augmented term frequency
AnalyseurATF::AnalyseurATF(){type="AnalyseurATF";}
AnalyseurATF::~AnalyseurATF(){}
vector<Token> AnalyseurATF::analyser(const vector<string>& text){
    AnalyseurWF analyseurwf;
    vector<Token> tokens=analyseurwf.analyser(text);
    int maxCount=0;
    for(auto& token :tokens){
        maxCount=max( (float) maxCount,token.getOcc());
    }
    for(auto& token : tokens){
        token.setOcc(0.5+0.5*token.getOcc()/maxCount);
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
Moteur::Moteur(Index_* i, Analyseur_* a, const int m){
    if(i==nullptr || a==nullptr) EXIT_FAILURE;
    MAX_N=m;
    analyseur = a;
    index =i;
}
Moteur::Moteur(){}
Moteur::~Moteur(){
    delete analyseur;
    delete index;
}
vector<Recherche> Moteur::rechercher(const vector<string>& recherche){
    vector<Recherche> results;
    for(string searchWord : recherche){
        searchWord=lower(searchWord);
        results.push_back(Recherche((*index)[searchWord]));
        results.back().updateWithIDF(index->getNumberOfDocuments());
        results.back().trier();
    }
    return results;
}

//Lecteur
void Lecteur::importIndex(Index_* index,const string& chemin){
    ifstream file(chemin);
    string read;
    file>>read;
    file>>read;
    int n;
    file>>n;
    index->setNumberOfDocuments(n);

    unordered_map<string, vector<Occ> > dict;
    string word;
    string doc;
    int statistique;
    int state=1;
    
    while(file>>read) switch(state){
        case 1 : {
            word=read;
            state++;
            break;
        }
        case 2 : {
            if(read=="|")
                state++;
            else
                state=1;
            break;
        }
        case 3 : {
            doc=read;
            state++;
            break;
        }
        case 4 : {
            statistique=stof(read);
            dict[word].push_back(Occ(doc,statistique));
            state++;
            break;
        }
        case 5 : {
            if(read=="|")
                state=3;
            if(read=="}")
                state=1;
            break;
        }
    }
    index->setDict(dict);
    
    return ;
}
void Lecteur::exportIndex(Index_* index,const string& chemin){
    ofstream fo(chemin);
    string indexType=index->type;
    if(indexType=="IndexUnorderedMap") fo << *dynamic_cast<IndexUnorderedMap*>(index);
    else if(indexType=="IndexMap") fo << *dynamic_cast<IndexMap*>(index);
    else if(indexType=="IndexSet") fo << *dynamic_cast<IndexSet*>(index);
}
void Lecteur::exportMoteur(Moteur* moteur,const string& chemin){
    ofstream fo(chemin);
    string typeIndex=moteur->index->type;
    string typeAnalyseur=moteur->analyseur->type;
    
    fo<<typeIndex<<endl<<typeAnalyseur<<endl;
    if(typeIndex=="IndexUnorderedMap") fo << *dynamic_cast<IndexUnorderedMap*>(moteur->index);
    else if(typeIndex=="IndexMap") fo << *dynamic_cast<IndexMap*>(moteur->index);
    else if(typeIndex=="IndexSet") fo << *dynamic_cast<IndexSet*>(moteur->index);

    fo.close();
}
vector<string> Lecteur::parseQuery(const string& query){
    vector<string> vquery;
    string word;
    int i=query.size();
    if(i==0) return vquery;
    for(auto& c : query){
        if(c==' '){
            vquery.push_back(word);
            word="";
        }
        else if(i==1){
            word.push_back(c);
            vquery.push_back(word);
            word="";
        }
        else if(c=='"') ;
        else
            word.push_back(c);
        i--;
    }
    return vquery;
}

//stream
ostream& operator<<(ostream& out,const Recherche& recherche){
    for(auto& item : recherche.recherche)
        out<<item.getStat()<<" "<<item.getDoc()<<endl;
    return out;
}
ostream& operator<<(ostream& out,const IndexUnorderedMap& index){
    out<<index.getNumberOfDocuments()<<endl;
    for(auto& entry : index.dict)
        if(entry.second.size()>0){
        out<<entry.first;
        for(Occ occ : entry.second) 
            out<<" | "<<occ.getDoc()<<" "<<occ.getStat();
        out<<" }"<<endl;
        }
    return out;
}
ofstream& operator<<(ofstream& out,const IndexUnorderedMap& index){
    out<<index.getNumberOfDocuments()<<endl;
    for(auto& entry : index.dict)
        if(entry.second.size()>0){
        out<<entry.first;
        for(auto& occ : entry.second) 
            out<<" | "<<occ.getDoc()<<" "<<occ.getStat();
        out<<" }"<<endl;
        }
    return out;
}
ofstream& operator<<(ofstream& out,const IndexMap& index){
    out<<index.getNumberOfDocuments()<<endl;
    for(auto& entry : index.dict)
        if(entry.second.size()>0){
        out<<entry.first;
        for(auto& occ : entry.second) 
            out<<" | "<<occ.getDoc()<<" "<<occ.getStat();
        out<<" }"<<endl;
        }
    return out;
}
ofstream& operator<<(ofstream& out,const IndexSet& index){
    out<<index.getNumberOfDocuments();
    for(auto& entry : index.dict)
        if(entry.second.size()>0){
        out<<entry.first;
        for(Occ occ : entry.second) out<<" | "<<occ.getDoc()<<" "<<occ.getStat();
        out<<" }"<<endl;
    }
    return out;    
}

#endif