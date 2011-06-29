import javax.swing.table.*;
import javax.swing.table.TableColumn.*;
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

import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;

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

class CliqueMSTCentralityTool extends Tool
{
	JPanel objectViewPanel;

	public void ParseCliqueMSTCentralityResults(BufferedReader reader, String cliqueName)
	{

		try {
/*
			MyTreeTable table;
			Document d = DOMUtil.createDocument(new InputSource(reader));

			NodeList c1 = d.getFirstChild().getChildNodes();
			for (int i = 0; i < c1.getLength(); i++) {
				if (c1.item(i).getNodeName().equals("Cliques")) {
					table = new MyTreeTable(new XMLTreeTableModel(c1.item(i)));
					objectViewPanel.add(table);
					break;
				}
			}
*/
			Document d = DOMUtil.createDocument(new InputSource(reader));

                        NodeList c1 = d.getFirstChild().getChildNodes();
                        for (int i = 0; i < c1.getLength(); i++) {
                                if (c1.item(i).getNodeName().equals("Graph")) {
                                        NodeList c2 = c1.item(i).getChildNodes();
                                        for (int j = 0; j < c2.getLength(); j++) {
                                                if (c2.item(j).getNodeName().equals("Name")) {
							if (c2.item(j).getTextContent().equals(cliqueName)) {
								Doit(c1.item(i).getChildNodes());
							}
						}
					}
				}
			}
		} catch (Exception e) {
System.out.format("exception %s\n", e.getMessage());
		}
	}
	public void Doit(NodeList c3)
	{
		Vector<Vector> rowData = new Vector<Vector>();

		for (int k = 0; k < c3.getLength(); k++) {
			if (c3.item(k).getNodeName().equals("Node")) {
				Vector<String> row = new Vector<String>();
				String name = "", degreeCentrality = "";
				NodeList c4 = c3.item(k).getChildNodes();
				for (int l = 0; l < c4.getLength(); l++) {
					if (c4.item(l).getNodeName().equals("Name")) {
						name = c4.item(l).getTextContent();
					} else if (c4.item(l).getNodeName().equals("DegreeCentrality")) {
						degreeCentrality= c4.item(l).getTextContent();
					}
				}
				row.addElement(degreeCentrality);
				row.addElement(name);
				rowData.addElement(row);
			}
		}
		Vector<String> colNames = new Vector<String>();
		colNames.addElement("Degree Centrality");
		colNames.addElement("Node");

                       TableModel model = new DefaultTableModel(rowData, colNames) {
                                public Class getColumnClass(int c) {
                                        Class r;
        
                                        if ((c >= 0) && (c < getColumnCount())) {
                                                r = getValueAt(0, c).getClass();
                                        } else {
                                                r = Object.class;
                                        }
                                        return r;
                                }
                        };
                        JTable table = new JTable(model);
                        table.setRowSorter(new TableRowSorter<TableModel>(model));

		objectViewPanel.setLayout(new BorderLayout());
		objectViewPanel.add(table.getTableHeader(), BorderLayout.NORTH);
		objectViewPanel.add(table, BorderLayout.CENTER);
	}


	public void Draw(String cliqueName)
	{
		try {        
                        FileReader readFile = null;
                        BufferedReader reader = null;
                        readFile = new FileReader("PackageSpanningTreesCentrality.xml");
                        reader = new BufferedReader(readFile);
			ParseCliqueMSTCentralityResults(reader, cliqueName);
		} catch (Exception e) {
		}
	}
	public CliqueMSTCentralityTool(ApplicationFrame applicationFrame, String distro, String cliqueName)
	{
		super(applicationFrame);

		m_Frame = new JInternalFrame(String.format("Clique MST Centrality %s", cliqueName), true, true);

                objectViewPanel = new JPanel();
                JScrollPane objectScrollPanel = new JScrollPane(objectViewPanel);
                objectViewPanel.setLayout(new BoxLayout(objectViewPanel, BoxLayout.PAGE_AXIS));

                m_Frame.setSize(400,400);
                m_Frame.setVisible(true);

                m_Frame.add(objectScrollPanel);

                Draw(cliqueName);

	}
};
