package expressions;

public interface Intermediate extends End{
	Intermediate plus(double value);
	Intermediate plus(Expr s);
	
	Intermediate minus(double value);
	Intermediate minus(Expr s);
	
	Intermediate times(double value);
	Intermediate times(Expr s);
	
	Intermediate divided(double value);
	Intermediate divided(Expr s);
}
