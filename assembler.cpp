#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include <map>
#include <utility>
#include <vector>

using namespace std;

class SymbolTable{
  private:
  map<string, int> table;
  uint16_t labelCnt;
  uint16_t varCnt;

  public:
  SymbolTable();
  ~SymbolTable();
  void addLabel(string newLabel);
  void addVar(string newVar);
  void incLabelCnt();
  uint16_t getSymbol(string inst);
  void getTable();
};

SymbolTable::SymbolTable(){
  table.insert(pair<string, uint16_t>("R0", 0));
  table.insert(pair<string, uint16_t>("R1", 1));
  table.insert(pair<string, uint16_t>("R2", 2));
  table.insert(pair<string, uint16_t>("R3", 3));
  table.insert(pair<string, uint16_t>("R4", 4));
  table.insert(pair<string, uint16_t>("R5", 5));
  table.insert(pair<string, uint16_t>("R6", 6));
  table.insert(pair<string, uint16_t>("R7", 7));
  table.insert(pair<string, uint16_t>("R8", 8));
  table.insert(pair<string, uint16_t>("R9", 9));
  table.insert(pair<string, uint16_t>("R10", 10));
  table.insert(pair<string, uint16_t>("R11", 11));
  table.insert(pair<string, uint16_t>("R12", 12));
  table.insert(pair<string, uint16_t>("R13", 13));
  table.insert(pair<string, uint16_t>("R14", 14));
  table.insert(pair<string, uint16_t>("R15", 15));
  table.insert(pair<string, uint16_t>("SCREEN", 16384));
  table.insert(pair<string, uint16_t>("KBD", 24576));

  table.insert(pair<string, uint16_t>("SP", 0));
  table.insert(pair<string, uint16_t>("LCL", 1));
  table.insert(pair<string, uint16_t>("ARG", 2));
  table.insert(pair<string, uint16_t>("THIS", 3));
  table.insert(pair<string, uint16_t>("THAT", 4));

  varCnt = 16;
}

SymbolTable::~SymbolTable(){
  table.clear();
}

void SymbolTable::addLabel(string newLabel){
  if(table.find(newLabel) != table.end()){
    throw runtime_error("addLabel(): Label already exists!");
  }
  else
    table.insert(pair<string, uint16_t>(newLabel, labelCnt));
}

void SymbolTable::addVar(string newVar){
  if(table.find(newVar) == table.end()){
    table.insert(pair<string, uint16_t>(newVar, varCnt));
    varCnt++;
  }
}

void SymbolTable::incLabelCnt(){
  labelCnt++;
}

uint16_t SymbolTable::getSymbol(string inst){
  addVar(inst);
  cout << table.find(inst)->second << endl;
  return table.find(inst)->second;
}

void SymbolTable::getTable(){
  vector<string> keys;
  for(auto it = table.begin(); it != table.end(); it++){
    keys.push_back(it->first);
  }

  for(const auto &key : keys){
    cout << key << " " << table.find(key)->second << endl;
  }
}

SymbolTable symboltable;

class Parser{
  private:
  map<string, string> compTable;
  map<string, string> destTable;
  map<string, string> jumpTable;

  public:
  Parser();
  ~Parser();
  string parseInst(string nowLine);
  string instA(string);
  string instC(string);
  string getComp(string);
  string getDest(string);
  string getJump(string);
};

Parser::Parser(){
  compTable.insert(pair<string, string>("0", "0101010"));
  compTable.insert(pair<string, string>("1", "0111111"));
  compTable.insert(pair<string, string>("-1", "0111010"));
  compTable.insert(pair<string, string>("D", "0001100"));
  compTable.insert(pair<string, string>("A", "0110000"));
  compTable.insert(pair<string, string>("!D", "0001101"));
  compTable.insert(pair<string, string>("!A", "0110001"));
  compTable.insert(pair<string, string>("-D", "0001111"));
  compTable.insert(pair<string, string>("-A", "0110011"));
  compTable.insert(pair<string, string>("D+1", "0011111"));
  compTable.insert(pair<string, string>("A+1", "0110111"));
  compTable.insert(pair<string, string>("1+D", "0011111"));
  compTable.insert(pair<string, string>("1+A", "0110111"));
  compTable.insert(pair<string, string>("D-1", "0001110"));
  compTable.insert(pair<string, string>("A-1", "0110010"));
  compTable.insert(pair<string, string>("D+A", "0000010"));
  compTable.insert(pair<string, string>("A+D", "0000010"));
  compTable.insert(pair<string, string>("D-A", "0010011"));
  compTable.insert(pair<string, string>("A-D", "0000111"));
  compTable.insert(pair<string, string>("D&A", "0000000"));
  compTable.insert(pair<string, string>("A&D", "0000000"));
  compTable.insert(pair<string, string>("D|A", "0010101"));
  compTable.insert(pair<string, string>("A|D", "0010101"));
  compTable.insert(pair<string, string>("M", "1110000"));
  compTable.insert(pair<string, string>("!M", "1110001"));
  compTable.insert(pair<string, string>("-M", "1110011"));
  compTable.insert(pair<string, string>("M+1", "1110111"));
  compTable.insert(pair<string, string>("1+M", "1110111"));
  compTable.insert(pair<string, string>("M-1", "1110010"));
  compTable.insert(pair<string, string>("D+M", "1000010"));
  compTable.insert(pair<string, string>("M+D", "1000010"));
  compTable.insert(pair<string, string>("D-M", "1010011"));
  compTable.insert(pair<string, string>("M-D", "1000111"));
  compTable.insert(pair<string, string>("D&M", "1000000"));
  compTable.insert(pair<string, string>("M&D", "1000000"));
  compTable.insert(pair<string, string>("D|M", "1010101"));
  compTable.insert(pair<string, string>("M|D", "1010101"));

  destTable.insert(pair<string, string>("null", "000"));
  destTable.insert(pair<string, string>("M", "001"));
  destTable.insert(pair<string, string>("D", "010"));
  destTable.insert(pair<string, string>("MD", "011"));
  destTable.insert(pair<string, string>("DM", "011"));
  destTable.insert(pair<string, string>("A", "100"));
  destTable.insert(pair<string, string>("AM", "101"));
  destTable.insert(pair<string, string>("MA", "101"));
  destTable.insert(pair<string, string>("AD", "110"));
  destTable.insert(pair<string, string>("DA", "110"));
  destTable.insert(pair<string, string>("AMD", "111"));
  destTable.insert(pair<string, string>("ADM", "111"));
  destTable.insert(pair<string, string>("MDA", "111"));
  destTable.insert(pair<string, string>("MAD", "111"));
  destTable.insert(pair<string, string>("DMA", "111"));
  destTable.insert(pair<string, string>("DAM", "111"));

  jumpTable.insert(pair<string, string>("null", "000"));
  jumpTable.insert(pair<string, string>("JGT", "001"));
  jumpTable.insert(pair<string, string>("JEQ", "010"));
  jumpTable.insert(pair<string, string>("JGE", "011"));
  jumpTable.insert(pair<string, string>("JLT", "100"));
  jumpTable.insert(pair<string, string>("JNE", "101"));
  jumpTable.insert(pair<string, string>("JLE", "110"));
  jumpTable.insert(pair<string, string>("JMP", "111"));
}

Parser::~Parser(){
  compTable.clear();
  destTable.clear();
  jumpTable.clear();
}

string Parser::parseInst(string inst){
  if (inst.find('@') != string::npos)
    return instA(inst.substr(1, inst.size() - 1));
  else
    return instC(inst);
}

string Parser::instA(string inst){
  bool alpha = false;
  for(int i = 0; i < inst.length() - 1; i++){
    if(int(inst[i]) < 48 || int(inst[i]) > 57){
      alpha = true;
      break;
    }
  }
  if(inst.find_first_of("0123456789") == 0 && !alpha)
    return "0" + bitset<15>(stoi(inst)).to_string();
  else if(inst.find_first_of("0123456789") != 0 && alpha)
    return "0" + bitset<15>(symboltable.getSymbol(inst)).to_string();
  else
    throw runtime_error("instA(): invalid");
}

string Parser::instC(string inst){
  int equal = inst.find('=');
  int semicolon = inst.find(';');

  string subD = (equal == string::npos) ? "null" : inst.substr(0, equal);
  string subC = inst.substr(equal + 1, semicolon - equal - 1);
  string subJ = (semicolon == string::npos) ? "null" : inst.substr(semicolon + 1, inst.size() - 1);

  return "111" + getComp(subC) + getDest(subD) + getJump(subJ);
}

string Parser::getComp(string inst){
  if(compTable.find(inst) == compTable.end())
    return compTable.find("0")->second;
  else
    return compTable.find(inst)->second;
}

string Parser::getDest(string inst){
  if (destTable.find(inst) == destTable.end())
    return destTable.find("null")->second;
  else
    return destTable.find(inst)->second;
}

string Parser::getJump(string inst){
  if (jumpTable.find(inst) == jumpTable.end())
    return jumpTable.find("null")->second;
  else
    return jumpTable.find(inst)->second;
}

Parser parser;

int main(int argc, char const *argv[]){
    string fnameIn, fnameOut;
    ifstream fIn;
    ofstream fOut;

    queue<string> pass1, pass2;


    if(argc < 2){
        throw runtime_error("main(): please specify input file");
    }
    else{
        fnameIn = string(argv[1]);
        if(fnameIn.find(".asm") == string::npos)
            throw runtime_error("main(): input file must be .asm");
        fIn.open(fnameIn);
        if(!fIn.is_open())
            throw runtime_error("main(): unable to open file " + fnameIn);

        if(argc == 2){
            string::size_type index = fnameIn.find_last_of('.');
            fnameOut = fnameIn.substr(0, index) + ".hack";
        }
        else if(argc == 3)
            fnameOut = string(argv[2]);
        else
            throw runtime_error("main(): too many parameters");

        fOut.open(fnameOut);
        if (!fOut.is_open())
            throw runtime_error("main(): unable to open file " + fnameOut);

        string nowLine;
        while(getline(fIn, nowLine)){
          if(nowLine == "\n" || nowLine == "\r" || nowLine == "\b" || nowLine == "\t")
            continue;
          string::size_type slash = nowLine.find("//");
          nowLine = nowLine.substr(0, slash);
          if(nowLine.size() == 0 || nowLine.empty())
            continue;
          string::iterator str_iter = std::remove(nowLine.begin(), nowLine.end(), ' ');
          nowLine.erase(str_iter, nowLine.end());
          str_iter = std::remove(nowLine.begin(), nowLine.end(), '\n');
          nowLine.erase(str_iter, nowLine.end());
          str_iter = std::remove(nowLine.begin(), nowLine.end(), '\r');
          nowLine.erase(str_iter, nowLine.end());
          str_iter = std::remove(nowLine.begin(), nowLine.end(), '\b');
          nowLine.erase(str_iter, nowLine.end());
          str_iter = std::remove(nowLine.begin(), nowLine.end(), '\t');
          nowLine.erase(str_iter, nowLine.end());
          pass1.push(nowLine);
        }
        while (!pass1.empty()){
          nowLine = pass1.front();
          pass1.pop();
          if(nowLine[0] == '(' && nowLine[nowLine.size() - 1] == ')'){
            symboltable.addLabel(nowLine.substr(1, nowLine.size() - 2));
            continue;
          }
          else
            symboltable.incLabelCnt();
          pass2.push(nowLine);
        }
        while(!pass2.empty()){
          nowLine = pass2.front();
          pass2.pop();
          cout << nowLine << endl;
          string output = parser.parseInst(nowLine);
          fOut << output << endl;
        }
        fIn.close();
        fOut.close();

        symboltable.getTable();
    }
    return 0;
}
