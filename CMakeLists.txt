cmake_minimum_required(VERSION 3.5)
project(scraper VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_Declare(webdriverxx GIT_REPOSITORY https://github.com/GermanAizek/webdriverxx
                         GIT_TAG 0b04c449b6f187ecff67addaf1a22ae23a43afd9
                         GIT_SUBMODULES "") # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases

FetchContent_MakeAvailable(cpr webdriverxx)

add_executable(static-scraper static-scraper.cpp)
target_compile_features(static-scraper PRIVATE cxx_std_20)

target_link_libraries(static-scraper PRIVATE cpr::cpr)

find_package(LibXml2 REQUIRED)
target_link_libraries(static-scraper PRIVATE LibXml2::LibXml2)

add_executable(dynamic-scraper dynamic-scraper.cpp)
target_compile_features(dynamic-scraper PRIVATE cxx_std_20)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
link_directories(${CURL_LIBRARIES})

target_include_directories(dynamic-scraper PRIVATE "${webdriverxx_SOURCE_DIR}/src/include")

target_link_libraries(dynamic-scraper PRIVATE ${CURL_LIBRARIES})