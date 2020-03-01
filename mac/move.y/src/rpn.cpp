//
//  rpn.cpp
//  movey
//
//  Created by YotioSoft on 2020/02/21.
//

#include "rpn.hpp"

bool ReversePolishNotation::set(String argFormula) {
	// 数式をまとまりに分ける
	Array<RPNnode*> formula;
	
	double tempFigure = 0.0;
	RPNnode* nodeTempP = new RPNnode();
	formula.push_back(nodeTempP);
	int afterDecimalCount = 0;
	
	for (int i=0; i<argFormula.length(); i++) {
		if (isFigure(argFormula[i])) {
			if (afterDecimalCount >= 1) {
				nodeTempP->figure = nodeTempP->figure + ctoi(argFormula[i]) * powf(10, -afterDecimalCount);
				afterDecimalCount ++;
			}
			else {
				nodeTempP->figure = nodeTempP->figure * 10 + ctoi(argFormula[i]);
			}
		}
		else if (argFormula[i] == '.') {
			afterDecimalCount = 1;
		}
		else {
			tempFigure = 0.0;
			afterDecimalCount = 0;
			
			nodeTempP = new RPNnode();
			formula.push_back(nodeTempP);
			if (argFormula.substr(i, 1) == U"+") {
				nodeTempP->type = symbols::addition;
			}
			else if (argFormula.substr(i, 1) == U"-") {
				nodeTempP->type = symbols::subtraction;
			}
			else if (argFormula.substr(i, 1) == U"*") {
				nodeTempP->type = symbols::multiplication;
			}
			else if (argFormula.substr(i, 1) == U"/") {
				nodeTempP->type = symbols::division;
			}
			else {
				vars << argFormula.substr(i, 1);
				nodeTempP->type = symbols::var;
				nodeTempP->var = argFormula.substr(i, 1);
				
				/*
				free(nodeTempP);
				formula.erase(formula.end()-1);
				 */
			}
			
			if (i + 1 < argFormula.length()) {
				nodeTempP = new RPNnode();
				formula.push_back(nodeTempP);
			}
		}
	}
	
	// 二分木に変換
	Array<RPNnode*> stack;
	RPNnode* topNode = nullptr;
	RPNnode* curNode = nullptr;
	
	for (int i=0; i<formula.size(); i++) {
		stack.push_back(formula[i]);
		
		switch (stack.back()->type) {
			case symbols::figure:
			case symbols::var:
				if (curNode == nullptr) {
					topNode = stack.back();
				}
				else {
					stack.back()->parent = curNode;
					curNode->right = stack.back();
				}
				stack.clear();
				break;
				
			case symbols::addition:
			case symbols::subtraction:
				stack.back()->left = topNode;
				topNode->parent = stack.back();
				topNode = stack.back();
				curNode = topNode;
				stack.clear();
				break;
				
			case symbols::multiplication:
			case symbols::division:
				if (curNode == nullptr) {
					stack.back()->left = topNode;
					topNode->parent = stack.back();
					topNode = stack.back();
					curNode = topNode;
				}
				else {
					if (curNode->type == symbols::figure) {
						stack.back()->left = curNode->right;
						curNode->right->parent = stack.back();
						curNode->right = stack.back();
						curNode = stack.back();
					}
					else if (curNode->type == symbols::multiplication || curNode->type == symbols::division) {
						stack.back()->left = curNode;
						stack.back()->parent = curNode->parent;
						if (curNode->parent != nullptr) {
							curNode->parent->right = stack.back();
						}
						curNode->parent = stack.back();
						curNode = stack.back();
					}
					else {
						stack.back()->left = curNode->right;
						stack.back()->parent = curNode;
						if (curNode->left != nullptr) {
							stack.back()->left->parent = stack.back();
						}
						curNode->right = stack.back();
						curNode = stack.back();
					}
				}
				stack.clear();
				break;
		}
	}
	
	// 二分木 -> 逆ポーランド記法
	nodeTempP = topNode;
	while (nodeTempP != nullptr) {
		if (nodeTempP->left != nullptr) {
			if (!nodeTempP->left->mark) {
				nodeTempP = nodeTempP->left;
				continue;
			}
		}
		if (nodeTempP->right != nullptr) {
			if (!nodeTempP->right->mark) {
				nodeTempP = nodeTempP->right;
				continue;
			}
		}
		
		rpn.push_back(nodeTempP);
		nodeTempP->mark = true;
		nodeTempP = nodeTempP->parent;
	}
	
	return true;
}

bool ReversePolishNotation::isFigure(char argChar) {
	switch (argChar) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
	}
	
	return false;
}

int ReversePolishNotation::ctoi(char argChar) {
	if (argChar >= '0' && argChar <= '9') {
		return argChar - '0';
	}
	return 0;
}

void ReversePolishNotation::reflection(Array<RPNnode*> &argNumStack, Array<RPNnode*> &argVarStack) {
	for (int i=0; i<argNumStack.size(); i++) {
		rpn.push_back(argNumStack[i]);
	}
	argNumStack.clear();
	for (int i=(int)argVarStack.size()-1; i>=0; i--) {
		rpn.push_back(argVarStack[i]);
	}
	argVarStack.clear();
}

String ReversePolishNotation::toString() {
	String rpnStr;
	
	for (int i=0; i<rpn.size(); i++) {
		switch (rpn[i]->type) {
			case symbols::figure:
				rpnStr += Format(rpn[i]->figure);
				break;
				
			case symbols::addition:
				rpnStr += U"+";
				break;
				
			case symbols::subtraction:
				rpnStr += U"-";
				break;
				
			case symbols::multiplication:
				rpnStr += U"*";
				break;
				
			case symbols::division:
				rpnStr += U"/";
				break;
		}
		
		if (i+1 != rpn.size()) {
			rpnStr += U" ";
		}
	}
	
	return rpnStr;
}

double ReversePolishNotation::calc(String argVarName, double argFigure) {
	Array<RPNnode*> newRPN;
	
	// 変数を数値に置き換えて計算する
	for (int i=0; i<rpn.size(); i++) {
		if (rpn[i]->type == symbols::var && rpn[i]->var == argVarName) {
			RPNnode newNode;
			newNode.figure = argFigure;
			newNode.type = symbols::figure;
				
			newRPN.push_back(&newNode);
		}
		else {
			newRPN.push_back(rpn[i]);
		}
	}
	
	return calc(newRPN);
}

double ReversePolishNotation::calc(Array<RPNnode*> argRPN) {
	stack<RPNnode> calculatingStack;
	double tempA;
	double tempB;
	
	for (int i=0; i<rpn.size(); i++) {
		switch (rpn[i]->type) {
			case symbols::figure:
			case symbols::var:
				calculatingStack.push(*argRPN[i]);
				break;
				
			case symbols::addition:
				if (calculatingStack.size() >= 2) {
					tempA = calculatingStack.top().figure;
					calculatingStack.pop();
					tempB = calculatingStack.top().figure;
					calculatingStack.pop();
					
					RPNnode newNode;
					newNode.figure = tempB+tempA;
					newNode.type = symbols::figure;
					calculatingStack.push(newNode);
				}
				break;
				
			case symbols::subtraction:
				if (calculatingStack.size() >= 2) {
					tempA = calculatingStack.top().figure;
					calculatingStack.pop();
					tempB = calculatingStack.top().figure;
					calculatingStack.pop();
					
					RPNnode newNode;
					newNode.figure = tempB-tempA;
					newNode.type = symbols::figure;
					calculatingStack.push(newNode);
				}
				break;
				
			case symbols::multiplication:
				if (calculatingStack.size() >= 2) {
					tempA = calculatingStack.top().figure;
					calculatingStack.pop();
					tempB = calculatingStack.top().figure;
					calculatingStack.pop();
					
					RPNnode newNode;
					newNode.figure = tempB*tempA;
					newNode.type = symbols::figure;
					calculatingStack.push(newNode);
				}
				break;
				
			case symbols::division:
				if (calculatingStack.size() >= 2) {
					tempA = calculatingStack.top().figure;
					calculatingStack.pop();
					tempB = calculatingStack.top().figure;
					calculatingStack.pop();
					
					RPNnode newNode;
					newNode.figure = tempB/tempA;
					newNode.type = symbols::figure;
					calculatingStack.push(newNode);
				}
				break;
		}
	}
	
	return calculatingStack.top().figure;
}

double ReversePolishNotation::calc() {
	return calc(rpn);
}
