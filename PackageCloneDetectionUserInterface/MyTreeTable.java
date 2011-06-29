import java.util.*;
import java.awt.*;
import java.awt.event.*;

// @author Santhosh Kumar T - santhosh@in.fiorano.com 
public class MyTreeTable extends JTreeTable{ 
 
    public MyTreeTable(TreeTableModel treeTableModel){ 
        super(treeTableModel); 
        setShowGrid(true); 
        setGridColor(new Color(234, 234, 234)); 
 
        setIntercellSpacing(new Dimension(1, 1)); 
    } 
 
    public boolean editCellAt(int row, int column, EventObject e){ 
        if(e instanceof MouseEvent){ 
            MouseEvent me = (MouseEvent)e; 
            // If the modifiers are not 0 (or the left mouse button), 
            // tree may try and toggle the selection, and table 
            // will then try and toggle, resulting in the 
            // selection remaining the same. To avoid this, we 
            // only dispatch when the modifiers are 0 (or the left mouse 
            // button). 
            if(me.getModifiers()==0 || 
                    me.getModifiers()==InputEvent.BUTTON1_MASK){ 
                for(int counter = getColumnCount()-1; counter>= 0; 
                    counter--){ 
                    if(getColumnClass(counter)==TreeTableModel.class){ 
                        MouseEvent newME = new MouseEvent 
                                (tree, me.getID(), 
                                        me.getWhen(), me.getModifiers(), 
                                        me.getX()-getCellRect(0, counter, true).x, 
                                        me.getY(), me.getClickCount(), 
                                        me.isPopupTrigger()); 
                        tree.dispatchEvent(newME); 
                        break; 
                    } 
                } 
            } 
            return false; 
        } 
        return super.editCellAt(row, column, e); 
    } 
}
