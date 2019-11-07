import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class NeuralNetwork {
    private List<Layer> layers;
    private TrainingSet trainingSet;

    /**
     * Initialize the neural network
     *
     * @param layerSizes: an array of ints, where each int entry represents a
     *                    new layer with entry value neurons.
     */
    public NeuralNetwork(int[] layerSizes, TrainingSet trainingSet) {
        this.layers = new ArrayList<>();
        this.trainingSet = trainingSet;

        for (int i = 0; i < layerSizes.length; i++) {
            Layer layer;
            if (i == 0) {
                layer = new Layer(layerSizes[i], null);
            } else {
                layer = new Layer(layerSizes[i], layers.get(i - 1));
            }

            this.layers.add(layer);
        }
        randomizeAll();
    }

    public List<Layer> getLayers() {
        return this.layers;
    }

    /**
     * sets the first layer of activations to a given input
     *
     * @param activations
     */
    public void inputActivations(Double activations[]) {
        for (int i = 0; i < activations.length; i++) {
            this.layers.get(0).getNeurons().get(i).setActivation(activations[i]);
        }
    }

    /**
     * runs the calculations for each neuron on each layer, hence, updating
     * the output and all neuron activation values
     */
    public void calculate() {
        for (int i = 1; i < this.layers.size(); i++) {
            this.layers.get(i).updateNeuronActivations();
        }
    }

    /**
     * calculates the cost/level of difference between the actual result and
     * expected result
     *
     * @param actual
     * @param expected
     * @return
     */
    public double cost(Double actual[], Double expected[]) {
        double sum = 0;
        for (int i = 0; i < actual.length; i++) {
            sum += Math.pow((actual[i] - expected[i]), 2.0);
        }
        return sum / actual.length;
    }

    /**
     * grabs the output/result of the NN
     *
     * @return the list of neurons activations on the last layer
     */
    public Double[] getOutput() {
        Layer last = this.layers.get(this.layers.size() - 1);
        Double output[] = new Double[last.getNeurons().size()];
        for (int i = 0; i < last.getNeurons().size(); i++) {
            output[i] = last.getNeurons().get(i).getActivation();
        }
        return output;
    }

    public void trainingLoop(int iterations, double learningRate) {

        for (int i = 0; i < iterations; i++) {
            int randomDataPoint =
                    (int) (Math.random()*this.trainingSet.getData().size());

        }
    }


    /**
     * randomises the bias's and weights for all layers other than the first
     */
    public void randomizeAll() {
        for (Layer layer : this.layers) {
            layer.randomiseNeurons();
        }
    }

    @Override
    public String toString() {
        String str = "";

        for (int i = 0; i < layers.size(); i++) {
            str += String.format("\nLayer %d:\n" + layers.get(i), i);
        }

        return str;
    }

    public static void main(String[] args) {
        int layers[] = {3, 2, 1};

        Double in1[] = {0.34, 0.1, 0.7};
        Double in2[] = {0.0, 1.0, 0.1};
        Double out1[] = {0.2};
        Double out2[] = {0.3};

        TrainingSet test = new TrainingSet(3, 1);
        test.addData(in1, out1);
        test.addData(in2, out2);

        NeuralNetwork nn = new NeuralNetwork(layers, test);
        nn.inputActivations(in1);
        nn.calculate();

        System.out.println(nn);

        System.out.println(nn.cost(nn.getOutput(), out1));
    }
}
