
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;
inline string lower(const string& word){string word1;for(char c : word) word1.push_back(tolower(c));return word1; };


class Token{
// we need this for extracting information from individual files
// the file is cut down into a collection of tokens
// a token is a pair (word,occ)
// where occ is the number of occurences of the word in the file
// this is going to come up useful later with the analyzer
    string word;
    int occ;
    public:
    Token(string w,int s=1): word{w},occ{s} {}
    ~Token(){};
    void increment() {occ++;};
    string getWord() const {return word;};
    int getOcc() const {return occ;};

};

class Occ{
    string doc;
    float stat;
    public:
    Occ(string d,float s):doc{d},stat{s} {}    
    ~Occ(){};
        
    float getStat() const {return stat;};
    string getDoc() const {return doc;}

    friend bool operator<=(const Occ& O1,const Occ& O2);
    friend bool operator>(const Occ& O1, const Occ& O2);
    friend bool operator<(const Occ& O1, const Occ& O2);
};

bool operator>(const Occ& O1, const Occ& O2) {
    return O1.stat>O1.stat;
    }
bool operator<=(const Occ& O1,const Occ& O2) {
    return O1.stat<=O2.stat;
    }
bool operator<(const Occ& O1,const Occ& O2){
    return O1.stat<O2.stat;
    }

class Word{
    string word;
    public:
    Word(string s) : word{s} {}
    string getWord() const {return word;};
};

class Request{
    vector<Occ> request;
    public :
    Request(const vector<Occ>& v) : request{v} {}
    ~Request(){};

    int size() const {return request.size();}
    friend ostream& operator<<(ostream& out,const Request& request);

};

ostream& operator<<(ostream& out,const Request& request){
    for(int i=0;i<request.size();i++)
        out<<request.request[i].getStat()<<" "<<request.request[i].getDoc()<<endl;
    return out;
}

class Index{
    unordered_map<string, vector<Occ> > dict; 
    public:
    Index(){};
    ~Index(){};

    void add(const vector<Token>& tokens,const string& filename);
    
    Request searc(const string& request,const int& max_);

    friend ostream& operator<<(ostream& out,const Index& index);
};

Request Index::searc(const string& chaine,const int& max_){
    vector<Occ> result;
    string request=lower(chaine);
    for(pair<string,vector<Occ> > entry : dict)
        if(entry.first==request){
            result=entry.second;
            break;
        }
    sort(result.begin(),result.end(),[](const Occ& O1,const Occ& O2){return O1.getStat()>=O2.getStat();});
    while(result.size()>max_)
        result.pop_back();
    return Request(result);
}

void Index::add(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens)
    dict[token.getWord()].push_back(Occ(filename,token.getOcc()));
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


class Analyseur{
    Index index;
    int MAX_N;
    public:
    Analyseur(int max=5) : MAX_N{max} {};
    ~Analyseur(){};

    /* analyser est une fonction qui accepte le nom d'un fichier
       elle doit retourner un vecteur de tokens qu'on degage du fichier
       un token est le pair {mot,n} 
       avec n le nombre d'occurences du mot dans le fichier */
    vector<Token> analyser(string filename);

    void indexer(const vector<Token>& tokens,const string& filename) {index.add(tokens,filename);};
    Request rechercher(const string& request){return index.searc(request,MAX_N);};
    void print(){cout<<index;};
};


vector<Token> Analyseur::analyser(const string filename){
    string word;
    vector<Token> words;
    ifstream file(filename);
    bool exists;
    while( file >> word ){
        word=lower(word);
        exists=false;
        for(int i=0;i<words.size();i++){
            if(words[i].getWord()==word){
                words[i].increment();
                exists=true;
                }
            }
        if(!exists)
                words.push_back(Token(word));
        }
    return words;
}


int main(){
    // on extrait une liste des fichiers qu'on vas tester avec
    string item;
    vector<string> fileList;
    ifstream ff("/home/ahmed2/Desktop/cpp/mp/fileList.txt");
    while( ff >> item) fileList.push_back("/home/ahmed2/Desktop/cpp/mp/petit_corpus/"+item);

    // on commence l'indexation 
    Analyseur analyseur(10);
    for(string file :fileList){
        vector<Token> words=analyseur.analyser(file);
        analyseur.indexer(words,file);
    }
    cout<<"Finished analyzing!"<<endl;
    string searchWord="pizza";
    
    // l'utilisateur peut maintenant rechercher
    while(true) {
    cout<<"Enter a word to search : ";
    cin>>searchWord;
    cout<<"Search results for : "<<searchWord<<endl;
    Request request=analyseur.rechercher(searchWord);
    cout<<request<<endl<<"found "<<request.size()<<" out of "<<fileList.size()<<" items lmao "<<endl;
    }
    return 0;
}
