// a program for generating Clone Hero patterns as game-ready chart folders
// #include <cmath>
#include <iostream>
#include <memory>

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
        std::unique_ptr<Pattern> m_quadVariant;

        u8 m_barsUntilInterpolatePattern = 0;
        ModifiableNoteSequence* m_interpolationSequence = nullptr;

        u8 m_defaultResolution = 16;
        std::vector<string> m_flags;
        std::vector<string> m_underchartStrings;
        std::vector<string> m_underchartFlags;
        std::unique_ptr<Pattern> m_underchartPattern;

        bool m_isUnderchart;

        Pattern(const string &inputRawString, bool isUnderchart = false)
        {
            m_isUnderchart = isUnderchart;
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

            if(isUnderchart) {
                this->m_underchartPattern = std::unique_ptr<Pattern>(this);
                return;
            }
            this->m_underchartStrings = slice(tokens, 8, tokens.size());
            if(CHPatterns_LOG_LEVEL >= CHPatternsLogLevelInfo) {
                std::cout << "assigned default underchartStrings" << std::endl;
            }
            createQuadVariant(tokens[3]);
            createUnderchartPattern();
        }
        // Pattern(const Pattern & p) {
        //     this->m_rawString = p.m_rawString;

        //     this->m_name = p.m_name;
        //     this->m_noteSeq = p.m_noteSeq;

        //     this->m_allowShifting = p.m_allowShifting;
        //     this->m_hasQuadVariant = p.m_hasQuadVariant;

        //     this->m_barsUntilInterpolatePattern = p.m_barsUntilInterpolatePattern;
        //     this->m_interpolationSequence = p.m_interpolationSequence;

        //     this->m_defaultResolution = p.m_defaultResolution;
        //     this->m_flags = p.m_flags;
        //     this->m_underchartStrings = p.m_underchartStrings;
        //     this->m_underchartFlags = p.m_underchartFlags;
        //     // this->m_quadVariant = std::unique_ptr<Pattern>(p.m_quadVariant.get());
        //     // this->m_underchartPattern = std::unique_ptr<Pattern>(p.m_underchartPattern.get());
        // }
    private:
        void createUnderchartPattern() {
            // TODO parse underchart stuff
            std::vector<string> underchartMethodFlags = split(m_underchartStrings[0], ',');
            std::cout << "underchartMethodFlags.size(): " << underchartMethodFlags.size() << std::endl;

            std::vector<string> underchartValues = slice(m_underchartStrings, 1, m_underchartStrings.size());

            this->m_underchartPattern = std::unique_ptr<Pattern>(new Pattern(this->m_rawString, true));
            this->m_underchartPattern->applyUndercharts(underchartMethodFlags, underchartValues);
        }
        void createQuadVariant(string &quadString) {
            this->m_quadVariant = std::unique_ptr<Pattern>(this);
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
        void applyUndercharts(std::vector<string> names, std::vector<string> values) {
            for(int i = 0; i < names.size(); i++) {
                applyUnderchart(names[i], values[i]);
            }
        }
        void applyUnderchart(string &name, string &value) {
            // TODO: byname, pattern and modflags underchart parsing
            if(name == "MODRES") {
                u8 newRes = m_defaultResolution;
                std::cout << "newRes: " << str(newRes) << std::endl;
                std::cout << "value: " << value << std::endl;
                for(int i = 0; i < value.length(); i += 2) {
                    string s = str(value[i+1]);
                    if(value[i] == '*') {
                        newRes *= parseU8(s);
                    } else if(value[i] == '/') {
                        newRes /= parseU8(s);
                    } else if(value[i] == '+') {
                        std::cout << "TODO: ADD MODRES '+'" << std::endl;
                    }
                }
                std::cout << "newRes: " << str(newRes) << std::endl;
                m_defaultResolution = newRes;
            }
        }
};


void logPattern(Pattern* pat, const string &name = "pat", const string &indentation = "") {
    std::cout << indentation << name << ": {" << std::endl;
    std::cout << indentation << "  raw string length: " << pat->m_rawString.length() << "," << std::endl;
    std::cout << indentation << "  name: " << pat->m_name << "," << std::endl;
    std::cout << indentation << "  allowShifting: " << pat->m_allowShifting << "," << std::endl;
    std::cout << indentation << "  hasQuadVariant: " << pat->m_hasQuadVariant << "," << std::endl;
    std::cout << indentation << "  barsUntilInterpolatePattern: " << u8ToString(pat->m_barsUntilInterpolatePattern) << "," << std::endl;
    std::cout << indentation << "  defaultResolution: " << u8ToString(pat->m_defaultResolution) << "," << std::endl;
    std::cout << indentation << "  flags.capacity(): " << pat->m_flags.capacity() << "," << std::endl;
    logStrings(pat->m_flags, ",", indentation + "  flags: [");
    std::cout << indentation << "  underchartStrings.capacity(): " << pat->m_underchartStrings.capacity() << std::endl;
    logStrings(pat->m_underchartStrings, ",", indentation + "  underchartStrings: [");
    if(!(pat->m_isUnderchart)) {
        logPattern(pat->m_underchartPattern.get(), "underchartPattern", indentation + "  ");
    }
    std::cout << indentation << "}" << std::endl;
}
int main(int argc, char* argv[])
{
    std::cout << "CHPatterns v" << CHPatterns_VERSION_MAJOR << "." << CHPatterns_VERSION_MINOR << std::endl;
    std::cout << "argcount: " << argc << ", argv: " << *argv << std::endl;
    std::cout << "creating pattern" << std::endl;
    Pattern* pat = new Pattern("trill	10	TRUE	FALSE	0		16		MODRES	/2");
    logPattern(pat);
    return 0;
}
