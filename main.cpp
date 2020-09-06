//
// Created by brado on 9/4/20.
//

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <chrono>
#include <filesystem>
#include <iterator>
#include <streambuf>
#include <fstream>
#include "VttParser.h"
using namespace std;
void testReadVttFile(const string& filePath){
    if(!std::filesystem::exists(filePath)){
        cerr << "file does not exist" << endl;
        return;
    }
    ifstream vttStream(filePath, ifstream::in);
    string tmp;
    getline(vttStream, tmp);
    //dont increase this to <= 4, breaks BAD
    for(int i = 1; i < 4; ++i, getline(vttStream, tmp)){
        cout << tmp << endl;
    }
    vector<SubtitlesParser::Classes::SubtitleItem> subs = SubtitlesParser::Classes::Parsers::VttParser::ParseStream(vttStream);
    for(auto sub : subs){
        cout << sub.toString() << endl;
    }
}

int main(){
    testReadVttFile("/home/brado/programs/Cpp/test/ITZY “Not Shy” M_V-wTowEKjDGkU.ko.vtt");
}