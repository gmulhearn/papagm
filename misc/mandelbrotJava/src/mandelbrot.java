import javax.swing.*;
import javax.swing.plaf.basic.BasicInternalFrameTitlePane;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

public class mandelbrot extends JFrame implements MouseListener {
    private double x;
    private double y;

    private double oldX;
    private double oldY;

    private int zoom;
    private int iterations;
    private int mode;

    /**
     * mode: 0 = mandelbrot, 1 = julia
    */
    public mandelbrot(int iterations, int mode) {
        //bottom corner x & y
        x = -2;
        y = -1;
        zoom = 1;
        this.iterations = iterations;
        this.mode = mode;

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

                Complex c = new Complex(this.x + i2 / (300.0 * zoom), this.y + j2 / (300.0 * zoom));
                float itt;
                if (this.mode == 0) {
                    itt = ((float) this.isMandel(c)) / ((float) iterations);
                } else {
                    itt = ((float) this.isJulia(c)) / ((float) iterations);
                }

                if (itt < 1.0 / 3) {
                    g.setColor(new Color(144, (int) (255 * 3 * itt), 255));
                    g.fillRect(i2, j2, 1, 1);
                } else if (itt < 2.0 / 3) {
                    g.setColor(new Color(144, 255, (int) (255 - 255 * (3 * itt - 1.0))));
                    g.fillRect(i2, j2, 1, 1);
                } else if (itt < 1) {
                    g.setColor(new Color((int) (144 + (255 - 144) * (3 * itt - 2.0)), 255, 0));
                    g.fillRect(i2, j2, 1, 1);
                } else {
                    g.setColor(Color.BLACK);
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

    public int isJulia(Complex c) {
        Complex jConst = new Complex(0.355534, -0.337292);

        int i;

        for (i = 0; i < iterations; i++) {
            //System.out.println(z.toString());
            c = c.times(c).plus(jConst);

            double mag = c.imag() * c.imag() + c.real() * c.real();

            if (mag > 4) {
                break;
            }

        }
        return i;
    }


    /**
     * handles the zoom event
     * @param e
     */
    public void mouseClicked(MouseEvent e) {
        //left click == zoom in
        if (e.getButton() == 1) {
            this.x += (e.getX() - 200) / (300.0 * zoom);
            this.y += (e.getY() - 150) / (300.0 * zoom);
            this.zoom *= 2;
        }

        //right click == zoom out
        if (e.getButton() == 3 && zoom > 1) {
            this.x -= (e.getX() - 200) / (300.0 * zoom);
            this.y -= (e.getY() - 150) / (300.0 * zoom);
            this.zoom *= (1.0/2);
        }

        repaint();

    }

    public void mousePressed(MouseEvent e) {
        this.oldX = e.getX();
        this.oldY = e.getY();
    }

    /**
     * handles the drag event - moving the camera
     * @param e
     */
    public void mouseReleased(MouseEvent e) {
        //System.out.println(e.getX() + "   " + this.oldX);
        double xDiff = e.getX() - this.oldX;
        double yDiff = e.getY() - this.oldY;

        this.x -= xDiff / (300.0 * zoom);
        this.y -= yDiff / (300.0 * zoom);

        repaint();
    }

    public void mouseEntered(MouseEvent e) {

    }

    public void mouseExited(MouseEvent e) {

    }

    public static void main(String[] args) {
        mandelbrot mandelbrot = new mandelbrot(1000, 1);
    }
}
