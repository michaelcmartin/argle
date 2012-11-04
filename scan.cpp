#include "scan.h"
#include <ctype.h>

namespace {
    typedef enum { NEUTRAL,
                   IN_TOKEN,
                   IN_STRING, STRING_ESCAPE,
                   IN_CHAR, CHAR_ESCAPE,
                   SLASH, WING_COMMENT, WING_STAR, LINE_COMMENT } state_t;
}

void
scan(FILE *f, ScannerCallback &cb)
{
    state_t state = NEUTRAL;
    std::string currentToken;
    while (true) {
        int c = fgetc(f);
        if (c == EOF) {
            if (state == IN_TOKEN) {
                cb.registerToken(currentToken);
            }
            return;
        }
        switch (state) {
        case NEUTRAL:
            if (isalpha(c) || c == '_') {
                state = IN_TOKEN;
                currentToken.clear();
                currentToken += c;
                break;
            }
            if (c == '/') {
                state = SLASH;
                break;
            }
            if (c == '\"') {
                state = IN_STRING;
                break;
            }
            if (c == '\'') {
                state = IN_CHAR;
                break;
            }
            // stay in NEUTRAL
            break;
        case IN_TOKEN:
            if (isalnum(c) || c == '_') {
                currentToken += c;
            } else {
                cb.registerToken(currentToken);
                currentToken.clear();
                state = NEUTRAL;
            }
            break;
        case IN_STRING:
            if (c == '\\') {
                state = STRING_ESCAPE;
            } else if (c == '\"') {
                state = NEUTRAL;
            } else if (c == '\n') {
                state = NEUTRAL;
                fprintf(stderr, "WARNING: encountered an unterminated string constant\n");
            }
            break;
        case STRING_ESCAPE:
            state = IN_STRING;
            break;
        case IN_CHAR:
            if (c == '\\') {
                state = CHAR_ESCAPE;
            } else if (c == '\'') {
                state = NEUTRAL;
            } else if (c == '\n') {
                state = NEUTRAL;
                fprintf(stderr, "WARNING: encountered an unterminated character constant\n");
            }
            break;
        case CHAR_ESCAPE:
            state = IN_CHAR;
            break;
        case SLASH:
            if (c == '*') {
                state = WING_COMMENT;
            } else if (c == '/') {
                state = LINE_COMMENT;
            } else if (isalpha(c)) {
                state = IN_TOKEN;
                currentToken.clear();
                currentToken += c;
            } else {
                state = NEUTRAL;
            }
            break;
        case WING_COMMENT:
            if (c == '*') {
                state = WING_STAR;
            }
            break;
        case WING_STAR:
            if (c == '/') {
                state = NEUTRAL;
            } else if (c != '*') {
                state = WING_COMMENT;
            }
            break;
        case LINE_COMMENT:
            if (c == '\n') {
                state = NEUTRAL;
            }
            break;
        default:
            fprintf (stderr, "WARNING: Got into an unknown state %d\n", state);
            return;
        }
    }
}
