import java.util.ArrayList;
import java.util.List;

public class NeuralNetwork {
    private List<Layer> layers;

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

    public void randomizeAll() {
        for (Layer layer : this.layers) {
            layer.randomiseNeurons();
        }
    }

    public static void main(String[] args) {
        int layers[] = {3, 2, 1};
        NeuralNetwork nn = new NeuralNetwork(layers);
        System.out.println(nn.getLayers());
    }
}
