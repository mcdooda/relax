#ifndef RELAX_XML_H
#define RELAX_XML_H

class Element;
#include "element.h"

namespace relax
{

namespace xml
{

Element* elementFromXML(const char* xml);
void addXML(Element* element, const char* xml);

Element* elementFromXMLFile(const char* fileName);
void addXMLFile(Element* element, const char* fileName);

}

}

#endif


