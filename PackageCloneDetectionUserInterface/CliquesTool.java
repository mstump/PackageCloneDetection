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

class CliquesTool extends Tool
{
	String m_CurrentClique;
	JPanel objectViewPanel;
	DefaultMutableTreeNode top;
	JTree tree;

	class TreeListener implements TreeSelectionListener {
		public void valueChanged(TreeSelectionEvent e) {
			DefaultMutableTreeNode node = (DefaultMutableTreeNode)tree.getLastSelectedPathComponent();

			if (node == null) return;

			Object nodeInfo = node.getUserObject();
			if (node.isLeaf()) {
			} else {
				m_CurrentClique = (String)nodeInfo;
			}
		}
	};

	public void ParseCliquesResults(BufferedReader reader)
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
                                if (c1.item(i).getNodeName().equals("Cliques")) {
                                        NodeList c2 = c1.item(i).getChildNodes();
                                        for (int j = 0; j < c2.getLength(); j++) {
                                                if (c2.item(j).getNodeName().equals("Clique")) {
                                                        Vector<String> row = new Vector<String>();
                                                        String cliqueName = "";
                                                        NodeList c3 = c2.item(j).getChildNodes();
                                                        for (int k = 0; k < c3.getLength(); k++) {
                                                                if (c3.item(k).getNodeName().equals("CliqueName")) {
                                                                        cliqueName = c3.item(k).getTextContent();
                                                                } else if (c3.item(k).getNodeName().equals("Name")) {
									String name;
                                                                        name = c3.item(k).getTextContent();
                                                        		row.addElement(name);
                                                                }
                                                        }
							DefaultMutableTreeNode clique = new DefaultMutableTreeNode(String.format("%s", cliqueName));
							top.add(clique);
							for (int k = 0; k < row.size(); k++) {
								DefaultMutableTreeNode p = new DefaultMutableTreeNode(String.format("%s", row.get(k)));
								clique.add(p);
							}
                                                }
                                        }
                                }
                        }

		} catch (Exception e) {
		}
	}

	public void Draw()
	{
		try {        
                        FileReader readFile = null;
                        BufferedReader reader = null;
                        readFile = new FileReader("Cliques.xml");
                        reader = new BufferedReader(readFile);
			ParseCliquesResults(reader);
		} catch (Exception e) {
		}
	}
	public CliquesTool(ApplicationFrame applicationFrame, String distro)
	{
		super(applicationFrame);

		JInternalFrame m_CliquesToolBar;
		JInternalFrame m_ListFrame;


		top = new DefaultMutableTreeNode("Cliques");
		tree = new JTree(top);
		tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
		tree.addTreeSelectionListener(new TreeListener());

		m_CliquesToolBar = new JInternalFrame("Cliques Configuration", true, true);
		m_CliquesToolBar.setBounds(0,0, 600, 100);
                m_CliquesToolBar.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
//		m_CliquesToolBar.getContentPane().setLayout(new BoxLayout(m_CliquesToolBar.getContentPane(), BoxLayout.LINE_AXIS));
		m_CliquesToolBar.getContentPane().setLayout(new FlowLayout());

		m_ListFrame = new JInternalFrame("Cliques List", true, true);
		m_ListFrame.setBounds(0, 100, 600, 600);
		m_Frame = new JInternalFrame("Cliques", true, true);

//		objectViewPanel = tree;
		JScrollPane objectScrollPanel = new JScrollPane(tree);
//                objectViewPanel.setLayout(new BoxLayout(objectViewPanel, BoxLayout.PAGE_AXIS));

		m_Frame.setSize(700,750);


		JButton cButton = new JButton("Clique");
		m_CliquesToolBar.add(cButton);
		cButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CliqueTool2 tool = new CliqueTool2(m_ApplicationFrame, "Debian");
				m_ApplicationFrame.m_Application.AddTool(tool);
				tool.GenerateClique(m_CurrentClique);
				m_ApplicationFrame.repaint();
			}
		});
		JButton spanButton = new JButton("Minimum Spanning Tree");
		m_CliquesToolBar.add(spanButton);
		spanButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CliqueTool tool = new CliqueTool(m_ApplicationFrame, "Debian");
				m_ApplicationFrame.m_Application.AddTool(tool);
				tool.GenerateSpanningTree(m_CurrentClique);
				m_ApplicationFrame.repaint();
			}
		});

		JButton sButton = new JButton("Maximum Weight Star Subgraph");
		m_CliquesToolBar.add(sButton);
		sButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CliqueTool3 tool = new CliqueTool3(m_ApplicationFrame, "Debian");
				m_ApplicationFrame.m_Application.AddTool(tool);
				tool.GenerateStar(m_CurrentClique);
				m_ApplicationFrame.repaint();
			}
		});
		JButton lButton = new JButton("Library Dependencies");
		m_CliquesToolBar.add(lButton);
		lButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CliqueTool4 tool = new CliqueTool4(m_ApplicationFrame, "Debian");
				m_ApplicationFrame.m_Application.AddTool(tool);
				tool.GenerateLibraries(m_CurrentClique);
				m_ApplicationFrame.repaint();
			}
		});
		JButton bButton = new JButton("Bicliques");
		m_CliquesToolBar.add(bButton);
		bButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CliqueTool tool = new CliqueTool(m_ApplicationFrame, "Debian");
				m_ApplicationFrame.m_Application.AddTool(tool);
//				tool.GenerateClique(m_CurrentClique);
				m_ApplicationFrame.repaint();
			}
		});

		Draw();
		m_ListFrame.add(objectScrollPanel);
		m_Frame.setLayout(null);
		m_Frame.add(m_ListFrame);
		m_Frame.add(m_CliquesToolBar);
		m_ListFrame.setVisible(true);
		m_Frame.setVisible(true);
		m_CliquesToolBar.setVisible(true);
	}
};
