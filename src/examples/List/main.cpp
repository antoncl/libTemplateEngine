#include <iostream>

#include "../../TemplateEngine/include/TemplateEngine.hpp"
namespace te = template_engine;

int main(int argc, char **argv) {
    
    
    // Template to compile
    te::StringScanner reader(TE_TEXT("<!-- Generated by {{APP}}-{{VERSION}} on {{TIME}}-->\n"
	"<chocolates>\n"
	"{{- the COLOR value is - in this example - taken from the dictionary in the outer scope}}"
	"{{#REPEAT chocolates}}"
	    "\t<chocolate color=\"{{COLOR}}\" type=\"{{TYPE}}\">{{NAME}}</chocolate>\n"
	"{{/REPEAT}}"
	"</chocolates>"));
    
    // Build the dictionarylist for expanding chokolate elements
    te::DictionaryListPtr chokolates = std::make_shared<te::DictionaryList>();
    chokolates->add(TE_TEXT("COLOR"), TE_TEXT("Brown"));
    
    // first element
    te::DictionaryPtr subDictionary = std::make_shared<te::Dictionary>();
    subDictionary->add(TE_TEXT("TYPE"), TE_TEXT("Bar"));
    subDictionary->add(TE_TEXT("NAME"), TE_TEXT("3 Musketeers"));
    chokolates->add(subDictionary);
    // second element
    subDictionary = std::make_shared<te::Dictionary>();
    subDictionary->add(TE_TEXT("COLOR"), TE_TEXT("Light brown"));
    subDictionary->add(TE_TEXT("TYPE"), TE_TEXT("Milk"));
    subDictionary->add(TE_TEXT("NAME"), TE_TEXT("Bournville"));
    chokolates->add(subDictionary);

    // build the dictionary for expanding the chokolate element
    te::DictionaryPtr chokolate = std::make_shared<te::Dictionary>();
    chokolate->add(TE_TEXT("chocolates"), chokolates);
    
    // Setup the template context
    te::ContextPtr context = te::Context::BuildContext();

    // Add root dictionary to context
    context->setDictionary(chokolate);
    
    // compile the template
    te::TemplatePtr compiledTemplate = te::Template::parse(reader);

    // render the template
    te::te_string result = compiledTemplate->render(context);

    // convert template result to UTF-8
    te::te_converter converter;

    // print the generated template
    std::cout << converter.to_bytes(result) << std::endl;

    return 0;
}
