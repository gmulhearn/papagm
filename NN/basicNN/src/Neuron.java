import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Neuron {
    private double activation;
    private HashMap<Neuron, Double> parentNeurons;
    private double bias;
    public int ID;

    public Neuron(int ID) {
        this.parentNeurons = new HashMap<>();
        this.ID = ID;
    }


    public void setWeight(Neuron neuron, double weight) {
        this.parentNeurons.put(neuron, weight);
    }

    public void setBias(double bias) {
        this.bias = bias;
    }

    public double getActivation() {
        return this.activation;
    }

    public void setActivation(double activation) {
        this.activation = activation;
    }

    public double calculateActivation() {
        double sum = 0;
        for (Map.Entry<Neuron, Double> entry : parentNeurons.entrySet()) {
            sum += entry.getKey().getActivation() * entry.getValue();
        }

        sum += this.bias;

        return sigmoid(sum);
    }

    public static double sigmoid(double x) {
        return (1 / (1 + Math.pow(Math.E, (-1 * x))));
    }

    @Override
    public String toString() {
        String str = "";
        for (Map.Entry<Neuron, Double> entry : parentNeurons.entrySet()) {
            str += String.format("(" + entry.getKey().ID + "," +
                    entry.getValue() + "), ");
        }
        return (String.format("\nID: " + ID + " activation: " + activation +
                " bias: " + bias + " weights: " + str));
    }
}
