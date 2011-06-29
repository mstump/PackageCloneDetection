import java.awt.geom.*;
import java.awt.image.*;
import javax.imageio.*;

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
			double zf = this._graphComponent.getGraph().getView().getScale();
			double x, y, w, h;

			Graphics2D g2 = (Graphics2D)g;
			AffineTransform backup = g2.getTransform();
			g2.scale(zf, zf);
			x = ((double)state.getX())/zf + ((double)translate.x)/zf;
			y = ((double)state.getY())/zf + ((double)translate.y)/zf;
			w = state.getWidth() / zf;
			h = state.getHeight() / zf;
                        rendererPane.paintComponent(g2, node.m_Component, this._graphComponent, (int)x, (int)y, (int) w, (int) h, true);

			g2.setTransform(backup);
                }

        };


