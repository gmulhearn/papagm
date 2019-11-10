package nn;

import java.io.*;

public class NeuralNetworkSave {

    public static void saveNN(String file, NeuralNetwork nn) throws IOException {

        FileOutputStream f = new FileOutputStream(new File(file));
        ObjectOutputStream o = new ObjectOutputStream(f);

        // Write objects to file
        o.writeObject(nn);

        o.close();
        f.close();
    }

    public static NeuralNetwork loadNN(String file) throws IOException, ClassNotFoundException {

        FileInputStream fi = new FileInputStream(new File(file));
        ObjectInputStream oi = new ObjectInputStream(fi);

        // Read objects
        NeuralNetwork nn = (NeuralNetwork) oi.readObject();

        oi.close();
        fi.close();

        return nn;
    }
}
