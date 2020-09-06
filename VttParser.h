//
// Created by brado on 9/3/20.
//

#ifndef VTTPARSER_VTTPARSER_H
#define VTTPARSER_VTTPARSER_H


#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>

#include "SubtitleItem.h"

using namespace std;
namespace SubtitlesParser::Classes::Parsers {
    class VttParser {
    public:
        static const vector<string> vttDelimiters;

        static vector<SubtitleItem> ParseStream(ifstream&);

        static const regex timeRegexWithHours;
        static const regex timeRegexWithoutHours;
        static chrono::milliseconds ParseVttTimecode(string s);

        static vector<string> GetVttSubTitleParts(ifstream &reader);

        static bool TryParseTimecodeLine(string line, chrono::milliseconds &startTc, chrono::milliseconds &endTc);
    };
#if 0
    typedef std::chrono::duration<int, std::ratio<60 * 60>> hours_type;
    typedef std::chrono::duration<int, std::ratio<60>> minute_type;
    typedef std::chrono::duration<int> seconds_type;
    typedef std::chrono::duration<int, std::milli> milliseconds_type;
#endif
    static std::string ReplaceString(std::string subject, const std::string &search, const std::string &replace);
    static vector<string> regexSplit(string s, vector<string> delims);
}
#endif //VTTPARSER_VTTPARSER_H
