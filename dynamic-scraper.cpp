#include "webdriverxx.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace webdriverxx;

int main() {
    WebDriver ff = Start(Firefox());
    ff.Navigate("https://www.scrapethissite.com/pages/ajax-javascript");
    ff.FindElement(ById("2015")).Click();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    auto find_films = [&]() { return ff.FindElements(ByClass("film-title")); };
    std::vector<Element> films = WaitForValue(find_films);
    std::cout << "Found " << films.size() << " films" << std::endl;
    for (Element film : films) {
        std::cout << film.GetText() << std::endl;
    }
    return 0;
}