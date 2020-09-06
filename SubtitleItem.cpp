//
// Created by brado on 9/3/20.
//

#include <numeric>
#include <utility>
#include <sstream>
#include <iomanip>
#include "SubtitleItem.h"
namespace SubtitlesParser::Classes {
    SubtitleItem::SubtitleItem() = default;

    SubtitleItem::SubtitleItem(chrono::milliseconds StartTime, chrono::milliseconds EndTime, std::vector<string> Lines)
            : StartTime(StartTime), EndTime(EndTime), Lines(std::move(Lines)) {

    }

    string durationToString(std::chrono::milliseconds duration) {
        ostringstream outstring;
        outstring << std::setfill('0') << std::setw(2)
                  << static_cast<int>(chrono::duration_cast<chrono::hours>(duration).count()) << ":"
                  << std::setfill('0') << std::setw(2)
                  << static_cast<int>(chrono::duration_cast<chrono::minutes>(duration).count()) % 60 << ":"
                  << std::setfill('0') << std::setw(2)
                  << static_cast<int>(chrono::duration_cast<chrono::seconds>(duration).count()) % 60 << "."
                  << std::setfill('0') << std::setw(3)
                  << static_cast<int>(chrono::duration_cast<chrono::milliseconds>(duration).count()) % 1000;
        return outstring.str();
    }

    std::ostream &operator<<(std::ostream &streamStr, const SubtitleItem &A) {
        streamStr << durationToString(A.StartTime) << " --> " << durationToString(A.EndTime) << ": " << A.Lines.size()
                  << std::accumulate(A.Lines.begin(), A.Lines.end(), std::string(""));
        return streamStr;
    }

    string SubtitleItem::toString(){
        return durationToString(this->StartTime) + " --> " + durationToString(this->EndTime) + ": " + to_string(this->Lines.size()) + " "
                  + std::accumulate(this->Lines.begin(), this->Lines.end(), std::string(""));
    }
}
