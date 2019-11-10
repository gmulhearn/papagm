package nn;

import java.io.Serializable;
import java.util.HashMap;

public class TrainingSet implements Serializable {
    private HashMap<Double[], Double[]> trainingSet;
    private int numIn;
    private int numOut;

    public TrainingSet(int numInputs, int numOutputs) {
        this.trainingSet = new HashMap<>();
        this.numIn = numInputs;
        this.numOut = numOutputs;
    }

    public void addData(Double input[], Double output[]) {
        if (input.length != numIn || output.length != numOut) {
            throw new IllegalArgumentException();
        } else {
            this.trainingSet.put(input, output);
        }
    }

    public HashMap<Double[], Double[]> getData() {
        return this.trainingSet;
    }
}
