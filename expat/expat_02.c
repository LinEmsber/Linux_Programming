#include <expat.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int i;

// struct xml_entry
struct xml_entry {
	int memory_state;
	char *memory;
	size_t size;
};


// XML_StartElementHandler
static void startElement(void *userData, const XML_Char *name, const XML_Char **atts){

	struct xml_entry *xml_data = (struct xml_entry *) userData;

	for (i = 0; atts[i]; i++)
		printf("%d-th, atts: %s\n", i, (char*)atts[i]);
}

// XML_EndElementHandler
static void endElement(void *userData, const XML_Char *name){
	struct xml_entry *xml_data = (struct xml_entry *) userData;
}


static const char* xml =
    "<data>\n"\
    "    <header length=\"4\">\n"\
    "            <item name=\"time\" type=\"time\">16</item>\n"\
    "            <item name=\"ref\" type=\"string\">3843747</item>\n"\
    "            <item name=\"port\" type=\"int16\">0</item>\n"\
    "            <item name=\"frame\" type=\"int16\">20</item>\n"\
    "    </header>\n"\
    "</data>\n";


int main (void ) {


	int ret_XML_Parse;
	XML_Parser parser;
	struct xml_entry * xml_data = (struct xml_entry *)malloc( sizeof(struct xml_entry ) );
	xml_data -> memory = (char *)malloc(sizeof(char)*100);

	// Initialize parser
	parser = XML_ParserCreateNS(NULL, '\0');
	XML_SetUserData(parser, xml);
	XML_SetElementHandler(parser, startElement, endElement);


	// parse xml
	//
	// int XML_Parse(XML_Parser p, const char *s, int len, int isFinal)
	// The string s is a buffer containing part (or perhaps all) of the document. The number
	// of bytes of s that are part of the document is indicated by len.
	// The isFinal parameter informs the parser that this is the last piece of the document.
	ret_XML_Parse = XML_Parse(parser,
				xml_data -> memory,
				strlen(xml_data -> memory),
				XML_TRUE);

	if (ret_XML_Parse == XML_STATUS_ERROR)
		printf("Error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));

	XML_ParserFree(parser);

	return 0;
}
