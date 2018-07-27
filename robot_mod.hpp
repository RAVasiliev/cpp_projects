#ifndef ROBOT_H_SENTRY
#define ROBOT_H_SENTRY

#include <stdio.h>

class Operations_info;
class Market;

struct input_info;

struct info_block {
        const char *name;
        int raw, prod, money, plants, autoplants;
        info_block *next;
};


struct bsinfo {
        const char *operation, *name;
        int amount;
        int price;
        bsinfo *next;
};

class Operations_info {
        bsinfo *first;
        bsinfo *now;
public:
        Operations_info() {first = NULL; now = NULL;}
        int get_prod_price(const char *name);
        int get_prod_sell(const char *name);
        int get_raw_price(const char *name);
        int get_raw_bought(const char *name);
	void del() {first = NULL;};
        void Add(const char **com);
};

class Market{
        int month, raw, minprice, prod, maxprice;
public:
        Market() {month = 1, raw = 0, minprice = 0; prod = 0; maxprice = 0;}
        void month_new() {month++;}
	void Set(const char **com);
        int get_month() {return month;}
        int get_raw() {return raw;}
        int get_minprice() {return minprice;}
        int get_prod() {return prod;}
        int get_maxprice() {return maxprice;}
};

struct players_tab {
	int num;
	const char *name;
	players_tab *next;
};

class Info{
        info_block *first;
        info_block *last;
	players_tab *gamers;
        int players_count;
        int turn_num;
        const char *winner;
        const char *my_name;
	int my_id;
public:
        Info(char *name){first = NULL; last = NULL; players_count = 0;
                winner = NULL; my_name = name; turn_num = 1;
		gamers = NULL; }
        void Set_tab(const char **com);
        void Set(const char **com);
        bool Set_winner(const char **com, Info *inf);
        bool have_winner() {return winner != NULL;}
        int get_players_count() {return players_count;}
        int get_turn_num() {return turn_num;}
        int get_my_id() {return my_id;};
        int get_raw(const char *name);
        int get_prod(const char *name);
        int get_money(const char *name);
        int get_plants(const char *name);
        int get_autoplants(const char *name);
	int player_to_num(const char *str);
	const char *num_to_player(int a);
        const char *get_winner() {return winner;}
        const char *get_myname() {return my_name;}
};

struct game_info{
	Info *info_players;
	Operations_info *info_op;
	Market *mark;
	const char **com;
	input_info *inp;
	int stat;
};

void turn(const char **com, input_info *inp, Operations_info *info_op,
		Market *mark, Info *info_players);
void sell(const char **com, input_info *inp, int a, int b);
void buy(const char **com, input_info *inp, int a, int b);
void prod(const char **com, input_info *inp, int a);


void tester_s();
int str_len(const char *str);
void reverse(char *str);
int str_cmp(const char *str1, const char *str2);
char *add_str(const char *str1, const char *str2);
game_info *create_connection(char **argv);

#endif
