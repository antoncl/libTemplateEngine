@echo off
rem
rem  grab the mainpage_template.txt file and build
rem  README.md and maindpage.md
rem
rem the sed thing strips out doxygen's toc markings
type README.header >../../README.md
type mainpage.template|sed "s/{#[a-zA-Z0-9-]*}//"|^
sed "s/~~~/\`\`\`/"|^
sed "s/#concept#/![Image depicting flow of data][ConceptSvg]/"|^
sed "s/#TemplateSyntax#/![TemplateSvg]/"|^
sed "s/#PlainTextSyntax#/![PlainTextSvg]/"|^
sed "s/#InstructionSyntax#/![InstructionSvg]/" >>../../README.md
type README.footer >>../../README.md
rem doctoc ../../README.md

rem
rem build the mainpage.md file
rem
type mainpage.header > mainpage.md
type mainpage.template |sed "s/#concept#/@diafile concept.dia width=10cm/"|^
sed "s/#TemplateSyntax#/@image html template.svg/"|^
sed "s/#PlainTextSyntax#/@image html plaintext.svg/"|^
sed "s/#InstructionSyntax#/@image html instruction.svg/" >> mainpage.md
type mainpage.footer >> mainpage.md

