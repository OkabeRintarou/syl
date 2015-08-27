#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <map>

class file_open_exception :public std::exception
{
public:
	file_open_exception(const std::string & msg)
		:exception(msg.c_str()){}
	file_open_exception(const char * msg)
		:exception(msg){}
};

extern void ScannerError(const std::string & msg);

typedef std::map<char, int> BinaryOpPrecedenceMap;

enum class TokenType
{
	TOKEN_UNKNOW,
	TOKEN_INTEGER,
	TOKEN_OPERATION,
	TOKEN_LEFT_PARAM,
	TOKEN_RIGHT_PARAM,
	TOKEN_END_OF_FILE
};

class Token
{
public:
	Token(TokenType type = TokenType::TOKEN_UNKNOW)
		:type_(type)
	{
		value_.number_ = 0;
	}

	Token(TokenType type,int number)
		:type_(type)
	{
		value_.number_ = number;
	}

	Token(TokenType type, char character)
		:type_(type)
	{
		value_.op_ = character;
	}

	TokenType GetType()const{ return type_; }
	char GetOperation()const{ return value_.op_; }
	int GetValue()const
	{
		switch (type_){
		case TokenType::TOKEN_INTEGER:
			return value_.number_; break;
		case TokenType::TOKEN_OPERATION:
			return value_.op_; break;
		case TokenType::TOKEN_LEFT_PARAM:
		case TokenType::TOKEN_RIGHT_PARAM:
			return value_.param_; break;
		default:return -1; 
		}
	}
private:
	TokenType type_;
	union TokenValue
	{
		int number_;
		char op_;
		char param_;
	}value_;
};

class Scanner
{
public:
	Scanner(const std::string & filename);
	Token GetToken()const{ return token_; }
	Token GetNextToken();
private:
	void Advance();
	char Peek();
	void Preprocess();
	Token HandleInteger();
	inline bool IsOperator()const;
	inline bool IsParam()const;
	inline bool IsLeftParam()const;
	inline bool IsRightParam()const;
private:
	std::fstream file_;
	Token token_;
	char cur_char_;
};

Scanner::Scanner(const std::string & filename)
{
	try{
		file_.open(filename);
		if (file_.fail()){
			throw file_open_exception{ filename + std::string(" open failed.") };
		}
		cur_char_ = -1;
	}
	catch (file_open_exception){
		ScannerError("file open failed.\n");
	}
	
}

Token Scanner::GetNextToken()
{
	Preprocess();
	if (file_.eof()){
		return Token(TokenType::TOKEN_END_OF_FILE, -1);
	}

	if (std::isdigit(cur_char_)){
		return HandleInteger();
	}
	else if (IsOperator()){
		char op = cur_char_;
		Advance();
		return Token(TokenType::TOKEN_OPERATION, op);
	}
	else if (IsLeftParam()){
		Advance();
		return Token(TokenType::TOKEN_LEFT_PARAM, '(');
	}
	else if (IsRightParam()){
		Advance();
		return Token(TokenType::TOKEN_RIGHT_PARAM, ')');
	}

	return Token();
}

void Scanner::Advance()
{
	cur_char_ = file_.get();
}

char Scanner::Peek()
{
	return file_.peek();
}

void Scanner::Preprocess()
{
	if (cur_char_ == -1){
		Advance();
	}
	while (std::isspace(cur_char_)){
		Advance();
	}
}

bool Scanner::IsOperator()const
{
	return cur_char_ == '+' ||
		cur_char_ == '-' ||
		cur_char_ == '*' ||
		cur_char_ == '/';
}

bool Scanner::IsParam()const
{
	return cur_char_ == '(' || cur_char_ == ')';
}

bool Scanner::IsLeftParam()const
{
	return cur_char_ == '(';
}

bool Scanner::IsRightParam()const
{
	return cur_char_ == ')';
}

Token Scanner::HandleInteger()
{
	int result = static_cast<int>(cur_char_ - '0');
	Advance();
	while (std::isdigit(cur_char_)){
		result *= 10;
		result += static_cast<int>(cur_char_ - '0');
		Advance();
	}

	return Token(TokenType::TOKEN_INTEGER, result);
}

class ExprAST
{
public:
	virtual ~ExprAST(){}
	virtual int GetValue() = 0;
};

class IntegerExpr:public ExprAST
{
public:
	IntegerExpr(int number) :number_(number){}
	virtual int GetValue(){ return number_; }
private:
	int number_;
};

class BinaryOpExpr :public ExprAST
{
public:
	BinaryOpExpr(int op, ExprAST * left, ExprAST * right)
		:op_(op), left_(left), right_(right){}
	virtual int GetValue()
	{
		
		int rv = right_->GetValue();
		switch (op_){
		case '+':
			return left_->GetValue() + right_->GetValue();
		case '-':
			return left_->GetValue() - right_->GetValue();
		case '*':
			return left_->GetValue() * right_->GetValue();
		case '/':
			
			if (0 == rv){
				throw std::runtime_error{ "denominator can't equal zero!" };
			}
			return left_->GetValue() / right_->GetValue();
		default:
			return 0;
		}
	}
private:
	int op_;
	ExprAST * left_;
	ExprAST * right_;
};


class FAO
{
public:
	FAO(const std::string & filename);
public:
	int Compute();
private:
	void Advance();
	// @@ parameter Expc indicateds the minimal operator precedence 
	// @@ that the function is allowed to eat
	ExprAST * ParseBinOpRHS(int Expc, ExprAST * lhs);
	ExprAST * ParseNumberExpr();
	ExprAST * ParsePrimary();
	ExprAST * ParseParentExpr();
	ExprAST * ParseExpression();
	ExprAST * Error(const std::string & msg);
	int GetPrecedence();
private:
	Scanner scanner_;
	Token cur_token_;
	BinaryOpPrecedenceMap binary_op_precedence_;
};

FAO::FAO(const std::string & filename)
	:scanner_(filename)
{
	binary_op_precedence_['+'] = 20;
	binary_op_precedence_['-'] = 20;
	binary_op_precedence_['*'] = 40;
	binary_op_precedence_['/'] = 40;

	Advance();
}

void FAO::Advance()
{
	cur_token_ = scanner_.GetNextToken();
}

ExprAST * FAO::Error(const std::string & msg)
{
	std::cerr << msg << std::endl;
	return 0;
}

ExprAST * FAO::ParseNumberExpr()
{
	ExprAST * result = new IntegerExpr(cur_token_.GetValue());
	Advance();
	return result;
}

ExprAST * FAO::ParsePrimary()
{
	switch (cur_token_.GetType()){
	case TokenType::TOKEN_INTEGER:
		return ParseNumberExpr(); 
	case TokenType::TOKEN_LEFT_PARAM:
		return ParseParentExpr();
	default:
		return Error("unknow token when expecting an expression");
	}
}

ExprAST * FAO::ParseExpression()
{
	ExprAST * lhs = ParsePrimary();
	if (lhs == nullptr){
		return nullptr;
	}

	return ParseBinOpRHS(0, lhs);
}

ExprAST * FAO::ParseParentExpr()
{
	Advance(); // eat "("
	ExprAST * ret = ParseExpression();
	if (ret == nullptr)return 0;

	if (cur_token_.GetType() != TokenType::TOKEN_RIGHT_PARAM){
		return Error("expected ')'");
	}
	Advance(); // eat ")"
	return ret;
}

ExprAST * FAO::ParseBinOpRHS(int Expc, ExprAST * lhs)
{
	// If this is a binary operator,find its precedence
	while (true){
		int TokPrec = GetPrecedence();
		// If this is a binary operator,find its precedence at least as tightly as the current binary operator
		// consume it,otherwise we are done.
		if (TokPrec < Expc){
			return lhs;
		}

		// Okay,we know this is binary operator
		char BinOp = cur_token_.GetOperation();
		Advance(); // eat binary operator

		// Parse the primary expression after the binary operator
		ExprAST * rhs = ParsePrimary();
		if (rhs == nullptr){
			return nullptr;
		}

		// If binary operator less tightly with rhs than the operator after rhs,let
		// the pending operator than rhs as its lhs
		int NextPrec = GetPrecedence();
		if (TokPrec < NextPrec){
			rhs = ParseBinOpRHS(TokPrec + 1, rhs);
			if (rhs == nullptr){
				return nullptr;
			}
		}
		
		// Merge lhs/rhs
		lhs = new BinaryOpExpr(BinOp, lhs, rhs);
	}
	return lhs;
}

int FAO::GetPrecedence()
{
	if (cur_token_.GetType() == TokenType::TOKEN_OPERATION){
		return this->binary_op_precedence_[cur_token_.GetOperation()];
	}
	return -1;
}

int FAO::Compute()
{
	ExprAST * tree = ParseExpression();
	return tree->GetValue();
}

int main()
{
	FAO fao("FAO.txt");
	
	try{
		std::cout << fao.Compute() << std::endl;
	}
	catch (std::exception & e){
		std::cout << e.what() << std::endl;
	}
	return 0;
}

void ScannerError(const std::string & msg)
{
	std::cerr << msg << std::endl;
	exit(-1);
}