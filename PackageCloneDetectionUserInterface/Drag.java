import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.io.*;


class Drag extends MouseInputAdapter
{
	private JComponent m_Viewport;
	private boolean m_ButtonOn;
	private Point m_StartPt;

	public Drag(JComponent viewport) {
		m_StartPt = new Point();
		m_ButtonOn = false;
		m_Viewport = viewport;
	}
	public void mouseDragged(MouseEvent e)
	{
		if (m_ButtonOn) {
			Rectangle v = m_Viewport.getVisibleRect();
			Point pt = e.getPoint();
			v.x = (int)(0.75*(double)(pt.x - m_StartPt.x));
			v.y = (int)(0.75*(double)(pt.y - m_StartPt.y));
			m_Viewport.scrollRectToVisible(v);
			m_StartPt.setLocation(pt);
		}
	}
	public void mousePressed(MouseEvent e)
	{
System.out.println("press");
			m_StartPt.setLocation(e.getPoint());
			m_ButtonOn = true;
	}
	public void mouseReleased(MouseEvent e)
	{
System.out.println("release");
			m_ButtonOn = false;
	}
};

