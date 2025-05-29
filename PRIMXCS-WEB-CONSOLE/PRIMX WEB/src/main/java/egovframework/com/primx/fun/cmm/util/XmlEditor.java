// package egovframework.com.primx.fun.cmm.util;

// import java.io.BufferedReader;
// import java.io.BufferedWriter;
// import java.io.File;
// import java.io.FileInputStream;
// import java.io.FileOutputStream;
// import java.io.FileReader;
// import java.io.FileWriter;
// import java.io.IOException;
// import java.io.OutputStream;
// import java.io.Writer;
// import java.util.Arrays;
// import java.util.List;
// import javax.xml.parsers.DocumentBuilder;
// import javax.xml.parsers.DocumentBuilderFactory;
// import javax.xml.parsers.ParserConfigurationException;
// import javax.xml.transform.Transformer;
// import javax.xml.transform.TransformerException;
// import javax.xml.transform.TransformerFactory;
// import javax.xml.transform.dom.DOMSource;
// import javax.xml.transform.stream.StreamResult;
// import org.apache.xerces.dom.CoreDocumentImpl;
// import org.apache.xml.serialize.OutputFormat;
// import org.apache.xml.serialize.XMLSerializer;
// import org.springframework.stereotype.Service;
// import org.w3c.dom.Document;
// import org.w3c.dom.Element;
// import org.w3c.dom.NamedNodeMap;
// import org.w3c.dom.Node;
// import org.w3c.dom.NodeList;
// import org.xml.sax.SAXException;

// @Service
// public class XmlEditor {
//     private List<String> sortAtts = Arrays.asList(new String[] { "name", "filter" });

//     public void createNewTag(String targetDir, XmlAttributes attrs, boolean isDefault, boolean isAdd, String indexName, String policyName) throws ParserConfigurationException, IOException, SAXException {
//         Document doc = createDocument(targetDir, indexName);
//         NodeList list = doc.getElementsByTagName("configurations");
//         for (int i = 0; i < list.getLength(); i++) {
//           Node configurations = list.item(i);
//           if (configurations.getNodeType() == 1) {
//             Element configuration = doc.createElement("configuration");
//             if (isDefault) {
//               configuration.setAttribute("name", "defaultpolicy");
//               configuration.setAttribute("priority", "3");
//               configuration.setAttribute("redirect", attrs.getRedirect());
//               configurations.appendChild(configuration);
//             } else if (isAdd) {
//               configuration.setAttribute("name", "personalpolicy");
//               configuration.setAttribute("priority", "1");
//               configuration.setAttribute("filter", "computer is " + attrs.getFilter());
//               configuration.setAttribute("redirect", attrs.getRedirect());
//               configurations.appendChild(configuration);
//             } 
//           } 
//         } 
//         try {
//           FileOutputStream outputStream = new FileOutputStream(targetDir + "/" + targetDir + "index.xml");
//           try {
//             writeXml(doc, outputStream);
//             removeEmptyLinesFromXML(targetDir + "/" + targetDir + "index.xml");
//             format(targetDir + "/" + targetDir + "index.xml", targetDir + "/" + targetDir + "index.xml");
//             outputStream.close();
//           } catch (Throwable throwable) {
//             try {
//               outputStream.close();
//             } catch (Throwable throwable1) {
//               throwable.addSuppressed(throwable1);
//             } 
//             throw throwable;
//           } 
//         } catch (IOException|TransformerException e) {
//           e.printStackTrace();
//         } catch (Exception e) {
//           throw new RuntimeException(e);
//         } 
//       }
      
//       public void removeTagByAttributeName(String targetDir, String redirect, String sysName, boolean isDefault, String indexName) throws ParserConfigurationException, IOException, SAXException {
//         Document doc = createDocument(targetDir, indexName);
//         NodeList list = doc.getElementsByTagName("configuration");
//         if (list.getLength() > 0)
//           for (int i = 0; i < list.getLength(); i++) {
//             Node node = list.item(i);
//             String attributeRedirect = node.getAttributes().getNamedItem("redirect").getNodeValue();
//             String attributeName = node.getAttributes().getNamedItem("name").getNodeValue();
//             if (!isDefault) {
//               if (node.getAttributes().getLength() > 3) {
//                 String attributeFilter = node.getAttributes().getNamedItem("filter").getNodeValue();
//                 if (node.getNodeType() == 1 && 
//                   attributeFilter.equalsIgnoreCase("computer is " + sysName) && attributeRedirect
//                   .equalsIgnoreCase(redirect))
//                   node.getParentNode().removeChild(node); 
//               } 
//             } else if (node.getNodeType() == 1 && 
//               attributeName.equalsIgnoreCase("defaultpolicy")) {
//               node.getParentNode().removeChild(node);
//             } 
//           }  
//         try {
//           FileOutputStream outputStream = new FileOutputStream(targetDir + "/" + targetDir + "index.xml");
//           try {
//             writeXml(doc, outputStream);
//             removeEmptyLinesFromXML(targetDir + "/" + targetDir + "index.xml");
//             outputStream.close();
//           } catch (Throwable throwable) {
//             try {
//               outputStream.close();
//             } catch (Throwable throwable1) {
//               throwable.addSuppressed(throwable1);
//             } 
//             throw throwable;
//           } 
//         } catch (IOException|TransformerException e) {
//           e.printStackTrace();
//         } 
//       }
      
//       public void editTagByAttributeName(String targetDir, String newRedirect, String sysName, boolean isDefault, boolean isAdd, String indexName, String policyName) throws ParserConfigurationException, IOException, SAXException {
//         removeTagByAttributeName(targetDir, newRedirect, sysName, isDefault, indexName);
//         XmlAttributes attr = new XmlAttributes();
//         attr.setRedirect(newRedirect);
//         attr.setFilter(sysName);
//         createNewTag(targetDir, attr, isDefault, isAdd, indexName, policyName);
//       }
      
//       private static void writeXml(Document doc, OutputStream output) throws TransformerException {
//         TransformerFactory transformerFactory = TransformerFactory.newInstance();
//         Transformer transformer = transformerFactory.newTransformer();
//         transformer.setOutputProperty("indent", "yes");
//         DOMSource source = new DOMSource(doc);
//         StreamResult result = new StreamResult(output);
//         transformer.transform(source, result);
//       }
      
//       private Document createDocument(String targetDir, String indexName) throws IOException, SAXException, ParserConfigurationException {
//         DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
//         DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
//         String filePath = targetDir + "/" + indexName + "index.xml";
//         Document doc = docBuilder.parse(filePath);
//         doc.getDocumentElement().normalize();
//         return doc;
//       }
      
//       public static void removeEmptyLinesFromXML(String file) {
//         try {
//           BufferedReader reader = new BufferedReader(new FileReader(file));
//           File tempFile = new File("temp.xml");
//           BufferedWriter writer = new BufferedWriter(new FileWriter(tempFile));
//           String line;
//           while ((line = reader.readLine()) != null) {
//             if (!line.trim().isEmpty()) {
//               writer.write(line);
//               writer.newLine();
//             } 
//           } 
//           reader.close();
//           writer.close();
//           boolean bool = tempFile.renameTo(new File(file));
//         } catch (IOException e) {
//           e.printStackTrace();
//         } 
//       }
      
//       public void format(String inFile, String outFile) throws Exception {
//         DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
//         DocumentBuilder builder = dbFactory.newDocumentBuilder();
//         Document outDocument = builder.newDocument();
//         FileInputStream inputStream = new FileInputStream(inFile);
//         try {
//           Document document = dbFactory.newDocumentBuilder().parse(inputStream);
//           Element sourceRoot = document.getDocumentElement();
//           Element outRoot = outDocument.createElementNS(sourceRoot.getNamespaceURI(), sourceRoot.getTagName());
//           outDocument.appendChild(outRoot);
//           copyAtts(sourceRoot.getAttributes(), outRoot);
//           copyElement(sourceRoot.getChildNodes(), outRoot, outDocument);
//           inputStream.close();
//         } catch (Throwable throwable) {
//           try {
//             inputStream.close();
//           } catch (Throwable throwable1) {
//             throwable.addSuppressed(throwable1);
//           } 
//           throw throwable;
//         } 
//         Writer outxml = new FileWriter(new File(outFile));
//         try {
//           OutputFormat format = new OutputFormat();
//           format.setLineWidth(0);
//           format.setIndenting(false);
//           format.setIndent(2);
//           XMLSerializer serializer = new XMLSerializer(outxml, format);
//           serializer.serialize(outDocument);
//           outxml.close();
//         } catch (Throwable throwable) {
//           try {
//             outxml.close();
//           } catch (Throwable throwable1) {
//             throwable.addSuppressed(throwable1);
//           } 
//           throw throwable;
//         } 
//       }
      
//       private void copyElement(NodeList nodes, Element parent, Document document) {
//         for (int i = 0; i < nodes.getLength(); i++) {
//           Node node = nodes.item(i);
//           if (node.getNodeType() == Node.ELEMENT_NODE) {
//             CustomObject object = new CustomObject(this, (CoreDocumentImpl)document, node.getNodeName());
//             copyAtts(node.getAttributes(), (Element)object);
//             copyElement(node.getChildNodes(), (Element)object, document);
//             parent.appendChild((Node)object);
//           } 
//         } 
//       }
      
//       private void copyAtts(NamedNodeMap attributes, Element target) {
//         for (int i = 0; i < attributes.getLength(); i++) {
//           Node att = attributes.item(i);
//           target.setAttribute(att.getNodeName(), att.getNodeValue());
//         } 
//       }
// }
