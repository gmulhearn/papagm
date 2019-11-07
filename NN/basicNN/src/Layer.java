import java.util.ArrayList;
import java.util.List;

public class Layer {
    private List<Neuron> neurons;
    private Layer previous;

    public Layer(int numNeurons, Layer previous) {
        this.neurons = new ArrayList<>();
        this.previous = previous;
        for (int i = 0; i < numNeurons; i++) {
            Neuron neuron = new Neuron(i);
            neuron.setActivation(0);
            this.neurons.add(neuron);
        }

        if (previous != null) {
            for (Neuron neuron : this.neurons) {
                for (Neuron parentNeuron : previous.getNeurons()) {
                    neuron.setWeight(parentNeuron, 0);
                }
            }
        }
    }

    public List<Neuron> getNeurons() {
        return this.neurons;
    }

    public void randomiseNeurons() {
        for (Neuron neuron : neurons) {

            if (previous == null) {
                neuron.setActivation(Math.random() * 2 - 1);
            } else {
                for (Neuron parentNeuron : previous.getNeurons()) {
                    neuron.setWeight(parentNeuron, Math.random() * 10 - 5); //tweak
                }
                neuron.setBias(Math.random() * 30 - 15); //tweak
                neuron.setActivation(neuron.calculateActivation());
            }
        }
    }

    public void updateNeuronActivations() {
        for (Neuron neuron : this.neurons) {
            neuron.setActivation(neuron.calculateActivation());
        }
    }

    @Override
    public String toString() {
        return String.format("Layer neurons: " + neurons + "\n");
    }

}
