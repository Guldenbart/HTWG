package main;

import expressions.Expr;
import expressions.ExpressionBuilder;

public class ExpressionTest {

	public static void main(String[] args) {
		
		// arithmetischer Ausdruck ohne Klammerung:
        Expr x = ExpressionBuilder.begin().expr(1).plus(2).times(3).minus(4).divided(5).end();
        System.out.printf("%s = %f%n", x, x.getValue());



        // arithmetischer Ausdruck mit Klammerung:
        Expr y = ExpressionBuilder.begin().expr(ExpressionBuilder.begin().expr(1).plus(2).end()).times(3).minus(ExpressionBuilder.begin().expr(4).divided(5).end()).end();
        System.out.printf("%s = %f%n", y, y.getValue());
	}

}
