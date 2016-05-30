/* An example of expat API */

#include <stdio.h>
#include <expat.h>
#define BUFFSIZE	8192

char Buff[BUFFSIZE];
int Depth;

void start(void *data, const char *el, const char **attr)
{
	int i;

	for (i = 0; i < Depth; i++)
		printf("  ");
	printf("%s", el);

	for (i = 0; attr[i]; i += 2)
		printf(" %s='%s'", attr[i], attr[i + 1]);

	printf("\n");
	Depth++;
}

void end(void *data, const char *el) {
    Depth--;
}

void main(int argc, char **argv)
{

	// Construct a new parser.
	// XML_Parser XML_ParserCreate(const XML_Char*encoding)
	XML_Parser p = XML_ParserCreate(NULL);
	if (p == 0) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}

	// XML_SetElementHandler
	//
	// XML_SetElementHandler(XML_Parser p,
	//                       XML_StartElementHandler start,
	//                       XML_EndElementHandler end);
	//
	// typedef void (*XML_StartElementHandler)(void *userData,
	// 					const XML_Char *name,
	// 					const XML_Char **atts);
	//
	// typedef void (*XML_EndElementHandler)(void *userData,
	// 				          const XML_Char *name);
	XML_SetElementHandler(p, start, end);

	for (;;) {

		int done;
		int len;
		int ret_XML_Parse;

		len = fread(Buff, 1, BUFFSIZE, stdin);
		if (ferror(stdin)) {
			fprintf(stderr, "Read error\n");
			exit(-1);
		}

		done = feof(stdin);

		// int XML_Parse(XML_Parser p, const char *s, int len, int isFinal)
		ret_XML_Parse = XML_Parse(p, Buff, len, done);
		if (ret_XML_Parse ==0) {

			fprintf(stderr, "Parse error at line %d:\n%s\n",

			XML_GetCurrentLineNumber(p),
			XML_ErrorString(XML_GetErrorCode(p)));
			
			exit(-1);
		}

		if (done)
		break;
	}
}
