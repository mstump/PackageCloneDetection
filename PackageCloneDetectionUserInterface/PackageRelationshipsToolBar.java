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

class PackageRelationshipsToolBar extends JInternalFrame {
	public ApplicationFrame m_LastActive;
	PackageRelationshipsTool m_PackageRelationshipsPanel;

	public PackageRelationshipsToolBar(ApplicationFrame frame, PackageRelationshipsTool programPanel)
	{
		super("PackageRelationships Tools", true);
		Dimension d;

		m_PackageRelationshipsPanel = programPanel;

		m_LastActive = frame;


		// PackageRelationships
		JSlider zoom = new JSlider(JSlider.HORIZONTAL, 1, 100, ZoomChangeListener.getStartValue());
                zoom.addChangeListener(new ZoomChangeListener(m_PackageRelationshipsPanel.graphComponent));

		add(zoom);

                JButton callgraphButton = new JButton("Display");
		add(callgraphButton);
		callgraphButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		d = callgraphButton.getPreferredSize();
		d.width = Integer.MAX_VALUE;
		callgraphButton.setMaximumSize(d);
                callgraphButton.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
				m_PackageRelationshipsPanel.GenerateCallGraph();

				invalidate();
				validate();
                        }
                });

		getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.LINE_AXIS));
		setVisible(true);
	}
};

