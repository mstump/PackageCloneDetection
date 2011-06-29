import com.mxgraph.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.JFrame;
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;

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

public class ApplicationFrame extends JFrame
{
	JTabbedPane m_Tab;
	Application m_Application;
	JComponent m_DesktopPane;
	JTabbedPane m_ToolBar;

	public ApplicationFrame(Application application)
	{
		super("hello");
		Doit(application, new JPanel(), "hello");
	}
	public ApplicationFrame(Application application, JPanel panel, String name) 
	{
		super(name);
		Doit(application, panel, name);
	}
	public void DoPanels()
	{

		m_ToolBar = new ToolBar(this);
		add(m_ToolBar, BorderLayout.PAGE_START);
		m_ToolBar.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
		m_Tab = new JTabbedPane();
		m_DesktopPane = new JDesktopPane();
		JPanel m_DesktopPane2 = new JPanel();
		m_Tab.add(m_DesktopPane, "Home");
		add(m_Tab, BorderLayout.CENTER);
		setVisible(true);
	}
	public void Doit(Application application, JPanel panel, String name) 
	{
		m_Application = application;
		m_Tab = null;

		final ApplicationFrame m_This = this;
		addWindowListener(new WindowAdapter() {
			public void windowActivated(WindowEvent e) {
				m_Application.m_LastActive = m_This;
			}
		});
        }
};
