
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
    float stat;
    
    public :
  
    Token(const string&,const float);
    ~Token();

    void operator++();

    void setStat(const float);
    float getStat() const;
    string getWord() const;
};

class Entry{
    string doc;
    float stat;

    public :

    Entry(const string&,const float&);
    ~Entry();

    float getStat() const ;
    string getDoc() const;
    void setStat(const float&);
    friend bool operator<(const Entry&,const Entry&);
    friend bool operator>(const Entry&,const Entry&);
};

class Response{

    vector<Entry> response;

    public :
    
    Response(const vector<Entry>& v);
    ~Response();
    
    int size() const;

    void sorting(int);
    void tfidf(const int&);
    
    friend ostream& operator<<(ostream&,const Response& );
};

class Index_{
    public :

    set<string> fileList;
    string type;

    virtual void setDict(const unordered_map<string,vector<Entry> >& )=0;
    virtual void setFileList(const set<string>&)=0;

    virtual ~Index_()=0;

    virtual int size() const =0;
    virtual int getNumberOfDocuments() const =0;

    virtual void indexing(const vector<Token>&, const string&) =0;
    virtual vector<Entry> operator[](const string&) =0;
    
    friend class EngineIO;
};

class IndexUmap : public Index_ {

    unordered_map<string,vector<Entry> > dict;
    
    void setDict(const unordered_map<string,vector<Entry> >&) override;
    void setFileList(const set<string>&) override;

    public :

    IndexUmap();
    IndexUmap(const unordered_map<string, vector<Entry> >&, const set<string>&);
    ~IndexUmap() override;
    IndexUmap& operator=(const IndexUmap&);
    
    int size() const override;
    int getNumberOfDocuments() const override;

    void indexing(const vector<Token>& ,const string&) override;
    vector<Entry> operator[](const string&) override;

    friend class EngineIO;
    friend ostream& operator<<(ostream&,const IndexUmap&);
    friend ofstream& operator<<(ofstream&,const IndexUmap&);
};

class IndexMap : public Index_{

    map<string,vector<Entry> > dict;

    void setDict(const unordered_map<string, vector<Entry> >&) override;
    void setFileList(const set<string>&) override;

    public :

    IndexMap();
    IndexMap(const map<string,vector<Entry> >,  const set<string>&);
    ~IndexMap() override ;

    int size() const override;
    int getNumberOfDocuments() const override;

    void indexing(const vector<Token>&,const string& ) override;
    vector<Entry> operator[](const string&) override;

    friend class EngineIO;
    friend ofstream& operator<<(ofstream&,const IndexMap&);
};

class IndexSet : public Index_ {

    set<pair<string,vector<Entry> > > dict;

    void setDict(const unordered_map<string, vector<Entry> >&) override;
    void setFileList(const set<string>&) override;

    public :

    
    IndexSet();
    IndexSet(const set<pair<string,vector<Entry> > >&, const set<string>&);
    ~IndexSet() override ;

    int size() const override;
    int getNumberOfDocuments() const override;

    void indexing(const vector<Token>&,const string&) override;
    vector<Entry> operator[](const string&) override;

    friend class EngineIO;
    friend ofstream& operator<<(ofstream&,const IndexSet&);
};

class Analyzer_{
    public :

    unordered_map<string,int> skipWords;
    string type;
    
    virtual int setSkipWords(unordered_map<string,int>)=0;
    virtual unordered_map<string,int> getSkipWords() const =0;

    virtual vector<Token> analyze(const vector<string>&) =0;
    
    friend class EngineIO;
};

class AnalyzerWF : public Analyzer_ {   // word frequency
    public:
    
    AnalyzerWF();
    AnalyzerWF(const unordered_map<string,int>&);
    ~AnalyzerWF();

    int setSkipWords(unordered_map<string,int>) override;
    unordered_map<string,int> getSkipWords() const override ;

    vector<Token> analyze(const vector<string>& text) override;

    friend class EngineIO;

};

class AnalyzerBinary : public Analyzer_ { // binary
    public :

    AnalyzerBinary();
    AnalyzerBinary(const unordered_map<string,int>&);
    ~AnalyzerBinary();

    int setSkipWords(unordered_map<string,int>) override;
    unordered_map<string,int> getSkipWords() const override ;

    vector<Token> analyze(const vector<string>&) override;   
 
    friend class EngineIO;
};

class AnalyzerATF : public Analyzer_{ // augmented term frequency
    public:

    AnalyzerATF();
    AnalyzerATF(const unordered_map<string,int>& );
    ~AnalyzerATF();

    int setSkipWords(unordered_map<string,int>) override;
    unordered_map<string,int> getSkipWords() const override ;

    vector<Token> analyze(const vector<string>&) override;
    
    friend class EngineIO;
};

class Engine{
    public :
    
    Index_* index;
    Analyzer_* analyzer;

    int maxNumberOfResults;

    Engine(Index_*, Analyzer_*, const int);
    Engine();
    ~Engine();
    
    int setIndex(Index_*);
    int setAnalyzer(Analyzer_*);
    int setMaxNumberOfResults(const int);

    vector<Response> search(const vector<string>&);

    friend class EngineIO;
};


class ParserData{
    map<string, pair<vector<string>,vector<string> > > commandList={ 
                            // list of available commands and the options that can go with them 
                            //  {command, {{mandatory options},{optional options}}} 
                                {"new"              ,{{},{"-d","-it","-at","-s",}}},
                                {"search"           ,{{"-s"},{"-it","-at",}}},
                                {"addFile"          ,{{"-f"},{"-it","-at",}}},
                                {"removeFile"       ,{{"-f"},{"-it","-at",}}},
                                {"printFileList"    ,{{},{"-it","-at"}}},
    };
    map<string,int> optionList={ // list of options and how many arguments they can have
                                {"-it"  ,1},    // index type, can have only one argument
                                {"-at"  ,1},    // analyzer type, can have only one argument
                                {"-s"   ,-1},   // search words, doesnt have a fixed size
                                {"-f"   ,1},    // file , can have only one argument
                                {"-d"   ,1},    // directory, can have only one argument
                                {"-none",0}
    };
    set<string> allowedIndexTypes={
                                    "IndexUmap",
                                    "IndexSet",
                                    "IndexMap",
    };
    set<string> allowedAnalyzerTypes={
                                    "AnalyzerBinary",
                                    "AnalyzerATF",
                                    "AnalyzerWF",
    };
    map<string ,pair<string,string> > fileTags={
                                    {"indexType", {"<it>","</it>"}},        // <it> indexType </it>
                                    {"analyzerType",{"<at>","</at>"}},      // <at> analyzerType </at>
                                    {"fileList",{"<fl>","</fl>"}},          // <fl> file1 file2 file3 .... </fl> 
                                    {"index",{"<i>","</i>"}},               // <i> index </i>
                                    {"fileHeader",{"engineFile",""}},       // first line in an engine file to recognize it.
    };
    unordered_map<string,int> skipWords={
                        {"the",1}, {"he",1}, {"she",1},
    };
    friend class EngineIO;
    friend class Query;
    friend class Parser;
    friend class Analyzer_;
    public:
    unordered_map<string,int> getSkipWords() const;
};

class EngineIO{     // should deal with everything engine import / engine export
    ParserData *parserData;
    public :

    EngineIO(ParserData* p);
    ~EngineIO();

    int importEngine(Engine* ,ParserData* ,const string& );
    int importIndex(Index_* ,ifstream& , const string& ,const set<string>& );

    int exportEngine(Engine* ,ParserData* ,const string& );
};
class Query{                                                // should deal with everything shell related 
    int argc=0;
    map<string, vector<string> > options;

    ParserData* parserData;

    public :
    
    string command              ="none";
    string indexType            ="IndexUmap";
    string analyzerType         ="AnalyzerATF";
    string file                 ="";
    string directory            ="";
    vector<string> searchWords  ={};
    string engineFile           ="/engine.txt";

    Query(ParserData* );
    ~Query();
    pair<int,vector<string> > mainArgs(int , char** );          // reads arguments from main
    int parse(int, char**);                                     // puts the arguments in query.options
    int verify();                                               // make sure main's arguments are making sense
    int setAttribute(string&);                                  // set attributes to query
    string printQuery();
    friend ostream& operator<<(ostream&,const Query&);  
};

class Parser{       // provides basic parsing methods and data 
    public :

    ParserData parserData;
    Query query=Query(&parserData);
    EngineIO engineIO=EngineIO(&parserData);

    Parser();
    ~Parser();

    vector<string> readFile(const string&);      // reads a file and puts it in a vector of string
    vector<string> readDirectory(const string&); // returns a vector of the names of files in a directory
    friend class EngineIO;
    friend class Query;

};

#endif