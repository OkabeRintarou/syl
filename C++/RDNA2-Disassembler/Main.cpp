#include <iostream>
#include <fstream>
#include "Common.h"
#include "Stream.h"
#include "Disassembler.h"

using namespace std;

int main() {
    ifstream fin("../tests/bufferclear_cs_shader", ios::in);
    if (fin.fail()) {
        cerr << "Fail to open file\n";
        return -1;
    }
    FileInputStream fs(std::move(fin));
    Disassembler dis{fs}; 
    dis.dump();
    return 0;
}
