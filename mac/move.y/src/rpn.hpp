//
//  rpn.hpp
//  movey
//
//  Created by YotioSoft on 2020/02/21.
//

#ifndef rpn_hpp
#define rpn_hpp

#include "header.h"

namespace symbols {
	enum Type {
		figure			= 0,
		addition		= 1,
		subtraction		= 2,
		multiplication	= 3,
		division		= 4,
		var				= 5,
	};
}

typedef struct RPNnode {
	RPNnode* left = nullptr;
	RPNnode* right = nullptr;
	RPNnode* parent = nullptr;
	
	bool mark = false;
	
	symbols::Type type;
	double figure;
	String var;
} RPNnode;

typedef struct RPNvar {
	String varName;
	double figure;
} RPNvar;

class ReversePolishNotation {
public:
	bool set(String argFormula);
	
	bool isFigure(char argChar);
	
	int ctoi(char argChar);
	
	void reflection(Array<RPNnode*> &argNumStack, Array<RPNnode*> &argVarStack);
	
	String toString();
	
	stack<RPNnode> simplification();
	
	double calc(Array<RPNvar> vars);
	
private:
	RPNnode* node;
	Array<RPNnode*> rpn;
	Array<String> vars;
};

#endif /* rpn_hpp */
