package expressions;

public class ExpressionBuilder implements Start{
	
	private final IntermediateScope intermediateScope;
	private Expr e;
	
	private ExpressionBuilder() {
		this.intermediateScope = this.new IntermediateScope();
	}
	
	public static ExpressionBuilder begin() {
		return new ExpressionBuilder();
	}	

	@Override
	public IntermediateScope expr(final double value) {
		ExpressionBuilder.this.e = new Value(value);
		return ExpressionBuilder.this.intermediateScope;
	}

	@Override
	public IntermediateScope expr(Expr e) {
		ExpressionBuilder.this.e = e;
		return ExpressionBuilder.this.intermediateScope;
	}

	
	public class IntermediateScope implements Intermediate {

		@Override
		public Intermediate plus(double value) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '+', new Value(value));
			return this;
		}

		@Override
		public Intermediate plus(Expr right) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '+', right);
            return this;
		}

		@Override
		public Intermediate minus(double value) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '-', new Value(value));
			return this;
		}

		@Override
		public Intermediate minus(Expr right) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '-', right);
            return this;
		}

		@Override
		public Intermediate times(double value) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '*', new Value(value));
			return this;
		}

		@Override
		public Intermediate times(Expr right) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '*', right);
            return this;
		}

		@Override
		public Intermediate divided(double value) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '/', new Value(value));
			return this;
		}

		@Override
		public Intermediate divided(Expr right) {
			ExpressionBuilder.this.e = new Operation(ExpressionBuilder.this.e, '/', right);
            return this;
		}
		
		@Override
		public Expr end() {
			return ExpressionBuilder.this.e;
		}
		
	}
}