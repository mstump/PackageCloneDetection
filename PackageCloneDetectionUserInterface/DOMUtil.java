import java.io.*;
import java.util.*;
import java.nio.*;
import java.nio.channels.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.InputSource;
import org.xml.sax.*;
import org.xml.sax.helpers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.sax.*;

// @author Santhosh Kumar T - santhosh@in.fiorano.com 
public class DOMUtil{ 
    public static Document createDocument(InputSource is) throws Exception{ 
        SAXParserFactory saxFactory = SAXParserFactory.newInstance(); 
        SAXParser parser = saxFactory.newSAXParser(); 
        XMLReader reader = new XMLTrimFilter(parser.getXMLReader()); 
 
        TransformerFactory factory = TransformerFactory.newInstance(); 
        Transformer transformer = factory.newTransformer(); 
        transformer.setOutputProperty(OutputKeys.INDENT, "no"); 
        DOMResult result = new DOMResult(); 
        transformer.transform(new SAXSource(reader, is), result); 
        return (Document)result.getNode(); 
    } 
} 
