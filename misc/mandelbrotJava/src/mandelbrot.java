import javax.swing.*;
import javax.swing.plaf.basic.BasicInternalFrameTitlePane;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

public class mandelbrot extends JFrame implements MouseListener {
    private double x;
    private double y;
    private double pixels[][];
    private int zoom;
    private int iterations;

    public mandelbrot(int iterations) {
        //bottom corner x & y
        x = -2;
        y = -1;
        zoom = 1;
        this.iterations = iterations;

        setTitle("Mandelbrot");
        setBounds(100, 100, 800, 600);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        Container c = this.getContentPane();
        c.setLayout(new BorderLayout());
        c.addMouseListener(this);
        setVisible(true);
    }

    public void paint(Graphics g) {
        Graphics2D g2d = (Graphics2D) g;

        for (int i = 0; i < 800; i++) {
            for (int j = 0; j < 600; j++) {

                //int i2 = (i*2 + (i/400)) % 800;
                //int j2 = (j*2 + (j/300)) % 600;

                int i2 = i;
                int j2 = j;

                Complex c = new Complex(this.x + i2/(300.0*zoom), this.y + j2/(300.0*zoom));

                float itt = ((float) this.isMandel(c)) / ((float) iterations);

                if (itt < 1.0/3) {
                    g.setColor(new Color(144, (int) (255 * 3*itt), 255));
                    g.fillRect(i2, j2, 1, 1);
                } else if (itt < 2.0/3) {
                    g.setColor(new Color(144, 255, (int) (255 - 255*(3*itt - 1.0))));
                    g.fillRect(i2, j2, 1, 1);
                } else if (itt < 1) {
                    g.setColor(new Color((int) (144 + (255-144)*(3*itt - 2.0)), 255, 0));
                    g.fillRect(i2, j2, 1, 1);
                } else {
                    g.setColor(Color.BLACK);
                    g.fillRect(i2, j2, 1, 1);
                }

                if (c.real() == 0) {
                    g.setColor(Color.RED);
                    g.fillRect(i2, j2, 1, 1);
                }

            }
        }
    }

    public int isMandel(Complex c) {
        Complex z = new Complex(0, 0);
        int i;

        for (i = 0; i < iterations; i++) {
            //System.out.println(z.toString());
            z = z.times(z).plus(c);

            double mag = z.imag() * z.imag() + z.real() * z.real();

            if (mag > 4) {
                break;
            }

        }
        return i;
    }




    public void mouseClicked(MouseEvent e) {

    }

    public void mousePressed(MouseEvent e) {
        this.x += (e.getX()-200)/(300.0*zoom);
        this.y += (e.getY()-150)/(300.0*zoom);
        this.zoom *= 2;
        //System.out.println(x);
        repaint();
    }

    public void mouseReleased(MouseEvent e) {

    }

    public void mouseEntered(MouseEvent e) {

    }

    public void mouseExited(MouseEvent e) {

    }

    public static void main(String[] args) {
        mandelbrot mandelbrot = new mandelbrot(1000);

        Complex c = new Complex(0.1, 0);

        System.out.println(mandelbrot.isMandel(c));
    }
}
