package expressions;

public class Value implements Expr {

	private double value;

    public Value(double value) {
        this.value = value;
    }

    @Override
    public double getValue() {
        return this.value;
    }

}
