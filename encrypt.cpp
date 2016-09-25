#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
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

void encrypt(char key){
  string encrypted="";
  for (int temp = 0; temp < input.size(); temp++){
    encrypted += input[temp] ^ (int(key) + temp) % 255;
  }
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

void decrypt(char key){
  string decrypted="";
  for (int temp = 0; temp < finalOutput.size(); temp++){
    decrypted += finalOutput[temp] ^ (int(key) + temp) % 255;
  }
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
      string ePass=genRandomString(1);
      //encrypt(ePass[0]);
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
      //decrypt(encryptionPass[0]);
      writeOutD();
    }else{
      cout<<"invalid input\n";
    }
}
