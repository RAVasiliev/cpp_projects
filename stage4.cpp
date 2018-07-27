#include "robot_mod.hpp"
#include <stdio.h>
#include <stdlib.h>


// You can define TABLES to see all tables in the end 
// You can define LEKS to see result of lexems analyze 
// You can define RPNsPRINT to see result of RPNs creating

#define START_ERR "ERROR: Every program and subroutine must start by {.\n"
#define LABEL_ERR "ERROR: After every label must be ':'\n"
#define ASSIGEND_ERR "ERROR: After every assigment must be ';'\n"
#define ASSIGWAIT_ERR "ERROR: Waited for assigment, but smth come wrong...\n"
#define ASSIG_ERR "ERROR: if you want to assign smth, please, write '='\n"
#define SQBR_ERR "ERROR: You must close '[' before doing smth\n"
#define BR_ERR "ERROR: You must close '(' before doing smth\n"
#define GOTO_ERR "ERROR: You must write label (started by '@') after 'goto'\n"
#define GOTOEND_ERR "ERROR: You must write ';' after 'goto <label>'\n"
#define IF_ERR "ERROR: You must write 'then' after 'if <Expr>'\n"
#define ELSE_ERR "ERROR: Every 'if' must have 'else'.\n"
#define FUNC_ERR "ERROR: You must write ';' after function 'endturn'\n"
#define FUNC_ERR1 "ERROR: You must write ';' after function <op>\n"
#define FUNC_ERR2 "ERROR: You must write ';' after 'function <op1> <op2>'\n"
#define OP_ERR "ERROR: You must write ',' between operands\n"
#define NOTH_ERR "ERROR: Expected assigment/'goto'/'if'/'print'/function...\n"
#define PRINT_ERR "ERROR: Write ';' after print arg.\n"
#define CRBR_ERR "ERROR: you need to close '(' after doing smth new\n"
#define EOF_ERR "ERROR: Unexpected EOF. You didn't close some '{'\n"
#define INT_ERR "ERROR: Must be int\n"
#define VAR_ERR "ERROR: Sorry, you didn't define some variable...\n"
#define LAB_ERR "ERROR: Sorry, you didn't define some labels...\n"
#define LAB_OP_ERR "ERROR: Jump on undefined label in goto\n"
#define PLUS_OP_ERR "ERROR: Undefined symb in plus operation\n"
#define MIN_OP_ERR "ERROR: Undefined symb in minus operation\n"
#define MULT_OP_ERR "ERROR: Undefined symb in multiply operation\n"
#define DEL_OP_ERR "ERROR: Undefined symb in division operation\n"
#define DEL_OP_ERR "ERROR: Undefined symb in division operation\n"
#define DEL_ON_0_OP_ERR "ERROR: Division on 0!\n"
#define AND_OP_ERR "ERROR: Undefined symb in AND operation\n"
#define OR_OP_ERR "ERROR: Undefined symb in OR operation\n"
#define NOT_OP_ERR "ERROR: Undefined symb in NOT operation\n"
#define SRAV_OP_ERR "ERROR: Undefined symb in comparison operation\n"
#define CREATE_MAS_ERR "ERROR: Undefined symb in MAS CREATING operation\n"
#define MAS_ERR "ERROR: Undefined symb in MAS DEFINING operation\n"
#define PRINTS_OP_ERR "ERROR: Undefined symb in PRINTS(PRINTString) operation\n"
#define PRINT_OP_ERR "ERROR: Undefined symb in PRINT(of integer) operation\n"
#define UNDEF_VAR "ERROR: Undefined variable in operation\n"
#define FUNSTART_ERR "ERROR: Smth wrong with work with functions\n"
#define FUNC_ARG_ERR "ERROR: Smth wrong in arguments of function\n"
#define FUNC_UNKNOWN_ERR "ERROR: Unknown function "

enum type_lex {lab, var, func, key, num,  str, sep, errl, End};

struct err {
	int err_str;
	char *err_name;
};

struct lexems {
	type_lex type;
	char *name;
	int str_num;
	lexems *next;
};

struct var_int {
	int n;
	char *name;
	var_int *next;
};

struct RPNItem;
	
struct var_lab {
	RPNItem *n;
	char *name;
	var_lab *next;
};

struct tables {
	var_int *table_int;
	var_lab  *table_label;
};


class RPNElem;

struct RPNItem {
	RPNElem *p;
	RPNItem *next;
};


char *int_to_str(int n);
//char *add_str(const char *str1, const char *str2);
//int str_cmp(const char *str1, const char *str2);

class Work_With_Tables
{
	var_int *table_int;
	var_lab *table_lab;
public:
	Work_With_Tables() {table_int = NULL; table_lab = NULL;}
	void int_add(char *var, int n);
	void int_all();
	void lab_all();
	int *int_get(char *var);
	RPNItem *lab_get(char *var);
	void add_labels(tables *tab);
	void mas_create(char *str, int num); 
};

void Work_With_Tables::mas_create(char *str, int num)
{
	int i;
	for (i = 0; i<=num; i++)
		int_add((add_str(add_str(str, "."), int_to_str(i))), 0);
}

void Work_With_Tables::add_labels(tables *tab)
{
	if (tab != NULL) 
	{
		table_lab = tab -> table_label; 
		#ifdef TABLES
			int_all();
			lab_all();
		#endif
	} 
}

void Work_With_Tables::int_all()
{
	var_int *now = table_int;
	printf("----INTS TABLE--:\n");
	while (now != NULL)
	{
		printf("NAME: %s, value: %d\n", now->name, now->n);
		now = now-> next;
	}
	printf("---END----\n");
}	

void Work_With_Tables::lab_all()
{
	var_lab *now = table_lab;
	printf("----LABS TABLE--:\n");
	while (now != NULL)
	{
		printf("NAME: %s\n", now->name);
		now = now-> next;
	}
	printf("---END----\n");
}	

void Work_With_Tables::int_add(char *var, int n)
{	
		
	if (table_int == NULL)
	{
		var_int *new_var = new var_int;
		new_var -> n = n;
		new_var -> name = var;
		new_var -> next = table_int;
		table_int = new_var;
	}
	else 
	{	
		var_int *now = table_int;
		while (now != NULL)
		{
			if (str_cmp(now -> name, var))
			{
				
				now -> n = n;
				return;
			}
			now = now -> next;
		}
		var_int *new_var = new var_int;
		new_var -> n = n;
		new_var -> name = var;
		new_var -> next = table_int;
		table_int = new_var;
	}
}

int *Work_With_Tables::int_get(char *var)
{
	var_int *now = table_int;
	if (now == NULL)
		return NULL;
	while (now != NULL)
	{
		if (str_cmp(now -> name, var))
		{
			return &(now -> n);
		}
		now = now -> next;
	}
	return NULL;
}

RPNItem *Work_With_Tables::lab_get(char *var)
{
	var_lab *now = table_lab;
	if (now == NULL)
	{	
		return NULL;
	}
	while (now != NULL)
	{
		if (str_cmp(now -> name, var))
		{
			return (now -> n);
		}
		now = now -> next;
	}
	return NULL;
}


class RPNElem {
public:
	virtual ~RPNElem() {};
	virtual void Set(RPNItem *a) {};
	virtual void PrintMe() = 0;
	virtual void Evaluate(RPNItem **stack, RPNItem **cur_cmd,
			Work_With_Tables *tab, game_info *all_info) const = 0;
	
protected:
	static void Push(RPNItem **stack, RPNElem *elem);
	static RPNElem* Pop(RPNItem **stack);
};

class RPNConst: public RPNElem {
public:
	virtual RPNElem *Clone() const = 0;
	void Evaluate(RPNItem **stack, RPNItem **cur_cmd, 
			Work_With_Tables *tab, game_info *all_info) const;
};

class RPNFunc: public RPNElem {
public:
	void Evaluate(RPNItem **stack, RPNItem **cur_cmd, 
			Work_With_Tables *tab, game_info *all_info) const;
	virtual RPNElem* EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const = 0;
};

class RPNInt : public RPNConst {
	int value;
public:
	RPNInt(int a) { value = a;}
	virtual ~RPNInt() {}
	virtual RPNElem* Clone() const
		{return new RPNInt(value); }
	virtual void PrintMe() {printf("RPNInt: %d\n", value);} 
	int Get() const {return value;}
};

char *copy_char(char *str);

class RPNString : public RPNConst {
	char *value;
public:
	RPNString(char *a) { value = a;}
	virtual ~RPNString() {}
	virtual RPNElem* Clone() const
		{return new RPNString(copy_char(value)); }
	virtual void PrintMe() {printf("RPNString: %s\n", value);} 
	char *Get() const {return value;}
};

class RPNLabel: public RPNConst {
	RPNItem *value;
	char *name;
public:
	RPNLabel(char *a) { name = a;}
	virtual ~RPNLabel() {}
	virtual void PrintMe() {printf("RPNLabel(%s)\n", name);} 
	virtual RPNElem* Clone() const
		{return new RPNLabel(name); }
	RPNItem *Get() const {return value; }
	char *Get_name() const {return name; }
};

class RPNLabelAddr: public RPNConst {
	RPNItem *value;
public:
	RPNLabelAddr(RPNItem *q){ value = q;}
	virtual ~RPNLabelAddr() {}
	virtual void PrintMe() {printf("RPNLabelAddr\n");} 
	void Set(RPNItem *k) {value = k; /*printf ("LABEL SETED %d\n", value);*/}
	virtual RPNElem* Clone() const
		{return new RPNLabelAddr(value); }
	RPNItem *Get() const {return value; }
};

bool func_0_param(char *str)
{
	return ((str_cmp(str, "?my_id"))||(str_cmp(str, "?turn"))||
		(str_cmp(str, "?players"))||
		(str_cmp(str, "?supply"))||(str_cmp(str, "?raw_price"))||
		(str_cmp(str, "?demand"))||(str_cmp(str, "?production_price")));
}

bool func_1_param(char *str)
{
	return ((str_cmp(str, "?money"))||
		(str_cmp(str, "?raw"))||
		(str_cmp(str, "?production"))||
		/*(str_cmp(str, "?factories"))||*/
		/*(str_cmp(str, "?manufactured"))||*/
		(str_cmp(str, "?result_raw_bought"))||
		(str_cmp(str, "?result_raw_price"))||
		(str_cmp(str, "?result_prod_sell"))||
		(str_cmp(str, "?result_prod_price")));
}

int start_fun_0_args(char *name, game_info *all_info)
{
	if (str_cmp(name, "?my_id"))
		return (all_info -> info_players -> get_my_id());
	if (str_cmp(name, "?players"))
		return (all_info -> info_players -> get_players_count());
	if (str_cmp(name, "?turn"))
		return (all_info -> mark -> get_month());
	if (str_cmp(name, "?supply"))
		return (all_info -> mark -> get_raw());
	if (str_cmp(name, "?raw_price"))
		return (all_info -> mark -> get_minprice());
	if (str_cmp(name, "?demand"))
		return (all_info -> mark -> get_prod());
	if (str_cmp(name, "?production_price"))
		return (all_info -> mark -> get_maxprice());
	return 1;
}

int start_fun_1_args(char *name, int arg, game_info *all_info)
{
	if (str_cmp(name, "?money"))
		return (all_info -> info_players -> get_money(
				all_info->info_players->num_to_player(arg)));
	if (str_cmp(name, "?raw"))
		return (all_info -> info_players -> get_raw(
				all_info->info_players->num_to_player(arg)));
	if (str_cmp(name, "?factories"))
		return (all_info -> info_players -> get_plants(
				all_info->info_players->num_to_player(arg)));
	if (str_cmp(name, "?result_raw_bought"))
		return (all_info -> info_op -> get_raw_bought(
				all_info->info_players->num_to_player(arg)));
	if (str_cmp(name, "?result_raw_price"))
		return (all_info -> info_op -> get_raw_price(
				all_info->info_players->num_to_player(arg)));
	if (str_cmp(name, "?result_prod_sell"))
		return (all_info -> info_op -> get_prod_sell(
				all_info->info_players->num_to_player(arg)));
	if (str_cmp(name, "?result_prod_price"))
		return (all_info -> info_op -> get_prod_price(
				all_info->info_players->num_to_player(arg)));
	return 0;
}

class RPNFuncStart: public RPNFunc {
public:
	RPNFuncStart() {}
	virtual ~RPNFuncStart() {}
	virtual void PrintMe() {printf("RPNFuncStart\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		if(!i1) 
			throw FUNSTART_ERR;
		if (func_1_param(i1->Get()))
		{
			RPNElem *operand2 = Pop(stack);
			RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
			if (!i2)
				throw FUNC_ARG_ERR;
			int res = start_fun_1_args(i1->Get(), i2->Get(),
								 all_info);
			return new RPNInt(res);
		}
		else if (func_0_param(i1->Get()))
		{		
			int res = start_fun_0_args(i1->Get(), all_info);
			return new RPNInt(res);
		}
		else
		{
			char *err;
			err = add_str(add_str(FUNC_UNKNOWN_ERR, i1->Get()),
									"\n");
			throw err;
		}
	}
};


class RPNFunPlus: public RPNFunc {
public:
	RPNFunPlus() {}
	virtual ~RPNFunPlus() {}
	virtual void PrintMe() {printf("RPNFunPlus\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw PLUS_OP_ERR;	
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		if (!i2)
			throw PLUS_OP_ERR;	
		int res = i1->Get() + i2->Get();
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

	
class RPNFunAnd: public RPNFunc {
public:
	RPNFunAnd() {}
	virtual ~RPNFunAnd() {}
	virtual void PrintMe() {printf("RPNFunAnd\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw AND_OP_ERR;	
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		if (!i2) 
			throw AND_OP_ERR;	
		int res = (i1->Get())&&(i2->Get());
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

class RPNFunOr: public RPNFunc {
public:
	RPNFunOr() {}
	virtual ~RPNFunOr() {}
	virtual void PrintMe() {printf("RPNFunOr\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw OR_OP_ERR;	
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		if (!i2) 
			throw OR_OP_ERR;	
		int res = (i1->Get())||(i2->Get());
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

class RPNFunSrav: public RPNFunc {
	char *name;
public:
	RPNFunSrav(char *a) {name = a;}
	virtual ~RPNFunSrav() {}
	virtual void PrintMe() {printf("RPNFunSrav\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		int res;
		RPNElem *operand1 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand1);
		if(!i2) 
			throw SRAV_OP_ERR;	
		RPNElem *operand2 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand2);
		if (!i1) 
			throw SRAV_OP_ERR;	
		if (str_cmp(name, ">"))
			res = (i1->Get())>(i2->Get());
		if (str_cmp(name, ">="))
			res = (i1->Get())>(i2->Get());
		if (str_cmp(name, "=="))
			res = (i1->Get())==(i2->Get());
		if (str_cmp(name, "<"))
			res = (i1->Get())<(i2->Get());
		if (str_cmp(name, "<="))
			res = (i1->Get())<=(i2->Get());
		if (str_cmp(name, "!="))
			res = (i1->Get())!=(i2->Get());
		
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

class RPNFunNot: public RPNFunc {
public:
	RPNFunNot() {}
	virtual ~RPNFunNot() {}
	virtual void PrintMe() {printf("RPNFunNot\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1)
			throw NOT_OP_ERR;	
		
		int res = !(i1->Get());
		delete operand1;
		return new RPNInt(res);
	}
};

class RPNFunOtr: public RPNFunc {
public:
	RPNFunOtr() {}
	virtual ~RPNFunOtr() {}
	virtual void PrintMe() {printf("RPNFunOtr\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw NOT_OP_ERR;	
		int res = 0 - (i1->Get());
		delete operand1;
		return new RPNInt(res);
	}
};

class RPNFunMinus: public RPNFunc {
public:
	RPNFunMinus() {}
	virtual ~RPNFunMinus() {}
	virtual void PrintMe() {printf("RPNFunMinus\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw MIN_OP_ERR; 
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		if(!i2) 
			throw MIN_OP_ERR; 
		int res = i2->Get() - i1->Get();
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

class RPNFunMult: public RPNFunc {
public:
	RPNFunMult() {}
	virtual ~RPNFunMult() {}
	virtual void PrintMe() {printf("RPNFunMult\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw MULT_OP_ERR;
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		if (!i2)
			throw MULT_OP_ERR;
		int res = (i1->Get()) * (i2->Get());
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

class RPNFunDel: public RPNFunc {
public:
	RPNFunDel() {}
	virtual ~RPNFunDel() {}
	virtual void PrintMe() {printf("RPNFunDel\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw DEL_OP_ERR;
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		if (!i2) 
			throw DEL_OP_ERR;
		if ((i1 -> Get()) == 0)
			throw DEL_ON_0_OP_ERR;
		int res = int((i2->Get())/(i1->Get()));
		delete operand1;
		delete operand2;
		return new RPNInt(res);
	}
};

class RPNFunGetVar: public RPNFunc {
public:
	RPNFunGetVar() {}
	virtual ~RPNFunGetVar() {}
	virtual void PrintMe() {printf("RPNFunGetVar\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		int *res1;
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		res1 = (tab -> int_get(i1->Get()));
		if (res1 == NULL)
			throw UNDEF_VAR;
		int res = *res1;;
		delete operand1;
		return new RPNInt(res);
	}
};

class RPNFunAssig: public RPNFunc {
public:
	RPNFunAssig() {}
	virtual ~RPNFunAssig() {}
	virtual void PrintMe() {printf("RPNFunAssig\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw ASSIG_ERR;;
		RPNElem *operand2 = Pop(stack);
		RPNString *i2 = dynamic_cast<RPNString*>(operand2);
		tab -> int_add(i2->Get(), i1->Get());
		if (!i2) 
			throw ASSIG_ERR;
		delete operand1;
		delete operand2;
		return NULL;
	}
};

class RPNFunPrint: public RPNFunc {
public:
	RPNFunPrint() {}
	virtual ~RPNFunPrint() {}
	virtual void PrintMe() {printf("RPNFunPrint\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw PRINT_OP_ERR;
		printf("%d\n", i1->Get());
		delete operand1;
		return NULL;
	}
};

class RPNFunPrints: public RPNFunc {
public:
	RPNFunPrints() {}
	virtual ~RPNFunPrints() {}
	virtual void PrintMe() {printf("RPNFunPrints\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		if(!i1) 
			throw PRINTS_OP_ERR;
		printf("%s\n", i1->Get());
		delete operand1;
		return NULL;
	}
};


class RPNFunMas: public RPNFunc {
public:
	RPNFunMas() {}
	virtual ~RPNFunMas() {}
	virtual void PrintMe() {printf("RPNFunMas\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw MAS_ERR;
		RPNElem *operand2 = Pop(stack);
		RPNString *i2 = dynamic_cast<RPNString*>(operand2);
		if (!i2) 
			throw MAS_ERR;
		delete operand1;
		delete operand2;
		return new RPNString(add_str(add_str(i2->Get(), "."), 
						int_to_str(i1->Get())));
	}
};

class RPNFunCreateMas: public RPNFunc {
public:
	RPNFunCreateMas() {}
	virtual ~RPNFunCreateMas() {}
	virtual void PrintMe() {printf("RPNFunCreateMas\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNInt *i1 = dynamic_cast<RPNInt*>(operand1);
		if(!i1) 
			throw CREATE_MAS_ERR;
		RPNElem *operand2 = Pop(stack);
		RPNString *i2 = dynamic_cast<RPNString*>(operand2);
		if(!i2) 
			throw CREATE_MAS_ERR;
		tab -> mas_create(i2->Get(), i1->Get());
		delete operand1;
		delete operand2;
		return NULL;
	}
};
class RPNGotoForIf: public RPNElem {
public:
	RPNGotoForIf() {}
	virtual ~RPNGotoForIf() {}
	virtual void PrintMe() {printf("RPNGotoForIf\n");}
	void Evaluate(RPNItem **stack, RPNItem **cur_cmd,
			Work_With_Tables *tab, game_info *all_info) const 
	{
		RPNElem *operand1 = Pop(stack);
		RPNLabelAddr *i1 = dynamic_cast<RPNLabelAddr*>(operand1);
		(*cur_cmd) = (i1 -> Get());
	}
};

class RPNGoto: public RPNElem {
public:
	RPNGoto() {}
	virtual ~RPNGoto() {}
	virtual void PrintMe() {printf("RPNGoto\n");}
	void Evaluate(RPNItem **stack, RPNItem **cur_cmd,
			Work_With_Tables *tab, game_info *all_info) const 
	{
		RPNElem *operand1 = Pop(stack);
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		if(!i1) 
			throw LAB_OP_ERR;
		RPNItem *res1 = tab -> lab_get (i1 -> Get());
		
		if(!res1) 
			throw LAB_OP_ERR;
		RPNItem *res = res1; 
		(*cur_cmd) = res;
	}
};

class RPNFunIf: public RPNElem {
public:
	RPNFunIf() {}
	virtual ~RPNFunIf() {}
	virtual void PrintMe() {printf("RPNFunIf\n");}
	void Evaluate(RPNItem **stack, RPNItem **cur_cmd,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNLabelAddr *i1 = dynamic_cast<RPNLabelAddr*>(operand1);
		RPNItem *res_else =  (i1 -> Get());
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		int cond =  (i2 -> Get());
		if (cond == 0)
		{
			(*cur_cmd) = res_else;
		}
		else
			*cur_cmd = (*cur_cmd)->next;
	}

};


class RPNFun0arg: public RPNFunc {
public:
	RPNFun0arg() {}
	virtual ~RPNFun0arg() {}
	virtual void PrintMe() {printf("RPNFun0arg\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		i1->Get();
		/*work with serv*/
		delete operand1;
		return NULL;
	}
};

class RPNFun1arg: public RPNFunc {
public:
	RPNFun1arg() {}
	virtual ~RPNFun1arg() {}
	virtual void PrintMe() {printf("RPNFun1arg\n");} 
	RPNElem *EvaluateFun(RPNItem **stack, 
			Work_With_Tables *tab, game_info *all_info) const
	{
		
		RPNElem *operand1 = Pop(stack);
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		i2->Get();
		i1->Get();
		/*work with serv*/
		delete operand1;
		delete operand2;
		return NULL;
	}
};

class RPNFun2arg: public RPNFunc {
public:
	RPNFun2arg() {}
	virtual ~RPNFun2arg() {}
	virtual void PrintMe() {printf("RPNFun2arg\n");} 
	RPNElem *EvaluateFun(RPNItem **stack,
			Work_With_Tables *tab, game_info *all_info) const
	{
		RPNElem *operand1 = Pop(stack);
		RPNString *i1 = dynamic_cast<RPNString*>(operand1);
		i1->Get();
		RPNElem *operand2 = Pop(stack);
		RPNInt *i2 = dynamic_cast<RPNInt*>(operand2);
		i2->Get();
		RPNElem *operand3 = Pop(stack);
		RPNInt *i3 = dynamic_cast<RPNInt*>(operand3);
		i3->Get();
		/*work with serv*/
		delete operand1;
		delete operand2;
		delete operand3;
		return NULL;
	}
};

void RPNFunc::Evaluate(RPNItem **stack, RPNItem **cur_cmd,
			Work_With_Tables *tab, game_info *all_info) const
{
	RPNElem *res = EvaluateFun(stack, tab, all_info);
	if (res) Push(stack, res);
	*cur_cmd = (*cur_cmd)->next;
}


void RPNConst::Evaluate(RPNItem **stack, RPNItem **cur_cmd,
			Work_With_Tables *tab, game_info *all_info) const
{
	Push(stack, Clone());
	*cur_cmd = (*cur_cmd) -> next;
}

void RPNElem::Push(RPNItem **stack, RPNElem *elem)
{
	RPNItem *n = new RPNItem;
	n -> p = elem;
	n -> next = (*stack);
	(*stack) = n;
}

RPNElem *RPNElem::Pop(RPNItem **stack)
{
	if (*stack) 
	{
		RPNItem *n = (*stack);
		(*stack) = (*stack) -> next;
		return n->p;
	}
	else 
		throw "GAVNO";
}



class MakeLexems {
	enum status  {H, N, I, K, A, S, Err};
	const char *err_name;
	char buf;
	char buf_err;
	int str_num;
	int len_err;
	enum status stat;
	void BufAnalyze();
	void Symb_Correct(char c);
	void AppendSymb(char c);
	void AddSymb(char c);
	void Home_Analyze(char c);
	void Err_Create(char c, const char *str);
	int Err_Analyze(lexems *first);
	void AddingSymb(bool fl, char c);
	void Add_End();
	lexems *lex_now;
	lexems *lex_first;
public:
	MakeLexems();
	void New_Symb(char c);
	lexems *Give_All();
};

class SyntaxAnalyze {
	RPNItem *RPNs;
	RPNItem *RPNsFirst;
	lexems *first_elem;
	lexems *now_elem;
	lexems *working_elem;
	tables *table_big;
	var_int *int_cur;
	var_lab *lab_cur;
	void L();
	void O();
	void Pr();
	void A();
	void M();
	void C();
	void Func();
	void Func_with0arg();
	void Func_with1arg();
	void Func_with2arg();
	void Par0();
	void Par1();
	void Par2();
	void Print();
	bool P_FIRST();
	void E();
	void L1();
	void N1();
	void C_T();
	void N2();
	void M_T();
	void N3();
	void T();
	void N4();
	void U_T();	
	void gc();
	void S();
	err *make_err(const char *s, const char *exp, const char *com);
	void add_label(char *name, RPNItem* addr);
public:
	void start_RPN(game_info *all_info);
	void Set(lexems *s) { first_elem = s; now_elem = s; RPNs = new RPNItem;
						RPNsFirst = RPNs; RPNs->next = NULL; table_big = NULL;}
	int analyze();
	RPNItem* put_rpn(RPNElem *put);
	RPNItem* put_rpn(RPNElem *put, RPNItem *ptr);
	RPNItem *get_RPN() {return RPNsFirst;}
};

/*int str_len(char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}*/

char *copy_char(char *str1)
{
	int i = 0;
	char *str2 = new char[str_len(str1)];
	while (str1[i] != '\0') 
	{
		str2[i] = str1[i];
		i++;
	}
	str2[i] = str1[i];
//	printf("Copy from [%s] to [%s]\n", str1, str2);
	return str2;
}

void SyntaxAnalyze::start_RPN(game_info *all_info)
{
	Work_With_Tables *tab = new Work_With_Tables;
	RPNItem *now;
	RPNItem **stack = new (RPNItem *);
	(*stack) = NULL;
	now = RPNsFirst;
	tab -> add_labels(table_big);
	#ifdef RPNsPRINT
		printf("----RPNs:-----\n");
		while (now -> next != NULL)
		{
			now -> p -> PrintMe();
			now = now -> next;
		}
		printf("--------------\n");
		now = RPNsFirst;
	#endif
	try {
		while (now -> next != NULL)
		{
			now -> p -> Evaluate(stack, &now, tab, all_info);
		}
	} 
	catch (const char *str)
	{
		printf("%s", str);
		exit(1);
	}
	#ifdef TABLES
		tab -> int_all();
	#endif 
}

RPNItem *SyntaxAnalyze::put_rpn(RPNElem *put)
{
	RPNItem *my = RPNs;
	RPNs -> p = put;
	RPNs -> next = new RPNItem;
	RPNs = RPNs -> next;
	RPNs -> next = NULL;
	return my;
}


RPNItem *SyntaxAnalyze::put_rpn(RPNElem *put, RPNItem *ptr)
{
	RPNItem *n = new RPNItem;
	n -> p = put;
	n -> next = NULL;
	ptr = n;
	return n;
}

/*int str_cmp(const char *str1, const char *str2)
{
	int i = 0;
	while ((str1[i] != '\0')&&(str2[i] != '\0')&&
			(str1[i] == str2[i]))
		i++;
	return (str1[i] == str2[i]);
}*/
	
/*int str_len(const char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}*/

const char *enum_to_str(type_lex n)
{
	switch (n) {
		case lab:
			return "label";
		case var:
			return "variable";
		case func:
			return "function";
		case key: 
			return "keyword";
		case num:
			return "num";
		case str:
			return "str";
		case sep:
			return "separator";
		case End:
			return "End";	 
		default:
			return "err";	
	}
}

/*void reverse (char *str)
{
	int i, j;
	char c;
	for (i = 0, j=str_len(str) - 1; i<j;i++,j--)
	{
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}
}*/

char *int_to_str(int n)
{
	int i, sign;
	char *str;
	str = new char[16*sizeof(char)];

	if ((sign = n) < 0)
		n = -n;
	i=0;
	do {
		str[i++] = n%10 + '0';
	} while ((n/=10)>0);
	if (sign<0)
		str[i++] = '-';
	str[i] = '\0';
	reverse(str);
	return str;
}

/*char *add_str(const char *str1, const char *str2)
{
	int len_res, i, len1 = 0;
	char *str_res;
	len_res = str_len(str1)+str_len(str2) + 1;
	str_res = new char [len_res*sizeof(char)];
	i = 0;
	while (str1[i] != '\0')
	{
		str_res[i] = str1[i];
		i++;
	}
	len1 = i;
	while (str2[i - len1] != '\0')
	{
		str_res[i] = str2[i - len1];
		i++;
	}
	str_res[i] = '\0';
	return str_res;
}*/

err *SyntaxAnalyze::make_err(const char *start_comment, const char *exp, 
		 const char *com)
{
	err* error = new err;
	char *str_res;
	error -> err_str = working_elem -> str_num;
	#ifdef DEBUG 
		printf("WORKING ELEM NAME IS %s\n", working_elem->name);
	#endif
	str_res = add_str(
			start_comment, 
			add_str(add_str(add_str(add_str("Expected: '",exp),
			"', but have: "), add_str(add_str("'", 
			working_elem->name), "'")), 
			com)
	);
	error->err_name = str_res;
	return error;
}

void SyntaxAnalyze::S()
{
	if (!(str_cmp((working_elem -> name ), "{")))
		throw make_err(START_ERR, "{", 
				" in the start of program or subrotine");
	#ifdef DEBUGi
		printf("S --> '{'L\n");
	#endif

	gc();
	L();
}

void SyntaxAnalyze::add_label(char *name, RPNItem *elem)
{
	var_lab *new_lab = new var_lab;
	new_lab -> n = elem;
	new_lab -> name = name;
	new_lab -> next = NULL;	
	if (table_big == NULL)
	{
		table_big = new tables;
		table_big -> table_int = NULL;
		table_big->table_label = new_lab;
		lab_cur = table_big->table_label;
	}
	else
		lab_cur -> next = new_lab;
}
	
void SyntaxAnalyze::L()
{
	if (working_elem -> type == End)
		throw make_err(EOF_ERR, "} or smth","");
	else if ((working_elem -> type) == lab)
	{
		#ifdef DUBUG
			printf("L --> label ':' OL\n");
		#endif

		add_label(working_elem->name, RPNs); 

		gc();
		if (str_cmp((working_elem->name), ":"))
			gc();
		else 
			throw make_err(LABEL_ERR, ":", " after label");
		O();
		L();
	}
	else if (str_cmp((working_elem -> name), "}"))
	{
		gc();
		#ifdef DUBUG
			printf("L --> '}'\n");
		#endif
	}
	else
	{
		#ifdef DEBUG
			printf("L --> OL\n");
		#endif
		O();
		L();
	}
}

void SyntaxAnalyze::O()
{

	if ((working_elem -> type) ==  var)
	{
		#ifdef DUBUG
			printf("O --> variable A\n");
		#endif
		put_rpn(new RPNString(working_elem->name));
		gc();
		A();
	}
	else if (str_cmp((working_elem -> name), "goto"))
	{
		#ifdef DEBUG
			printf("O --> goto M\n");
		#endif
		gc();
		M();
		put_rpn(new RPNGoto);
	}
	else if (str_cmp((working_elem -> name), "if"))
	{
		#ifdef DEBUG
			printf("O --> if C\n");
		#endif
		gc();
		C();
	}
	else if ((str_cmp((working_elem -> name), "print")) ||
		(str_cmp((working_elem -> name), "prints")))
	{
		#ifdef DEBUG
			printf("O --> print P_FIRST P_NEXT\n");
		#endif
		gc();
		if (P_FIRST())
			put_rpn(new RPNFunPrints);
		else 
			put_rpn(new RPNFunPrint);
		if (str_cmp((working_elem->name), ";"))
			gc();
		else 
			throw make_err(PRINT_ERR, ";", "print arg");
		
	}
	else 
	{
		#ifdef DEBUG
			printf("O --> Funk\n");
		#endif
		Func();
	}
}

void SyntaxAnalyze::A()
{
	if (str_cmp((working_elem -> name), "="))
	{
		#ifdef DEBUG
			printf("A --> = E ;\n");
		#endif
		gc();
//		printf("NOW ON %s\n", working_elem -> name);
		E();
		if (str_cmp((working_elem -> name), ";"))
			gc();
		else
			throw make_err(ASSIGEND_ERR, ";", " after assigment");
		put_rpn(new RPNFunAssig());
	}
	else if (str_cmp(working_elem -> name, "["))
	{
		#ifdef DEBUG
			printf("A --> [ E ]");
		#endif
		gc();
//		printf("NOW ON %s\n", working_elem -> name);
		E();
		if (str_cmp((working_elem -> name), "]"))
			gc();
		else
			throw make_err(SQBR_ERR, "]", " in assigment");


		if (str_cmp((working_elem -> name), "="))
		{
			put_rpn(new RPNFunMas);
			gc();
			E();
			put_rpn(new RPNFunAssig());
		}

		else if (str_cmp((working_elem -> name), "create"))
		{
			gc();
			put_rpn(new RPNFunCreateMas());
		}
		else
			throw make_err(ASSIG_ERR, "= or 'create'", " in assigment");


		if (str_cmp((working_elem -> name), ";"))
			gc();
		else
			throw make_err(ASSIGEND_ERR, ";",
				       	" in the end of assigment");
	}
	else 
		throw make_err(ASSIGWAIT_ERR, "= or [Expr]", "");
	
}

void SyntaxAnalyze::M()
{
	#ifdef DEBUG
		printf("M --> label ;\n");
	#endif
	if ((working_elem -> type) == lab)
	{
		put_rpn(new RPNString(working_elem -> name));
		gc();
	}
	else
		throw make_err(GOTO_ERR, "label", "");
	if (str_cmp((working_elem->name), ";"))
		gc();
	else
		throw make_err(GOTOEND_ERR, ";", " after label");
}

void SyntaxAnalyze::C()
{
	RPNLabelAddr *f, *f1;
	RPNItem *p, *p1;
	#ifdef DEBUG	
		printf("C --> E then S \n");
	#endif
	E();
	if (str_cmp((working_elem -> name), "then"))
		gc();
	else
		throw make_err(IF_ERR, "then"," after if");
	f = new RPNLabelAddr(NULL);
	p = put_rpn(f);
	put_rpn(new RPNFunIf);
	S();
	f1 = new RPNLabelAddr(NULL);
	p1 = put_rpn(f1);
	put_rpn(new RPNGotoForIf);
	if (str_cmp((working_elem -> name), "else"))
	{
		f -> Set(RPNs);
		p -> p -> Set(RPNs); 
		gc();
		S();
	}
	else 
		throw make_err(ELSE_ERR, "else"," after }");
	f1 -> Set(RPNs);
	p1 -> p -> Set(RPNs); 
	//f->Show();
	//f1->Show();
	put_rpn(f, p);
	put_rpn(f1, p1);
}

void SyntaxAnalyze::Func_with0arg()
{
	char *name = working_elem -> name;
	gc();
	if (str_cmp(working_elem -> name, ";"))
		gc();
	else 
		throw make_err(FUNC_ERR, ";", " after endturn");
	put_rpn(new RPNString(name));
	put_rpn(new RPNFun0arg);
}

void SyntaxAnalyze::Func_with1arg()
{
	char *name = working_elem -> name;
	gc();
	E();
	if (str_cmp(working_elem -> name, ";"))
		gc();
	else 
		throw make_err(FUNC_ERR1, ";", "after operand");
	put_rpn(new RPNString(name));
	put_rpn(new RPNFun1arg);
	
}

void SyntaxAnalyze::Func_with2arg()
{
	char *fun;
	fun = working_elem -> name;
	gc();
	E();
	if (str_cmp(working_elem -> name, ","))
		gc();
	else 
		throw make_err(OP_ERR, ",", " between operands");
	E();
	if (str_cmp(working_elem -> name, ";"))
		gc();
	else
		throw make_err(FUNC_ERR2, ";"," after function");
	put_rpn(new RPNString(fun));
	put_rpn(new RPNFun2arg);
	
}

void SyntaxAnalyze::Func()
{
	if (str_cmp(working_elem -> name, "endturn"))
		Func_with0arg();
	else if ((str_cmp(working_elem -> name, "build"))||
			(str_cmp(working_elem -> name, "prod")))
		Func_with1arg();
	else if ((str_cmp(working_elem -> name, "buy"))||
			(str_cmp(working_elem -> name, "sell")))
		Func_with2arg();
	else 
		throw make_err(NOTH_ERR, "function/assig/if/goto", "");
}

bool SyntaxAnalyze::P_FIRST()
{
	if ((working_elem -> type == str))
	{
		#ifdef DEBUG
			printf ("P_FIRST -> string\n");
		#endif
		put_rpn(new RPNString(working_elem->name));
		gc();
		return true;
	}
	else
	{
		U_T();
		return false;
	/*	if ((working_elem -> type) == var)
		{
			
		#ifdef DEBUG
			printf ("P_FIRST -> E");
		#endif
		E();
		if (str_cmp(working_elem -> name, "["))
		{
			#ifdef DEBUG
				printf("[ E ]\n");
			#endif
			gc();
			E();
			put_rpn(new RPNFunMas);
			if (str_cmp((working_elem -> name), "]"))
				gc();
			else
				throw make_err(SQBR_ERR, "]", " in assigment");
		}
	*/
	}
}
	
/*void SyntaxAnalyze::P_NEXT()
{
	if (str_cmp(working_elem -> name, ","))
	{
		#ifdef DEBUG
			printf ("P_NEXT -> , P_FIRST, P_NEXT ;\n");
		#endif
		gc();
		P_FIRST();
		P_NEXT();
	}
	else if (str_cmp(working_elem -> name, ";"))
	{
		#ifdef DEBUG
			printf ("P_NEXT -> ;\n");
		#endif
		gc();
	}
	else 
		throw make_err(PRINT_ERR, "; or ,", " in print");
}
*/

void SyntaxAnalyze::E()
{
	#ifdef DEBUG
		printf ("E -> L1 N1\n");
	#endif
	L1();
	N1();
}


void SyntaxAnalyze::N1()
{
	if (str_cmp(working_elem -> name, "&"))
	{
		#ifdef DEBUG
			printf ("N1 -> &L1 N1\n");
		#endif
		gc();
		L1();
		N1();
		put_rpn(new RPNFunAnd);
	}
	else if (str_cmp(working_elem -> name, "|"))
	{
		#ifdef DEBUG
			printf ("N1 -> |L1 N1\n");
		#endif
		gc();
		L1();
		N1();
		put_rpn(new RPNFunOr);
	}
	else {
		#ifdef DEBUG
			printf ("N1 -> [0] \n");
		#endif
	}
}

void SyntaxAnalyze::L1()
{
	C_T();
	N2();
}

void SyntaxAnalyze::N2()
{
	if ((str_cmp(working_elem->name, ">"))||
		(str_cmp(working_elem->name, "<"))||
		(str_cmp(working_elem->name, ">="))||
		(str_cmp(working_elem->name, "<="))||
		(str_cmp(working_elem->name, "=="))||
		(str_cmp(working_elem->name, "!=")))
	{
		char *save = working_elem->name;
		gc();
		#ifdef DEBUG
			printf ("N2->sign C_T N2\n");
		#endif
		C_T();
		N2();

		put_rpn(new RPNFunSrav(save));
	}
	else { 
		#ifdef DEBUG
			printf ("N2-> [0] \n");
		#endif
	}
}

void SyntaxAnalyze::C_T()
{
	M_T();
	N3();
}

void SyntaxAnalyze::N3()
{
	if (str_cmp(working_elem -> name, "+"))
	{
		#ifdef DEBUG
			printf ("N3 -> + M_T N3\n");
		#endif
		gc();
		M_T();
		N3();
		put_rpn(new RPNFunPlus);
	}
	else if (str_cmp(working_elem -> name, "-"))
	{
		#ifdef DEBUG
			printf ("N3 -> - M_T N3\n");
		#endif
		gc();
		M_T();
		N3();
		put_rpn(new RPNFunMinus);
	}
	else {
		#ifdef DEBUG
			printf ("N3 -> [0] \n");
		#endif
	}
}

void SyntaxAnalyze::M_T()
{
	T();
	N4();
}

void SyntaxAnalyze::N4()
{
	if (str_cmp(working_elem -> name, "*"))
	{
		#ifdef DEBUG
			printf ("N4 -> * T N4\n");
		#endif
		gc();
		T();
		N4();
		put_rpn(new RPNFunMult);
	}
	else if (str_cmp(working_elem -> name, "/"))
	{
		#ifdef DEBUG
			printf ("N4 -> / T N4\n");
		#endif
		gc();
		T();
		N4();
		put_rpn(new RPNFunDel);
	}
	else {
		#ifdef DEBUG
			printf ("N4 -> [0] \n");
		#endif
	}
}

void SyntaxAnalyze::T()
{
	if (str_cmp(working_elem -> name, "!"))
	{
		gc();
		#ifdef DEBUG
			printf("T -> ! U_T\n");
		#endif
		U_T();
		put_rpn(new RPNFunNot);
	}
	else if (str_cmp(working_elem -> name, "-"))
	{
		gc();
		#ifdef DEBUG
			printf("T -> - U_T\n");
		#endif
		U_T();
		put_rpn(new RPNFunOtr);
		
	}
	else 
	{
		#ifdef DEBUG
			printf("T -> U_T\n");
		#endif
		U_T();
	}	
}

int string_to_int(char *str)
{
	int val = 0;
	int i = 0;
	for (i = 0; str[i]; ++i)
	{
		val *= 10;
		val += str[i] - '0';
	}
//	printf("From string '%s' to int '%d'\n", str, val);
	return val;
}



void SyntaxAnalyze::U_T()
{
	if ((working_elem -> type) == var)
	{
		#ifdef DEBUG
			printf("U_T -> <var>\n");
		#endif
		put_rpn(new RPNString(working_elem->name));
		gc();
		if (str_cmp(working_elem -> name, "["))
		{
			gc();
			E();
			if (str_cmp(working_elem -> name, "]"))
				gc();
			else
				throw make_err(SQBR_ERR, "]", "");
			put_rpn(new RPNFunMas); 
		}
		put_rpn(new RPNFunGetVar);
	}
	else if ((working_elem -> type) == func)
	{
		char *name = working_elem -> name;
		gc();
		#ifdef DEBUG
			printf("U_T -> <func>\n");
		#endif
		if (str_cmp(working_elem -> name, "("))
		{
			printf("SKOBKI\n");
			gc();
			E();
			if (str_cmp(working_elem -> name, ")"))
				gc();
			else
				throw make_err(BR_ERR, ")", "");
		}
		put_rpn(new RPNString(name));	
		put_rpn(new RPNFuncStart);	
	}
	else if ((working_elem -> type) == num)
	{
		#ifdef DEBUG
			printf("U_T -> <num> (%s)\n", working_elem->name);
		#endif
		put_rpn(new RPNInt(string_to_int(working_elem->name)));	
		gc();
	}
	else if(str_cmp(working_elem -> name, "("))
	{
		#ifdef DEBUG
			printf("U_T -> ( E )\n");
		#endif
		gc();
		E();
		if (str_cmp(working_elem -> name, ")"))
			gc();
		else 
		{
			throw make_err(CRBR_ERR, ")", "");
		}
	}
	else 
		throw make_err(NOTH_ERR, "function/num/variable", "");
}		
		 

void SyntaxAnalyze::gc()
{
	working_elem = now_elem;
	if ((now_elem != NULL))
		now_elem = now_elem->next;
	else 
		throw make_err(EOF_ERR, "} or smth","");
}

int SyntaxAnalyze::analyze()
{
	try 
	{
		gc();
		S();
		
		if (working_elem -> type != End)
			throw "!end!";
		printf("SUCCESS, NO ERRORS!\n\n");
		//start_RPN();
		return 0;
	}
	catch (err *error)
	{
		if (error -> err_str == 0)
			printf("%s in the last line\n", error->err_name);
		else
			printf("%s at line %d\n", error->err_name,
						error -> err_str);
		return 1;
	}
}
	
void MakeLexems::AddSymb(char c)
{
	char *str_new;
	lexems *lex_new;
	str_new = new char [2];
	str_new[0] = c;
	str_new[1] = '\0';
	lex_new = new lexems[1];
	lex_new->type = num;
	switch (stat) {
		case N:
			lex_new->type = num;
			break;
		case I:
			if (c == '@')
				lex_new->type = lab;
			if (c == '$')
				lex_new->type = var;
			if (c == '?')
				lex_new->type = func;
			break;
		case K:
			lex_new->type = key;
			break;
		case A:
			lex_new->type = sep;
			break;
		case S:
			lex_new->type = str;
			break;
		default:
			lex_new->type = sep;
			break;
	}
	lex_new->name = str_new;
	lex_new->str_num = str_num;
	lex_new->next = NULL;
	lex_now -> next = lex_new;
	lex_now = lex_now -> next;
}
		
void MakeLexems::AppendSymb(char c)
{
	char *str, *str_new;
	int len, i;
	str = lex_now -> name;
	len = str_len(str);
	str_new = new char [len + 2];
	for (i = 0; i <= len; i++)
		str_new[i] = str[i];
	str_new[len] = c;
	str_new[len+1] = '\0';
	lex_now -> name = str_new;
}

void MakeLexems::Err_Create(char c, const char *str)
{
	stat = Err;
	err_name = str;
	buf_err = c;
	len_err = str_num;
}

bool home_may(char c)
{
	return ((c == ' ')||(c =='\n')||(c == '\t')||((c >= '0')&&(c<='9'))||
	((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))||(c == '+')||
	(c == '-') || (c == '*')||(c == '/')||(c == '%')||
	(c == '<') || (c == '>')||(c == '=')||(c == ':')||
	(c == ';') || (c == ',')||(c == '@')||(c == '$')|| (c == '!')||
	(c == '?') || (c == '"')||(c == '_')||(c == '(')||(c == ')')||
	(c == '}') || (c == '{')||(c == '[')||(c == ']')||(c == '|')||
	(c == '&'));
}

bool ident_may(char c)
{
	return ((c == ' ')||(c =='\n')||(c == '\t')||((c >= '0')&&(c<='9'))||
	((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))||(c == '+')||
	(c == '-') || (c == '*')||(c == '/')||(c == '%')||
	(c == '<') || (c == '>')||(c == '=')||(c == ':')||
	(c == ';') || (c == ',')||(c == '@')||(c == '$')|| (c == '!')||
	(c == '?') || (c == '"')||(c == '_')||(c == '(')||(c == ')')||
	(c == '}') || (c == '{')||(c == '[')||(c == ']')||(c =='|') ||
	(c == '&'));
}
bool key_may(char c)
{
	return 	((c == ' ')||(c =='\n')||(c == '\t')||
	((c>='A')&&(c<='Z'))||
	((c>='a')&&(c<='z'))||(c == '+')||(c == '-') ||
	(c == '*')||(c == '/')||(c == '%')||(c == '<')||
	(c == '>')||(c == '=')||(c == ';') || (c == ',')||
	(c == '_')||(c == '}') || (c == '{')||(c == ':')
	||(c == ';')||(c == ']')||(c =='['));
}

void MakeLexems::Symb_Correct(char c)
{
	
	if (!home_may(c))
		Err_Create(c, "Undefined symb");

	switch (stat){
		case I:
			if (!ident_may(c))
				Err_Create(c,
					"Undefined symb at identificator");
			break;
		case N:
			break;	
		case K: 
			if (!((c ==')')||(c == ' ')||(c =='\n')||(c == '\t')||
			((c>='A')&&(c<='Z'))||
			((c>='a')&&(c<='z'))||(c == '+')||(c == '-') ||
			(c == '*')||(c == '/')||(c == '%')||(c == '<')||
			(c == '>')||(c == '=')||(c == ';') || (c == ',')||
			(c == '_')||(c == '}') || (c == '{')||(c == ':')
			||(c == ';')||(c == ']')||(c =='[')))
				Err_Create(c,"Undefined symb at keyword");
			break;
		default:
			break;
	}	
			
}
	
void MakeLexems::BufAnalyze()
{
	if (buf != '\0')
	{
		char buf_new = buf;
//		printf("Analyze with buf: %c, str_num is: %d\n",
//							 buf, str_num );
		buf = '\0';
		New_Symb(buf_new);
	}
}


void MakeLexems::Home_Analyze(char c)
{
		if (c == '\n')
			str_num++;
		//printf("c='%c'\n", c); 
		if ((c >= '0')&&(c <= '9'))
			{stat = N; AddSymb(c);}
		else if ((c == '?')||(c == '@')||(c == '$'))
			{stat = I; AddSymb(c);}
		else if (((c >= 'a')&&(c <= 'z'))
				||((c >= 'A')&&(c <= 'Z')))
			{stat = K; AddSymb(c);}
		else if ((c == ':') ||(c == '<')||(c == '>')||(c== '=')||
							(c == '!'))
			{stat = A; AddSymb(c);}
		else if (c == '"')
			{stat = S; AddSymb('\0');}
		else if ((c == '{') || (c == '}')||(c == '=')||
				(c == ';')||(c == ',')||(c =='[')||
				(c == ']')||(c == ')')||(c == '(')||
				(c == '+')||(c == '-')||(c == '*')||
				(c == '/')||(c == '|')||(c == '/'))
			{AddSymb(c); }
		else if ((c == ' ') || (c == '\n')||(c == '\t'))
			{}
}

void MakeLexems::AddingSymb(bool fl, char c)
{
	if (fl)
		AppendSymb(c);
	else {
		buf = c;
		stat = H;
	}
}


void MakeLexems::New_Symb(char c)
{
//	printf("get new symb %c in state %d\n", c, stat); 
	if (stat == Err)
		return;
	Symb_Correct(c);
	switch (stat) {
		case H:
			Home_Analyze(c);
			break;
		case N: 
			if ((c >= '0')&&(c <= '9'))
				AppendSymb(c);
			else {
				stat = H;
				buf = c;
			}
			break; 
		case I: 
			AddingSymb((((c>='a')&&(c<='z'))||
			((c>='A')&&(c<='Z'))||((c>='0')&&(c<='9'))
					||(c=='_')), c);
			break;
		case K: 
			AddingSymb((((c >='a')&&(c <='z'))
				||((c >='A')&&(c <='Z'))), c);
			break;
		case A:
			AddingSymb((c == '='), c);
			break;
		case S:
			if (c == '\n')
				str_num++;
	
			if (c != '"')
				AppendSymb(c);
			else {
				stat = H;
			}
			break;	
		case Err:
			break;
	}
	BufAnalyze();
}


char *add_char(const char *str1, char c)
{
	int len_res, i;
	char *str_res;
	len_res = str_len(str1)+3;
	str_res = new char [len_res*sizeof(char)];
	i = 0;
	while (str1[i] != '\0')
	{
		str_res[i] = str1[i];
		i++;
	}
	str_res[i] = ' ';
	str_res[i+1] = c;
	str_res[i+2] = '\0';
	return str_res;
}


int MakeLexems::Err_Analyze(lexems *first) //No Errors=>return 0.
{
	char *res;
	if (stat == S)
	{
		stat = Err;
		err_name = "Wrong work with strings";
	}
/*
	if (first->next == NULL)
	{
		stat = Err;
		err_name = "Empty input";
	}
*/
	if (stat != Err)
		return 0;
	else {
		if (buf_err != '\0')
		{
			
//			printf("ERROR: %s '%c' in str %d\n", 
//					err_name, buf_err, len_err);
			res = add_str("ERROR: ", err_name);
			res = add_str(res,"");
			res = add_char(res, buf_err);
			res = add_str(res, " in str");
			err_name = res;
			//itoa(len_err, strnum, 10);
			//res = add_str(res, strnum);
			
		}
		return 1;
	}
}
	

MakeLexems::MakeLexems()
{
	str_num = 1;
	stat = H;
	buf = '\0';
	buf_err = '\0';
	len_err = 0;
	lex_first = new lexems [1];
	lex_first -> type = num;
	lex_first -> name = '\0';
	lex_first -> str_num = 0;
	lex_first -> next = NULL;
	lex_now = lex_first;
}

void MakeLexems::Add_End()
{
	lexems *lex_new;
	lex_new = new lexems;
	lex_new -> type = End;
	lex_new -> name = add_str("","EOF");
	lex_new -> str_num = str_num;
	lex_new->next = NULL;
	lex_now->next=lex_new;
}

lexems *MakeLexems::Give_All()
{
	lexems *err;
	char *error;
	err = new lexems;
	if (Err_Analyze(lex_first)) 
	{
		err->type = errl;
		error = add_str(err_name, "");
		err->name = error;
		err->str_num = len_err;
		return err;
	}
	else {
		Add_End();	
		return lex_first->next;
	}
	
}

void WriteAll(lexems *first_el)
{
	lexems *first = first_el;
	if ((first -> type) ==  errl)
	{
		if (first->str_num != '\0')
			printf ("%s %d\n", first -> name, first->str_num);
		else 
			printf ("%s\n", first -> name);
		return;
	}
	#ifdef LEKS
		while (first -> type != End)
		{
			printf("lexem now is %s, type is: %s, str is: %d\n",
				first->name, enum_to_str(first->type), first->str_num);
			first = first->next;
		}
	#endif
}

void start_er()
{
	printf("Please, call me like\n");
	printf("./rob <prog.txt> <myname> <create> <num><ip>port>or\n");
	printf("./rob <prog.txt> <myname> <join> <name> <ip>port>or\n");
	exit(1);
}

//void start_RPN(tables *tab, RPNItem *first)
//{
//	printf("EEEH, START!\n");
//}

int main(int argc, char **argv)
{
	FILE *f;
	char c;
	lexems *first;
	game_info *all_info;
	MakeLexems *mas = new MakeLexems;
	SyntaxAnalyze *synt = new SyntaxAnalyze;
	if (argc != 7)
		start_er();
	f = fopen(argv[1], "r");
	if (!f)
	{
		perror("error");
		exit(1);
	}
	while ((c = fgetc(f)) != EOF)
		(*mas).New_Symb(c);
	first = (*mas).Give_All();
	WriteAll(first);

	if ((first -> type) !=  errl)
	{
		synt->Set(first);
		printf("\nRESULT OF SYNT ANALYZE: \n");
		synt->analyze();
	}

	all_info = (create_connection(&argv[1]));
	if (all_info -> stat == 1)
		return 1;

	synt->start_RPN(all_info);

	return 0;
}
