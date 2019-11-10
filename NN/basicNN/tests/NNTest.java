import org.junit.Test;

import java.util.ArrayList;

import static org.junit.Assert.*;

public class NNTest {

    @Test
    public void testXOR2() {
        int layers[] = {2, 6, 1};

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

        //System.out.println(nn);

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
}
