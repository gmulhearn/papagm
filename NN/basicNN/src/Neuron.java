import java.util.*;

public class Neuron {

    private LinkedHashMap<Neuron, Double> parentNeurons; //weights
    private LinkedHashMap<Neuron, Double> storedParentNeurons;

    private double activation;
    private double bias;
    private double storedDelta;

    public int ID;

    public Neuron(int ID) {
        this.parentNeurons = new LinkedHashMap<>();
        this.storedParentNeurons = new LinkedHashMap<>();
        this.ID = ID;
    }

    public void setWeight(Neuron neuron, double weight) {
        this.parentNeurons.put(neuron, weight);
    }

    public void storeNewWeight(Neuron neuron, double weight) {
        this.storedParentNeurons.put(neuron, weight);
    }

    public LinkedHashMap<Neuron, Double> getParentNeurons() {
        return this.parentNeurons;
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

    public void updateNeuronParentWeights() {
        for (int i = 0; i < this.parentNeurons.size(); i++) {
            Neuron parent =
                    (Neuron) this.parentNeurons.keySet().toArray()[i];

            this.setWeight(parent, this.storedParentNeurons.get(parent));
        }
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
