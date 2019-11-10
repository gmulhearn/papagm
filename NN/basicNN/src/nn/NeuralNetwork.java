package nn;

import java.io.Serializable;
import java.util.*;

//big inspiration from : https://github.com/yacineMahdid/DeepLearning/blob/master/Neural%20Network%20From%20Scratch/src/NeuralNetwork.java

public class NeuralNetwork implements Serializable {
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

    public static double indexOfHighestOut(Double[] output) {
        double highest = -1;
        double index = -1;
        for (int i = 0; i < output.length; i++) {
            if (output[i] > highest) {
                highest = output[i];
                index = i;
            }
        }

        return index;
    }

    public void trainingLoop(int iterations, double learningRate) {

        for (int i = 0; i < iterations; i++) {
            int randomDataPoint =
                    (int) (Math.random()*this.trainingSet.getData().size());
            Double[] in =
                    (Double[]) this.trainingSet.getData().keySet().toArray()[randomDataPoint];
            Double[] expected = this.trainingSet.getData().get(in);

            inputActivations(in);
            calculate();

            backPropogate(expected, learningRate);
        }
    }

    public void backPropogate(Double[] expectedOut, double learningRate) {
        //method from https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/

        //output layer:
        List<Neuron> neurons =
                this.layers.get(this.layers.size() - 1).getNeurons();

        for (int i = 0;  i < neurons.size(); i++) {
            double delta = // = - ((target - out) * out(1 - out))
                    -1 * (expectedOut[i] - getOutput()[i]) * (getOutput()[i] * (1 - getOutput()[i]));
            neurons.get(i).setStoredDelta(delta);

            neurons.get(i).setStoredBias(neurons.get(i).getBias() + delta * learningRate * 0.0);


            for (int j = 0; j < neurons.get(i).getParentNeurons().size(); j++) {
                Neuron parent =
                        (Neuron) neurons.get(i).getParentNeurons().keySet().toArray()[j];
                double err =
                        delta * parent.getActivation();
                double newWeight =
                        neurons.get(i).getParentNeurons().get(parent) - learningRate * err;

                neurons.get(i).storeNewWeight(parent, newWeight);
            }
        }

        //hidden layers
        for (int i = this.layers.size() - 2; i > 0; i--) {
            neurons = this.layers.get(i).getNeurons();

            for (int j = 0; j < neurons.size(); j++) {
                Neuron neuron = neurons.get(j);

                double deltaSum = 0;

                //sum all deltas * weights of the neurons connected to this
                // neuron (neuron j in layer i).
                for (Neuron child : this.layers.get(i + 1).getNeurons()) {
                    deltaSum += child.getParentNeurons().get(neuron) * child.getStoredDelta();
                }

                // delta = deltaSum * out * (1 - out)
                double delta =
                        deltaSum * neuron.getActivation() * (1 - neuron.getActivation());

                neuron.setStoredDelta(delta);

                neuron.setStoredBias(neuron.getBias() + delta * learningRate * 0.0);

                for (int k = 0; k < neuron.getParentNeurons().size(); k++) {
                    Neuron parent =
                            (Neuron) neuron.getParentNeurons().keySet().toArray()[k];
                    double err =
                            delta * parent.getActivation();
                    double newWeight =
                            neuron.getParentNeurons().get(parent) - learningRate * err;

                    neuron.storeNewWeight(parent, newWeight);
                }

            }
        }

        //update weights
        for (int i = 1; i < this.layers.size(); i++) {
            for (Neuron neuron : this.layers.get(i).getNeurons()) {
                neuron.updateNeuron();
            }
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
            str += String.format("\nnn.Layer %d:\n" + layers.get(i), i);
        }

        return str;
    }

    public static void main(String[] args) {

    }
}
