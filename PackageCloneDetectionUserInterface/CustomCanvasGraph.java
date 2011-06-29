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


public class CustomCanvasGraph extends mxGraph
{
	public CustomCanvasGraph()
	{
		super();
	}
	public void drawState(mxICanvas canvas, mxCellState state, String label)
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
