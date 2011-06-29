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

class CliqueToolBar extends JInternalFrame {
	public  ApplicationFrame m_LastActive;
	CliqueTool m_CliqueTool;

	public CliqueToolBar(ApplicationFrame frame, CliqueTool programPanel)
	{
		super("Clique Tools", true);
		Dimension d;

		m_LastActive = frame;
		m_CliqueTool = programPanel;

		// Clique
		JSlider zoom = new JSlider(JSlider.HORIZONTAL, 1, 100, ZoomChangeListener.getStartValue());
                zoom.addChangeListener(new ZoomChangeListener(m_CliqueTool.graphComponent));

		add(zoom);

                JButton centButton = new JButton("Centrality");
		add(centButton);
		centButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		d = centButton.getPreferredSize();
		d.width = Integer.MAX_VALUE;
		centButton.setMaximumSize(d);
                centButton.addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                                Tool tool = new CliqueMSTCentralityTool(m_LastActive, "Centrality", m_CliqueTool.m_CurrentClique);
                                m_LastActive.m_Application.AddTool(tool);
                        }
                });

		getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.LINE_AXIS));
		setVisible(true);
	}
};

