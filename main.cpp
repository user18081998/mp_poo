// #include <filesystem>

#include <boost/filesystem.hpp>
#include "main/moteur.h"
#include "main/moteur.cpp"
#include <chrono>


namespace fs = boost::filesystem;
namespace ch = std::chrono;
using namespace std;

Parser parser;

int main(int argc ,char** argv){
    auto start = ch::high_resolution_clock::now();

    // parsing inputs
    int i=parser.query.parse(argc,argv);
    cout<<endl<<parser.query<<endl;
    if(i==1) return 1;

    string engineFile = fs::current_path().string() +parser.query.engineFile;

    //declaring variables
    Index_* index=0;
    Analyzer_* analyzer=0;
    
    if(parser.query.indexType=="IndexUmap") index = new IndexUmap;
    else if(parser.query.indexType=="IndexMap") index = new IndexMap;
    else if(parser.query.indexType=="IndexSet") index = new IndexSet;
    else return 1;
    if(parser.query.analyzerType=="AnalyzerATF") analyzer = new AnalyzerATF;
    else if(parser.query.analyzerType=="AnalyzerBinary") analyzer = new AnalyzerBinary;
    else if(parser.query.analyzerType=="AnalyzerWF") analyzer = new AnalyzerWF;
    else return 1;
    
    Engine* engine = new Engine(index,analyzer);
    engine->analyzer->setSkipWords(parser.parserData.getSkipWords());

    // if command != new , we import the engine, else its blank
    if(parser.query.command!="new"){
        parser.engineIO.importEngine(engine, &(parser.parserData), engineFile);
    }

    
    //if directory option was specified, we index it
    if(parser.query.directory!=""){
        cout<<"indexing directory '"<<parser.query.directory<<"'..."<<endl;
        fs::recursive_directory_iterator dirIter(parser.query.directory),end;
        while(dirIter!=end){
            vector<string> text = parser.readFile(dirIter->path().string());
            vector<Token> tokens=engine->analyzer->analyze(text);
            engine->index->indexing(tokens,dirIter->path().string());

            dirIter++;
        }
    }
        
        
        // cout<<*dynamic_cast<IndexUmap*>(engine->index);
    

    //if file option was specified, we index it
    if(parser.query.file!=""){
        vector<string> text = parser.readFile(parser.query.file);
        vector<Token> tokens=engine->analyzer->analyze(text);
        engine->index->indexing(tokens,parser.query.file);
    }

    //searching 
    if((parser.query.command=="search" && parser.query.searchWords.size()>0) || parser.query.searchWords.size()>0){
        cout<<"making a search query for '"; for(auto& q : parser.query.searchWords) cout<<q<<" "; cout<<"' ..."<<endl;
        vector<Response> response=engine->search(parser.query.searchWords);
        for(auto& i : response) cout<<i<<endl;
    }

    else if(parser.query.command=="printFileList"){
        cout<<"printing file list ...."<<endl;
        for(auto& file : engine->index->fileList) cout<<file;
    }

    // exporting index
    cout<<"exporting index ..."<<endl;
    cout<<"changes will be saved to "<<engineFile<<endl;
    parser.engineIO.exportEngine(engine,&parser.parserData,engineFile);
    
    auto finish = ch::high_resolution_clock::now();
    ch::duration<double> elapsed = finish - start;
    cout<<endl << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}
