package nn;

import java.io.Serializable;
import java.util.*;

public class Neuron implements Serializable {

    private LinkedHashMap<Neuron, Double> parentNeurons; //weights
    private LinkedHashMap<Neuron, Double> storedParentWeights;

    private double activation;
    private double bias;
    private double storedDelta;
    private double storedBias;

    public int ID;

    public Neuron(int ID) {
        this.parentNeurons = new LinkedHashMap<>();
        this.storedParentWeights = new LinkedHashMap<>();
        this.ID = ID;
    }

    public void setWeight(Neuron neuron, double weight) {
        this.parentNeurons.put(neuron, weight);
    }

    public void storeNewWeight(Neuron neuron, double weight) {
        this.storedParentWeights.put(neuron, weight);
    }

    public LinkedHashMap<Neuron, Double> getParentNeurons() {
        return this.parentNeurons;
    }

    public double getBias() {
        return this.bias;
    }

    public void setBias(double bias) {
        this.bias = bias;
    }

    public void setStoredBias(double bias) {
        this.storedBias = bias;
    }

    public double getActivation() {
        return this.activation;
    }

    public void setActivation(double activation) {
        this.activation = activation;
    }

    public double getStoredDelta() {
        return this.storedDelta;
    }
     public void setStoredDelta(double delta) {
        this.storedDelta = delta;
     }


    public double calculateActivation() {
        double sum = 0;
        for (Map.Entry<Neuron, Double> entry : parentNeurons.entrySet()) {
            sum += entry.getKey().getActivation() * entry.getValue();
        }

        sum += this.bias;

        return sigmoid(sum);
    }

    public void updateNeuron() {
        for (int i = 0; i < this.parentNeurons.size(); i++) {
            Neuron parent =
                    (Neuron) this.parentNeurons.keySet().toArray()[i];

            this.setWeight(parent, this.storedParentWeights.get(parent));
        }
        this.bias = this.storedBias;
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
