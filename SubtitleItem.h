//
// Created by brado on 9/3/20.
//

#ifndef VTTPARSER_SUBTITLEITEM_H
#define VTTPARSER_SUBTITLEITEM_H


#include <ostream>
#include <vector>
#include <chrono>

using namespace std;
namespace SubtitlesParser::Classes {
    class SubtitleItem {
    public:
        //Constructor
        SubtitleItem();

        SubtitleItem(chrono::milliseconds StartTime, chrono::milliseconds EndTime, vector<string> Lines);

        //Start time in ms
        chrono::milliseconds StartTime;
        //End time in ms
        chrono::milliseconds EndTime;
        //TODO:Add line return combing lines with some separator
        std::vector<std::string> Lines;


        //Overrides
        friend std::ostream &operator<<(std::ostream &, const SubtitleItem &);

        string toString();
    };
    static string durationToString(std::chrono::milliseconds duration);
}
#endif //VTTPARSER_SUBTITLEITEM_H
