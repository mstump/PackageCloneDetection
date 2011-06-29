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


        public class ZoomChangeListener implements ChangeListener {
                mxGraphComponent m_GraphComponent;

		public static int getStartValue()
		{
			return 1;
		}
                public ZoomChangeListener(mxGraphComponent graphComponent)
                {
                        m_GraphComponent = graphComponent;
                }
                public void stateChanged(ChangeEvent e) {
                        JSlider source = (JSlider)e.getSource();
                        if (true || !source.getValueIsAdjusting()) {
                                double zf = (double)(int)source.getValue() / 100.0;

mxRectangle r = m_GraphComponent.getGraph().getMinimumGraphSize();
System.out.format("in zoom %f %f\n", r.getWidth(), r.getHeight());
//                                m_GraphComponent.getGraph().getModel().beginUpdate();
//                                m_GraphComponent.setZoomFactor(zf);
//                                m_GraphComponent.zoomTo(zf, true);
                                m_GraphComponent.zoomTo(zf, true);
//                                m_GraphComponent.getGraph().getModel().endUpdate();
                        }
                }
        }

