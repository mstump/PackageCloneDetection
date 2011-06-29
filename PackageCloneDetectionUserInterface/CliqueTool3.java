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

import com.mxgraph.util.*;
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

public class CliqueTool3 extends Tool
{
        JTabbedPane m_Tab;
	GraphBuilder m_ProgramGraphBuilder;
	private final mxGraph m_Graph;
	final mxGraphComponent graphComponent;
	mxIGraphLayout m_Layout;
	JInternalFrame graphRootPanel;
	GraphOverview m_Overview;
	JInternalFrame programIFrame;
	JInternalFrame m_ButtonPanel;
	JPanel mainPanel;
	String m_CurrentClique;

	public void GenerateStar(String cliqueName)
	{
		m_CurrentClique = cliqueName;

		try {
	System.out.format("checking for %s\n", cliqueName);
	                FileReader readFile = null;
               	        BufferedReader reader = null;
               	        readFile = new FileReader("Stargraph.xml");
               	        reader = new BufferedReader(readFile);

			m_ProgramGraphBuilder.ParseStarResults(reader, cliqueName);
		} catch (Exception e) {
			System.out.format("exception %s", e.getMessage());
		}
	}

	public CliqueTool3(ApplicationFrame applicationFrame, String distro)
	{
		super(applicationFrame);

		m_Tab = null;

		m_Graph = new mxGraph()
		{
			public void drawState(mxICanvas canvas, mxCellState state,
                                        String label)
                        {
                                // Indirection for wrapped swing canvas inside image canvas (used for creating
                                // the preview image when cells are dragged)
                                if (getModel().isVertex(state.getCell())
                                                && canvas instanceof mxImageCanvas
                                                && ((mxImageCanvas) canvas).getGraphicsCanvas() instanceof SwingCanvas)
                                {
                                        ((SwingCanvas) ((mxImageCanvas) canvas).getGraphicsCanvas())
                                                        .drawVertex(state, label);
                                }
                                // Redirection of drawing vertices in SwingCanvas
                                else if (getModel().isVertex(state.getCell())
                                                && canvas instanceof SwingCanvas)
                                {
                                        ((SwingCanvas) canvas).drawVertex(state, label);
                                }
                                else
                                {
                                        super.drawState(canvas, state, label);
                                }
                        }
                };
		Object parent = m_Graph.getDefaultParent();

		graphComponent = new mxGraphComponent(m_Graph)
                {
                        public mxInteractiveCanvas createCanvas()
                        {
                                return new SwingCanvas(this);
                        }
                };
		graphComponent.getViewport().setOpaque(false);
		graphComponent.setBackground(new Color(249, 251, 250));

//		m_Graph.setMinimumGraphSize(new mxRectangle(0.0, 0.0, 800.0, 800.0));
		m_Layout = new mxHierarchicalLayout(m_Graph);
//		m_Layout = new mxCircleLayout(m_Graph);
//		m_Layout = new mxOrganicLayout(m_Graph);
		m_Overview = new GraphOverview("Overview", m_Graph, 0, 300, 300);

		m_ProgramGraphBuilder = new GraphBuilder(m_Graph, graphComponent, m_Layout);

		mainPanel = new JPanel();

		m_Frame = new JInternalFrame("Maximum Weight Star Subgraph of Clique", true, true);
		m_Frame.setSize(1150, 800);
		m_Frame.setResizable(true);
		m_Frame.setBackground(new Color(242, 242, 242));

//		m_ButtonPanel = new CliqueToolBar(m_ApplicationFrame, this);
//		m_ButtonPanel.setBounds(0,0,450 + 650, 100);

//		m_ButtonPanel.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));

		Drag drag = new Drag(graphComponent.getViewport());
		graphComponent.getGraphControl().addMouseMotionListener(drag);
		graphComponent.getGraphControl().addMouseListener(drag);

		graphRootPanel = new JInternalFrame("Graph");
		graphRootPanel.setSize(600,500);
		graphRootPanel.setLocation(m_Overview.m_Width, 0);

		graphRootPanel.setResizable(true);

		graphRootPanel.add(graphComponent);

		m_Graph.setMinimumGraphSize(new mxRectangle(0, 0, 500,300));
		graphRootPanel.setBounds(300,100,300 + 500, 650);
		m_Overview.setBounds(0,100,300, 400);

		String docString;

		graphComponent.getGraphControl().addMouseListener(new MouseAdapter()
		{
			public void mouseReleased(MouseEvent e)
			{
				Object cell = graphComponent.getCellAt(e.getX(), e.getY());
				if (cell != null) {
/*
					UserNode node = (UserNode)((mxCell)cell).getValue();
					PackageTool tool = new PackageTool(m_ApplicationFrame);
					m_ApplicationFrame.m_Application.AddTool(tool);
					tool.GenerateControlFlowGraph(node.m_Label, 4, tool.m_Pane);
*/
				}
			}
		});

/*
                m_Frame.addComponentListener(new ComponentAdapter()
                {
                        public void componentResized(ComponentEvent evt) {
                                Component c = (Component)evt.getSource();

                                Dimension newSize = c.getSize();
				mainPanel.setSize(c.getWidth(), c.getHeight());
				m_ButtonPanel.setSize(c.getWidth(), c.getHeight());
                                mainPanel.invalidate();
                                mainPanel.validate();
                                m_ButtonPanel.invalidate();
                                m_ButtonPanel.validate();
                        }
                });
*/
		m_Frame.setLayout(null);
//		m_Frame.add(m_ButtonPanel);
		m_Frame.add(m_Overview);
		m_Frame.add(graphRootPanel);
		graphRootPanel.setVisible(true);
		mainPanel.setVisible(true);
		m_Frame.setVisible(true);
	}

        public class SwingCanvas extends mxInteractiveCanvas
        {
                protected CellRendererPane rendererPane = new CellRendererPane();

                protected JLabel vertexRenderer = new JLabel();

                protected mxGraphComponent _graphComponent;

                public SwingCanvas(mxGraphComponent _graphComponent)
                {
                        this._graphComponent = _graphComponent;
               }

                public void drawVertex(mxCellState state, String label)
                {
			UserNode node = (UserNode)(((mxCell)state.getCell()).getValue());

                        rendererPane.paintComponent(g, node.m_Component, this._graphComponent,
                                        (int) state.getX() + translate.x, (int) state.getY()
                                                        + translate.y, (int) state.getWidth(), (int) state
                                                        .getHeight(), true);
                }

        }

};
