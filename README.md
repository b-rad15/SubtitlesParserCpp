# SubtitlesParserCpp
Parser for subtitle files (Currently only Vtt supported)

Based on https://github.com/AlexPoint/SubtitlesParser

If you want to add parsers for other formats, put in a pull request

To use simply pass an ifstream (via reference) to SubtitlesParser::Classes::Parsers::VttParser::ParseStream()

You'll probably wanna change the namespace cause thats long as anything
