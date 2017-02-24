add_library(markdown
    forwards.hpp
    stringParsers.hpp stringParsers.ipp stringParsers.cpp
    htmlParsers.hpp htmlParsers.ipp htmlParsers.cpp
    inlineParsers.hpp inlineParsers.ipp inlineParsers.cpp
    blockParser.cpp blockParser.ipp blockParser.hpp
    blockParser_heading.cpp blockParser_heading.ipp
    blockParser_list.cpp blockParser_list.ipp
    documentParser.hpp documentParser.ipp documentParser.cpp
)
