#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
using namespace std;

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

int main(){
  string s="This is a test";
  cout<<exec("ls");
}
