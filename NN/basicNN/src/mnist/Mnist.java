package mnist;

import nn.NeuralNetwork;
import nn.TrainingSet;

import java.io.File;

/**
 * Created by Luecx on 10.08.2017.
 */
public class Mnist {

    public static void main(String[] args) {
//        Network network = new Network(784, 70, 35, 10);
          TrainingSet set = createTrainSet(0,4999, 0);
//        trainData(network, set, 100, 50, 100);
//
//        TrainSet testSet = createTrainSet(5000,9999);
//        testTrainSet(network, testSet, 10);
    }

    public static TrainingSet createTrainSet(int start, int end, int type) {

        TrainingSet set = new TrainingSet(28 * 28, 10);

        try {

            String path = new File("").getAbsolutePath();

            MnistImageFile m = new MnistImageFile(path + "/resources" +
                    "/train-images.idx3-ubyte", "rw");
            MnistLabelFile l = new MnistLabelFile(path + "/resources" +
                    "/train-labels.idx1-ubyte", "rw");
            if (type == 1) {
                m = new MnistImageFile(path + "/resources/t10k-images.idx3-ubyte", "rw");
                l = new MnistLabelFile(path + "/resources/t10k-labels.idx1-ubyte", "rw");
            }

            for(int i = start; i <= end; i++) {
                if(i % 100 ==  0){
                    System.out.println("prepared: " + i);
                }

                Double[] input = new Double[28 * 28];
                Double[] output = new Double[10];
                for (int num = 0; num < input.length; num++) {
                    input[num] = (double) 0;
                }
                for (int num = 0; num < output.length; num++) {
                    output[num] = (double) 0;
                }

                output[l.readLabel()] = 1d;
                for(int j = 0; j < 28*28; j++){
                    input[j] = (double)m.read() / (double)256;
                }

                set.addData(input, output);
                m.next();
                l.next();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

         return set;
    }

//    public static void trainData(Network net,TrainSet set, int epochs, int loops, int batch_size) {
//        for(int e = 0; e < epochs;e++) {
//            net.train(set, loops, batch_size);
//            System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>   "+ e+ "   <<<<<<<<<<<<<<<<<<<<<<<<<<");
//        }
//    }

    public static void testTrainSet(NeuralNetwork net, TrainingSet set, int printSteps) {
        int correct = 0;
        for(int i = 0; i < set.getData().size(); i++) {

            Double[] in = (Double[]) set.getData().keySet().toArray()[i];
//            double highest =
//                    NetworkTools.indexOfHighestValue(net.calculate(set.getInput(i)));
            net.inputActivations(in);
            net.calculate();
            double highest = NeuralNetwork.indexOfHighestOut(net.getOutput());

//            double actualHighest = NetworkTools.indexOfHighestValue(set.getOutput(i));
            double actualHighest =
                    NeuralNetwork.indexOfHighestOut(set.getData().get(in));

            //System.out.println(highest);

            if(highest == actualHighest) {

                correct ++ ;
            }
            if(i % printSteps == 0) {
                System.out.println(i + ": " + (double)correct / (double) (i + 1));
            }
        }
        System.out.println("Testing finished, RESULT: " + correct + " / " + set.getData().size() + "  -> " + (double)correct / (double)set.getData().size() +" %");
    }
}
