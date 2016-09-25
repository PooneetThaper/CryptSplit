#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cstdio>
using namespace std;

//Encryption variables
string input;
vector<string> record;
string randomSource=
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

//Decryption variables
string recordIn;
string encryptionPass;
vector<string> fileNames;

//Either
size_t numSplits;
vector<string> splitText;
string finalOutput;

//All purpose functions

string charToString(char c){
  string retval="";
  stringstream ss;
  ss<<c;
  ss>>retval;
  return retval;
}

string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}


//Encryption functions

void split(string s,int x){
  size_t curr=0;
  for(size_t i=0;i<s.length();i++){
    string in;
    if(s[i]!=' '){
      in=charToString(s[i]);
    }else{
      in="~";
    }
    splitText[curr].append(in);
    curr=(curr+1)%x;
  }
}

void getText(string fileName){
  input="";
  string line;
  ifstream file (fileName);
  if (file.is_open()){
    while ( getline(file,line)){
      input.append(line);
      input.append("/n");
    }
    file.close();
  }else cout << "Unable to open file";
}

void writeOutE(string pass){
  for (size_t i = 0; i < numSplits; i++) {
    string name=record[i];
    ofstream outfile(name);
    outfile<<splitText[i];
    outfile.close();
  }
  string recordFinal="{"+pass+"}\n{";
  for (size_t i = 0; i < numSplits; i++) {
    recordFinal+=record[i];
    if(i!=(numSplits-1)) recordFinal+=",";
  }
  recordFinal+="}";
  ofstream rec("record.txt");
  rec<<recordFinal;
  rec.close();
}

string genRandomString(const int len) {
    string s="";
    string next="";
    for (int i = 0; i < len; i++) {
        s.append(charToString(randomSource[rand() % (randomSource.length())]));
    }
    return s;
}

void encrypt(){
  string excrypted;
  string command= "python -c encrypt.py '"+input+"'";
  excrypted =exec(command);
  input=encrypted;
}

//Decryption functions

void getRecord(){
  string line="";
  ifstream rec("record.txt");
  if (rec.is_open()){
    int i=0;
    while ( getline(rec,line)){
      recordIn.append(line);
    }
    rec.close();
  }else cout << "Unable to open file";
}

void parseRecord(){
  bool gotPass=false;
  int state=0;
  string name="";
  for(size_t i = 0; i < recordIn.length(); i++) {
    if(recordIn[i]=='}'){
      if (state==1){
          gotPass=true;
      }else{
        fileNames.push_back(name);
        numSplits++;
      }
      state=0;
    }
    if(state==1) encryptionPass+=charToString(recordIn[i]);
    if(state==2) {
      if(recordIn[i]==','){
        fileNames.push_back(name);
        numSplits++;
        name="";
      }else{
        name+=charToString(recordIn[i]);
      }
    }
    if(recordIn[i]=='{'){
      if (!gotPass) {
        state=1;
      }else{
        state=2;
      }
    }
  }
}

void getFiles(){
  string s="";
  for (size_t i = 0; i < fileNames.size(); i++) {
    string line;
    ifstream file(fileNames[i]);
    if (file.is_open()){
      while (getline(file,line)){
        s.append(line);
      }
      file.close();
      splitText.push_back(s);
      s="";
    }else cout << "Unable to open file";
  }
}

void defrag(){
  bool skipNext=false;
  string potato;
  for(size_t i = 0; i < splitText[numSplits-1].length(); i++) {
    for (size_t j = 0; j < numSplits; j++) {
      if(!skipNext){
        if(splitText[j][i]!='/'){
          stringstream ss;
          ss<<splitText[j][i];
          ss>>potato;
          if(potato=="~"){
            finalOutput+=" ";
          }else{
            finalOutput+=potato;
          }
        }else{
          if(j==numSplits-1){
            if(splitText[0][i+1]=='n'){
              finalOutput+="\n";
              skipNext=true;
            }
          }else{
            if (splitText[j+1][i]=='n') {
              finalOutput+="\n";
              skipNext=true;
            }
          }
        }
      }else{
        skipNext=false;
      }
    }
  }
}

void writeOutD(){
  ofstream finalOut("text2.txt");
  finalOut<<finalOutput;
  finalOut.close();
}

void decrypt(){
  string decrypted;
  string command= "python -c decrypt.py '"+finalOutput+"'";
  decrypted =exec(command);
  finalOutput=decrypted;
}

int main(){
  char p;
  cin>>p;
  if(p=='e'){
      cin>>numSplits;
      for (size_t i = 0; i < numSplits; i++) {
        splitText.push_back("");
      }
      getText("text.txt");
      encrypt();
      split(input,numSplits);
      for (size_t i = 0; i < numSplits; i++) {
        string rand=genRandomString(12);
        rand+=".txt";
        record.push_back(rand);
      }
      writeOutE(encryptionPass);
    }else if(p=='d'){
      getRecord();
      //decrypt record
      parseRecord();
      getFiles();
      defrag();
      decrypt();
      //decrypt(encryptionPass[0]);
      writeOutD();
    }else{
      cout<<"invalid input\n";
    }
}
