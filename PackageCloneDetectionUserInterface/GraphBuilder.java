import com.mxgraph.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.JFrame;
import javax.swing.border.*;
import javax.swing.border.Border;
import javax.swing.BorderFactory;
import javax.swing.JTabbedPane;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JLabel;
import javax.swing.JComponent;
import javax.swing.CellRendererPane;
import java.net.*;
import java.io.*;
import java.util.*;
import java.nio.*;
import java.nio.channels.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.InputSource;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.*;
import javax.swing.border.BevelBorder;

import com.mxgraph.swing.handler.mxRubberband;
import com.mxgraph.canvas.mxICanvas;
import com.mxgraph.canvas.mxImageCanvas;
import com.mxgraph.layout.mxIGraphLayout;
import com.mxgraph.layout.mxGraphLayout;
import com.mxgraph.layout.hierarchical.mxHierarchicalLayout;
import com.mxgraph.layout.*;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxCellState;
import com.mxgraph.model.mxCell;
import com.mxgraph.swing.view.mxInteractiveCanvas;

public class GraphBuilder 
{
	private final mxGraph m_Graph;
	final mxGraphComponent m_GraphComponent;
        mxIGraphLayout m_Layout;

	public GraphBuilder(mxGraph graph, mxGraphComponent graphComponent, mxIGraphLayout layout)
	{
		m_Graph = graph;
		m_GraphComponent = graphComponent;
		m_Layout = layout;
	}
	public void ParseLibrariesResults(BufferedReader reader, String cliqueName)
	{
		m_Graph.getModel().beginUpdate();
		m_Graph.selectAll();
		m_Graph.removeCells();
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder parser = factory.newDocumentBuilder();
//			Document d = parser.parse(new InputSource(new StringReader(docString)));
			Document d = parser.parse(new InputSource(reader));
			Map<String, Object> map = new HashMap<String, Object>();

			NodeList c1 = d.getFirstChild().getChildNodes();
					for (int i = 0; i < c1.getLength(); i++) {
						if (c1.item(i).getNodeName().equals("CommonLibraries")) {
System.out.format("got clique\n");
							NodeList c2 = c1.item(i).getChildNodes();
							for (int j = 0; j < c2.getLength(); j++) {
								if (c2.item(j).getNodeName().equals("CliqueName")) {
System.out.format("got cliquename\n");
									if (c2.item(j).getTextContent().equals(cliqueName)) {
System.out.format("got clique\n");
										Doit4(c1.item(i).getChildNodes());
										return;
									}
								}
							}
						}
			}
		} catch (Exception e) {
			System.out.format("exception %s\n", e.getMessage());
		}

	}
	public void Doit4(NodeList c2)
	{
		Object parent = m_Graph.getDefaultParent();
		Map<String, Object> map = new HashMap<String, Object>();

		for (int j = 0; j < c2.getLength(); j++) {
			if (c2.item(j).getNodeName().equals("Node_A") || c2.item(j).getNodeName().equals("Node_B")) {
				String node;
						node = c2.item(j).getTextContent();
//							System.out.format("node %s\n", node);
/*
String [] colNames = {"a", "b"};
Object[][] data = { { "1", "2" } };
JTable label = new JTable(data, colNames);
*/
							JLabel label = new JLabel(node);
//							JLabel label = new JLabel("x");
                        label.setBorder(BorderFactory
                                        .createBevelBorder(BevelBorder.RAISED));
                        label.setHorizontalAlignment(JLabel.CENTER);
                        label.setBackground(m_GraphComponent.getBackground()
                                        .darker());
                        label.setOpaque(true);
							Dimension dim = label.getPreferredSize();

							Object o = m_Graph.insertVertex(parent, null, new UserNode(label, node), 20, 20, dim.width, dim.height);
							m_Graph.addCell(o);
							map.put(new String(node), o);

				} else if (c2.item(j).getNodeName().equals("Edge")) {
							String src = null, dst = null; 
							NodeList c3 = c2.item(j).getChildNodes();
							for (int k = 0; k < c3.getLength(); k++) {
								if (c3.item(k).getNodeName().equals("src")) {
									src = c3.item(k).getTextContent();
								} else if (c3.item(k).getNodeName().equals("dst")) {
									dst = c3.item(k).getTextContent();
								}
							}
//							System.out.format("edge %s -> %s\n", src, dst);
                        				m_Graph.insertEdge(parent, null, "", map.get(src), map.get(dst));
				}
		}

		m_Graph.getModel().endUpdate();
		m_Layout.execute(m_Graph.getDefaultParent());
	}


	public void ParseStarResults(BufferedReader reader, String cliqueName)
	{
		m_Graph.getModel().beginUpdate();
		m_Graph.selectAll();
		m_Graph.removeCells();
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder parser = factory.newDocumentBuilder();
//			Document d = parser.parse(new InputSource(new StringReader(docString)));
			Document d = parser.parse(new InputSource(reader));
			Map<String, Object> map = new HashMap<String, Object>();

			NodeList c1 = d.getFirstChild().getChildNodes();
					for (int i = 0; i < c1.getLength(); i++) {
						if (c1.item(i).getNodeName().equals("Graph")) {
System.out.format("got clique\n");
							NodeList c2 = c1.item(i).getChildNodes();
							for (int j = 0; j < c2.getLength(); j++) {
								if (c2.item(j).getNodeName().equals("Name")) {
System.out.format("got cliquename\n");
									if (c2.item(j).getTextContent().equals(cliqueName)) {
System.out.format("got clique\n");
										Doit3(c1.item(i).getChildNodes());
										return;
									}
								}
							}
						}
			}
		} catch (Exception e) {
			System.out.format("exception %s\n", e.getMessage());
		}

	}
	public void Doit3(NodeList c2)
	{
		Object parent = m_Graph.getDefaultParent();
		Map<String, Object> map = new HashMap<String, Object>();

		for (int j = 0; j < c2.getLength(); j++) {
			if (c2.item(j).getNodeName().equals("Node")) {
				String node;
						node = c2.item(j).getTextContent();
//							System.out.format("node %s\n", node);
/*
String [] colNames = {"a", "b"};
Object[][] data = { { "1", "2" } };
JTable label = new JTable(data, colNames);
*/
							JLabel label = new JLabel(node);
//							JLabel label = new JLabel("x");
                        label.setBorder(BorderFactory
                                        .createBevelBorder(BevelBorder.RAISED));
                        label.setHorizontalAlignment(JLabel.CENTER);
                        label.setBackground(m_GraphComponent.getBackground()
                                        .darker());
                        label.setOpaque(true);
							Dimension dim = label.getPreferredSize();

							Object o = m_Graph.insertVertex(parent, null, new UserNode(label, node), 20, 20, dim.width, dim.height);
							m_Graph.addCell(o);
							map.put(new String(node), o);

				} else if (c2.item(j).getNodeName().equals("Edge")) {
							String src = null, dst = null; 
							NodeList c3 = c2.item(j).getChildNodes();
							for (int k = 0; k < c3.getLength(); k++) {
								if (c3.item(k).getNodeName().equals("src")) {
									src = c3.item(k).getTextContent();
								} else if (c3.item(k).getNodeName().equals("dst")) {
									dst = c3.item(k).getTextContent();
								}
							}
//							System.out.format("edge %s -> %s\n", src, dst);
                        				m_Graph.insertEdge(parent, null, "", map.get(src), map.get(dst));
				}
		}

		m_Graph.getModel().endUpdate();
		m_Layout.execute(m_Graph.getDefaultParent());
	}




	public void ParseCliqueResults(BufferedReader reader, String cliqueName)
	{
		m_Graph.getModel().beginUpdate();
		m_Graph.selectAll();
		m_Graph.removeCells();
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder parser = factory.newDocumentBuilder();
//			Document d = parser.parse(new InputSource(new StringReader(docString)));
			Document d = parser.parse(new InputSource(reader));
			Map<String, Object> map = new HashMap<String, Object>();

			NodeList c0 = d.getFirstChild().getChildNodes();
			for (int i0 = 0; i0 < c0.getLength(); i0++) {
				if (c0.item(i0).getNodeName().equals("Cliques")) {
System.out.format("got cliques\n");
					NodeList c1 = c0.item(i0).getChildNodes();
					for (int i = 0; i < c1.getLength(); i++) {
						if (c1.item(i).getNodeName().equals("Clique")) {
System.out.format("got clique\n");
							NodeList c2 = c1.item(i).getChildNodes();
							for (int j = 0; j < c2.getLength(); j++) {
								if (c2.item(j).getNodeName().equals("CliqueName")) {
System.out.format("got cliquename\n");
									if (c2.item(j).getTextContent().equals(cliqueName)) {
System.out.format("got clique\n");
										Doit2(c1.item(i).getChildNodes());
										return;
									}
								}
							}
						}
					}
				}
			}
		} catch (Exception e) {
			System.out.format("exception %s\n", e.getMessage());
		}

	}
	public void Doit2(NodeList c2)
	{
		Object parent = m_Graph.getDefaultParent();
		Map<String, Object> map = new HashMap<String, Object>();

		for (int j = 0; j < c2.getLength(); j++) {
			if (c2.item(j).getNodeName().equals("Name")) {
				String node;
						node = c2.item(j).getTextContent();
//							System.out.format("node %s\n", node);
/*
String [] colNames = {"a", "b"};
Object[][] data = { { "1", "2" } };
JTable label = new JTable(data, colNames);
*/
							JLabel label = new JLabel(node);
//							JLabel label = new JLabel("x");
                        label.setBorder(BorderFactory
                                        .createBevelBorder(BevelBorder.RAISED));
                        label.setHorizontalAlignment(JLabel.CENTER);
                        label.setBackground(m_GraphComponent.getBackground()
                                        .darker());
                        label.setOpaque(true);
							Dimension dim = label.getPreferredSize();

							Object o = m_Graph.insertVertex(parent, null, new UserNode(label, node), 20, 20, dim.width, dim.height);
							m_Graph.addCell(o);
							map.put(new String(node), o);

						}
			}
		for (Map.Entry<String,Object> entry1 : map.entrySet()) {
			String key1 = entry1.getKey();
			Object value1 = entry1.getValue();
			for (Map.Entry<String,Object> entry2 : map.entrySet()) {
				String key2 = entry2.getKey();
				Object value2 = entry2.getValue();
				if (!key1.equals(key2)) {
                       			m_Graph.insertEdge(parent, null, "", value1, value2);
				}
			}
		}

		m_Graph.getModel().endUpdate();
		m_Layout.execute(m_Graph.getDefaultParent());
	}

	public void ParseSpanningTreeResults(BufferedReader reader, String cliqueName)
	{
		m_Graph.getModel().beginUpdate();
		m_Graph.selectAll();
		m_Graph.removeCells();
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder parser = factory.newDocumentBuilder();
//			Document d = parser.parse(new InputSource(new StringReader(docString)));
			Document d = parser.parse(new InputSource(reader));
			Map<String, Object> map = new HashMap<String, Object>();

			NodeList c1 = d.getFirstChild().getChildNodes();
			for (int i = 0; i < c1.getLength(); i++) {
				if (c1.item(i).getNodeName().equals("Graph")) {
					NodeList c2 = c1.item(i).getChildNodes();
					for (int j = 0; j < c2.getLength(); j++) {
						if (c2.item(j).getNodeName().equals("Name")) {
							if (c2.item(j).getTextContent().equals(cliqueName)) {
								Doit(c1.item(i).getChildNodes());
								return;
							}
						}
					}
				}
			}
		} catch (Exception e) {
			System.out.format("exception %s\n", e.getMessage());
		}
	}
	public void Doit(NodeList c2)
	{
		Object parent = m_Graph.getDefaultParent();
		Map<String, Object> map = new HashMap<String, Object>();

		for (int j = 0; j < c2.getLength(); j++) {
			if (c2.item(j).getNodeName().equals("Node")) {
				String node;
						node = c2.item(j).getTextContent();
//							System.out.format("node %s\n", node);
/*
String [] colNames = {"a", "b"};
Object[][] data = { { "1", "2" } };
JTable label = new JTable(data, colNames);
*/
							JLabel label = new JLabel(node);
                        label.setBorder(BorderFactory
                                        .createBevelBorder(BevelBorder.RAISED));
                        label.setHorizontalAlignment(JLabel.CENTER);
                        label.setBackground(m_GraphComponent.getBackground()
                                        .darker());
                        label.setOpaque(true);
							Dimension dim = label.getPreferredSize();

							Object o = m_Graph.insertVertex(parent, null, new UserNode(label, node), 20, 20, dim.width, dim.height);
							m_Graph.addCell(o);
							map.put(new String(node), o);

						} if (c2.item(j).getNodeName().equals("Edge")) {
							String src = null, dst = null; 
							NodeList c3 = c2.item(j).getChildNodes();
							for (int k = 0; k < c3.getLength(); k++) {
								if (c3.item(k).getNodeName().equals("src")) {
									src = c3.item(k).getTextContent();
								} else if (c3.item(k).getNodeName().equals("dst")) {
									dst = c3.item(k).getTextContent();
								}
							}
//							System.out.format("edge %s -> %s\n", src, dst);
                        				m_Graph.insertEdge(parent, null, "", map.get(src), map.get(dst));
						}
			}

		m_Graph.getModel().endUpdate();
		m_Layout.execute(m_Graph.getDefaultParent());
	}

	public void ParsePackageRelationshipsResults(BufferedReader reader)
	{
		m_Graph.getModel().beginUpdate();
		m_Graph.selectAll();
		m_Graph.removeCells();
		Object parent = m_Graph.getDefaultParent();
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder parser = factory.newDocumentBuilder();
//			Document d = parser.parse(new InputSource(new StringReader(docString)));
			Document d = parser.parse(new InputSource(reader));
			Map<String, Object> map = new HashMap<String, Object>();

			NodeList c1 = d.getFirstChild().getChildNodes();
			for (int i = 0; i < c1.getLength(); i++) {
				if (c1.item(i).getNodeName().equals("Graph")) {
					NodeList c2 = c1.item(i).getChildNodes();
					for (int j = 0; j < c2.getLength(); j++) {
						if (c2.item(j).getNodeName().equals("Node")) {
							String node;
							node = c2.item(j).getTextContent();
//							System.out.format("node %s\n", node);
/*
String [] colNames = {"a", "b"};
Object[][] data = { { "1", "2" } };
JTable label = new JTable(data, colNames);
*/
//							JLabel label = new JLabel(node);
							JLabel label = new JLabel("x");
                        label.setBorder(BorderFactory
                                        .createBevelBorder(BevelBorder.RAISED));
                        label.setHorizontalAlignment(JLabel.CENTER);
                        label.setBackground(m_GraphComponent.getBackground()
                                        .darker());
                        label.setOpaque(true);
							Dimension dim = label.getPreferredSize();

							Object o = m_Graph.insertVertex(parent, null, new UserNode(label, node), 20, 20, dim.width, dim.height);
							m_Graph.addCell(o);
							map.put(new String(node), o);

						} if (c2.item(j).getNodeName().equals("Edge")) {
							String src = null, dst = null; 
							NodeList c3 = c2.item(j).getChildNodes();
							for (int k = 0; k < c3.getLength(); k++) {
								if (c3.item(k).getNodeName().equals("src")) {
									src = c3.item(k).getTextContent();
								} else if (c3.item(k).getNodeName().equals("dst")) {
									dst = c3.item(k).getTextContent();
								}
							}
//							System.out.format("edge %s -> %s\n", src, dst);
                        				m_Graph.insertEdge(parent, null, "", map.get(src), map.get(dst));
						}
					}
				}
			}
		} catch (Exception e) {
			System.out.format("exception %s\n", e.getMessage());
		}

		m_Graph.getModel().endUpdate();
		m_Layout.execute(m_Graph.getDefaultParent());
	}
};
