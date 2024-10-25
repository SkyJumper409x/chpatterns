// a program for generating Clone Hero patterns as game-ready chart folders
#include <cmath>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>

#include "VersionInfo.h"
#include "types.h"
#include "utils.hxx"
#include "LogFlags.hxx"

class Note {
    u8 lane;
    bool isHopo;
    bool isTap;
};

class NoteSequence {
    public:
        string rawString;
        std::vector<Note> notes;
        NoteSequence(string inputRawString) {
            this->rawString = inputRawString;
        }
        Note at(int index) {
            return notes[index];
        }
};
class ModifiableNoteSequence : public NoteSequence {
    public:
        bool shiftOpenNotes = false;
        ModifiableNoteSequence(string inputRawString) : NoteSequence(inputRawString) {

        }
        ModifiableNoteSequence* shift(s8 amount)
        {
            ModifiableNoteSequence* m = new ModifiableNoteSequence(this->rawString);
            // TODO
            return m;
        }
        ModifiableNoteSequence* shiftUp()
        {
            return shift(1);
        }
        ModifiableNoteSequence* shiftDown()
        {
            return shift(-1);
        }

};

class Pattern {
    public:
        string m_rawString;

        string m_name;
        ModifiableNoteSequence* m_noteSeq = nullptr;

        bool m_allowShifting = false;
        bool m_hasQuadVariant = false;
        Pattern* m_quadVariant = nullptr;

        u8 m_barsUntilInterpolatePattern = 0;
        ModifiableNoteSequence* m_interpolationSequence = nullptr;

        u8 m_defaultResolution = 16;
        std::vector<string> m_flags;
        std::vector<string> m_underchartStrings;
        std::vector<string> m_underchartFlags;
        Pattern* m_underchartPattern;

        Pattern(const string &inputRawString)
        {
            if(inputRawString == "empty") {
                return;
            }
            this->m_rawString = inputRawString;
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "splitting tokens" << std::endl;
            }
            std::vector<string> tokens = split(inputRawString, '\t');
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "split tokens" << std::endl;
            }
            this->m_name = tokens[0];

            this->m_noteSeq = new ModifiableNoteSequence(tokens[1]);
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "assigned noteSeq" << std::endl;
            }

            if(tokens[2] == "TRUE") {
                this->m_allowShifting = true;
            }

            this->m_barsUntilInterpolatePattern = parseU8(tokens[4]);
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "assigned barsUntilInterpolatePattern" << std::endl;
            }

            if(m_barsUntilInterpolatePattern > 0) {
                this->m_interpolationSequence = new ModifiableNoteSequence(tokens[5]);
                if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                    std::cout << "assigned interpolationSequence" << std::endl;
                }
            }

            this->m_defaultResolution = parseU8(tokens[6]);
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "assigned defaultResolution" << std::endl;
            }

            this->m_flags = split(tokens[7], ','); // TODO more specific flag parsing
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "assigned flags" << std::endl;
            }

            this->m_underchartStrings = slice(tokens, 8, tokens.size());
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "assigned default underchartStrings" << std::endl;
            }
            this->createQuadVariant(tokens[3]);

            createUnderchartPattern();
        }
    private:
        void createUnderchartPattern() {
            // TODO parse underchart stuff
            std::vector<string> underchartMethodFlags = split(m_underchartStrings[0], ',');
            std::cout << "underchartMethodFlags.size(): " << underchartMethodFlags.size() << std::endl;
            s8 patternIdx = -1, modresIdx = -1, bynameIdx = -1, modflagsIdx = -1;
            string patternString;
            string modresString;
            string bynameString;
            string modflagsString;

            for(s8 i = 0; i < underchartMethodFlags.size(); i++) {
                std::cout << "in loop, processing underchartMethodFlags[i]: " << std::endl;
                std::cout << '"'<< underchartMethodFlags[i] << '"' << std::endl;
                if(underchartMethodFlags[i] == "PATTERN")  {
                    std::cout << "got PATTERN" << std::endl;
                    patternIdx = i;
                    patternString = underchartMethodFlags[patternIdx+1];
                }
                if(underchartMethodFlags[i] == "MODRES")  {
                    std::cout << "got MODRES" << std::endl;
                    modresIdx = i;
                    std::cout << "got modresIdx" << std::endl;
                    modresString = underchartMethodFlags[(modresIdx+1)];
                    std::cout << "got modresString" << std::endl;
                }
                if(underchartMethodFlags[i] == "BYNAME")  {
                    std::cout << "got BYNAME" << std::endl;
                    bynameIdx = i;
                    bynameString = underchartMethodFlags[bynameIdx+1];
                }
                if(underchartMethodFlags[i] == "MODFLAGS")  {
                    std::cout << "got MODFLAGS" << std::endl;
                    modflagsIdx = i;
                    modflagsString = underchartMethodFlags[modflagsIdx+1];
                }
            }
            std::cout << "endloop" << std::endl;

            std::cout << "underchart method flags: [ " << std::to_string((signed)patternIdx) << ", " << std::to_string((signed)modresIdx) << ", " << std::to_string((signed)bynameIdx) << ", " << std::to_string((signed)modflagsIdx) << " ]" << std::endl;
            if((patternIdx > 0) || (bynameIdx > 0)) {
                // TODO: byname and pattern underchart parsing
                this->m_underchartPattern = new Pattern("empty");
            } else {
                this->m_underchartPattern = new Pattern(this->m_rawString);
                if(modresIdx > 0) {
                    u8 newRes = this->m_defaultResolution;
                    for(int i = 0; i < modresString.length(); i += 2) {
                        if(modresString[i] == '*') {
                            newRes *= parseU8(std::to_string(modresString[i+1]));
                        } else if(modresString[i] == '/') {
                            newRes /= parseU8(std::to_string(modresString[i+1]));
                        } else if(modresString[i] == '+') {
                            std::cout << "TODO: ADD MODRES '+'";
                        }
                    }
                }
            }
        }
        void createQuadVariant(string &quadString) {
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
            std::cout << "called createQuadVariant" << std::endl;
            std::cout << "quadString: \"" << (quadString) << "\"" << std::endl;
            }
            if(quadString == "AUTO") {
                // TODO
            } else if(substring("", 0, 8) == "PATTERN,") { // TODO check if slicing like this is even a thing in c++
                // TODO
            } else if(quadString == "FALSE") {
                return;
            }
        }
};
int main(int argc, char* argv[])
{
    std::cout << "CHPatterns v" << CHPatterns_VERSION_MAJOR << "." << CHPatterns_VERSION_MINOR << std::endl;
    std::cout << "argcount: " << argc << ", argv: " << *argv << std::endl;
    std::cout << "creating pattern" << std::endl;
    Pattern pat("trill	10	TRUE	FALSE	0		16		MODRES	/2");
    std::cout << "pat: {" << std::endl;
    std::cout << "  raw string length: " << pat.m_rawString.length() << "," << std::endl;
    std::cout << "  name: " << pat.m_name << "," << std::endl;
    std::cout << "  allowShifting: " << pat.m_allowShifting << "," << std::endl;
    std::cout << "  hasQuadVariant: " << pat.m_hasQuadVariant << "," << std::endl;
    std::cout << "  barsUntilInterpolatePattern: " << u8ToString(pat.m_barsUntilInterpolatePattern) << "," << std::endl;
    std::cout << "  defaultResolution: " << u8ToString(pat.m_defaultResolution) << "," << std::endl;
    std::cout << "  flags.capacity(): " << pat.m_flags.capacity() << "," << std::endl;
    std::cout << "  underchartStrings.capacity(): " << pat.m_underchartStrings.capacity() << std::endl;
    std::cout << "}" << std::endl;
    return 0;
}
