#ifndef _XML_CONFIGURE_
#define _XML_CONFIGURE_
/*
	XML-Configuration-Parser Interface File
	by Loren Petrich,
	April 14, 2000

	Definition of an object that reads an XML file and configures the Marathon engine
	appropriately, based on its parsing of that file 
*/

#include "xmlparse.h"
#include "XML_ElementParser.h"

class XML_Configure
{
private:
	// Callbacks for the parser routines;
	// typecast "UserData" to this object
	static void StaticStartElement(void *UserData, const char *Name, const char **Attributes);
	static void StaticEndElement(void *UserData, const char *Name);
	static void StaticCharacterData(void *UserData, const char *String, int Length);
	
	// Approriate calls on this object
	void StartElement(const char *Name, const char **Attributes);
	void EndElement(const char *Name);
	void CharacterData(const char *String, int Length);
	
	// For interpretation errors; the count is for printing only the first few
	int NumInterpretErrors;
	
	// The parser itself
	XML_Parser Parser;
	
protected:
	// Callbacks:
	
	// Gets some XML data to parse; may be called repeatedly until the parsing is done.
	// Returns whether the data reading was successful.
	virtual bool GetData()=0;
	
	// All these must be filled out:
	// Character data to be parsed:
	char *Buffer;
	// Number of bytes to
	int BufLen;
	// Whether the current buffer is the last one
	bool LastOne;
	
	// Reports a read error
	virtual void ReportReadError() {}
	
	// Reports an XML parsing error
	virtual void ReportParseError(const char *ErrorString, int LineNumber) {}
	
	// Reports an interpretation error
	virtual void ReportInterpretError(const char *ErrorString) {}
	
	// Requests aborting of parsing (reasonable if there were lots of errors)
	virtual bool RequestAbort() {return false;}

public:

	// Does parsing; indicates whether the parsing was successful or not
	bool DoParse();
	
	// How many interprtation errors so far?
	int GetNumInterpretErrors() {return NumInterpretErrors;}
	
	// Compose an interpretation error; use <>printf syntax
	void ComposeInterpretError(char *Format, ...);
	
	// Pointer to the currently-active element.
	// Initially, this is to be set to a behind-the-scenes element
	// that has all the valid XML-file root elements as its child elements.
	XML_ElementParser *CurrentElement;
	
	// Idiot-proofing measure
	XML_Configure() {CurrentElement = NULL;}
	virtual ~XML_Configure() {}
};

#endif