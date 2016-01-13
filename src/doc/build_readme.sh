#
# grab the mainpage_template.txt file and build
# README.md and maindpage.md
#

# the sed thing strips out doxygen's toc markings
cat README.header >../../README.md
cat mainpage.template|sed s/{#[a-zA-Z0-9-]*}//|sed s/~~~/\`\`\`/\
		     |sed "s/#concept#/![Image depicting flow of data][ConceptSvg]/"\
		     |sed "s/#TemplateSyntax#/![TemplateSvg]/"\
			 |sed "s/#PlainTextSyntax#/![PlainTextSvg]/"\
		     |sed "s/#InstructionSyntax#/![InstructionSvg]/"\
		      >>../../README.md
cat README.footer >>../../README.md
doctoc ../../README.md

#
# build the mainpage.md file
#
cat mainpage.header > mainpage.md
cat mainpage.template |sed "s/#concept#/@diafile concept.dia width=10cm/"\
		      |sed "s/#TemplateSyntax#/@image html template.svg/"\
              |sed "s/#PlainTextSyntax#/@image html plaintext.svg/"|\
		      |sed "s/#InstructionSyntax#/@image html instruction.svg/"\
		       >> mainpage.md
cat mainpage.footer >> mainpage.md

