#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "robot_mod.hpp"

#define BUFSIZE 1000

char msgerr[] = "err";
char msgerrport[] = "Please, give me port more than 1023\n";
char msgerrsocket[] = "Invalid socket\n";
char msgerrip[] = "Invalid IP\n";
char msgerrconnect[] = "Sorry, connect error...\n";
char msgerrselect[] = "Sorry, select error...\n";
char msgcreate[] = ".create\n";
char msgstart[] = "start\n";
char msg_market[] = "market\n";
char msg_info[] = "info\n";
char msg_prod[] = "prod 2\n";
char msg_turn[] = "turn\n";


/*struct bsinfo {
	const char *operation, *name;
	int amount;
	int price;
	bsinfo *next;
};*/

struct input_info {
	char *buf;
	int buf_used, fd;
};

void tester_s()
{
	printf("TEEEST!\n");
}

int str_cmp(const char *str1, const char *str2);

/*
class Market{
	int month, raw, minprice, prod, maxprice;
public:
	Market() {month = 0, raw = 0, minprice = 0; prod = 0; maxprice = 0;}
	void month_new() {month++;}
	void raw_set(int r) {raw = r;}
	void minprice_set(int min) {minprice = min;}
	void prod_set(int pr) {prod = pr;}
	void maxprice_set(int max) {maxprice = max;}
	void Set(const char **com);
	int get_month() {return month;}
	int get_raw() {return raw;}
	int get_minprice() {return minprice;}
	int get_prod() {return prod;}
	int get_maxprice() {return maxprice;}
};*/

void Market::Set(const char **com)
{
	if (str_cmp(com[0], "market"))	
	{
		raw = (atoi(com[1])); 
		minprice = (atoi(com[2]));
		prod = (atoi(com[3]));
		maxprice = (atoi(com[4]));
	}
}

/*struct info_block {
	const char *name;
	int raw, prod, money, plants, autoplants;
	info_block *next;
};*/
/*
class Info{
	info_block *first;
	info_block *last;
	int players_count;
	int turn_num;
	const char *winner;
	char *my_name;
public:
	Info(char *name){first = NULL; last = NULL; players_count = 0;
		winner = NULL; my_name = name; turn_num = 1;}
	void Set(const char **com);
	bool Set_winner(const char **com);
	bool have_winner() {return winner != NULL;}
	int get_players_count() {return players_count;}
	int get_raw(char *name);
	int get_prod(char *name);
	int get_money(char *name);
	int get_plants(char *name);
	int get_autoplants(char *name);
	const char *get_winner() {return winner;}
};*/

int Info::get_raw(const char *name)
{
	info_block *now = first;
	while (now -> next != NULL)
	{
		if (str_cmp(now -> name, name))
			return now -> raw;
		now = now -> next;
	}
	if (str_cmp(now -> name, name))
		return now -> raw;
	return 0;
}

int Info::get_prod(const char *name)
{
	info_block *now = first;
	while (now -> next != NULL)
	{
		if (str_cmp(now -> name, name))
			return now -> prod;
		now = now -> next;
	}
	if (str_cmp(now -> name, name))
		return now -> prod;
	return 0;
}

int Info::get_money(const char *name)
{
	info_block *now = first;
	while (now -> next != NULL)
	{
		if (str_cmp(now -> name, name))
			return now -> money;
		now = now -> next;
	}
	if (str_cmp(now -> name, name))
		return now -> money;
	return 0;
}

int Info::get_plants(const char *name)
{
	info_block *now = first;
	while (now -> next != NULL)
	{
		if (str_cmp(now -> name, name))
			return now -> plants;
		now = now -> next;
	}
	if (str_cmp(now -> name, name))
		return now -> plants;
	return 0;
}

int Info::get_autoplants(const char *name)
{
	info_block *now = first;
	while (now -> next != NULL)
	{
		if (str_cmp(now -> name, name))
			return now -> autoplants;
		now = now -> next;
	}
	if (str_cmp(now -> name, name))
		return now -> autoplants;
	return 0;
}

bool Info::Set_winner(const char **com, Info *inf)
{
//	printf("IN SET_WINNER: %s\n", com[0]);
	if ((!str_cmp(com[0], "win"))&&(!str_cmp(com[0], "winner"))&&
	!((str_cmp(com[0], "bankrupt")&&(str_cmp(com[1], inf->get_myname())))))
		return 0;
	
	if (str_cmp(com[0], "win"))
	{	
		winner = my_name;
	}
	else
		winner = com[1];
	if ((str_cmp(com[0], "bankrupt")&&(str_cmp(com[1], inf->get_myname()))))
	{
		printf("I lose...\n");
		exit(1);
	}
	return 1;
}
		
void Info::Set(const char **com)
{
	if ((!str_cmp(com[0], "info"))&&(!str_cmp(com[0], "players")))
		return;
	if (str_cmp(com[0], "players"))
	{
		players_count = atoi(com[1]);
		return;
	}
		
	if (first == NULL)
	{
		first = new info_block;
		last = first;
		first -> name = com[1];
		first -> raw = atoi(com[2]);
		first -> prod = atoi(com[3]);
		first -> money = atoi(com[4]);
		first -> plants = atoi(com[5]);
		first -> autoplants = atoi(com[6]);
		first -> next = NULL;
	} else {
		info_block *now = first;
		while ((now -> next) != NULL)
		{
			now = now -> next;
			if (str_cmp(now -> name, com[1]))
			{
				now -> raw = atoi(com[2]);
				now -> prod = atoi(com[3]);
				now -> money = atoi(com[4]);
				now -> plants = atoi(com[5]);
				now -> autoplants = atoi(com[6]);
				return;
			}
		}
		now -> next = new info_block;
		now = now -> next;
		now -> name = com[1];
		now -> raw = atoi(com[2]);
		now -> prod = atoi(com[3]);
		now -> money = atoi(com[4]);
		now -> plants = atoi(com[5]);
		now -> autoplants = atoi(com[6]);
		now -> next = NULL;
	}
}

const char *Info::num_to_player(int a)
{
	players_tab *now;
	now = gamers;
	while (now -> next != NULL)
	{
		if (now -> num == a)
			return now -> name;
		now = now -> next;
	}
	if (now -> num == a)
		return now -> name;
	
	return "not finded";
}

int Info::player_to_num(const char *str)
{
	players_tab *now;
	now = gamers;
	while (now -> next != NULL)
	{
		if (str_cmp(now -> name, str))
			return now -> num;
		now = now -> next;
	}
	if (str_cmp(now -> name, str))
		return now -> num;
	return 0;
}

void Info::Set_tab(const char **com)
{
	if ((!str_cmp(com[0], "info")))
		return;
	if (gamers == NULL)
	{
		gamers = new players_tab;
		gamers -> name = com[1];
		if (str_cmp(com[1], my_name))
			my_id = 1;
		gamers -> num = 1;
		gamers -> next = NULL;
	} else {
		players_tab *now = gamers;
		while ((now -> next) != NULL)
		{
			now = now -> next;
			if (str_cmp(now -> name, com[1]))
				return;
		}
		now -> next = new players_tab;
		int i = (now -> num) ;
		now = now -> next;
		now -> num = ++i;
		now -> name = com[1];
		if (str_cmp(com[1], my_name))
		{
			my_id = now -> num;
		}
		if (now -> num > players_count)
			players_count = now -> num; 
		now -> next = NULL;
	}
}


/*class Operations_info {
	bsinfo *first;
	bsinfo *now;
public:
	Operations_info() {first = NULL; now = NULL;}
	int get_prod_price(char *name);
	int get_prod_sell(char *name);
	int get_raw_price(char *name);
	int get_raw_bought(char *name);
	void Add(const char **com);
};*/

int Operations_info::get_prod_price(const char *name)
{
	bsinfo *now = first;
	if (first == NULL)
		return 0;
	while (now -> next != NULL)
	{
		if ((str_cmp(now -> name, name))&&
			(str_cmp(now -> operation, "sold"))) 
			return now -> price;
		now = now -> next;
	}
	if ((str_cmp(now -> name, name))&&
		(str_cmp(now -> operation, "sold"))) 
		return now -> price;
	return 0;
}

int Operations_info::get_raw_price(const char *name)
{
	bsinfo *now = first;
	//printf("!!!!get_raw_price:::: %s\n", name);
	if (first == NULL)
		return 0;
	while (now -> next != NULL)
	{
		if ((str_cmp(now -> name, name))&&
			(str_cmp(now -> operation, "bought")))
		{
			return now -> price;
		}
		now = now -> next;
	}
	if ((str_cmp(now -> name, name))&&
		(str_cmp(now -> operation, "sell"))) 
		return now -> price;
	return 0;
}

int Operations_info::get_prod_sell(const char *name)
{
	bsinfo *now = first;
	if (first == NULL)
		return 0;
	while (now -> next != NULL)
	{
		if ((str_cmp(now -> name, name))&&
			(str_cmp(now -> operation, "sell"))) 
			return now -> amount;
		now = now -> next;
	}
	if ((str_cmp(now -> name, name))&&
		(str_cmp(now -> operation, "sell"))) 
		return now -> amount;
	return 0;
}

int Operations_info::get_raw_bought(const char *name)
{
	bsinfo *now = first;
	if (first == NULL)
		return 0;
	while (now -> next != NULL)
	{
		if ((str_cmp(now -> name, name))&&
			(str_cmp(now -> operation, "bought"))) 
			return now -> amount;
		now = now -> next;
	}
	if ((str_cmp(now -> name, name))&&
		(str_cmp(now -> operation, "bought"))) 
		return now -> amount;
	return 0;
}
/*struct game_info{
	Info *info_players;
	Operations_info *info_op;
	Market *mark;
	const char **com;
	input_info *inp;
	int stat;
};*/

void Operations_info::Add (const char **com)
{
	if ((str_cmp(com[0], "bought"))||(str_cmp(com[0], "sold")))
	{
		if (first == NULL)
		{
			first = new bsinfo;
			now = first;
		} else {
			now -> next = new bsinfo;
			now = now -> next;
		}
		(now -> operation) = com[0];
		(now -> name) = com[1];
		(now -> amount) = atoi(com[2]);
		(now -> price) = atoi(com[3]);
		(now -> next) = new bsinfo;
		(now -> next) = NULL;
	}
}


int str_cmp(const char *str1, const char *str2)
{
	int i = 0;
	while ((str1[i] != '\0') && (str2[i] != '\0') && (str1[i]==str2[i]))
		i++;
	return (str1[i]==str2[i]);
}

int symb_cmp(const char *str1, const char *str2)
{
	int i = 0;
	while ((str1[i] != '\0') && (str2[i] != '\0') && (str1[i]==str2[i]))
		i++;
	return i;
}

void synt_error()
{
	printf("Please, call me like: ./robot<myname><create><num><IP><port>");
	printf(" or \n	./robot<myname><join><PlayerName><IP><port>\n");
	exit(1);
}

int str_len(const char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}

char *add_str(const char *str1, const char *str2)
{
	int len_res, i, len1 = 0;
	char *str_res;
	len_res = str_len(str1) + str_len(str2) + 1;
	str_res = new char[len_res*sizeof(char)];
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
	return str_res;
}
	
const char *get_str(const char *str, int num)
{
	int i = 0;
	char *new_str;
	new_str = new char [sizeof(char) * 10];
	while ((str[num+i] != '\0')&&(str[num+i] != '\n')&&(str[num+i]!=' '))
	{
		new_str[i] = str[num + i];
		i++;
	}
	new_str[i] = '\0';
	return new_str;
}
	
const char *get_str_num(const char *str, int num)
{
	int i, count = 0, fl = 0;
	const char *err = "err";
	for (i = 0; i < str_len(str); i++)
	{
		if ((str[i] != ' ') && (str[i] != '\n') && (fl == 0))
		{
			fl = 1;
			count ++;
			if (count == num)
				return (get_str(str, i));
		}
		if ((str[i] == ' ') || (str[i] == '\n'))
			fl = 0; 
	}
	return err;
}
		
		
const char **market_analyze(const char *str)
{
	int i;
	const char **itog;
	itog = new const char *[5*sizeof(char *)]; //5 = lenght of market string
	itog[0] = "market"; 
	for (i = 1; i <= 4; i++) //4 = num of elements in market string
		itog[i] = get_str_num(str, i+2);//cause first symbs'&'and'MAR..'
	return itog;
}

const char **info_analyze(const char *str)
{
	const char **itog;
	int i;
	itog = new const char *[7* sizeof(char *)]; //7 = lenght of info string
	itog[0] = "info";
	for (i = 1; i <= 6; i++) // 6 = num of elements in info string
		itog[i] = get_str_num(str, i+2);//cause first symbs '&'and'INFO'
	return itog;
}

const char **players_analyze(const char *str)
{
	const char **itog;
	itog = new const char *[3 * sizeof(char *)];//3=lenght of players string
	itog[0] = "players";
	itog[1] = get_str_num(str, 3);
	itog[2] = get_str_num(str, 5);
	return itog;

}

const char **bought_analyze(const char *str)
{
	int i;
	const char **itog;
	itog = new const char *[4*sizeof(char *)]; //4 = lenght of bought string
	itog[0] = "bought";
	for (i = 1; i <= 3; i++) // 3 = num of elements in info string
		itog[i] = get_str_num(str, i+2);//cause first symbs'&'and'BOU..'
	return itog;

}

const char **sold_analyze(const char *str) 
{
	int i;
	const char **itog;
	itog = new const char *[4* sizeof(char *)]; //4 = lenght of info string
	itog[0] = "sold";
	for (i = 1; i <= 3; i++) // 3 = num of elements in sold string
		itog[i] = get_str_num(str, i+2);//cause first symbs '&'and'SOLD'
	return itog;

}

const char **bankrupt_analyze(const char *str)
{
	const char **itog;
	itog = new const char *[2*sizeof(char *)]; //2=lenght of bankrup  string
	itog[0] = "bankrupt";
	itog[1] = get_str_num(str, 3); //on 3th position - name of bankrupt
	return itog;

}

const char **winner_analyze(const char *str)
{
	const char **itog;
	itog = new const char *[2*sizeof(char *)]; //2=lenght of bankrup string
	itog[0] = "winner";
	itog[1] = get_str_num(str, 3); //on 3th position - name of winner
	return itog;
}

const char **analyze_command(const char *str)
{
	const char **itog_start, **itog_er, **itog_new, **itog_lost, **itog_ok;
	const char **itog_endturn, **itog_win, **itog_bad;
	itog_endturn = new const char *[sizeof(char *)];
	itog_start = new const char *[sizeof(char *)];
	itog_er = new const char *[sizeof(char *)];
	itog_new = new const char *[sizeof(char *)];
	itog_lost = new const char *[sizeof(char *)];
	itog_ok = new const char *[sizeof(char *)];
	itog_win = new const char *[sizeof(char *)];
	itog_bad = new const char *[sizeof(char *)];
	itog_ok[0] = "ok";
	itog_bad[0] = "bad";
	itog_lost[0] = "lost player";
	itog_new[0] = "new player";
	itog_start[0] = "start";	
	itog_er[0] = "unusable";
	itog_endturn[0] = "end of turn";
	itog_win[0] = "win";
	//printf("STR NOW IS: %s\n", str);
	//NOWINNER!!!!!!	
	if (symb_cmp(str, "& OK") == 4)// = lenght of "& OK" 
		return itog_ok;
	if (str_cmp(str, "& START"))
		return itog_start;
	if (symb_cmp(str, "& YOU_WIN") == 9)//9 = length of "& YOU_WIN"
		return itog_win;
	if (symb_cmp(str, "& ENDTURN") == 9 )//6 = lenght of "& ENDTURN"
		return itog_endturn;
	if (symb_cmp(str, "& BOUGHT") == 8) //8 = lenght of "& BOUGHT"
		return (bought_analyze(str));
	if (symb_cmp(str, "& SOLD") == 6) //6 = lenght of "& SOLD"
		return (sold_analyze(str));
	if (symb_cmp(str, "& MARKET") == 8) //8 = lenght of "& MARKET"
		return (market_analyze(str));
	if (symb_cmp(str, "& INFO") == 6) //6 = lenght of "& MARKET"
		return (info_analyze(str));
	if (symb_cmp(str, "& PLAYERS") == 9 )//6 = lenght of "& PLAYERS"
		return (players_analyze(str));
	if (symb_cmp(str, "& ENDTURN") == 9 )//6 = lenght of "& ENDTURN"
		return itog_endturn;
	if (symb_cmp(str, "& BANKRUPT") == 10)//10 = lenght of "& BANKRUPT"
		return (bankrupt_analyze(str));
	if (symb_cmp(str, "& WINNER") == 8)//8 = lenght of "& WINNER"
		return (winner_analyze(str));
	if (symb_cmp(str, "@+") == 2)//2 = lenght of "@+"
		return itog_new;
	if (symb_cmp(str, "@-") == 2)//2 = lenght of "@-"
		return itog_lost;
	if (symb_cmp(str, "&-") == 2)//2 = lenght of "@-"
		return itog_bad;
	
	return itog_er;
}

void move_str(char *buf, int n)
{
	int i;
	for (i = n ; i < BUFSIZE; i++)
	{
		buf[i - n] = buf[i];
	}
}
		
char *make_str(const char *buf, int num)
{
	int i;
	char *new_str;
	new_str = new char[(num+1)*sizeof(char)];
	for (i = 0; i < num; i++)
		new_str[i] = buf[i];
	new_str[i] = '\0';
	return(new_str);
}
	



const char **search_com(input_info *inp)
{
	const char *str;
	int i;
	const char **itog_empty;
	itog_empty = new const char *[sizeof(char *)];
	itog_empty[0] = "EMPTY";	
	for (i = 0; i < (inp->buf_used); i++)
	{
		if ((inp->buf)[i] == '\n')
		{	 
			str = make_str(inp->buf, i);
			//printf("first symb of buf is: %c\n", buf[0]);
			move_str(inp->buf, i + 1);
			(inp->buf_used) = (inp->buf_used) - i - 1;
			return analyze_command(str);
			
		}
	}
	return itog_empty;
}
			

const char **an_com (input_info *inp)		
{
	int succs_readed;
	const char **itog_empty_1, **itog_er, **com;
	itog_empty_1 = new const char *[sizeof(char *)];
	itog_er = new const char *[sizeof(char *)];
	itog_er[0] = "Error\n";	
	itog_empty_1[0] = "empty";
	com = search_com(inp);
	if (!str_cmp(com[0], "EMPTY"))
	{
		return com;
	}
//	printf("info about buf: buf:[%s], buf_used: [%d]\n", inp->buf, inp->buf_used);
	succs_readed = read(inp->fd, &(inp->buf)[(inp->buf_used)], 10);
//	printf("SUC READED: %d\n", succs_readed);
	//printf("USED: %d\n", (*buf_used));
	if (succs_readed == -1) 
		return itog_er;
	if (succs_readed == 0)
		return itog_empty_1;
	if (succs_readed > 0)
	{
		//printf("Succs readed: %d\n", succs_readed);
		(inp->buf_used) = (inp->buf_used) + succs_readed;
//		printf("BUFER:buf_used :%d, bufer is:[%s]\n", (inp->buf_used), inp->buf);
		return search_com (inp);
	}
	return itog_er;
} 

void wait_friends_creator(int cl_count, input_info *inp)
{
	int counter = 0;
	const char **com;
	printf("I'm waiting other players...\n");
	while (counter < cl_count)
	{
		com = an_com(inp);				/////
//		printf("COM[0] is %s\n", com[0]);
		if (str_cmp(com[0], "new player"))
			counter ++;
		if (str_cmp(com[0], "lost player"))
			counter --;
	
	}

}

int wait_joined_friends(input_info *inp)
{
	int counter = 0;
	const char **com;
	printf("I'm waiting other players...\n");
	for (;;)
	{
		com = an_com(inp);
		if (str_cmp(com[0], "new player"))
			counter ++;
		if (str_cmp(com[0], "lost player"))
			counter --;
		if (str_cmp(com[0], "start"))
			break;
	}
	return counter;
}

void reverse (char *str)
{
	int i, j;
	char c;
	for (i = 0, j =str_len(str) - 1; i < j; i++, j--)
	{
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}
}

char *num_to_str(int n)
{
	int i, sign;
	char *str;
	str = new char[16*sizeof(char)];
	
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		str[i++] = n%10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		str[i++] = '-';
	str[i] = '\0';
	reverse(str);
	return str; 
}
	


char *make_buy_str(int a, int b)
{
	return add_str(add_str(add_str(add_str("buy ", num_to_str(a)),
						" "), num_to_str(b)), "\n");

}	

char *make_sell_str(int a, int b)
{
	return add_str(add_str(add_str(add_str("sell ", num_to_str(a)),
						" "), num_to_str(b)), "\n");
}

char *make_prod_str(int a)
{
	return add_str(add_str("prod ", num_to_str(a)), "\n");
}

/*void write_allinfo(Market *mark, Info *players, int cl_count)
{
	printf("#############################\n");
	printf("MARKET:\n");
	printf("Month now is: %d\n", (*mark).get_month());
	printf("Row: %d\n", (*mark).get_raw());
	printf("Minprice: %d\n", (*mark).get_minprice());
	printf("Prod: %d\n", (*mark).get_prod());
	printf("Maxprice: %d\n", (*mark).get_maxprice());
	printf("-----------------------------\n");
	printf("Info about players:\n");
	for (i = 0; i< cl_count; i++)
	{
		printf("PLAYER'S NAME IS: %s\n", (players[i]).get_name()); 
		printf("His raw: %d\n", (players[i]).get_raw()); 
		printf("His prod: %d\n", (players[i]).get_prod()); 
		printf("His money: %d\n", (players[i]).get_money()); 
		printf("His plants: %d\n", (players[i]).get_plants()); 
		printf("His autoplants: %d\n", (players[i]).get_autoplants());
		printf("-----------------------------\n");
	}
}

void write_sb(bsinfo **first)
{
	printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
	printf("Info about operations: \n");	
	while (((*first) -> next) != NULL)
	{
		printf("Operation: %s\n", (*first) -> operation);
		printf("Name: %s\n", (*first) -> name);
		printf("amount: %d\n", (*first) -> amount);
		printf("price: %d\n\n", (*first) -> price);
		(*first) = (*first) -> next;
	}	
}*/
	
int analyze(const char **com, Market *mark, Info *players, int cl_count)
{
	if (str_cmp(com[0], "info"))
	{	
		//info_analyze(com, players, cl_count);
		return 1;
	}
	return 0;
}

void analyze_bought_sold(const char **com, bsinfo **first)
{
//	printf("BOUGHT AND SOLD ANALYZE\n");
//	printf("COM[0] is %s\n", com[0]);
	if ((str_cmp(com[0], "bought"))||(str_cmp(com[0], "sold")))
	{
		//printf("ENTERED\n");
		while (((*first) -> next) != NULL)
			(*first) = (*first) -> next;
	//	printf("ENTERED2\n");
		((*first) -> operation) = com[0];
		((*first) -> name) = com[1];
		((*first) -> amount) = atoi(com[2]);
		((*first) -> price) = atoi(com[3]);
		((*first) -> next) = new bsinfo [sizeof(bsinfo)];
		(*first) = ((*first) -> next);
		((*first) -> next) = NULL;
	}
}


bool analyze_market(input_info *inp, Market *mark, const char **com,
						Info *players_info)
{
//	printf("!!!!!!!IN ANMARKET\n");
	while (!(str_cmp(com[0], "market")))
	{
		com = an_com(inp);
		if (players_info -> Set_winner(com, players_info))
			return (players_info -> have_winner());	
		mark->Set(com);
	}
	while (!(str_cmp(com[0], "unusable")))
	{
		com = an_com(inp);
		if (players_info -> Set_winner(com, players_info))
			return (players_info -> have_winner());	
	}
	return (players_info -> have_winner());	
}	

void analyze_info(input_info *inp, Info *players_info, const char **com)
{
	//printf("@@@@@@@@IN ANINFO\n");
	while (!(str_cmp(com[0], "players")))
	{
		com = an_com(inp);
		players_info->Set(com);
		players_info->Set_tab(com);
		if (players_info -> Set_winner(com, players_info))
			exit(1);
	}
	while (!(str_cmp(com[0], "unusable")))
	{
		com = an_com(inp); 
	}
}	

void sell(const char **com, input_info *inp, int a, int b)
{
	char *sell_str = make_sell_str(a, b);
	write(inp->fd, sell_str , sizeof(char)*str_len(sell_str));
	while (!((str_cmp(com[0], "ok"))||(str_cmp(com[0], "bad"))))
	{
		//printf("%s\n", com[0]);
		com = an_com(inp);
	}
	
}		

void buy(const char **com, input_info *inp, int a, int b)
{
	char *buy_str = make_buy_str(a, b);
	write(inp->fd, buy_str , sizeof(char)*str_len(buy_str));
	while (!((str_cmp(com[0], "ok"))||(str_cmp(com[0], "bad"))))
	{
		com = an_com(inp);
	}
	
}

void prod(const char **com, input_info *inp, int a)
{
	char *prod_str = make_prod_str(a);
	write(inp->fd, prod_str , sizeof(char)*str_len(prod_str));
	while (!((str_cmp(com[0], "ok"))||(str_cmp(com[0], "bad"))))
	{
		com = an_com(inp);
	}
	
}

void turn(const char **com, input_info *inp, Operations_info *info_op,
		Market *mark, Info *info_players)
{
	char *turn_str = add_str("turn ", "\n");
	info_op -> del();
	write(inp->fd, turn_str , sizeof(char)*str_len(turn_str));
	while (!(str_cmp(com[0], "end of turn")))
	{
		com = an_com(inp);
		info_op->Add(com);
		if (info_players -> Set_winner(com, info_players))
			exit(1);

	}
	write(inp->fd, msg_market, sizeof(char)*str_len(msg_market));
	analyze_market(inp, mark, com, info_players);
	write(inp->fd, msg_info, sizeof(char)*str_len(msg_market));
	analyze_info(inp, info_players, com);
	write(inp->fd, "\n", sizeof(char)*(str_len("\n")));
}

game_info *print_read_commands(input_info *inp, char *myname)
{
	const char **com = NULL;
	Market *mark = new Market;;
	Info *info_players = new Info(myname);
	Operations_info *info_op = new Operations_info;
	game_info *all_inf = new game_info;
	com = an_com(inp);

	//while (!info_players -> have_winner())
	write(inp->fd, msg_market, sizeof(char)*str_len(msg_market));

	analyze_market(inp, mark, com, info_players);
//	if (analyze_market(inp, mark, com, info_players))
//		break;
	write(inp->fd, msg_info, sizeof(char)*str_len(msg_market));
	analyze_info(inp, info_players, com);
	write(inp->fd, "\n", sizeof(char)*(str_len("\n")));
	all_inf -> info_players = info_players;
	all_inf -> info_op = info_op;
	all_inf -> mark = mark;
	all_inf -> com = com;
	all_inf -> inp = inp;
	all_inf -> stat = 0;
	return all_inf;	
		//sell(com, inp, 2, mark->get_maxprice());
		//buy(com, inp, 2, mark->get_minprice());
		//prod(com, inp, 2);
		//turn(com, inp, info_op);
//	if (str_cmp(info_players -> get_winner(), myname))
////		printf("I WIN, NACHALNIKA!!!\n");
//	else
//		printf("SORRY, I LOSE...\n");
}

game_info *workwithserv(input_info *inp, char *mode, char *param, char *myname)
{
	char *msg_ss, *msg_s;
	char c;
	Market mark;
	int  cl_count;
	inp->buf_used = 0;
	inp->buf = new char[BUFSIZE*sizeof(char)];
	cl_count = atoi(param);
	while (c != ':')
		read(inp->fd, &c, sizeof(char));
	read(inp->fd, &c, sizeof(char));
	write(inp->fd, myname, sizeof(char)*str_len(myname));
	write(inp->fd, "\n", sizeof(char));
	if (str_cmp("create", mode))
	{
		write(inp->fd, msgcreate, sizeof(char)*str_len(msgcreate));
		cl_count = atoi(param);
		wait_friends_creator(cl_count, inp);
		write(inp->fd, msgstart, sizeof(char)*str_len(msgstart));
		write(0, msgstart, sizeof(char)*str_len(msgstart));
	}
	else {
		msg_s = add_str(".join ", param);
		msg_ss = add_str(msg_s, "\n");
		write(inp->fd, msg_ss, sizeof(char)*str_len(msg_ss));
		cl_count = wait_joined_friends(inp);
		//printf("CL_COUNT IS: %d\n", cl_count);
	}
	return(print_read_commands(inp, myname));
	/*
	if(str_cmp(com[0],"bankrupt")&&(str_cmp(com[1],myname)))
		printf("I lose, Nacalnika...\n");
	if (str_cmp(com[0], "win"))
		printf("I win, Nachalnika!\n");
	if (str_cmp(com[0], "winner"))
		printf("Sorry, Nachalnika, winner is %s...\n", com[1]);
	*/
}

game_info *create_connection(char **argv)
{
	char *str_ip, *str_port, *mode, *param, *myname;
	input_info *inp = new input_info;
	int  port;
	struct sockaddr_in addr;
	char *endptr;
	game_info *info_all = new game_info;
	str_ip = argv[4];
	str_port = argv[5];
	mode = argv[2];
	param = argv[3];
	myname = argv[1];
	port = strtol(str_port, &endptr, 10); /*from string to long*/
	if (port<1024)
	{
		write(0, msgerrport, sizeof(msgerrport));
		info_all -> stat = 1;
		return info_all;
	}

	inp->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (inp->fd == -1)
	{
		write(0, msgerrsocket, sizeof(msgerrsocket));
		info_all -> stat = 1;
		return info_all;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (!inet_aton(str_ip, &(addr.sin_addr))) 
	{
		write(0, msgerrip, sizeof(msgerrip));
		info_all -> stat = 1;
		return info_all;
	}
	if (0 != connect(inp->fd, (struct sockaddr *)&addr, sizeof(addr)))
	{
		write(0, msgerrconnect, sizeof(msgerrconnect));
		info_all -> stat = 1;
		return info_all;
	}
	return(workwithserv(inp, mode, param, myname));
}	


/*int main(int argc, char **argv)
{
	if (argc != 6)
		synt_error();
	if ((str_cmp(argv[2],  "create")) ||  (str_cmp(argv[2],  "join")))
		create_connection(argv);
	else
		synt_error();

	//printf("%d\n", argc);
	return 0;
}*/

