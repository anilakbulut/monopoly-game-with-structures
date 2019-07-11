/*ANIL AKBULUT HW07----MONOPOLY GAME---- 151044029*/
/*I used this libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*I used these defines*/
#define ID_BLOCK 12 /*Number of array owned_block_id*/
#define LEN_BOARD 20/*Number of array map's elements*/
#define GAINS_START 10000/*If player's complete one tour,bank given money to player's*/
#define MONEY_START 100000/*Start money for players*/

typedef enum{noone,cap,car}player_type;
typedef enum{start,property,tax,punish}block_type;

struct player
{
	player_type type; /*Holds type of the player.*/
	int current_block_id;/*Holds player location as block id.*/
	int owned_block_ids[ID_BLOCK];/*Holds property block ids that are owned by the user.*/
	int account;/*Holds amount of the current money of the player.*/
	int turn_to_wait;/*Holds number of turns to wait if the player got a punishment.*/
	char *name;/*Holds the name of the player.*/
};

struct block
{
	int block_id;/*Holds id of the block.*/
	char *name;/*Holds text of the block that is shown on the top of the block.*/
	int price;/*Holds the price of property.*/
	int rent;/*Holds the default rent of a property (no house).*/
	int rent_1;/*Holds the rent of a property with one house.*/
	int rent_2;/*Holds the rent of a property with two houses.*/
	int rent_3;/*Holds the rent of a property with three houses.*/
	int house_price;/*Holds price of building a house on the block.*/
	int house_count;/*Holds the number of the houses on the block that are already built.*/
	struct player owner;/*Holds the owner of the block.*/
	block_type type;/*Holds type of the block.*/
};

void init_the_board(struct block board[LEN_BOARD]);
void show_board(struct block board[LEN_BOARD],struct player player_one, struct player player_two);
void show_properties(struct block board[LEN_BOARD]);
void gameplay (struct block board[LEN_BOARD], struct player player_one, struct player player_two);
void buy_property(struct block* current_block, struct player* current_player);
void sell_property(struct block board[LEN_BOARD], struct player* current_player);
void gameplay (struct block board[LEN_BOARD], struct player player_one, struct player player_two);
void show_my_properties(struct block board[LEN_BOARD],struct player player);
void buy_house(struct block *current_block,struct player* current_player);
void show_menu(struct player *current_player);
int check_list(struct player *current_player);

/*This function check players's proporties*/
int check_list(struct player *current_player){
	int i;
	for(i=0;i<ID_BLOCK;i++){
		if(current_player->owned_block_ids[i]!=-1) return 1;
	}
	return 0;
}

/*This function displays the menu options on the screen.*/
void show_menu(struct player *current_player){
	printf("*****_______%s_______*****\n",current_player->name);
	printf("1 - Roll the dice \n");
	printf("2 - Show my account\n");
	printf("3 - Show my properties\n");
	printf("4 - Show properties deeds\n");
	printf("5 - Buy Property\n");
	printf("6 - Buy House\n");
	printf("7 - Sell property\n");
	printf("Please select an option to continue\n");	
}
/*This function allows the player to build a house on the property he/she owns.*/
void buy_house(struct block *current_block,struct player* current_player){

	int num;
	int warning=0;
	if(current_block->owner.type==current_player->type){
		printf("You can buy up to 3 houses\n");
		printf("Number of homes available = %d\n",current_block->house_count );
		if(current_block->house_count >= 3){
			printf("You can't buy more houses\n");
		}
		else if(current_player->account > current_block->house_price){
			printf("Your money = %d $\n", current_player->account);
			printf("A house price = %d $\n",current_block->house_price );
			printf("How many houses do you want to buy?:\n");
			do{
				scanf("%d",&num);
				warning=0;
				if(current_player->account < current_block->house_price * num) {
					warning=1;

					printf("Your money is not enough to buy %d houses.Enter number again\n",num );
				}
			}while((current_block->house_count + num > 3) || warning==1 || (num<0));
			current_block->house_count += num;
			current_player->account -= (num * current_block->house_price);
		}
	}
	printf("\n");
}
/*This function only shows the properties of the selected user*/
void show_my_properties(struct block board[LEN_BOARD],struct player player){
	int i;
	printf("My properties:\n");
	for(i=0;i<LEN_BOARD;i++){
		if(board[i].owner.type==player.type){
			printf("%d - %s\n",i,board[i].name );
		}
	}
	printf("\n");
}

void gameplay (struct block board[LEN_BOARD], struct player player_one, struct player player_two){
	show_board(board,player_one,player_two); /*The menu is displayed before the game starts.*/
	int queue=1; /*This variable controls which player is in the queue.*/
	int choice;
	int num;
	int i;
	int check_1=0,check_2=0; /*These variables determine which player's money is over and the winner of the game.*/
	int check_buy=3;
	while(check_1==0 && check_2==0){ /*The game continues as long as one of the two players has no money.*/
		check_buy=3;
		/*This condition checks whether one of the players has waited for the wait_turn penalty.*/
		if(player_one.turn_to_wait!=0 && player_two.turn_to_wait!=0){ 
			if(player_one.turn_to_wait==player_two.turn_to_wait){
				player_one.turn_to_wait=0;
				player_two.turn_to_wait=0;
			}
			else{
				player_one.turn_to_wait--;
				player_two.turn_to_wait--;
			}
		}
		if(queue==1 && player_one.turn_to_wait==0){/*If this is the first player, this switch works.*/
			show_menu(&player_one);
			scanf("%d",&choice);
			switch(choice){
				/*If the player chooses 1, the player throws the dice.
				  If the player comes up on an unclaimed property and has enough money, then the player is asked if he wants to buy the house.
				  If the player has to pay a fine, he / she pays the penalty.
				  If the money is not enough and if the property is asked to sell.
				  The player loses the game if his money is not enough.
				*/
				case 1:			player_one.current_block_id += 1 + rand()%6;
								if(player_one.current_block_id>LEN_BOARD-1){
									player_one.current_block_id = player_one.current_block_id%LEN_BOARD;
									player_one.account += GAINS_START;
								}
								show_board(board,player_one,player_two);
								if(board[player_one.current_block_id].owner.type==noone && board[player_one.current_block_id].type==property){
									if(player_one.account > board[player_one.current_block_id].price){
										printf("Do you want to buy here?\n");
										printf("1 - Yes\n");
										printf("2 - No\n");
										while(check_buy!=1 && check_buy!=2){
											scanf("%d",&check_buy);
										}
										if(check_buy==1){
											buy_property(&board[player_one.current_block_id],&player_one);
											if(player_one.account > board[player_one.current_block_id].price){
												buy_house(&board[player_one.current_block_id],&player_one);
											}
											queue=2;
										}
									}
								}
								if(board[player_one.current_block_id].type==tax || board[player_one.current_block_id].type==punish){
									player_one.account -= board[player_one.current_block_id].rent;
								}
								if(board[player_one.current_block_id].owner.type == player_two.type){
									if(board[player_one.current_block_id].house_count==0){
										player_one.account -= board[player_one.current_block_id].rent;
									}
									if(board[player_one.current_block_id].house_count==1){
										player_one.account -= board[player_one.current_block_id].rent_1;
									}
									if(board[player_one.current_block_id].house_count==2){
										player_one.account -= board[player_one.current_block_id].rent_2;
									}
									if(board[player_one.current_block_id].house_count==3){
										player_one.account -= board[player_one.current_block_id].rent_3;
									}
								}
								if(player_one.account<0){
									printf("You need to pay!\n");
									while(player_one.account < 0){
										if(check_list(&player_one)==1)	sell_property(board,&player_one);
										else{
											printf("%s winning the game. Congratulations!\n",player_two.name);
											return;
										}
									}
								}
								else{
									if(player_two.turn_to_wait!=0){
										player_two.turn_to_wait--;
										queue=1;
									}
									else queue=2;
									if(player_one.current_block_id==5){
										player_one.turn_to_wait=2;
										queue=2;	
									}
									else if(player_one.current_block_id==15){
										player_one.turn_to_wait=1;
										queue=2;
									}
								}	
							break;
				/*If the player selects 2, the player's account is shown on the screen*/
				case 2:		printf("My account : %d$\n",player_one.account );
							break;
				/*If the player selects 3, the player's properties is displayed.*/
				case 3:		show_my_properties(board,player_one);
							break;
				/*If the player selects 4, all the properties's information in the game is displayed.*/
				case 4:		show_properties(board);
							break;
				/*If the player chooses 5, he / she buys the property on which*/
				case 5:		buy_property(&board[player_one.current_block_id],&player_one);
							break;
				/*If the player selects 6, he can install a house on the property.*/
				case 6:		buy_house(&board[player_one.current_block_id],&player_one);
							break;
				/*If the player selects 6, he can sell the property.*/
				case 7:		sell_property(board,&player_one);
							break;
				default :   break;
			}
		}

		else if(queue==2  && player_two.turn_to_wait==0){/*If this is the second player, this switch works.*/
			show_menu(&player_two);
			scanf("%d",&choice);
			switch(choice){
				/*If the player chooses 1, the player throws the dice.
				  If the player comes up on an unclaimed property and has enough money, then the player is asked if he wants to buy the house.
				  If the player has to pay a fine, he / she pays the penalty.
				  If the money is not enough and if the property is asked to sell.
				  The player loses the game if his money is not enough.
				*/
				case 1:			player_two.current_block_id += 1 + rand()%6;
								if(player_two.current_block_id>LEN_BOARD-1){
									player_two.current_block_id = player_two.current_block_id%LEN_BOARD;
									player_two.account += GAINS_START;
								}
								show_board(board,player_one,player_two);
								if(board[player_two.current_block_id].owner.type==noone && board[player_two.current_block_id].type==property){
									if(player_two.account > board[player_two.current_block_id].price){
										printf("Do you want to buy here?\n");
										printf("1 - Yes\n");
										printf("2 - No\n");
										while(check_buy!=1 && check_buy!=2){
											scanf("%d",&check_buy);
										}
										if(check_buy==1){
											buy_property(&board[player_two.current_block_id],&player_two);
											if(player_two.account > board[player_two.current_block_id].house_price){
												buy_house(&board[player_two.current_block_id],&player_two);
											}
											queue=1;
										}
									}
								}
								if(board[player_two.current_block_id].type==tax || board[player_two.current_block_id].type==punish){
									player_two.account -= board[player_two.current_block_id].rent;
								}
								if(board[player_two.current_block_id].type == player_one.type){
									if(board[player_two.current_block_id].house_count==0){
										player_two.account -= board[player_two.current_block_id].rent;
									}
									if(board[player_two.current_block_id].house_count==1){
										player_two.account -= board[player_two.current_block_id].rent_1;
									}
									if(board[player_two.current_block_id].house_count==2){
										player_two.account -= board[player_two.current_block_id].rent_2;
									}
									if(board[player_two.current_block_id].house_count==3){
										player_two.account -= board[player_two.current_block_id].rent_3;
									}
								}
								if(player_two.account<0){
									printf("You need to pay!\n");
									while(player_two.account < 0){
										if(check_list(&player_two)==1)	sell_property(board,&player_two);
										else{
											printf("%s winning the game. Congratulations!\n",player_one.name);
											return;
										}
									}
								}
								else{
									if(player_one.turn_to_wait!=0){
										player_one.turn_to_wait--;
										queue=2;
									}
									else queue=1;

									if(player_two.current_block_id==5){
										player_two.turn_to_wait=2;
										queue=1;
									}
									else if(player_two.current_block_id==15){
										player_two.turn_to_wait=1;
										queue=1;
									}
								}
							break;
				/*If the player selects 2, the player's account is shown on the screen*/
				case 2:		printf("My account : %d$\n",player_two.account );
							break;
				/*If the player selects 3, the player's properties is displayed.*/
				case 3:		show_my_properties(board,player_two);
							break;
				/*If the player selects 4, all the properties's information in the game is displayed.*/
				case 4:		show_properties(board);
							break;
				/*If the player chooses 5, he / she buys the property on which*/
				case 5:		buy_property(&board[player_two.current_block_id],&player_two);						
							break;
				/*If the player selects 6, he can install a house on the property.*/
				case 6:		buy_house(&board[player_two.current_block_id],&player_two);
							break;
				/*If the player selects 6, he can sell the property.*/
				case 7:		sell_property(board,&player_two);
							break;
				default :   break;
			}
		}
	}
	if(check_1==1) printf("%s winning the game. Congratulations!\n",player_two.name);
	else if(check_2==1) printf("%s winning the game. Congratulations!\n",player_one.name);
}
/*This function is used to sell properties*/
void sell_property(struct block board[LEN_BOARD], struct player* current_player){
	int i,num;
	int count_property=0;
	printf("Choose the property you want to sell :  ");
	for(i=0;i<LEN_BOARD;i++){
		if(board[i].owner.type==current_player->type){
			printf("\n%d - %s",board[i].block_id,board[i].name);
			count_property++;
		}
	}
	printf("\n");
	if(count_property!=0){
		do{
			scanf("%d",&num);
		}while(board[num].owner.type!=current_player->type);
		current_player->account += board[num].price/2;
		current_player->account += (board[num].house_price/2) * board[num].house_count;
		board[num].owner.type = noone;
		for(i=0;i<ID_BLOCK;i++){
			if(current_player->owned_block_ids[i]==num){
				current_player->owned_block_ids[i]=-1;
				break;
			}
		}
		printf("%s has been sold\n\n",board[num].name );
	}
	else printf("You do not have any property to sell.\n\n");
}
/*This function is used to buy property*/
void buy_property(struct block* current_block, struct player* current_player){
	int i;
	if(current_block->type!=property || current_block->owner.type!=noone){
		printf("You cannot buy this property\n");
	}
	else if(current_player->account < current_block->price){
		printf("You can't buy this property because you can't afford it\n");
	}
	else if(current_block->owner.type==noone){
		printf("You bought this property\n");
		current_block->owner.type = current_player->type;
		current_player->account -= current_block->price;
		for(i=0;i<ID_BLOCK;i++){
			if(current_player->owned_block_ids[i]==-1){
				current_player->owned_block_ids[i]=current_block->block_id;
				break;
			}
		}
	}
	else{
		printf("You can't buy this property.\n");
	}
	printf("\n");
}
/*This function shows the properties of all properties in the game*/
void show_properties(struct block board[LEN_BOARD]){
	int i;
	int num;
	char space=' ';
	printf("Please select a property to see details:\n");
	for(i=1;i<LEN_BOARD;i++){
		if(i<LEN_BOARD/2){
			if(board[i].price!=0) printf("%d -    %s\n",i,board[i].name);	
		}
		else if(board[i].price!=0) printf("%d -   %s\n",i,board[i].name);	
	}
	printf("0  -   Exit\n");
	do{
		scanf("%d",&num);
	}while(num==2 || num==5 || num==8 || num==15 || num==10 || num==12 || num==18 );
	
	if(num!=0){
		printf("\n\n\n-----------------------------\n");
		printf("|  %12s             |\n",board[num].name);
		printf("-----------------------------\n");
		printf("|     Rent           %5d$ |\n",board[num].rent);
		printf("|     Rent 1 H       %5d$ |\n",board[num].rent_1);
		printf("|     Rent 2 H       %5d$ |\n",board[num].rent_2);
		printf("|     Rent 3 H       %5d$ |\n",board[num].rent_3);
		printf("-----------------------------\n");
		printf("|     House Price    %5d$ |\n",board[num].house_price);
		printf("-----------------------------\n");
	}	
}
/*In this function our game board is shown on the screen*/
void show_board(struct block board[LEN_BOARD],struct player player_one, struct player player_two){
	int i,j=0;
	char space=' ';
	printf("-------------------------------------------------------------------------------------------------------\n");
	while(j<3){
		for(i=0;i<6;i++){
			if(j==0)	printf("|%12s    ",board[i].name );
			if(j==1){
				if(board[i].price!=0) printf("|%12d$   ",board[i].price);
				else if(board[i].rent>2) printf("|%12d$   ",board[i].rent);
				else printf("|%12c    ",space);
			}
			if(j==2){
				if(player_one.current_block_id==player_two.current_block_id &&	player_one.current_block_id==board[i].block_id) {
						printf("|%6s %6s   ",player_one.name,player_two.name );
					}
				else{
					if(player_one.current_block_id==i )	printf("|%12s    ",player_one.name );
					else if(player_two.current_block_id==i )	printf("|%12s    ",player_two.name );
					else printf("|%12c    ",space);
				}
			}
		}
		if(j==0)	printf("|\n");
		if(j==1)	printf("|\n");
		if(j==2)	printf("|\n");
		j++;
	}
	printf("-------------------------------------------------------------------------------------------------------\n");
	j=6;
	for(i=19;i>15;i--){
		printf("|%12s    |",board[i].name );	
		printf("\t\t\t\t\t\t\t\t     " );
		printf("|%12s    |\n",board[j].name );
		if(board[i].price!=0) printf("|%12d$   |",board[i].price);
		else if(board[i].rent!=0) printf("|%12d$   |",board[i].rent);
		else printf("|%12c    ",space);
		printf("\t\t\t\t\t\t\t\t     " );
		if(board[j].price!=0) printf("|%12d$   |\n",board[j].price);
		else if(board[j].rent!=0) printf("|%12d$   |\n",board[j].rent);
		else printf("|%12c    |\n",space);
			
		if(player_one.current_block_id==player_two.current_block_id &&	player_one.current_block_id==board[i].block_id) {
				printf("|%6s %6s   |",player_one.name,player_two.name );
			}
		else{
			if(player_one.current_block_id==i )	printf("|%12s    |",player_one.name );
			else if(player_two.current_block_id==i )	printf("|%12s    |",player_two.name );
			else printf("|%12c    |",space);
		}
		printf("\t\t\t\t\t\t\t\t     " );

		if(player_one.current_block_id==player_two.current_block_id &&	player_one.current_block_id==board[j].block_id) {
				printf("|%6s %6s   |",player_one.name,player_two.name );
			}
		else{
			if(player_one.current_block_id==j )	printf("|%12s    |",player_one.name );
			else if(player_two.current_block_id==j )	printf("|%12s    |",player_two.name );
			else printf("|%12c    |",space);
		}		
		printf("\n");

		if(i==16)printf("-------------------------------------------------------------------------------------------------------\n");
		else 
		{	
			printf("------------------");
			printf("\t\t\t\t\t\t\t\t     " );
			printf("------------------\n");}
		j++;
	}
	j=0;
	while(j<3){
		for(i=15;i>9;i--){
			if(j==0)	printf("|%12s    ",board[i].name );
			if(j==1){
				if(board[i].price!=0) printf("|%12d$   ",board[i].price);
				else if(board[i].rent>2) printf("|%12d$   ",board[i].rent);
				else printf("|%12c    ",space);
			}
			if(j==2){
				if(player_one.current_block_id==player_two.current_block_id &&	player_one.current_block_id==board[i].block_id) {
						printf("|%6s %6s   ",player_one.name,player_two.name );
					}
				else{
					if(player_one.current_block_id==i )	printf("|%12s    ",player_one.name );
					else if(player_two.current_block_id==i )	printf("|%12s    ",player_two.name );
					else printf("|%12c    ",space);
				}
			}
		}
		if(j==0)	printf("|\n");
		if(j==1)	printf("|\n");
		if(j==2)	printf("|\n");
		j++;
	}
	printf("-------------------------------------------------------------------------------------------------------\n");
	printf("\n");
}
/*In this function our game board is filled*/
void init_the_board(struct block board[LEN_BOARD]){
	board[0].block_id=0;
	board[0].name="Start";
	board[0].price=0;
	board[0].rent=0;
	board[0].rent_1=0;
	board[0].rent_2=0;
	board[0].rent_3=0;
	board[0].house_price=0;
	board[0].type=start;
	board[0].owner.type=noone;
	board[0].house_count=0;

	board[1].block_id=1;
	board[1].name="Esenyurt";
	board[1].price=16000;
	board[1].rent=800;
	board[1].rent_1=4000;
	board[1].rent_2=9000;
	board[1].rent_3=25000;
	board[1].house_price=10000;
	board[1].type=property;
	board[1].owner.type=noone;
	board[1].house_count=0;

	board[2].block_id=2;
	board[2].name="Car Park";
	board[2].price=0;
	board[2].rent=1500;
	board[2].rent_1=0;
	board[2].rent_2=0;
	board[2].rent_3=0;
	board[2].house_price=0;
	board[2].type=tax;
	board[2].owner.type=noone;
	board[2].house_count=0;

	board[3].block_id=3;
	board[3].name="Tuzla";
	board[3].price=16500;
	board[3].rent=850;
	board[3].rent_1=4250;
	board[3].rent_2=9500;
	board[3].rent_3=26000;
	board[3].house_price=12000;
	board[3].type=property;
	board[3].owner.type=noone;
	board[3].house_count=0;

	board[4].block_id=4;
	board[4].name="Arnavutkoy";
	board[4].price=17000;
	board[4].rent=875;
	board[4].rent_1=4500;
	board[4].rent_2=10000;
	board[4].rent_3=28000;
	board[4].house_price=12000;
	board[4].type=property;
	board[4].owner.type=noone;
	board[4].house_count=0;

	board[5].block_id=5;
	board[5].name="Wait 2 Turn";
	board[5].price=0;
	board[5].rent=2;
	board[5].rent_1=0;
	board[5].rent_2=0;
	board[5].rent_3=0;
	board[5].house_price=0;
	board[5].type=punish;
	board[5].owner.type=noone;
	board[5].house_count=0;

	board[6].block_id=6;
	board[6].name="Catalca";
	board[6].price=20000;
	board[6].rent=1000;
	board[6].rent_1=5000;
	board[6].rent_2=12000;
	board[6].rent_3=30000;
	board[6].house_price=13000;
	board[6].type=property;
	board[6].owner.type=noone;
	board[6].house_count=0;

	board[7].block_id=7;
	board[7].name="Beykoz";
	board[7].price=23000;
	board[7].rent=1100;
	board[7].rent_1=5500;
	board[7].rent_2=12500;
	board[7].rent_3=33000;
	board[7].house_price=13000;
	board[7].type=property;
	board[7].owner.type=noone;
	board[7].house_count=0;

	board[8].block_id=8;
	board[8].name="Car fix";
	board[8].price=0;
	board[8].rent=1750;
	board[8].rent_1=0;
	board[8].rent_2=0;
	board[8].rent_3=0;
	board[8].house_price=0;
	board[8].type=tax;
	board[8].owner.type=noone;
	board[8].house_count=0;

	board[9].block_id=9;
	board[9].name="Maltepe";
	board[9].price=30000;
	board[9].rent=1350;
	board[9].rent_1=7000;
	board[9].rent_2=15000;
	board[9].rent_3=40000;
	board[9].house_price=15000;
	board[9].type=property;
	board[9].owner.type=noone;
	board[9].house_count=0;

	board[10].block_id=10;
	board[10].name="Bills";
	board[10].price=0;
	board[10].rent=2000;
	board[10].rent_1=0;
	board[10].rent_2=0;
	board[10].rent_3=0;
	board[10].house_price=0;
	board[10].type=tax;
	board[10].owner.type=noone;
	board[10].house_count=0;

	board[11].block_id=11;
	board[11].name="Sisli";
	board[11].price=33000;
	board[11].rent=1500;
	board[11].rent_1=8000;
	board[11].rent_2=16000;
	board[11].rent_3=42000;
	board[11].house_price=16000;
	board[11].type=property;
	board[11].owner.type=noone;
	board[11].house_count=0;

	board[12].block_id=12;
	board[12].name="Oil";
	board[12].price=0;
	board[12].rent=2250;
	board[12].rent_1=0;
	board[12].rent_2=0;
	board[12].rent_3=0;
	board[12].house_price=0;
	board[12].type=tax;
	board[12].owner.type=noone;
	board[12].house_count=0;

	board[13].block_id=13;
	board[13].name="Atasehir";
	board[13].price=35000;
	board[13].rent=1600;
	board[13].rent_1=8500;
	board[13].rent_2=17000;
	board[13].rent_3=45000;
	board[13].house_price=17000;
	board[13].type=property;
	board[13].owner.type=noone;
	board[13].house_count=0;

	board[14].block_id=14;
	board[14].name="Sariyer";
	board[14].price=40000;
	board[14].rent=1750;
	board[14].rent_1=9500;
	board[14].rent_2=19000;
	board[14].rent_3=48000;
	board[14].house_price=19000;
	board[14].type=property;
	board[14].owner.type=noone;
	board[14].house_count=0;

	board[15].block_id=15;
	board[15].name="Wait 1 Turn";
	board[15].price=0;
	board[15].rent=1;
	board[15].rent_1=0;
	board[15].rent_2=0;
	board[15].rent_3=0;
	board[15].house_price=0;
	board[15].type=punish;
	board[15].owner.type=noone;
	board[15].house_count=0;

	board[16].block_id=16;
	board[16].name="Kadikoy";
	board[16].price=43000;
	board[16].rent=1900;
	board[16].rent_1=11000;
	board[16].rent_2=21500;
	board[16].rent_3=55000;
	board[16].house_price=23000;
	board[16].type=property;
	board[16].owner.type=noone;
	board[16].house_count=0;

	board[17].block_id=17;
	board[17].name="Besiktas";
	board[17].price=60000;
	board[17].rent=2500;
	board[17].rent_1=15000;
	board[17].rent_2=28000;
	board[17].rent_3=60000;
	board[17].house_price=30000;
	board[17].type=property;
	board[17].owner.type=noone;
	board[17].house_count=0;

	board[18].block_id=18;
	board[18].name="Vocation";
	board[18].price=0;
	board[18].rent=5000;
	board[18].rent_1=0;
	board[18].rent_2=0;
	board[18].rent_3=0;
	board[18].house_price=0;
	board[18].type=tax;
	board[18].owner.type=noone;
	board[18].house_count=0;

	board[19].block_id=19;
	board[19].name="Bebek";
	board[19].price=70000;
	board[19].rent=3500;
	board[19].rent_1=20000;
	board[19].rent_2=35500;
	board[19].rent_3=65000;
	board[19].house_price=35000;
	board[19].type=property;
	board[19].owner.type=noone;
	board[19].house_count=0;

}
void main(){
	srand(time(NULL));
	struct block board[LEN_BOARD];
	struct player player_one,player_two;
	int i;
	player_one.type=car;
	player_one.current_block_id=0;
	player_one.name="car";
	for(i=0;i<ID_BLOCK;i++) player_one.owned_block_ids[i]=-1;
	player_one.account=MONEY_START;
	player_one.turn_to_wait=0;
	
	player_two.type=cap;
	player_two.current_block_id=0;
	player_two.name="cap";
	for(i=0;i<ID_BLOCK;i++) player_two.owned_block_ids[i]=-1;
	player_two.account=MONEY_START;
	player_two.turn_to_wait=0;

	init_the_board(board);
	gameplay(board,player_one,player_two);
}