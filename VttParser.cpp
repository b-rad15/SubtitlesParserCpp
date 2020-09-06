//
// Created by brado on 9/3/20.
//

/// <summary>
/// Parser for the .vtt subtitles files. Does not handle formatting tags within the text; that has to be parsed separately.
///
/// A .vtt file looks like:
/// WEBVTT
///
/// CUE - 1
/// 00:00:10.500 --> 00:00:13.000
/// Elephant's Dream
///
/// CUE - 2
/// 00:00:15.000 --> 00:00:18.000
/// At the left we can see...
/// Code based on https://github.com/AlexPoint/SubtitlesParser/blob/master/SubtitlesParser/Classes/Parsers/VttParser.cs
/// </summary>
using namespace std;


#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include "VttParser.h"

namespace SubtitlesParser::Classes::Parsers {

    const vector<string> VttParser::vttDelimiters = {"-->", "- >", "->"};
    // Note: WebVTT standard only supports a "." not a "," left in for compatibilty
    // Regex compile very slow, leave here for speed
    const regex VttParser::timeRegexWithHours("([0-9]+):([0-9]+):([0-9]+)[,\\.]([0-9]+)");
    const regex VttParser::timeRegexWithoutHours("([0-9]+):([0-9]+)[,\\.]([0-9]+)");

    std::vector<SubtitleItem> VttParser::ParseStream(ifstream &vttStream) {
        // test if stream if readable and seekable (just a check, should be good)
        if (!vttStream.is_open()) {
            stringstream message;
            message << "Stream must be open and readable in a subtitles parser. " <<
                    "Operation interrupted; isOpen: " << vttStream.is_open();
            throw std::runtime_error(message.str());
        }

        // seek the beginning of the stream (unnecessary I think)
        //vttStream.seekg(0);

        std::vector<SubtitleItem> items;
        std::vector<string> vttSubParts = GetVttSubTitleParts(vttStream);
        if (!vttSubParts.empty()) { //Check if returned good value
            for (const string &vttSubPart : vttSubParts) {
                std::vector<string> lines;
                string tmp;
                istringstream vttSubStream(vttSubPart);
                while (getline(vttSubStream, tmp)) {
                    //trim before place?
                    boost::algorithm::trim(tmp);
                    if (!tmp.empty())
                        lines.emplace_back(tmp);
                }
                SubtitleItem item;
                auto linesIterator = lines.begin(), linesEnd = lines.end();
                bool success = TryParseTimecodeLine(*linesIterator, item.StartTime, item.EndTime);
                if (success) {
                    //TODO:Do some check
                } else {
                    //TODO:throw an error?
                }
                for (++linesIterator; linesIterator != linesEnd; ++linesIterator) {
                    // we found the timecode, now we get the text
                    item.Lines.push_back(*linesIterator);
                }
                if ((item.StartTime != chrono::milliseconds::zero() || item.EndTime != chrono::milliseconds::zero()) &&
                    !item.Lines.empty()) {
                    // parsing succeeded
                    items.push_back(item);
                }
            }

            return items;
        } else {
            throw std::runtime_error("Parsing as VTT returned no VTT part.");
        }
    }

/// <summary>
/// Enumerates the subtitle parts in a VTT file based on the standard line break observed between them.
/// A VTT subtitle part is in the form:
///
/// CUE - 1
/// 00:00:20.000 --> 00:00:24.400
/// Altocumulus clouds occur between six thousand
///
/// The first line is optional, as well as the hours in the time codes.
/// </summary>
/// <param name="reader">The textreader associated with the vtt file</param>
/// <returns>An IEnumerable(string) object containing all the subtitle parts</returns>
    vector<string> VttParser::GetVttSubTitleParts(ifstream &reader) {
        string line;
        stringstream sb;
        vector<string> parts;
        for (getline(reader, line); reader.good(); getline(reader, line)) {
            boost::algorithm::trim(line);
            if (line.empty()) {
                // add to list only if not empty
                string res = sb.str();
                if (!res.empty()) {
                    parts.push_back(res);
                }
                stringstream newone;
                sb.swap(newone);
            } else {
                sb << line << std::endl;
            }
        }

        if (sb.tellp() > 0) {
            string res = sb.str();
            if (!res.empty()) {
                parts.push_back(res);
            }
        }
        return parts;
    }

/// <summary>
/// Takes an VTT timecode as a string and parses it into a double (in seconds). A VTT timecode reads as follows:
/// 00:00:20.000
/// or
/// 00:20.000
/// </summary>
/// <param name="s">The timecode to parse</param>
/// <returns>The parsed timecode as a TimeSpan instance. If the parsing was unsuccessful, -1 is returned (subtitles should never show)</returns>
    std::chrono::milliseconds VttParser::ParseVttTimecode(string s) {
        string timeString;
        smatch matches;
        std::chrono::milliseconds timestamp = chrono::milliseconds(0);
        if (regex_match(s, matches, timeRegexWithHours)) {
            //First Match is the whole string
            //TODO:Check if match has 5 elements here, throw error
            timeString = matches[0];
            timestamp = chrono::hours(stoi(matches[1])) + chrono::minutes(stoi(matches[2])) +
                        chrono::seconds(stoi(matches[3])) + chrono::milliseconds(stoi(matches[4]));
        } else {
            // Note: WebVTT standard only supports a . not a , left in for compatibilty
            if (regex_match(s, matches, timeRegexWithoutHours)) {
                //First Match is the whole string
                //TODO:Check if match has 4 elements here, throw error
                timeString = "00:" + matches[0].str();
                timestamp = chrono::minutes(stoi(matches[1])) + chrono::seconds(stoi(matches[2])) +
                            chrono::milliseconds(stoi(matches[3]));
            }
        }

        if (timeString.empty()) {
            throw runtime_error("Time not found in string" + s);
        }

        return timestamp;
    }

    bool VttParser::TryParseTimecodeLine(string line, chrono::milliseconds &startTc, chrono::milliseconds &endTc) {
        vector<string> parts;
        vector<string> vec;
        auto it = line.begin(), end = line.end();
        //TODO: Refactor to work with any number of delimiters in list
        bool res = boost::spirit::qi::parse(it, end,
                                            boost::spirit::qi::as_string[*(boost::spirit::qi::char_ - vttDelimiters[0] -
                                                                           vttDelimiters[1] - vttDelimiters[2])] %
                                            (boost::spirit::qi::lit(vttDelimiters[0]) |
                                             boost::spirit::qi::lit(vttDelimiters[1]) |
                                             boost::spirit::qi::lit(vttDelimiters[2])),
                                            parts);
//        parts = regexSplit(line, vttDelimiters); //slower than above even if simpler
        if (parts.size() != 2) {
            // this is not a timecode line
            startTc = chrono::milliseconds::zero();
            endTc = chrono::milliseconds::zero();
            return false;
        } else {
            boost::algorithm::trim(parts[0]);
            boost::algorithm::trim(parts[1]);
            startTc = ParseVttTimecode(parts[0]);
            endTc = ParseVttTimecode(parts[1]);
            return true;
        }
    }

#if 0 //unused for now, ignore
    std::string ReplaceString(std::string subject, const std::string &search,
                              const std::string &replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }

    vector<string> regexSplit(string s, vector<string> delims) {
        auto delimIter = delims.begin(), delimEnd = delims.end();
        // matches any characters that need to be escaped in RegEx
        std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
        string rgxString = std::regex_replace(*delimIter, specialChars, R"(\$&)");

        for (++delimIter; delimIter != delimEnd; ++delimIter) {
            std::string sanitized = std::regex_replace(*delimIter, specialChars, R"(\$&)");
            rgxString += "|" + sanitized;
        }
        regex rgx(rgxString);
        std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
        std::sregex_token_iterator end;
        vector<string> splits;
        for (; iter != end; ++iter)
            splits.push_back(*iter);
        return splits;
    }
#endif
}