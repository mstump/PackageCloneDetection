import java.lang.Math;
import com.mxgraph.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.event.*;
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
import com.mxgraph.model.mxCell;
import com.mxgraph.swing.view.mxInteractiveCanvas;

public class GraphOverview extends JInternalFrame {
	private final mxGraph m_OverviewGraph;
	final mxGraphComponent m_OverviewGraphComponent;
	mxIGraphLayout m_OverviewLayout;
	GraphBuilder m_OverviewGraphBuilder;
//	JComponent m_Overview;
	int m_Width;
	int m_Height;
//	mxGraph m_Graph;
	int m_Type;

	public GraphOverview(String title, mxGraph graph, int type, int w, int h)
	{
		super(title);

		m_Type = type;
//		m_Graph = graph;
		m_Width = w;
		m_Height = h;


                m_OverviewGraph = new mxGraph()
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
                m_OverviewGraphComponent = new mxGraphComponent(m_OverviewGraph)
                {
                        public mxInteractiveCanvas createCanvas()
                        {
                                return new SwingCanvas(this);
                        }
                };


//		m_OverviewGraph = new CustomCanvasGraph();
//		m_OverviewGraphComponent = new CustomCanvasGraphComponent(m_OverviewGraph);

		m_OverviewLayout = new mxFastOrganicLayout(m_OverviewGraph);
//		m_OverviewLayout = new mxCircleLayout(m_OverviewGraph);
//		m_OverviewGraph.setMinimumGraphSize(new mxRectangle(0, 0, m_Width,m_Height));
		m_OverviewGraphBuilder = new GraphBuilder(m_OverviewGraph, m_OverviewGraphComponent, m_OverviewLayout);
//		m_Overview = new JPanel();
		setSize(m_Width, m_Height);
		setVisible(true);

		m_OverviewGraphComponent.getViewport().setOpaque(false);
		m_OverviewGraphComponent.setBackground(new Color(231, 239, 237));
//		m_OverviewGraphComponent.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		add(m_OverviewGraphComponent);
	}
	public void GenerateOverview(BufferedReader reader)
	{
                m_OverviewGraph.getModel().beginUpdate();
                m_OverviewGraphComponent.zoomTo(1.0, false);
                m_OverviewGraph.getModel().endUpdate();
               try {
			m_OverviewGraphBuilder.ParsePackageRelationshipsResults(reader);
                } catch (Exception e) {
			System.out.format("exception %s", e.getMessage());
                }
                Dimension d = m_OverviewGraphComponent.getViewport().getPreferredSize();
//		Dimension d = new Dimension(m_Width, m_Height);
		Dimension vd = m_OverviewGraphComponent.getViewport().getSize();
System.out.format("overview %d %d\n", d.width, d.height);
                double xzf = (double)(vd.width - 5)/ d.width;
                double yzf = (double)(vd.height - 5)/ d.height;
System.out.format("testing %d,%d %f %f\n", vd.width, vd.height, xzf, yzf);
                double zf = Math.min(xzf, yzf);
//                m_OverviewGraph.setMinimumGraphSize(new mxRectangle(0, 0, m_Width*zf,m_Height*zf));
                m_OverviewGraph.getModel().beginUpdate();
                m_OverviewGraphComponent.zoomTo(zf, false);
                m_OverviewGraph.getModel().endUpdate();
	}
}

