import java.util.ArrayList;
import java.util.List;

public class NeuralNetwork {
    private List<Layer> layers;

    /**
     * Initialize the neural network
     *
     * @param layerSizes: an array of ints, where each int entry represents a
     *                    new layer with entry value neurons.
     */
    public NeuralNetwork(int[] layerSizes) {
        this.layers = new ArrayList<>();

        for (int i = 0; i < layerSizes.length; i++) {
            Layer layer;
            if (i == 0) {
                layer = new Layer(layerSizes[i], null);
            } else {
                layer = new Layer(layerSizes[i], layers.get(i - 1));
            }

            this.layers.add(layer);
        }
    }

    public List<Layer> getLayers() {
        return this.layers;
    }

    /**
     * sets the first layer of activations to a given input
     * @param activations
     */
    public void inputActivations(double activations[]) {
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
     * grabs the output/result of the NN
     * @return the list of neurons on the last layer
     */
    public List<Neuron> getOutput() {
        return this.layers.get(this.layers.size()).getNeurons();
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
        double input[] = {0.34, 0.1, 0.7};
        NeuralNetwork nn = new NeuralNetwork(layers);
        nn.inputActivations(input);
        nn.randomizeAll();
        System.out.println(nn);
    }
}
