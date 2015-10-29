package expressions;

public interface Start {
	Intermediate expr(double value);
	Intermediate expr(Expr e);
}
