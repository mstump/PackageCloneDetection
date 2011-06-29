// this doesnt seem to work - silvio

import java.io.*;
import java.util.*;
import java.nio.*;
import java.nio.channels.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.InputSource;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

// @author Santhosh Kumar T - santhosh@in.fiorano.com 
public class XMLTrimFilter extends XMLFilterImpl{ 
 
    private CharArrayWriter contents = new CharArrayWriter(); 
 
    public XMLTrimFilter(XMLReader parent){ 
        super(parent); 
    } 
 
    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException{ 
        writeContents(); 
        super.startElement(uri, localName, qName, atts); 
    } 
 
    public void characters(char ch[], int start, int length){ 
        contents.write(ch, start, length); 
    } 
 
    public void endElement(String uri, String localName, String qName) throws SAXException{ 
        writeContents(); 
        super.endElement(uri, localName, qName); 
    } 
 
    public void ignorableWhitespace(char ch[], int start, int length){} 
 
    private void writeContents() throws SAXException{ 
        char ch[] = contents.toCharArray(); 
        if(!isWhiteSpace(ch)) 
            super.characters(ch, 0, ch.length); 
        contents.reset(); 
    } 
 
    private boolean isWhiteSpace(char ch[]){ 
        for(int i = 0; i<ch.length; i++){ 
            if(!Character.isWhitespace(ch[i])) 
                return false; 
        } 
        return true; 
    } 
} 
