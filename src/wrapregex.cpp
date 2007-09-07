#include <iostream>
#include <string>
#include <ctype.h>
#include <stdexcept>
#include "wrapregex.h"
#include "contexthandler.h"

using namespace std;

WrapRegex::WrapRegex (
    const string& pattern,
    bool matchCase,
    const string& replaceParameter,
    const int arrayLengthParameter ) :
        replace ( replaceParameter ),
        arrayLength ( arrayLengthParameter )
{
    if ( pattern.empty() || pattern == ".*" )
    {
        disabled = true;
        return;
    }
    disabled = false;

    matchArray = new int[arrayLength];

    // compile
    int optionsFlag = ( matchCase ) ? PCRE_UTF8 : PCRE_CASELESS | PCRE_UTF8;
    const char *errorPointer;
    int errorOffset;

    if ( ( patternStructure = pcre_compile (
                                  pattern.c_str(),
                                  optionsFlag,
                                  &errorPointer,
                                  &errorOffset,
                                  NULL ) ) == NULL )
    {
        throw runtime_error ( errorPointer );
    }

    patternExtraStructure = pcre_study ( patternStructure, 0, &errorPointer );
}

WrapRegex::~WrapRegex()
{
    if ( disabled )
        return;

    try
    {
        pcre_free ( patternStructure );
        pcre_free ( patternExtraStructure );
        delete[] matchArray;
    }
    catch ( ... )
    {
        throw runtime_error ( "Wrapregex::~WrapRegex" );
    }
}

int WrapRegex::matchPatternGlobal (
    string &buffer,
    vector<ContextMatch> &matchVector,
    unsigned elementCount,
    int context )
{
    if ( disabled )
        return 0;

    return matchPatternGlobal_ (
               buffer.c_str(),
               buffer.size(),
               matchVector,
               elementCount,
               context );
}

string WrapRegex::replaceGlobal (
    const string& buffer,
    int *matchCount )
{
    *matchCount = 0;

    if ( disabled )
        return buffer;

    char *s = ( char * ) buffer.c_str();

    string output, match;

    output.reserve ( buffer.size() );
    while ( ( returnValue = pcre_exec (
                                patternStructure,
                                patternExtraStructure,
                                s,
                                strlen ( s ),
                                0,
                                0,
                                matchArray,
                                arrayLength ) ) >= 0 )
    {
        ++ ( *matchCount );

        output.append ( s, matchArray[0] );

        match.clear();
        match.append ( s + matchArray[0], matchArray[1] - matchArray[0] );
        output.append ( getInterpolatedString_ ( s, ( char * ) replace.c_str() ) );
        s += matchArray[1];
    }
    output.append ( s );
    return output;
}

int WrapRegex::matchPatternGlobal_ (
    const char *buffer,
    size_t buflen,
    vector<ContextMatch> &matchVector,
    unsigned elementCount,
    int context )
{
    if ( disabled )
        return 0;

    char *s, *origin;
    int matchcount;
    size_t offset;
    ContextMatch match;

    s = origin = ( char * ) buffer;
    matchcount = 0;
    offset = 0;

    while ( ( returnValue = pcre_exec (
                                patternStructure,
                                patternExtraStructure,
                                s,
                                buflen,
                                offset,
                                0,
                                matchArray,
                                arrayLength ) ) >= 0 )
    {
        ++matchcount;

        if ( context )
        {
            match = ContextHandler::getContext (
                        s + matchArray[0],
                        matchArray[1] - matchArray[0],
                        origin,
                        context );
        }
        else
        {
            match.prelog = match.postlog = "";
            match.match.assign ( s + matchArray[0], matchArray[1] - matchArray[0] );
        }

        // record element and offset information
        match.elementCount = elementCount;
        match.offset = matchArray[0];

        if ( replace != "" )
            match.replace = getInterpolatedString_ ( s, ( char * ) replace.c_str() );

        matchVector.push_back ( match );

        if ( ( offset = matchArray[1] ) >= buflen )
            break;
    }
    return matchcount;
}

string WrapRegex::getInterpolatedString_ ( char *buffer, char *source )
{
    if ( disabled )
        return "";

    char *s, *origin;
    s = origin = ( char * ) source;

    string interpol_string;

    int escapeState = false;
    for ( ; *s; ++s )
    {
        if ( *s == '\\' )
        {
            escapeState = ( escapeState ) ? false : true;
            if ( escapeState )
            {
                if ( isdigit ( * ( s + 1 ) ) )
                {
                    char *number, *it;
                    number = s + 1;
                    for ( it = number; *it && isdigit ( * ( it + 1 ) ); ++it )
                        ;
                    size_t len = it - s;
                    char *tmp = new char[len + 1];
                    memcpy ( tmp, number, sizeof ( char ) * len );
                    * ( tmp + len ) = '\0';
                    int i = atoi ( tmp );
                    delete[] tmp;

                    interpol_string += getSubpattern_ ( buffer, i );

                    s += len;
                    escapeState = false;
                }
                else if ( * ( s + 1 ) == 't' )
                {
                    interpol_string += '\t';
                    ++s;
                    escapeState = false;
                }
                else if ( * ( s + 1 ) == 'n' )
                {
                    interpol_string += '\n';
                    ++s;
                    escapeState = false;
                }
                else
                    interpol_string += *s;
            }
            else
                interpol_string += *s;
        }
        else
            interpol_string += *s;
    }
    return interpol_string;
}

string WrapRegex::getSubpattern_ ( char *s, unsigned subpattern )
{
    if ( disabled )
        return "";

    const char *sub;
    int ret = pcre_get_substring ( s, matchArray, returnValue, subpattern, &sub );
    if ( ret == PCRE_ERROR_NOSUBSTRING || ret == PCRE_ERROR_NOMEMORY )
        return "";
    string subString ( sub );
    pcre_free ( ( char * ) sub );
    return subString;
}
