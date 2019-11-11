import mnist.Mnist;
import nn.NeuralNetwork;
import nn.NeuralNetworkSave;
import nn.TrainingSet;
import org.junit.Test;

import java.io.IOException;
import java.util.Map;

import static org.junit.Assert.*;

public class NNTest {

    @Test
    public void testXOR2() {
        int layers[] = {2, 6, 3, 1};

        Double in1[] = {1.0, 0.0};
        Double in2[] = {0.0, 1.0};
        Double in3[] = {0.0, 0.0};
        Double in4[] = {1.0, 1.0};
        Double out1[] = {1.0};
        Double out2[] = {1.0};
        Double out3[] = {0.0};
        Double out4[] = {0.0};

        TrainingSet test = new TrainingSet(2, 1);
        test.addData(in1, out1);
        test.addData(in2, out2);
        test.addData(in3, out3);
        test.addData(in4, out4);

        NeuralNetwork nn = new NeuralNetwork(layers, test);

        System.out.println(nn);

        nn.trainingLoop(100000, 0.4);

        System.out.println(nn);

        nn.inputActivations(in1);
        nn.calculate();
        assertTrue(Math.round(nn.getOutput()[0]) == out1[0]);

        nn.inputActivations(in2);
        nn.calculate();
        assertTrue(Math.round(nn.getOutput()[0]) == out2[0]);

        nn.inputActivations(in3);
        nn.calculate();
        assertTrue(Math.round(nn.getOutput()[0]) == out3[0]);

        nn.inputActivations(in4);
        nn.calculate();
        assertTrue(Math.round(nn.getOutput()[0]) == out4[0]);
    }

    @Test
    public void testXOR3() {
        int layers[] = {3, 6, 1};

        Double in1[] = {0.0, 0.0, 0.0};
        Double in2[] = {0.0, 0.0, 1.0};
        Double in3[] = {0.0, 1.0, 0.0};
        Double in4[] = {0.0, 1.0, 1.0};
        Double in5[] = {1.0, 0.0, 0.0};
        Double in6[] = {1.0, 0.0, 1.0};
        Double in7[] = {1.0, 1.0, 0.0};
        Double in8[] = {1.0, 1.0, 1.0};

        Double out1[] = {0.0};
        Double out2[] = {1.0};
        Double out3[] = {1.0};
        Double out4[] = {0.0};
        Double out5[] = {1.0};
        Double out6[] = {0.0};
        Double out7[] = {0.0};
        Double out8[] = {1.0};

        TrainingSet test = new TrainingSet(3, 1);
        test.addData(in1, out1);
        test.addData(in2, out2);
        test.addData(in3, out3);
        test.addData(in4, out4);
        test.addData(in5, out5);
        test.addData(in6, out6);
        test.addData(in7, out7);
        test.addData(in8, out8);

        NeuralNetwork nn = new NeuralNetwork(layers, test);

        //System.out.println(nn);

        nn.trainingLoop(100000, 0.4);

        System.out.println(nn);

        for (Map.Entry<Double[], Double[]> entry : test.getData().entrySet()) {
            nn.inputActivations(entry.getKey());
            nn.calculate();
            assertTrue(Math.round(nn.getOutput()[0]) == entry.getValue()[0]);
        }
    }

    @Test
    public void MnistTest1() throws IOException, ClassNotFoundException {
        TrainingSet set = Mnist.createTrainSet(0, 10000, 0);

        int layers[] = {784, 70, 35, 10};


        NeuralNetwork nn = new NeuralNetwork(layers, set);

        nn.trainingLoop(1000, 0.4);

        //NeuralNetworkSave.saveNN("Mnist1000iterations.txt", nn);
        Mnist.testTrainSet(nn, set, 10);
    }

    @Test
    public void MnistTest10000PreBuilt() throws IOException, ClassNotFoundException {
        NeuralNetwork nn = NeuralNetworkSave.loadNN("Mnist10000iterations.txt");

        TrainingSet set = Mnist.createTrainSet(0, 5000, 1);

        Mnist.testTrainSet(nn, set, 100);

    }

    @Test
    public void loadSaveNN() throws IOException, ClassNotFoundException {
        int layers[] = {2, 6, 3, 1};

        Double in1[] = {1.0, 0.0};
        Double in2[] = {0.0, 1.0};
        Double in3[] = {0.0, 0.0};
        Double in4[] = {1.0, 1.0};
        Double out1[] = {1.0};
        Double out2[] = {1.0};
        Double out3[] = {0.0};
        Double out4[] = {0.0};

        TrainingSet test = new TrainingSet(2, 1);
        test.addData(in1, out1);
        test.addData(in2, out2);
        test.addData(in3, out3);
        test.addData(in4, out4);

        NeuralNetwork nn = new NeuralNetwork(layers, test);

        nn.trainingLoop(100000, 0.4);

        NeuralNetworkSave.saveNN("save.txt", nn);

        NeuralNetwork loaded = NeuralNetworkSave.loadNN("save.txt");

        nn.calculate();

        loaded.calculate();

        assertEquals(nn.getOutput()[0], loaded.getOutput()[0]);
    }
}
