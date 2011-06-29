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

class ToolBar extends JTabbedPane {
	public ApplicationFrame m_LastActive;
        public JTabbedPane m_ToolBarTab;
	public JPanel m_DistributionsToolBar;

	public void AddTool(Tool tool)
	{
		m_LastActive.m_DesktopPane.add(tool.m_Frame);
		m_LastActive.m_DesktopPane.repaint();
		tool.m_Frame.moveToFront();
	}

	public ToolBar(ApplicationFrame frame)
	{
		Dimension d;

		m_LastActive = frame;

//		m_ToolBarTab = new JTabbedPane();

		JTabbedPane m_ToolBarTab = this;

//		m_ToolBarTab.setLayout(new BoxLayout(m_ToolBarTab, BoxLayout.PAGE_AXIS));

//		add(m_ToolBarTab);
		
		m_DistributionsToolBar = new JPanel();
/*
		m_MainToolBar.setBounds(0, 0, 900, 200);
		m_BinaryToolBar.setBounds(0, 0, 900, 200);
		m_HomeToolBar.setBounds(0, 0, 900, 200);
		m_UnpackerToolBar.setBounds(0, 0, 900, 200);
		m_SymbolsToolBar.setBounds(0, 0, 900, 200);
		m_ObjectToolBar.setBounds(0, 0, 900, 200);
		m_HashRegistryToolBar.setBounds(0, 0, 900, 200);
		m_ClassificationToolBar.setBounds(0, 0, 900, 200);

		m_MainToolBar.setPreferredSize(new Dimension(900, 120));
*/
/*
		m_MainToolBar.setLayout(new BoxLayout(m_MainToolBar, BoxLayout.LINE_AXIS));
*/
                JButton progButton = new JButton("Package Relationships");
		m_DistributionsToolBar.add(progButton);
		progButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		d = progButton.getPreferredSize();
		d.width = Integer.MAX_VALUE;
		progButton.setMaximumSize(d);
                progButton.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                PackageRelationshipsTool tool = new PackageRelationshipsTool(m_LastActive, "Debian");
                                AddTool(tool);
				m_LastActive.repaint();
//				tool.GenerateCallGraph();
//				invalidate();
//				validate();
                        }
                });

                JButton cliqButton = new JButton("Cliques");
		m_DistributionsToolBar.add(cliqButton);
		cliqButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		d = cliqButton.getPreferredSize();
		d.width = Integer.MAX_VALUE;
		cliqButton.setMaximumSize(d);
                cliqButton.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                CliquesTool tool = new CliquesTool(m_LastActive, "Debian");
                                AddTool(tool);
				m_LastActive.repaint();
//				tool.GenerateCallGraph();
//				invalidate();
//				validate();
                        }
                });


		m_ToolBarTab.add(m_DistributionsToolBar, "Distributions");
		setVisible(true);
	}
};

