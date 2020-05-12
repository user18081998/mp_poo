
#ifndef MOTEUR_CPP
#define MOTEUR_CPP

#include "moteur.h"

using namespace std;

inline string lower(const string& word){string word1;for(char c : word) word1.push_back(tolower(c));return word1; };

// #############################################################################################################
// #############################################################################################################
// ################################################ CLASS TOKEN ################################################
// #############################################################################################################
// #############################################################################################################


Token::Token(const string& w,const float s=1){
    word=w;
    stat=s;
}

Token::~Token(){}
void Token::setStat(const float value){
    stat=value;
}

float Token::getStat() const {
    return stat;
}

string Token::getWord() const {
    return word;
}

void Token::operator++(){
    stat++;
}

// #############################################################################################################
// #############################################################################################################
// ################################################ CLASS ENTRY ################################################
// #############################################################################################################
// #############################################################################################################

Entry::Entry(const string& d,const float& s){
    doc=d;
    stat=s;
}

Entry::~Entry(){}

float Entry::getStat() const {
    return stat;
}

string Entry::getDoc() const {
    return doc;
}

void Entry::setStat(const float& value) {
    stat=value;
}

bool operator<(const Entry& o1,const Entry& o2) {
    return o1.getStat()<o2.getStat();
}

bool operator>(const Entry& o1,const Entry& o2) {
    return o1.getStat()>o2.getStat();
}

// #############################################################################################################
// #############################################################################################################
// ############################################### CLASS RESPONSE ###############################################
// #############################################################################################################
// #############################################################################################################

Response::Response(const vector<Entry>& v){
    response=v;
}

Response::~Response(){}

int Response::size() const{
    return response.size();
}

void Response::sorting(int maxNumberOfResults=0){
    sort(response.begin(),response.end(),greater<Entry>());
    if(maxNumberOfResults>0) while(response.size()>maxNumberOfResults)
        response.pop_back();
}

void Response::tfidf(const int& numberOfDocuments){
    int N=numberOfDocuments;
    float nt=size();
    float idf=log((N-nt)/nt); // probabilistic inverse document frequency
    // float idf=log(N/(1+nt))+1; // inverse document frequency
    for(auto& entry : response)
        entry.setStat(entry.getStat()*idf);
}

ostream& operator<<(ostream& out,const Response& response){
    for(auto& item : response.response)
        out<<item.getStat()<<" "<<item.getDoc()<<endl;
    return out;
}

// #############################################################################################################
// #############################################################################################################
// ################################################ CLASS INDEX_ ###############################################
// #############################################################################################################
// #############################################################################################################

Index_::~Index_(){}

// #############################################################################################################
// #############################################################################################################
// ######################################### CLASS INDEX UNORDERED MAP #########################################
// #############################################################################################################
// #############################################################################################################

void IndexUmap::setDict(const unordered_map<string ,vector<Entry> >& d) {
    dict=d;
}

void IndexUmap::setFileList(const set<string>& files){
    fileList=files;
}

IndexUmap::IndexUmap(){
    type="IndexUmap";
}

IndexUmap::IndexUmap(const unordered_map<string, vector<Entry> >& d, const set<string>& files){
    dict=d; 
    fileList=files; 
    type="IndexUmap";
}

IndexUmap::~IndexUmap(){}


IndexUmap& IndexUmap::operator=(const IndexUmap& other){
    fileList=other.fileList;
    dict=other.dict;
    return *this;
}


int IndexUmap::size() const{
    return dict.size();
}

int IndexUmap::getNumberOfDocuments() const{
    return fileList.size();
}

vector<Entry> IndexUmap::operator[](const string& s){
    return dict[s];
}

void IndexUmap::indexing(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens){
        dict[token.getWord()].push_back(Entry(filename,token.getStat()));
    }
    fileList.insert(filename);
    return ;
}

// #############################################################################################################
// #############################################################################################################
// ############################################### CLASS INDEX MAP #############################################
// #############################################################################################################
// #############################################################################################################

IndexMap::IndexMap(){
    type="IndexMap";
}

IndexMap::IndexMap(const map<string,vector<Entry> > s, const set<string>& files){
    dict=s;
    fileList=files;
    type="IndexMap";
}

IndexMap::~IndexMap(){}

int IndexMap::size() const {return dict.size();}

int IndexMap::getNumberOfDocuments() const {return fileList.size();}

void IndexMap::setDict(const unordered_map<string, vector<Entry> >& d){
    map<string,vector<Entry> > newDict;
    for(auto& entry : d)
        newDict[entry.first]=entry.second;
    dict=newDict;
}

void IndexMap::setFileList(const set<string>& files){
    fileList=files;
}

vector<Entry> IndexMap::operator[](const string& s){
    return dict[s];
}

void IndexMap::indexing(const vector<Token>& tokens,const string& filename){
    for(auto& token : tokens)
        dict[token.getWord()].push_back(Entry(filename,token.getStat()));
    fileList.insert(filename);
}

// #############################################################################################################
// #############################################################################################################
// ############################################## CLASS INDEX SET ##############################################
// #############################################################################################################
// #############################################################################################################


IndexSet::IndexSet(){
    type="IndexSet";
}

IndexSet::IndexSet(const set<pair<string,vector<Entry> > >& s,const set<string>& files){
    dict=s; 
    fileList=files; 
    type="IndexSet";
}

IndexSet::~IndexSet(){}

int IndexSet::size() const {
    return dict.size();
}

int IndexSet::getNumberOfDocuments() const {
    return fileList.size();
}

void IndexSet::setDict(const unordered_map<string, vector<Entry> >& d){
    set<pair<string,vector<Entry> > > newDict;
    for(auto& entry: d) 
        newDict.insert(entry);
    dict=newDict;
}

void IndexSet::setFileList(const set<string>& files){
    fileList=files;
}

vector<Entry> IndexSet::operator[](const string& s){
    for(auto& element : dict){
        if(element.first==s)
            return element.second;
    }
    vector<Entry> v;
    dict.insert(make_pair(s,v));
    return v;
}

void IndexSet::indexing(const vector<Token>& tokens,const string& filename){
    for(auto& token :tokens){
        vector<Entry> element=this->operator[](token.getWord());
        dict.erase(dict.find(make_pair(token.getWord(),element)));
        element.push_back(Entry(filename,token.getStat()));
        dict.insert(make_pair(token.getWord(),element));
    }
    fileList.insert(filename);
}

// #############################################################################################################
// #############################################################################################################
// ###################################### CLASS ANALYZER WORD FREQUENCY ########################################
// #############################################################################################################
// #############################################################################################################

AnalyzerWF::AnalyzerWF(){
    type="AnalyzerWF";
}

AnalyzerWF::AnalyzerWF(const unordered_map<string,int>& sw){
    type="AnalyzerWF";
    skipWords=sw;
}

AnalyzerWF::~AnalyzerWF(){}

vector<Token> AnalyzerWF::analyze(const vector<string>& text){
    vector<Token> tokens;
    bool exists;
    for(string word : text){ word=lower(word); if(skipWords[word]!=1){
        exists=false;
        for(int i=0;i<tokens.size();i++){
            if(tokens[i].getWord()==word){
                tokens[i].operator++();
                exists=true;
                }
            }
        if(!exists)
                tokens.push_back(Token(word));
        }
    }
    return tokens;
}

int AnalyzerWF::setSkipWords(unordered_map<string,int> sw){
    skipWords=sw;
    return 0;
}

unordered_map<string,int> AnalyzerWF::getSkipWords() const {
    return skipWords;
}

// #############################################################################################################
// #############################################################################################################
// ########################################### CLASS ANALYZER BINARY ###########################################
// #############################################################################################################
// #############################################################################################################

AnalyzerBinary::AnalyzerBinary(){
    type="AnalyzerBinary";
}

AnalyzerBinary::AnalyzerBinary(const unordered_map<string,int>& sw){
    type="AnalyzerBinary";
    skipWords=sw;
}

AnalyzerBinary::~AnalyzerBinary(){}

vector<Token> AnalyzerBinary::analyze(const vector<string>& text){
    vector<Token> tokens;
    map<string,int> mapTokens;
    for(string word : text) if(skipWords[word]!=1) mapTokens[word]=1;
    for(auto& word : mapTokens)
        tokens.push_back(Token(word.first,word.second));
    return tokens;
} 

int AnalyzerBinary::setSkipWords(unordered_map<string,int> sw){
    skipWords=sw;
    return 0;
}

unordered_map<string,int> AnalyzerBinary::getSkipWords() const {
    return skipWords;
}

// #############################################################################################################
// #############################################################################################################
// ############################## CLASS ANALYZER AUGMENTED TERM FREQUENCY ######################################
// #############################################################################################################
// #############################################################################################################

AnalyzerATF::AnalyzerATF(){
    type="AnalyzerATF";
}

AnalyzerATF::AnalyzerATF(const unordered_map<string,int>& sw){
    type="AnalyzerATF";
    skipWords=sw;
}

AnalyzerATF::~AnalyzerATF(){}

vector<Token> AnalyzerATF::analyze(const vector<string>& text){
    AnalyzerWF analyzerwf(skipWords);
    vector<Token> tokens=analyzerwf.analyze(text);
    int maxCount=0;
    for(auto& token :tokens){
        maxCount=max( (float) maxCount,token.getStat());
    }
    for(auto& token : tokens){
        token.setStat(0.5+0.5*token.getStat()/maxCount);
    }
    return tokens;
}

int AnalyzerATF::setSkipWords(unordered_map<string,int> sw){
    skipWords=sw;
    return 0;
}

unordered_map<string,int> AnalyzerATF::getSkipWords() const {
    return skipWords;
}

// #############################################################################################################
// #############################################################################################################
// ############################################### CLASS ENGINE ################################################
// #############################################################################################################
// #############################################################################################################

Engine::Engine(Index_* i, Analyzer_* a, const int m=5){
    index=i;
    analyzer=a;
    maxNumberOfResults=m;
}

Engine::Engine(){}

Engine::~Engine(){
    delete analyzer;
    delete index;
}

int Engine::setIndex(Index_* i){
    index=i;
    return 0;
}

int Engine::setAnalyzer(Analyzer_* a){
    analyzer=a;
    return 0;
}

int Engine::setMaxNumberOfResults(const int i){
    maxNumberOfResults=i;
    return 0;
}

vector<Response> Engine::search(const vector<string>& recherche){
    vector<Response> results;
    for(string searchWord : recherche){
        searchWord=lower(searchWord);
        results.push_back(Response((*index)[searchWord]));
        results.back().tfidf(index->getNumberOfDocuments());
        results.back().sorting(maxNumberOfResults);
    }
    return results;
}

// #############################################################################################################
// #############################################################################################################
// ################################# STREAM OPERATORS FOR ENGINE COMPONENTS ####################################
// #############################################################################################################
// #############################################################################################################

ostream& operator<<(ostream& out,const IndexUmap& index){
    for(auto& dictEntry : index.dict)
        if(dictEntry.second.size()>0){
        out<<dictEntry.first;
        for(Entry entry : dictEntry.second) 
            out<<" | "<<entry.getDoc()<<" "<<entry.getStat();
        out<<" }"<<endl;
        }
    return out;
}
ofstream& operator<<(ofstream& out,const IndexUmap& index){
    for(auto& dictEntry : index.dict)
        if(dictEntry.second.size()>0){
        out<<dictEntry.first;
        for(auto& occ : dictEntry.second) 
            out<<" | "<<occ.getDoc()<<" "<<occ.getStat();
        out<<" }"<<endl;
        }
    return out;
}
ofstream& operator<<(ofstream& out,const IndexMap& index){
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
    for(auto& entry : index.dict)
        if(entry.second.size()>0){
        out<<entry.first;
        for(Entry occ : entry.second) out<<" | "<<occ.getDoc()<<" "<<occ.getStat();
        out<<" }"<<endl;
    }
    return out;    
}


// #############################################################################################################
// #############################################################################################################
// ################################################ CLASS QUERY ################################################
// #############################################################################################################
// #############################################################################################################

Query::Query(ParserData* p){
    parserData=p;
}

Query::~Query(){}

pair<int,vector<string> > Query::mainArgs(int argc,char** argv){
    pair<int, vector<string> > query;
    query.first=argc;
    for(int i=0;i<argc;i++) query.second.push_back((string) argv[i]);
    return query;
}
int Query::verify(){
    int validity=0;                      // return variable
    /*
    for(auto& option : options){
        // option.first : option word
        // option.second : arguments for that option
        // first verify mandatory options
        if(! optionList[option.first]==option.second.size()){
            cerr<<"ERROR : provided an invalid number of arguments for option '"<<option.first<<"' ."<<endl;
            return 1;
        }
        vector<string> mandatoryCommands=commandList[command].first;
        vector<string>::iterator mandatoryCommandsIter=mandatoryCommands.begin();
        while(! (mandatoryCommandsIter==commandList[command].first.end())){
            if(*mandatoryCommandsIter==option.first) break;
            else mandatoryCommandsIter++;
        }
        if(mandatoryCommandsIter==mandatoryCommands.end()){
            cerr<<"ERROR : mandatory option '"<<option.first<<"' was not provided !";
            return 1;
        }
    }
    */
    // testing for -none
    if(options.find("-none")!=options.end()){
        cerr<<"ERROR : invalid command !";
        return 1;
    }
    // testing for mandatory options first
    vector<string> mandatoryOptions=parserData->commandList[command].first;
    for(auto& mandatoryOption : mandatoryOptions){
        // error control
        map<string, vector<string> >::iterator optionsIter=options.find(mandatoryOption);
        if(optionsIter==options.end()){
            cerr<<"ERROR : mandatory option '"<<mandatoryOption<<"' was not provided!"<<endl;
            return 1;
        }
        if(parserData->optionList[mandatoryOption]!=-1 && optionsIter->second.size()!= parserData->optionList[mandatoryOption]){
            cerr<<"ERROR : mandatory option '"<<mandatoryOption<<"' missing arguments!"<<endl;
            return 1;
        }
        // making query object
        validity+=setAttribute(mandatoryOption);
    }
    // testing for optional options
    vector<string> otionalOptions=parserData->commandList[command].second;
    for(auto& optionalOption : otionalOptions){
        map<string, vector<string> >::iterator optionsIter=options.find(optionalOption);
        if(optionsIter!=options.end()){
            if(optionsIter->second.size()!= parserData->optionList[optionalOption] && parserData->optionList[optionalOption]!=-1){
                cerr<<"ERROR : optional option '"<<optionalOption<<"' missing arguments!"<<endl;
                return 1;
            }
            validity+=setAttribute(optionalOption);
        }
        
    }
    if(validity>0) validity=1;
    return validity;
}

int Query::parse(int argc,char** argv){
    this->argc=argc;
    vector<string> query=mainArgs(argc,argv).second;
    if(argc==1){
        cerr<<"Error : insufficient number of arguments!"<<endl;
        return 1;
    }
    // for(auto& word : commandList) if(query[1]==word.first) command=query[1];
    if(parserData->commandList.find(query[1])!=parserData->commandList.end()) command=query[1];
    if(command=="none"){
        cerr<<"ERROR : invalid command!"<<endl;
        return 1;
    }
    string option="-none";
    for(int i=2;i<query.size();i++){
        string word = query[i];
        if(word[0]=='-'){
            bool optionValid=false;
            for(auto& test : parserData->optionList) 
                if(test.first==word){
                    optionValid=true;
                    option = word;
                    options[option];
                }
            if(optionValid==false){
                cerr<<"ERROR : invalid option '"<<word<<"' !"<<endl;
                return 1;
            }
        }
        else{
            if(word[0]=='"' || word[word.size()-1]=='"'){ // removing " from arguments, like word" or "word . 
                string newWord;
                for(auto ch : word) if(ch!='"') newWord.push_back(ch);
                word=newWord;
            }
            options[option].push_back(word);
        }
    }
    return verify();
}
int Query::setAttribute(string& option){                // takes arguments from options and assigns them to query attributes
    if(option=="-it"){
        if(parserData->allowedIndexTypes.find(options["-it"][0])!=parserData->allowedIndexTypes.end())
            indexType=options["-it"][0];
        else {
            cerr<<"ERROR : index type '"<<options["-it"][0]<<"' unrecognized!";
            return 1;
        }
    }
    else if(option=="-at"){
        if(parserData->allowedAnalyzerTypes.find(options["-at"][0])!=parserData->allowedAnalyzerTypes.end())
            analyzerType=options["-at"][0];
        else {
            cerr<<"ERROR : analyzer type '"<<options["-at"][0]<<"' unrecognized!";
            return 1;
        }
    }
    else if(option=="-s") searchWords=options["-s"];
    else if(option=="-f") file=options["-f"][0];
    else if(option=="-d") directory=options["-d"][0];
    return 0;
}
string Query::printQuery(){
    string out;
    out+=command+" ";
    for(auto& option : options){
        out+=option.first+" ";
        for(auto& argument : option.second)
        out+=argument+" ";
    }
    return out;
}
ostream& operator<<(ostream& out,const Query& query){
    out<<"command : "<<query.command<<endl;
    out<<"index type : "<<query.indexType<<endl;
    out<<"analyser type : "<<query.analyzerType<<endl;
    out<<"file : "<<query.file<<endl;
    out<<"directory : "<<query.directory<<endl;
    out<<"search words : "; for(auto& word : query.searchWords) out<<word<<" ";
    out<<endl;
    return out;
}

unordered_map<string,int> ParserData::getSkipWords() const {
    return skipWords;
}

// #############################################################################################################
// #############################################################################################################
// ############################################### CLASS  PARSER ###############################################
// #############################################################################################################
// #############################################################################################################

Parser::Parser(){
    query=Query(&parserData);
    engineIO=EngineIO(&parserData);
}

Parser::~Parser(){}

vector<string> Parser::readFile(const string& chemin){
    ifstream file(chemin);
    vector<string> text;
    string word;
    while( file >> word ) text.push_back(word);
    file.close();
    return text;
}


// #############################################################################################################
// #############################################################################################################
// ############################################### CLASS ENGINEIO ##############################################
// #############################################################################################################
// #############################################################################################################

EngineIO::EngineIO(ParserData* p){
    parserData=p;
}

EngineIO::~EngineIO(){}
int EngineIO::importEngine(Engine* engine,ParserData* parserData,const string& chemin){
    // here's an engine file sample
    /*
        engineFile
        <it> IndexUmap </it>
        <at> AnalyzerATF </at>
        <fl> 
        file1
        file2 
        file_n
        </fl>
        <i>
        word1 : file1 stat1 | file2 stat2 | file_n stat_n <>
        word2 : file1 stat1 | file2 stat2 | file_n stat_n <>
        word_n : file1 stat1 | file2 stat2 | file_n stat_n <>
        </i>
    */

    // importing mimics the behavior of a finite state machine 
    int state=0;

    ifstream file(chemin);
    string read;
    string indexType;
    string analyzerType;
    set<string> fileList;

    // read first line to verify that the file is an engine file
    file>>read;
    if(read!=parserData->fileTags["fileHeader"].first){
        cerr<<"ERROR : Incorrect file"<<endl;
        return 1;
    }
    // from now on we assume the file format is correct
    state=1;
    while(file>>read) switch(state){
        case 0 : {
            cerr<<"ERROR : imcompatible file"<<endl;
            return 1;
        }
        case 1 : {
            if(read==parserData->fileTags["indexType"].first) state=2; // state 2 for reading index type
            else if(read==parserData->fileTags["analyzerType"].first) state=3; // state 3 fir readng analyzer type
            else if(read==parserData->fileTags["fileList"].first) state=4; // state 4 for reading file list
            else if(read==parserData->fileTags["index"].first) state=5;  // state 5 for reading index
            else state=0;             // state 0 means an error
            break;
        }
        case 2 : {
            if(parserData->allowedIndexTypes.find(read)==parserData->allowedIndexTypes.end()){
                cerr<<"ERROR : state : 2 : ";
                state =0;
                break;
            }
            indexType=read;
            state=12;
            break;
        }
        case 12 : {
            if(read!=parserData->fileTags["indexType"].second){
                cerr<<"ERROR : state : 12 : ";
                state=0;
                break;
            }
            state=1;
            break;
        }
        case 3 : {
            if(parserData->allowedAnalyzerTypes.find(read)==parserData->allowedAnalyzerTypes.end()){
                cerr<<"ERROR : state : 3 : ";
                state=0;
                break;
            }
            analyzerType=read;
            state=13;
            break;
        }
        case 13 : {
            if(read!=parserData->fileTags["analyzerType"].second){
                cerr<<"ERROR : state : 13 : ";
                state=0;
                break;
            }
            state=1;
            break;
        }
        case 4 : {
            if(read==parserData->fileTags["fileList"].second){
                state=1;
                break;
            }
            fileList.insert(read);
            break;
        }
        case 5 : {
            int i =importIndex(engine->index,file,indexType,fileList);
            if(i==1) state=0; // we add an extra </i> line so that in case importIndex returns 1 we can return it as well 
        }
    }
    file.close();
    return 0;
}

int EngineIO::importIndex(Index_* index,ifstream& file,const string& it, const set<string>& fl){
    index->fileList=fl;
    
    // importIndex mimics the behavior of a finite state machine too

    // declaring variables
    unordered_map<string, vector<Entry> > dict;
    string read;
    string word;
    string doc;
    int statistic;
    int state=1;
    try{
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
                statistic=stof(read);
                dict[word].push_back(Entry(doc,statistic));
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
    } catch(const char* m) {
        cerr << m <<endl;
        return 1;
    }

    index->setDict(dict);
    return 0;
}

int EngineIO::exportEngine(Engine* engine,ParserData* parserData,const string& chemin){
    // declaring variables
    ofstream file(chemin);
    string typeIndex=engine->index->type;
    string typeAnalyzer=engine->analyzer->type;

    // adding file header
    file<<parserData->fileTags["fileHeader"].first<<endl;
    file<<parserData->fileTags["indexType"].first<<" "<<typeIndex<<" "<<parserData->fileTags["indexType"].second<<endl;
    file<<parserData->fileTags["analyzerType"].first<<" "<<typeAnalyzer<<" "<<parserData->fileTags["analyzerType"].second<<endl;
    file<<parserData->fileTags["fileList"].first<<endl;
    for(auto& fileName : engine->index->fileList) file<<fileName<<endl;
    file<<parserData->fileTags["fileList"].second<<endl;
    file<<parserData->fileTags["index"].first<<endl;

    // exporting indexes via dynamic type casting using ofstream& operator<< overload
    if(typeIndex=="IndexUmap") file << *dynamic_cast<IndexUmap*>(engine->index);
    else if(typeIndex=="IndexMap") file << *dynamic_cast<IndexMap*>(engine->index);
    else if(typeIndex=="IndexSet") file << *dynamic_cast<IndexSet*>(engine->index); 

    file<<endl<<parserData->fileTags["index"].second<<endl;
    return 0;
}

/*
int main(int argc, char** argv){
    Parser parser;
    int i=parser.query.parse(argc,argv);
    cout<<parser.query.printQuery();
    cout<<endl<<parser.query;
    return i;
}
*/

/*
int main(){
    return 0;
}
*/
#endif
