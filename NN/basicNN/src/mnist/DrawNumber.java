package mnist;

import nn.NeuralNetwork;
import nn.NeuralNetworkSave;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class DrawNumber extends JFrame implements MouseMotionListener {

    private int x = 0;
    private int y = 0;
    private Double[] input;
    private NeuralNetwork nn;

    private int prevPixelX = 0;
    private int prevPixelY = 0;

    public DrawNumber() throws IOException, ClassNotFoundException {

        input = new Double[784];
        for (int i = 0; i < 784; i++) {
            input[i] = 0.0;
        }

        System.out.println("Loading Neural Network...");
        this.nn = NeuralNetworkSave.loadNN("Mnist10000iterations.txt");
        System.out.println("Finished.");

        setTitle("Drawing to Number");
        //setSize(29, 29);
        setBounds(100, 100, 840, 840);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        JLabel instruction = new JLabel("Draw a number", JLabel.RIGHT);
        JButton guess = new JButton("Guess");
        JButton clear = new JButton("Clear");

        Container c = this.getContentPane();
        c.setLayout(new BorderLayout());
        c.add(instruction, BorderLayout.SOUTH);
        //c.add(guess, BorderLayout.SOUTH);
        c.add(clear, BorderLayout.SOUTH);

        clear.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                for (int i = 0; i < 784; i++) {
                    input[i] = 0.0;
                }
                Graphics g = getGraphics();
                g.clearRect(0, 0, 840, 840);
            }
        });

        c.addMouseMotionListener(this);

        setVisible(true);
    }

    public void mouseMoved(MouseEvent e) {

    }

    public void mouseDragged(MouseEvent e) {
        x = ((e.getX() + 20) / 30) * 30;
        y = ((e.getY() + 50) / 30) * 30;
        repaint();
        //System.out.println(y);
    }

    public void paint(Graphics g) {
        if (x / 30 != prevPixelX || y / 30 != prevPixelY) {

            g.setColor(Color.BLACK);
            g.fillRect(x, y, 30, 30);
            int index = (y / 28) * 28 + (x / 28);
            this.input[index] = 1.0;

            nn.inputActivations(this.input);
            nn.calculate();
            System.out.println("Guess: " + NeuralNetwork.indexOfHighestOut(nn.getOutput()));

            g.setColor(Color.GRAY);
            //1 pixel right
            if (this.input[index + 1] == 0) {
                this.input[index + 1] = 0.5;
                g.fillRect(x + 30, y, 30, 30);
            } //1 pixel left
            if (this.input[index - 1] == 0) {
                this.input[index - 1] = 0.5;
                g.fillRect(x - 30, y, 30, 30);
            } //1 pixel below
            if (this.input[index + 28] == 0) {
                this.input[index + 28] = 0.5;
                g.fillRect(x, y + 30, 30, 30);
            }
            if (this.input[index - 28] == 0) {
                this.input[index + 1] = 0.5;
                g.fillRect(x, y - 30, 30, 30);
            }


            prevPixelX = x/30;
            prevPixelY = y/30;
        }
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException {
        DrawNumber canvas = new DrawNumber();
    }
}
