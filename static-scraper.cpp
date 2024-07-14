#include <iostream>
#include "cpr/cpr.h"
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"
#include <format>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"https://quotes.toscrape.com/"});
    if (r.status_code != 200) {
        std::cerr << "Failed to fetch the page" << std::endl;
        return 1;
    }

    htmlDocPtr doc = htmlReadMemory(r.text.c_str(), r.text.length(), nullptr, nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
    if (doc == nullptr) {
        std::cerr << "Failed to parse the page" << std::endl;
        return 1;
    }

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == nullptr) {
        std::cerr << "Failed to create XPath context" << std::endl;
        return 1;
    }

    xmlXPathObjectPtr quotes = xmlXPathEvalExpression((xmlChar *) "//div[@class='quote']", context);
    if (quotes == nullptr) {
        std::cerr << "Failed to evaluate XPath expression" << std::endl;
        return 1;
    }

    for (int i = 0; i < quotes->nodesetval->nodeNr; i++) {
        xmlNodePtr quote = quotes->nodesetval->nodeTab[i];
        xmlXPathSetContextNode(quote, context);
        xmlNodePtr author = xmlXPathEvalExpression((xmlChar *) ".//small[@class='author']", context)->nodesetval->nodeTab[0];
        if (author == nullptr) {
            std::cerr << "Failed to evaluate XPath expression" << std::endl;
            return 1;
        }
        xmlNodePtr text = xmlXPathEvalExpression((xmlChar *) ".//span[@class='text']", context)->nodesetval->nodeTab[0];
        if (text == nullptr) {
            std::cerr << "Failed to evaluate XPath expression" << std::endl;
            return 1;
        }
        std::string author_text = reinterpret_cast<const char *>(xmlNodeGetContent(author));
        std::string quote_text = reinterpret_cast<const char *>(xmlNodeGetContent(text));

        std::cout << std::format("{}: {}", author_text, quote_text) << std::endl;
    }
    return 0;
}