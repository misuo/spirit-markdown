cmake_minimum_required(VERSION 3.0)

project(spirit-markdown CXX)

# Set compiler according C++XX support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX17)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++17 support.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(COMPILER_SUPPORTS_CXX14)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++14 support.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++11 support.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++0x support.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_BUILD_TYPE Debug)



find_package(Boost 1.42 REQUIRED COMPONENTS filesystem iostreams program_options unit_test_framework system thread)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_definitions("-DBOOST_TEST_DYN_LINK")

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)

add_executable(spirit-markdown
    ${spiritMarkdown_files}
    src/parseError.hpp
    src/spiritMarkdown.cpp
    )

target_link_libraries(spirit-markdown
    dom
    markdown
    html
    ${Boost_LIBRARIES}
    )

add_executable(grammarTests
    #${spiritMarkdown_files}
    src/generatorPredicates.hpp
    src/parserPredicates.hpp
    src/markdown/stringParserTests.cpp
    src/markdown/htmlParserTests.cpp
    src/markdown/inlineParserTests.cpp
    src/html/inlineGeneratorTests.cpp
    src/markdown/headingParserTests.cpp
    src/markdown/listParserTests.cpp
    src/markdown/blockParserTests.cpp
    src/markdown/documentParserTests.cpp
    src/html/blockGeneratorTests.cpp
    )

target_link_libraries(grammarTests
    dom
    markdown
    html
    ${Boost_LIBRARIES}
    )

enable_testing()
#add_test(NAME grammarTests COMMAND grammarTests)
#set_tests_properties( grammarTests PROPERTIES DEPENDS grammarTests)

# Tip for collecting Boost unit tests couresy https://eb2.co/blog/2015/06/driving-boost-dot-test-with-cmake/
function(add_boost_test SOURCE_FILE_NAME TEST_MODULE TEST_EXECUTABLE_NAME)
    #get_filename_component(TEST_EXECUTABLE_NAME ${SOURCE_FILE_NAME} NAME_WE)
 
    #add_executable(${TEST_EXECUTABLE_NAME} ${SOURCE_FILE_NAME})
    #target_link_libraries(${TEST_EXECUTABLE_NAME} ${DEPENDENCY_LIB} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
 
    file(READ "${SOURCE_FILE_NAME}" SOURCE_FILE_CONTENTS)
    string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)" 
           FOUND_TESTS ${SOURCE_FILE_CONTENTS})
 
    foreach(HIT ${FOUND_TESTS})
        string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${HIT})
 
        add_test(NAME "${TEST_MODULE}.${TEST_NAME}" COMMAND ${TEST_EXECUTABLE_NAME}  -t "${TEST_MODULE}/${TEST_NAME}" --catch_system_error=yes)
    endforeach()
endfunction()

add_boost_test(src/markdown/stringParserTests.cpp StringParserTests grammarTests)
add_boost_test(src/markdown/htmlParserTests.cpp HtmlParserTests grammarTests)
add_boost_test(src/markdown/inlineParserTests.cpp InlineParserTests grammarTests)
add_boost_test(src/html/inlineGeneratorTests.cpp InlineGeneratorTests grammarTests)
add_boost_test(src/markdown/headingParserTests.cpp HeadingParserTests grammarTests)
add_boost_test(src/markdown/listParserTests.cpp ListParserTests grammarTests)
add_boost_test(src/markdown/blockParserTests.cpp BlockParserTests grammarTests)
add_boost_test(src/markdown/documentParserTests.cpp DocumentParserTests grammarTests)
add_boost_test(src/html/blockGeneratorTests.cpp BlockGeneratorTests grammarTests)

# Old way to run the grammarTests - make testx
add_custom_target(testx COMMAND grammarTests DEPENDS grammarTests )
