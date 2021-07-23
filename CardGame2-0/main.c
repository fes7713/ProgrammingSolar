#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <windows.h>
#define CARD 52
#define FACE 13
#define SUIT 4
#define HAND 2
#define TBL 5
#define TBLHD 7
#define PLAYER 6
#define CALL 300
#define HMN 1
#define FACE_EX 14
#define MULTI 0
#define BOX 5
typedef struct{
    char * face;
    char * suit;
    int num;
} Card;

typedef struct{
    int hand;
    int ref_1;
    int ref_2;
    int ref_3;
} Strenght;

typedef struct{
    Strenght Hand_id;
    Card Hands[HAND];
    Card Tb_Hd_Hands [TBLHD];
    int coin;
    int bet;
    int Error;
    int status;
    int Total_bet;
    char name[20];
    float prob_win;
} Player;

typedef struct{
    Player players[PLAYER];
    Card TB_Hands[TBL];
    int status[PLAYER];
    int call;
    int raise;
    int stage;
    int nCall;
    int pot;
    int min_bet;
    int lead;
    int nPlayer_Pos;
    int area;
    int info;
    int range_Ai;
    int shift_Ai;
    int AiIQ;
    int Click_Overlay;
} Game;

typedef struct{
    int Start_X;
    int Start_Y;
    int Lenght_X;
    int Lenght_Y;
} Box;

const char Hands_Rank[10][25] = {"Royal        Flush", 
                            "Straight      Flush", 
                            "Four   of   a   Kind", 
                            "Full      House", 
                            "Flush", 
                            "Straight", 
                            "Three   of   a   Kind", 
                            "Two      Pair", 
                            "One      Pair", 
                            "No      Pair"};

const char Hands_Rank_Ext[10][25] = {"Royal Flush", 
                            "Straight Flush", 
                            "Four of a Kind", 
                            "Full House", 
                            "Flush", 
                            "Straight", 
                            "Three of a Kind", 
                            "Two Pair", 
                            "One Pair", 
                            "No Pair"};

const char * wFace[FACE] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
const char * wSuit[SUIT] = {"Spade", "Heart", "Clover", "Diamond"};
const char status_Stack[BOX][20] = {"check", "call", "raise", "fold", "No Status"}; 
int odds[10] = {10000, 1000, 250, 75, 50, 25, 5, 2, 1, 0};
float probability[10] = {0, 0.000015, 0.00024, 0.0014, 0.002, 0.0039, 0.021, 0.0475, 0.4225, 0.5};
float probability_Ext[10] = {0.0031, 0.0311, 0.168, 2.60, 3.03, 4.62, 4.83, 23.5, 43.8, 17.8};
int odds_Ext[10] = {2000, 200, 50, 25, 10, 5, 1, 0, 0, 0};
 
void fillDeck(Card * wDeck, char * wFace[], char * wSuit[]);
void fillDeck_hand(Card * wDeck, char * wFace[], char * wSuit[], int *Hand_set, int size);
void shuffleDeck(Card * wDeck);
void deal(Card * wDeck);
int range(int num);
void Map_Import(int tableHeight, int tableWidth, char(*table)[tableWidth], char* fname);
void nTable_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int DrawFromX, int DrawFromY, Card *Hand, int size, int Tb_Open, int letter_size);
void Letter_Draw_Normal(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int DrawFromX, int DrawFromY, char* string, int letter_size);
void printArray3(int fieldHeight, int fieldWidth, char field[fieldHeight][fieldWidth]);
void Chng_Crds(Card * wDeck, Card *Hand, int *Draw_Start, bool *Chng_Pos, int size);
void Cpy_Hnds(Card *wDeck, Card *Hands, int size, int *pos);
Strenght Hnds_Idntfy(Card *Hands, int size);
void S_C_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], bool *Chng_Pos, int size, int DrawFromX, int DrawFromY, int letter_size, int Card_DesignWidth);
int Control(bool *Chng_Pos, int size);
int Betting(Game *game, int player_id, int multi, char c);
void Odds_Prob_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int bet, int DrawFromX, int DrawFromY, int letter_size);
void Pay_Bet(int *money, int *pot, int *Total_bet);
void Get_Award(int *money, int *bet, int Hand_id);
void Get_Award_Ext(int *money, int bet, int Hand_id, int *odds);
void Get_Pot(int *money, int *pot);
void Mny_Bt_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int money, int bet, int earned, int call, int *Error, int DrawFromX, int DrawFromY, int letter_size);
Strenght HandIni(int Hand_id, int ref_1, int ref_2, int ref_3);
int Ext_Array(int *From_arr, int ref_1, int ref_2, int ref_3, int size);
void Drawer(int h_size, int nPlayer, int (*posHand)[h_size], int *Draw_Start);
void Hand_cat(Card *Hands, Card *cat_1, Card *cat_2, int cat_1_size, int cat_2_size);
bool Status_Same(int *status, int size, int excluding);
int Hand_Cmp(Player * players, int size, int h_size);
void saveGameData(Game *game, char* fname, int *key, int *Next);
int loadGameData(Game *game, char* fname, int *key, int *Next);
void BG_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int DrawFromX, int DrawFromY, int letter_size, int h_size, int Draw_Height);
void shuffleDeck_From(Card * wDeck, int size_excld);
void fillDeck_From(Card * wDeck, char * wFace[], char * wSuit[], int *Hand_Pos, int size_hand, int size_table, int size_excld);
bool Excld_Hand(int input, int *excld_Hands, int size_excld);
float Prob_Cal(Card *Hands, int size_player, int size_hand, int size_table, int size_excld, int *Hand_Counter, int precision);
void Odds_Prob_Draw_float(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], float *data, float *data_Ext, int DrawFromX, int DrawFromY, int letter_size);
int Status_count(Player *players, int size, int status_number);
char kb_m_Input(Box *Clicks, int *x, int *y, int fieldWidth, char (*field)[fieldWidth], int fieldWidth_display, int displayWidth, int displayHeight_Shift, int displayWidth_Shift, int *cnt, int *pre_cnt, int size_box);
bool Click_Box(int m_x, int m_y, int m_x_Sht, int m_y_Sht, int Start_X, int Start_Y, int Lenght_X, int Lenght_Y, float Coe);
Box Box_Init(int Start_X, int Start_Y, int Lenght_X, int Lenght_Y);
void Get_Mouse_Pos(int *x, int *y);
void Box_Draw(Box *Clicks, int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int size_box, int letter_size);

int main(int argc, char **argv)
{
    Card deck[CARD];
    Box Clicks[BOX];
    Box Clicks_Show[BOX];
    
    char Card_Design[3620][101] = {};
    int Card_DesignHeight = 3620;
    int Card_DesignWidth = 101;
    int Card_Height = 67;
    char field[160][1400] = {};
    int fieldHeight = 160;
    int fieldWidth = 1400;
    int fieldHeight_display = 163;
    int fieldWidth_display = 634;
    int displayHeight = 780;
    int displayHeight_Shitf = 25;
    int displayWidth = 1520;
    static char fontData[500][50];
    int fontDataHeight = 500;
    int fontDataWidth = 50;
    static char fontData_Ext[500][50];
    int fontDataHeight_Ext = 500;
    int fontDataWidth_Ext = 50;
    int font_size = 6;
    char print_field[50] = {};
    int player_id = 0;
    int multi = MULTI;
    int key = 0;
    char fname[50] = "PokerData.dat";
    int non_Next = 0;
    int Next = 1;
    int counter_temp = 0;
    int counter = 0;
    int hit_counter = 0;
    int x;
    int y;
    int cnt = 0;
    int pre_cnt = 0;
    int posHand[PLAYER][HAND];
    int posTable[TBL];
    int Draw_Start = 0;
    char names[PLAYER][50] = {"John", "Tom"};
    bool Chng_Pos[HAND] = {false, false, false, false, false};
    int Hand_Counter[PLAYER][10] = {};
    float prob_Hand[PLAYER][10] = {};
    Game Poker;
    
    for(int i = 0; i < PLAYER; i++)
        Poker.players[i].coin = 20000;
    Poker.min_bet = 100;
    Poker.pot = 0;

    srand(time(NULL));
    Map_Import(Card_DesignHeight, Card_DesignWidth, Card_Design, "Cards Full Fixed Ace.txt");
    Map_Import(fontDataHeight, fontDataWidth, fontData, "font Ivrit Space1 Ext_40.txt");
    Map_Import(fontDataHeight_Ext, fontDataWidth_Ext, fontData_Ext, "font Ivrit Ext_40.txt");
    
    for(int i = 0; i < BOX; i++)
    {
        Clicks_Show[i] = Box_Init(i * 80, 20, 40, 20);
        Clicks[i] = Box_Init((i % 2) * fieldWidth_display / 2, (i / 2) * fieldHeight_display / 2, fieldWidth_display / 2, fieldHeight_display / 2);
    }
    
    fillDeck(deck, wFace, wSuit);
    shuffleDeck(deck);
    deal(deck);
    
    Drawer(HAND, PLAYER, posHand, &Draw_Start);
    Drawer(TBL, 1, posTable, &Draw_Start);
    
    if(multi == 1){
        printf("Player_id\n");
        scanf("%d", &player_id);
    }

    for(int i = 0; i < PLAYER; i++)
        Cpy_Hnds(deck, Poker.players[i].Hands, HAND, posHand[i]);
    Cpy_Hnds(deck, Poker.TB_Hands, TBL, posTable);
    
    for(int i = 0; i < PLAYER; i++)
        Hand_cat(Poker.players[i].Tb_Hd_Hands, Poker.players[i].Hands, Poker.TB_Hands, HAND, 0);
    Player *self = &Poker.players[player_id];
    
    Poker.info = -1;
    Poker.AiIQ = 100;
    while(1){
        self->bet = 0;
        for(int i = 0; i < PLAYER; i++){
            strcpy(&Poker.players[i].name, names[i]);
            Poker.players[i].bet = 0;
            if(Poker.players[i].coin < Poker.min_bet)
                Poker.players[i].Error = 5;
            else
                Poker.players[i].Error = 0;
            Poker.players[i].Total_bet = 0;
            Poker.status[i] = -1;
        }

        Poker.call = 0;
            Poker.lead = -1;
        Poker.nPlayer_Pos = 0;
        Poker.area = 1;
        Poker.range_Ai = 1;
        Poker.shift_Ai = 5;
//        Poker.info = -1;
//Betting time
//Online saving
        if(multi == 1 && player_id == 0){
            key = rand();
            Poker.area = 1;
            saveGameData(&Poker, fname, &key, &non_Next);
        }
        while(1){
            memset(field, 0, fieldWidth * fieldHeight);
            if(TBL == 0)
                Odds_Prob_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, self->bet, 0, Card_Height + font_size*4 + 2, font_size);
            for(int i = 0; i < PLAYER; i++){
                    memset(print_field, 0, 50);
                    sprintf(print_field, "Player %d  %d Bet %d", i, Poker.players[i].coin, Poker.players[i].Total_bet);
                    Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * i, Card_Height + font_size  + 2, print_field, font_size);
                    Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, -1, Poker.players[i].bet, -1, Poker.call, &Poker.players[i].Error, Card_DesignWidth * 2 * i, Card_Height + font_size*2 + 2, font_size);
            }
            
            if(player_id == Poker.nPlayer_Pos && TBL > 0)
                BG_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, Card_DesignWidth * 2 * Poker.nPlayer_Pos, Card_Height + 2 + font_size / 3, Card_Height, HAND, font_size / 3);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * Poker.nPlayer_Pos + Card_DesignWidth * HAND / 2 - font_size / 2 * 6, Card_Height + 2, "Play", font_size);
//Stage & pot
            memset(print_field, 0, 50);
            sprintf(print_field, "Stage %d Pot %d", Poker.stage,  Poker.pot);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, 0, print_field, font_size);
            memset(print_field, 0, 50);
            sprintf(print_field, "Player_Pos %d", Poker.nPlayer_Pos);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size, print_field, font_size);
            memset(print_field, 0, 50);
            sprintf(print_field, "Lead %d", Poker.lead);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size * 2, print_field, font_size);
            memset(print_field, 0, 50);
            sprintf(print_field, "Minimum_bet %d", Poker.min_bet);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size * 3, print_field, font_size);
            system("cls");
//            deal(deck);
            
            printArray3(fieldHeight, fieldWidth, field);
            Player * player_sh = &Poker.players[Poker.nPlayer_Pos];
//Online loading
            if(multi == 1 && player_id == 1){
                int temp = Poker.stage;
                while(1)
                {
                    int temp = 0;
                    int area = 1;
                    if(loadGameData(&Poker, fname, &key, &temp) == 1)
                        break;
                    Sleep(50);
                }
                if(Poker.area != 1){
                    key = 0;
                    break;
                }
                continue;
            }
            
//            if((player_sh->Error = Betting(&Poker, player_id, multi == 0 ? -1 : Poker.nPlayer_Pos, Poker.nPlayer_Pos < HMN ? 0 : 'c')) >= 0){
            
            if((player_sh->Error = Betting(&Poker, player_id, multi == 0 ? -1 : Poker.nPlayer_Pos, 'c')) >= 0){
                    
                for(int i = 0; i < PLAYER; i++)
                    if(Poker.players[i].Error == 5)
                        Poker.status[i] = Poker.lead;
                        
                int temp = Poker.nPlayer_Pos;
                Poker.nPlayer_Pos++;
                Poker.nPlayer_Pos = Poker.nPlayer_Pos % PLAYER;
                while(Poker.players[Poker.nPlayer_Pos].Error == 5){
                    Poker.nPlayer_Pos++;
                    Poker.nPlayer_Pos = Poker.nPlayer_Pos % PLAYER;
                }
                if(Poker.nPlayer_Pos == temp)
                    break;
            }
            if(Poker.lead != -1 && Status_Same(Poker.status, PLAYER, 5) == true)  
                break; 
//Online saving
            if(multi == 1 && player_id == 0){
                key = rand();
                saveGameData(&Poker, fname, &key, &non_Next);
            }
        }
        for(int i = 0; i < PLAYER; i++)
            Pay_Bet(&Poker.players[i].coin, &Poker.pot, &Poker.players[i].Total_bet);
            
        for(int i = 0; i < HAND; i++)
            Chng_Pos[i] = false;
//Online saving
        if(multi == 1 && player_id == 0){
            key = rand();
            Poker.area = 2;
            saveGameData(&Poker, fname, &key, &Next);
        }
//Change Cards
        while(Poker.stage < TBL || (counter_temp--) == 0)
        {
            for(int i = 0; i < PLAYER; i++){
                if(Poker.players[i].Error == 5)
                    continue;
                Poker.players[i].Error = 0;
                Poker.players[i].bet = 0;
                Poker.players[i].Total_bet = 0;
                Poker.status[i] = -1;
            }
            Poker.lead = -1;
            Poker.call = 0;

            while(1){
                self->Hand_id = Hnds_Idntfy(self->Tb_Hd_Hands, HAND + Poker.stage);
//Online saving
                if(multi == 1 && player_id == 0){
                    key = rand();
                    saveGameData(&Poker, fname, &key, &non_Next);
                }
                memset(field, 0, fieldWidth * fieldHeight);
//Table Card Drawing
                if(TBL > 0){
                    nTable_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, 0, 0, Poker.TB_Hands, TBL, Poker.stage, Card_Height);
                    for(int i = 0; i < PLAYER; i++)
                        nTable_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, Card_DesignWidth * 2 * i, Card_Height + font_size * 4 + 2, Poker.players[i].Hands, HAND, player_id == i ? 2 : 0, Card_Height);
                    if(player_id == Poker.nPlayer_Pos)
                        BG_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, Card_DesignWidth * 2 * Poker.nPlayer_Pos, Card_Height + 2 + font_size / 3, Card_Height, HAND, font_size / 3);
                    Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * Poker.nPlayer_Pos + Card_DesignWidth * HAND / 2 - font_size / 2 * 6, Card_Height + 2, "Play", font_size);
                }
                else
                    nTable_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, 0, 0, Poker.players[player_id].Hands, HAND, HAND, Card_Height);
                
                for(int i = 0; i < PLAYER; i++){
                    memset(print_field, 0, 50);
//Players status & info
                    sprintf(print_field, "Player %d  %d Bet %d", i, Poker.players[i].coin, Poker.players[i].Total_bet);
                    Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * i, Card_Height + font_size  + 2, print_field, font_size);
                    memset(print_field, 0, 50);
                    sprintf(print_field, "%d", Poker.players[i].coin);
                    Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size * (i + 6), print_field, font_size);
                    Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, -1, Poker.players[i].bet, -2, Poker.call, &Poker.players[i].Error, Card_DesignWidth * 5.5, font_size * (i + 6), font_size);
                    Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, -1, Poker.players[i].bet, -1, Poker.call, &Poker.players[i].Error, Card_DesignWidth * 2 * i, Card_Height + font_size*2 + 2, font_size);
                }
//Ai win prob cal
                for(int i = 0; i < PLAYER; i++)
                    Poker.players[i].prob_win = Prob_Cal(Poker.players[i].Tb_Hd_Hands, PLAYER - Status_count(Poker.players, PLAYER, 5), HAND, TBL, HAND + Poker.stage, Hand_Counter[i], Poker.AiIQ);
//Stay or Change
//                if(TBL == 0 || Poker.info == player_id || player_id == Poker.Click_Overlay){
                if(TBL == 0 || Poker.info == player_id){
                    if(TBL == 0){
                        Odds_Prob_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Poker.info != -1 ? Poker.min_bet :  Poker.pot, Poker.info != -1 ? Card_DesignWidth * 2 * ((player_id + 1) % PLAYER) : 0, Card_Height + font_size*4 + 2, font_size);
                        S_C_Draw(fontDataWidth, fontData, fieldWidth, field, Chng_Pos, HAND, 0, Card_Height + 1, font_size, Card_DesignWidth);
                    }
//                    else if(player_id == Poker.Click_Overlay)
//                    {
//                        Box_Draw(Clicks, fontDataWidth, fontData, fieldWidth, field, BOX, font_size);
//                    }
                    else{
                        float prob;
                        if(player_id == Poker.nPlayer_Pos)
                            prob = Prob_Cal(Poker.players[player_id].Tb_Hd_Hands, PLAYER - Status_count(Poker.players, PLAYER, 5), HAND, TBL, HAND + Poker.stage, Hand_Counter[player_id], 100000);
                        memset(print_field, 0, 50);
                        sprintf(print_field, "Winning Rate %.5f (%.2f)", prob, (float)100 / (PLAYER - Status_count(Poker.players, PLAYER, 5)));
                        Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * ((player_id + 1) % PLAYER), Card_Height + font_size*3 + 2, print_field, font_size);
                        for(int i = 0; i < 10; i++)
                            prob_Hand[player_id][i] = (float)Hand_Counter[player_id][i] / 1000;
                        Odds_Prob_Draw_float(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, prob_Hand[player_id], probability_Ext, Card_DesignWidth * 2 * ((player_id + 1) % PLAYER), Card_Height + font_size*4 + 2, font_size);
                    }
                    
                }
//Hands Rank
                Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * player_id, Card_Height + font_size * 3 + 2, Hands_Rank_Ext[self->Hand_id.hand], font_size);
                
//Stage & Pot
                memset(print_field, 0, 50);
                sprintf(print_field, "Stage %d Pot %d", Poker.stage,  Poker.pot);
                Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, 0, print_field, font_size);
                memset(print_field, 0, 50);
                sprintf(print_field, "Minimum_bet %d", Poker.min_bet);
                Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size, print_field, font_size);
                system("cls");
//                deal(deck);
                printArray3(fieldHeight, fieldWidth, field);
                
//////Draw Poker Control
                if(TBL == 0){
                    if(Control(Chng_Pos, HAND) == 1)
                        break;
                }
//Texus Hodem
                else{
                    
                    Player * player_sh = &Poker.players[Poker.nPlayer_Pos];
//Online loading
                    if(multi == 1 && player_id == 1){
                        int temp = 0;
                        while(1)
                        {
                            int temp = 0;
                            int area = 2;
                            if(loadGameData(&Poker, fname, &key, &temp) == 1)
                                break;
                            Sleep(50);
                        }
                        if(Poker.area != 2){
                            key = 0;
                            break;
                        }
                        continue;
                    }
                    
                    if((player_sh->Error = Betting(&Poker, player_id, multi == 0 ? -1 : Poker.nPlayer_Pos, 
                                    Status_count(Poker.players, PLAYER, 5) == PLAYER - 1 ? 'c' :
                                    Poker.nPlayer_Pos < HMN ? 
                                    kb_m_Input(Clicks, &x, &y, fieldWidth, field, fieldWidth_display, displayWidth, displayHeight_Shitf, 0, &cnt, &pre_cnt, BOX) :  
                                                                    player_sh->prob_win < (float)100 / (PLAYER + Poker.range_Ai) + Poker.shift_Ai ? 'f' : 
                                                                    player_sh->prob_win > (float)100 / (PLAYER + Poker.range_Ai) * (PLAYER + Poker.range_Ai - 1) + Poker.shift_Ai ? Poker.call < Poker.min_bet * pow(2, 3) 
                                                                                                                                                                                  ? 'r' : 'c'
                                                                                                                                                                                  : 'c')) >= 0)
                    {
                        for(int i = 0; i < PLAYER; i++)
                            if(Poker.players[i].Error == 5)
                                Poker.status[i] = Poker.lead;
                                
                        int temp = Poker.nPlayer_Pos;
                        Poker.nPlayer_Pos++;
                        Poker.nPlayer_Pos = Poker.nPlayer_Pos % PLAYER;
                        while(Poker.players[Poker.nPlayer_Pos].Error == 5){
                            Poker.nPlayer_Pos++;
                            Poker.nPlayer_Pos = Poker.nPlayer_Pos % PLAYER;
                        }
                        if(Poker.nPlayer_Pos == temp)
                            break;
                    }
                    if(Poker.lead != -1 && Status_Same(Poker.status, PLAYER, 5) == true)  
                        break;
//Online saving
                    if(multi == 1 && player_id == 0){
                        key = rand();
                        saveGameData(&Poker, fname, &key, &non_Next);
                    }
                }
//Here online uplink
                    
            }
            if(Poker.stage == 0 && TBL > 0)
                Poker.stage = 3;
            else
                if(Poker.stage < 5 && TBL > 0)
                    Poker.stage++;
            for(int i = 0; i < PLAYER; i++)
                Hand_cat(Poker.players[i].Tb_Hd_Hands, Poker.players[i].Hands, Poker.TB_Hands, HAND, Poker.stage);
            Poker.call = CALL;
            for(int i = 0; i < PLAYER; i++)
                Pay_Bet(&Poker.players[i].coin, &Poker.pot, &Poker.players[i].Total_bet);
        }
        
        Chng_Crds(deck, self->Hands, &Draw_Start, Chng_Pos, HAND);
        for(int i = 0; i < PLAYER; i++)
            Poker.players[i].Hand_id = Hnds_Idntfy(Poker.players[i].Tb_Hd_Hands, HAND + Poker.stage);
       
        int won = Hand_Cmp(Poker.players, PLAYER, HAND);
//Online saving
        if(multi == 1 && player_id == 0){
            key = rand();
            Poker.area = 3;
            saveGameData(&Poker, fname, &key, &Next);
        }

            memset(field, 0, fieldWidth * fieldHeight);
            if(TBL != 0){
                nTable_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, 0, 0, Poker.TB_Hands, TBL, Poker.stage, Card_Height);
                for(int i = 0; i < PLAYER; i++)
                    nTable_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, Card_DesignWidth * 2 * i, Card_Height + font_size * 4 + 2, Poker.players[i].Hands, HAND, 2, Card_Height);
            }
            else
                nTable_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, 0, 0, Poker.players[player_id].Hands, HAND, HAND, Card_Height);
            for(int i = 0; i < PLAYER; i++){
                memset(print_field, 0, 50);
                sprintf(print_field, "Player %d  %d Bet %d", i, Poker.players[i].coin, Poker.players[i].Total_bet);
                Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * i, Card_Height + font_size  + 2, print_field, font_size);
//                   Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, -1, Poker.players[i].bet, -2, Poker.call, &Poker.players[i].Error, Card_DesignWidth * 5, font_size + font_size * 2 * (i + 1), font_size);
                Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, -1, Poker.players[i].bet, -1, Poker.call, &Poker.players[i].Error, Card_DesignWidth * 2 * i, Card_Height + font_size*2 + 2, font_size);
                Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, -1, Poker.players[i].bet, -2, Poker.call, &Poker.players[i].Error, Card_DesignWidth * 5.8, font_size * (i + 6), font_size);
                Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size * (i + 6), Hands_Rank_Ext[Poker.players[i].Hand_id.hand], font_size);
                Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * i, Card_Height + font_size * 3 + 2, Hands_Rank_Ext[Poker.players[i].Hand_id.hand], font_size);
            }  

//Stage & Pot
            memset(print_field, 0, 50);
            sprintf(print_field, "Stage %d Pot %d", Poker.stage,  Poker.pot);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, 0, print_field, font_size);
            memset(print_field, 0, 50);
            sprintf(print_field, "Player_Pos %d", Poker.nPlayer_Pos);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size * 2, print_field, font_size);
            memset(print_field, 0, 50);
            sprintf(print_field, "Player Won %d", won);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 5, font_size * 3, print_field, font_size);
//Info Display
        if(TBL == 0){
            Odds_Prob_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Poker.pot, 0, Card_Height + font_size*3 + 2, font_size);
            Mny_Bt_Draw(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, self->coin, Poker.pot, Poker.pot * odds[self->Hand_id.hand], -1, &self->Error, 0, Card_Height + font_size*2 + 2, font_size);
        }
        else{
            if(player_id == won)
                BG_Draw(Card_DesignWidth, Card_Design, fieldWidth, field, Card_DesignWidth * 2 * won, Card_Height + 2 + font_size / 3, Card_Height, HAND, font_size / 3);
            Letter_Draw_Normal(fontDataWidth_Ext, fontData_Ext, fieldWidth, field, Card_DesignWidth * 2 * won + Card_DesignWidth * HAND / 2 - font_size / 2 * 6, Card_Height + 2, "Won", font_size);
        }

            system("cls");
            deal(deck);
            printArray3(fieldHeight, fieldWidth, field);
        
        counter++;
//Online saving
        if(multi == 1 && player_id == 0){
            key = rand();
            saveGameData(&Poker, fname, &key, &Next);
        }
            _getch();
        printf("*");
        if(TBL > 0){
            Get_Pot(&Poker.players[won].coin, &Poker.pot);
            for(int i = 0; i < PLAYER; i++)
                if(Poker.players[i].Error != 5)
                    Get_Award_Ext(&Poker.players[i].coin, Poker.min_bet, Poker.players[i].Hand_id.hand, odds_Ext);
        }
        else
            Get_Award(&Poker.players[player_id].coin, &Poker.pot, Poker.players[player_id].Hand_id.hand);
        won = Hand_Cmp(Poker.players, PLAYER, HAND);
        Poker.stage = 0;
        Poker.area = 1;
        shuffleDeck(deck);
        Draw_Start = 0;
        Drawer(HAND, PLAYER, posHand, &Draw_Start);
        Drawer(TBL, 1, posTable, &Draw_Start);
        for(int i = 0; i < PLAYER; i++)
            Cpy_Hnds(deck, Poker.players[i].Hands, HAND, posHand[i]);
        Cpy_Hnds(deck, Poker.TB_Hands, TBL, posTable);
        for(int i = 0; i < PLAYER; i++){
            Hand_cat(Poker.players[i].Tb_Hd_Hands, Poker.players[i].Hands, Poker.TB_Hands, HAND, TBL);
            Poker.players[i].Error = 0;
        }
        counter_temp = 0;
        Poker.min_bet += pow(10, floor(log10(Poker.min_bet)) - 1) * 2;
    }
    
	return 0;
}

void fillDeck(Card * wDeck, char * wFace[], char * wSuit[])
{
    for(int i = 0; i < CARD; i++)
    {
        wDeck[i].face = wFace[i % FACE];
        wDeck[i].suit = wSuit[i / FACE];
        wDeck[i].num = i;
    }
}

void fillDeck_hand(Card * wDeck, char * wFace[], char * wSuit[], int *Hand_set, int size)
{
    for(int i = 0; i < size; i++)
    {
        wDeck[i].face = wFace[Hand_set[i] % FACE];
        wDeck[i].suit = wSuit[Hand_set[i] / FACE];
        wDeck[i].num = Hand_set[i];
    }
}

void shuffleDeck(Card * wDeck)
{
    for(int i = 0; i < CARD; i++)
    {
        size_t j = rand() % CARD;
        Card temp = wDeck[i];
        wDeck[i] = wDeck[j];
        wDeck[j] = temp;
    }
}

void deal(Card * wDeck)
{
    for(int i = 0; i < CARD; i++)
    {
        printf("%5s of %-8s (%d)%s", wDeck[i].face, wDeck[i].suit, wDeck[i].num, (i + 1) % 4 ? " " : "\n");
    }
}

int range(int num)
{
    if(0 > num)
        return 0;
    return num;
}

void Map_Import(int tableHeight, int tableWidth, char(*table)[tableWidth], char* fname)
{
    FILE* cfPtr;
    if((cfPtr = fopen(fname, "r"))== NULL)
    {
        printf("File cannot be opened");
        return;
    }
    else{
        int p = 0;
        while(fgets(table[p], 1600, cfPtr) != NULL) {
            
            p++;
        }
        fclose(cfPtr);
    }
    for(int i = 0; i < tableHeight; i++)
        for(int j = 0; j < tableWidth; j++)
            if(table[i][j] == 10)
                table[i][j] = 32;
}

void nTable_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int DrawFromX, int DrawFromY, Card *Hand, int size, int Tb_Open, int letter_size)
{
    char string_field[1600];
    for(int i = 0; i < letter_size; i++) // 5 is letter hight
    {
        memset(string_field, 0, 1600);
        
        for(int j = 0; j < Tb_Open; j++)
            strcat(string_field, fontData[Hand[j].num * letter_size + i]);
        for(int j = 0; j < size - Tb_Open; j++)
                strcat(string_field, fontData[53 * letter_size + i]);
        strcpy(&field[DrawFromY + i][DrawFromX], string_field);
//        strcpy(&field[DrawFromY + i][DrawFromX], string_field);
    }
}

void Letter_Draw_Normal(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int DrawFromX, int DrawFromY, char* string, int letter_size)
{
    char string_field[800];
    for(int i = 0; string[i] != 0; i++)
        if(string[i] < 40 && string[i] != 32)
            string[i] = 32;

    for(int i = 0; i < letter_size; i++) // 5 is letter hight
    {
        memset(string_field, 0, 800);
        
        for(int j = 0; string[j] != 0; j++)
            strcat(string_field, fontData[(string[j] == 32 ? 83 : (string[j] - 40)) * letter_size + i]);
//        strcpy(&field[DrawFromY + i][DrawFromX], string_field);
        memcpy(&field[DrawFromY + i][DrawFromX], string_field, strlen(string_field));
    }
}

void printArray3(int fieldHeight, int fieldWidth, char field[fieldHeight][fieldWidth])
{
//    for(int i = 0; i < fieldHeight; i++)
//        field[i][fieldWidth - 1] = '\0';
    for(int i = 0; i < fieldHeight; i++)
        for(int j = 0; j < fieldWidth; j++){
            if(field[i][j] == 0 || field[i][j] == '\n')
                field[i][j] = ' ';
            if(j == fieldWidth - 1)
                field[i][j] = '\n';
        }
    field[fieldHeight - 1][fieldWidth - 1] = 0;
    printf("%s", field);
}

void Chng_Crds(Card * wDeck, Card *Hand, int *Draw_Start, bool *Chng_Pos, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(Chng_Pos[i] == false)
            continue;
        Hand[i] = wDeck[*Draw_Start];
        (*Draw_Start)++;
    }
}

void Cpy_Hnds(Card *wDeck, Card *Hands, int size, int *pos)
{
        for(int i = 0; i < size; i++)
        {
            Hands[i] = wDeck[pos[i]];
        }
}

Strenght Hnds_Idntfy(Card *Hands, int size)
{
    int Card_Strenght[TBLHD] = {};
    int suit[SUIT];
    int face[FACE];
    memset(suit, 0, SUIT * sizeof(int));
    memset(face, 0, FACE * sizeof(int));
    for(int i = 0; i < size; i++)
    {
        suit[Hands[i].num / FACE] ++;
        face[Hands[i].num % FACE] ++;
    }

    for(int i = FACE - 1, j = 0; i >= 0; i--)
        if(face[i] > 0)
            Card_Strenght[j++] = i;
            
    //flash
    Strenght flash = HandIni(-1, -1, -1, -1);
    int flash_suit = 0;
    int straight = -1;
    int three = -1;
    int two = -1;
    Strenght two_pair = HandIni(-1, -1, -1, -1);
    for(int i = 0; i < 4; i++)
    {
        if(suit[i] >= 5)
        {
//Flush
            for(int j = 0; j < size; j++){
                if(Hands[j].num / 13 != i && face[Hands[j].num % 13] == 1)
                    for(int k = 0; k < size; k++)
                        if(Card_Strenght[k] == Hands[j].num % 13){
                            Card_Strenght[k] = -1;
                            break;
                        }
                if(Hands[j].num / 13 != i && face[Hands[j].num % 13] > 1)
                    face[Hands[j].num % 13]--;
            }
            flash_suit = i;
            flash = HandIni(4, Ext_Array(Card_Strenght, -1, -1, -1, size), Ext_Array(Card_Strenght, -2, -1, -1, size), Ext_Array(Card_Strenght, -2, -2, -1, size));
            //                                         finding biggest    finding second biggest by using single skip       finding third biggest using double skip
        }
    }
    
    for(int i = 0; i < FACE; i++) // 13 numbers of faces
    {    
        if(face[i] == 0)
            continue;
            
        if(face[i] != 0 && i <= 8)
        {
            for(int j = 4; j >= 1; j--)
            {
                if(face[i + j] == 0 && !(face[FACE - 1] > 0 && i == 0 && j == 4))
                    break;
                if(j == 1){
                    if(face[FACE - 1] > 0 && i == 0){
                        straight = 3;
                        if(flash.hand != -1)
                        {
                            int stFlush[CARD] = {};
                            for(int k = 0; k < size; k++)
                                stFlush[Hands[k].num]++;
                            for(int k = 0; k < CARD; k++){
                                if(flash_suit * FACE == k && stFlush[k + FACE - 1] != 0){
                                    for(int p = 0; p < 4; p++){
                                        if(stFlush[k+p] == 0)
                                            break;
                                        if(p == 3)
                                            return HandIni(1, 3, -1, -1);
                                    }
                                }
                            }
                        }
                    }
                    else{
                        straight = i+4;
                        if(flash.hand != -1)
                        {
                            int stFlush[CARD] = {};
                            for(int k = 0; k < size; k++)
                                stFlush[Hands[k].num]++;
                            for(int k = 0; k < CARD; k++){
                                if(flash_suit * FACE + i == k){
                                    for(int p = 0; p < 5; p++){
                                        if(stFlush[k+p] == 0)
                                            break;
                                        if(p == 4 && straight == 12)
                                            return HandIni(0, straight, -1, -1);
                                        if(p == 4)
                                            return HandIni(1, straight, -1, -1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if(two != -1 && face[i] == 2)
            two_pair = HandIni(7,  two > i ? two : i, two < i ? two : i, Ext_Array(Card_Strenght, two, i, -1, size));
        if(face[i] == 4)
            return HandIni(2, i, Card_Strenght[0] == i ? Card_Strenght[1] : Card_Strenght[0], -1);
        if(face[i] == 3)
            three = i;
        if(face[i] == 2)
            two = i;
        if((two != -1 && face[i] == 3)|| (three != -1 && face[i] == 2))
            return HandIni(3, three != -1 ? three : i, three != -1 ?  i : three, -1);
    }


//Flush
    if(flash.hand != -1)
        return flash;
            //                                         finding biggest    finding second biggest by using single skip       finding third biggest using double skip
        
//Straight
    if(straight != -1)
        return HandIni(5, straight, -1, -1);
//Three of a Kind
    if(three != -1)
        return HandIni(6, three, Ext_Array(Card_Strenght, three, -1, -1, size), Ext_Array(Card_Strenght, three, -2, -1, size));
//Two Pair
    if(two_pair.hand != -1)
        return two_pair;
//One Pair
    if(two != -1)
        return HandIni(8, two,  Ext_Array(Card_Strenght, two, -1, -1, size), Ext_Array(Card_Strenght, two, -2, -1, size));

        
    return HandIni(9, Ext_Array(Card_Strenght, -1, -1, -1, size), Ext_Array(Card_Strenght, -2, -1, -1, size), Ext_Array(Card_Strenght, -2, -2, -1, size));
}

int Control(bool *Chng_Pos, int size)
{
    switch(_getch())
    {
        case '1': Chng_Pos[0] = !(Chng_Pos[0]); return 0;
        case '2': Chng_Pos[1] = !(Chng_Pos[1]); return 0;
        case '3': Chng_Pos[2] = !(Chng_Pos[2]); return 0;
        case '4': Chng_Pos[3] = !(Chng_Pos[3]); return 0;
        case '5': Chng_Pos[4] = !(Chng_Pos[4]); return 0;
        case 10: return 1;
        case '0': return 1;
        default : return 1;
    }
}

int Betting(Game *game, int player_id, int multi, char c)
{   
    Player * player_sh = &game->players[game->nPlayer_Pos];
    int *bet = &player_sh->bet;
    int *money = &player_sh->coin;
    int min_bet = game->min_bet;
    int *call = &game->call;
    int *lead = &game->lead;
    int *status = &game->status[game->nPlayer_Pos];
    int *pot = &game->pot;
    int *Total_bet = &player_sh->Total_bet;
    int *info = &game->info;
    int *nPlayer_Pos = &game->nPlayer_Pos;
    int *Click_Overlay = &game->Click_Overlay;
    
    switch(c != 0 ? c :  _getch())
    {//All of them --check , next round
        case ' ' : if(*lead == -1 || *lead == 0){
                        if(*bet > 0 && *bet < min_bet)
                            return -1;//min bet error
                        if(*bet > 0)
                        {
                            *Total_bet = *bet;
                            *status = 2; //bet
                            *lead = 2;
                            *call = *bet;
                            return 1; //bet
                        }
                        if(*bet == 0)
                        {
                            *status = 0; //check
                            *lead = 0;
                            return 0; //check
                        }
                    }
                    if(*bet > 0 && *bet == *call)
                    {
                        *Total_bet = *bet;
                        *status = 2; //call
                        *lead = 2;
                        *call = *bet;
                        return 2; //call
                    }
                    if(*bet > 0 && *bet > *call)
                    {
                        *Total_bet = *bet;
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    return -2;// No checlk or bet option
                    
        case 'c' : if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                    if(*lead == -1 || *lead == 0){
                        *Total_bet = min_bet;
                        *bet = min_bet;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet;
                        return 1; //bet minimum
                    }
                    *Total_bet = (*call);
                    *bet = (*call);
                    *status = *lead;
                    return 2; //call
        case 'r' : if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*lead != 0 && *lead != -1){
                        *Total_bet = 2 * (*call);
                        *bet = 2 * (*call);
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    else{
                        *Total_bet = min_bet * 2;
                        *bet = min_bet * 2;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet * 2;
                        return 1; //bet minimum
                    }
//                    else{
//                        return -3; //No Raise Option
//                    }
        case 'q' : if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*lead != 0 && *lead != -1){
                        *Total_bet = 4 * (*call);
                        *bet = 4 * (*call);
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    else{
                        *Total_bet = min_bet * 4;
                        *bet = min_bet * 4;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet * 4;
                        return 1; //bet minimum
                    }
        case 'u' : if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*lead != 0 && *lead != -1){
                        *Total_bet = 10 * (*call);
                        *bet = 10 * (*call);
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    else{
                        *Total_bet = min_bet * 10;
                        *bet = min_bet * 10;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet * 10;
                        return 1; //bet minimum
                    }
        case 'f' : if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   *status = *lead; return 5;
//        case 'a' : *status = *lead; return 4;
        
        
        case 'w': if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   (*bet) = (*bet) + 100;
                    if(*bet > *money){
                        *status = -6;
                        return -4;
                    }
                    return -6;
        case 's': if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*Total_bet < *bet - 100) 
                        (*bet) = range((*bet) - 100);
                    *status = -7;
                    return -7;

        case 'd': if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*bet == 0)
                        *bet = min_bet * 2;
                    else
                        (*bet) = (*bet) * 2; 
                    if(*bet > *money){
                        *status = -6;
                        return -4;
                    }
                    return -6;
        case 'a': if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*Total_bet < *bet / 2) 
                        (*bet) = range((*bet) / 2);
                    *status = -7;
                    return -7;
        case 'i': if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*info == player_id)
                    *info = -1;
                    else
                        *info = *nPlayer_Pos;
                    return -7;
        case 'o': if(!(multi == -1 || multi >= HMN - 1))
                        return -5;
                   if(*Click_Overlay == player_id)
                    *Click_Overlay = -1;
                    else
                        *Click_Overlay = *nPlayer_Pos;
                    return -7;
                    
        case 'C' : if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*lead == -1 || *lead == 0){
                        *Total_bet = min_bet;
                        *bet = min_bet;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet;
                        return 1; //bet minimum
                    }
                    *Total_bet = (*call);
                    *bet = (*call);
                    *status = *lead;
                    return 2; //call
        case 'R' : if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*lead != 0 && *lead != -1){
                        *Total_bet = 2 * (*call);
                        *bet = 2 * (*call);
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    else{
                        *Total_bet = min_bet * 2;
                        *bet = min_bet * 2;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet * 2;
                        return 1; //bet minimum
                    }
        case 'Q' : if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*lead != 0 && *lead != -1){
                        *Total_bet = 4 * (*call);
                        *bet = 4 * (*call);
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    else{
                        *Total_bet = min_bet * 4;
                        *bet = min_bet * 4;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet * 4;
                        return 1; //bet minimum
                    }
        case 'U' : if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*lead != 0 && *lead != -1){
                        *Total_bet = 10 * (*call);
                        *bet = 10 * (*call);
                        (*call) = *bet;
                        (*lead)++;
                        if(*lead < 3)
                            *lead = 3;
                        *status = (*lead);
                        
                        return 3; //raise
                    }
                    else{
                        *Total_bet = min_bet * 10;
                        *bet = min_bet * 10;
                        *status = 2;
                        *lead = 2;
                        *call = min_bet * 10;
                        return 1; //bet minimum
                    }
        case 'F' : if(!(multi == -1 || multi == 0))
                        return -5;
                   *status = *lead; return 5;
//        case 'a' : *status = *lead; return 4;
        
        
        case 'W':  if(!(multi == -1 || multi == 0))
                        return -5;
                    (*bet) = (*bet) + 100;
                    if(*bet > *money){
                        *status = -6;
                        return -4;
                    }
                    return -6;
        case 'S': if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*Total_bet < *bet - 100) 
                        (*bet) = range((*bet) - 100);
                    *status = -7;
                    return -7;

        case 'D': if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*bet == 0)
                        *bet = min_bet * 2;
                    else
                        (*bet) = (*bet) * 2; 
                    if(*bet > *money){
                        *status = -6;
                        return -4;
                    }
                    return -6;
        case 'A': if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*Total_bet < *bet / 2) 
                        (*bet) = range((*bet) / 2);
                    *status = -7;
                    return -7;
        case 'I': if(!(multi == -1 || multi == 0))
                        return -5;
                   if(*info == player_id)
                    *info = -1;
                    else
                        *info = *nPlayer_Pos;
                    return -7;
        default : *status = -7; return -7;
    }
}

void S_C_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], bool *Chng_Pos, int size, int DrawFromX, int DrawFromY, int letter_size, int Card_DesignWidth)
{
    char print_string[300];
    memset(print_string, 0, 300);
    
    for(int i = 0; i < size; i++)
    {
        if(Chng_Pos[i] == true)
        {
            for(int j = 0; j < (Card_DesignWidth - 47) / 4; j++)
                strcat(print_string, " ");
            strcat(print_string, "Change"); // 47 ~53~
            for(int j = 0; j < (Card_DesignWidth - 47) / 4; j++)
                strcat(print_string, " ");
        }
        else{
            for(int j = 0; j < (Card_DesignWidth - 30) / 4; j++)
                strcat(print_string, " ");
            strcat(print_string, "Stay"); // 30 42 ~58~
            for(int j = 0; j < (Card_DesignWidth - 30) / 4; j++)
                strcat(print_string, " ");
        }
    }
    Letter_Draw_Normal(fontDataWidth, fontData, fieldWidth, field, DrawFromX, DrawFromY, print_string, letter_size);
}

void Odds_Prob_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int bet, int DrawFromX, int DrawFromY, int letter_size)
{
    char print_field[300];

    for(int i = 0; i < 10; i++)
    {
        memset(print_field, 0, 300);
        sprintf(print_field, "%-18s%d", Hands_Rank_Ext[i], odds[i] * bet);
        Letter_Draw_Normal(fontDataWidth, fontData, fieldWidth, field, DrawFromX, DrawFromY + letter_size * i, print_field, letter_size);
    }
}

void Pay_Bet(int *money, int *pot, int *Total_bet)
{
    *money = *Total_bet < *money ? *money - *Total_bet : 0; 
    *pot += *Total_bet;
}

void Get_Award(int *money, int *bet, int Hand_id)
{
    *money = *money + *bet * odds[Hand_id];
    *bet = 0;
}

void Get_Award_Ext(int *money, int bet, int Hand_id, int *odds)
{
    *money = *money + bet * odds[Hand_id];
}

void Get_Pot(int *money, int *pot)
{
    *money += *pot;
    *pot = 0;
}

void Mny_Bt_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int money, int bet, int earned, int call, int *Error, int DrawFromX, int DrawFromY, int letter_size)
{
    char print_field[300];
    memset(print_field, 0, 300);
    
    if(earned == -1){
        sprintf(print_field, "Coin  %-10d Bet  %-10d", money, bet);
        if(money == -1)
            sprintf(print_field, "Bet  %-10d", bet);
    }
    else if(earned == -2)
        ;
    else{
        sprintf(print_field, "Coin  %-10d Bet  %-10d Award %-10d", money, bet, earned);
        if(money == -1)
            sprintf(print_field, "Bet  %-10d Award %-10d", bet);
    }
    
    if(*Error == -1)
        strcat(print_field, "Minimum Bet Error");
    if(*Error == -2)
        strcat(print_field, "No Check or Bet Option");
    if(*Error == -3)
        strcat(print_field, "No Raise Option");
    if(*Error == -4)
        strcat(print_field, "Over Badget");
    if(call != 0){
        if(*Error == -6)
            sprintf(print_field, "%s %d To Call", print_field, call);
        if(*Error == -7)
            sprintf(print_field, "%s %d To Call", print_field, call);
    }
    
//        if(*Error == 0)
//            sprintf(print_field, "%s %d To Call", print_field, call);
        if(*Error == 0){
            if(call == 0)
                sprintf(print_field, "%s Check", print_field);
            else
                sprintf(print_field, "%s %d To Call", print_field, call);
        }
    if(call != -1 && call != 0){
        if(*Error == 1){
            if(call > bet)
                sprintf(print_field, "%s %d To Call", print_field, call);
            else
                sprintf(print_field, "%s Bet %d", print_field, bet);
            
        }
        if(*Error == 2)
            sprintf(print_field, "%s Call %d", print_field, call);
        if(*Error == 3){
            if(call > bet)
                sprintf(print_field, "%s %d To Call", print_field, call);
            else
                sprintf(print_field, "%s Raise %d", print_field, call);
        }
        
    }
    if(*Error == 5)
        sprintf(print_field, "%s Fold", print_field);
    Letter_Draw_Normal(fontDataWidth, fontData, fieldWidth, field, DrawFromX, DrawFromY, print_field, letter_size);
//    *Error = 0;
}

Strenght HandIni(int Hand_id, int ref_1, int ref_2, int ref_3)
{
    Strenght Hand;
    
    Hand.hand = Hand_id;
    Hand.ref_1 = ref_1;
    Hand.ref_2 = ref_2;
    Hand.ref_3 = ref_3;
    
    return Hand;
}

int Ext_Array(int *From_arr, int ref_1, int ref_2, int ref_3, int size)
{
    ///if ref_1 is -1, need to find ref_1 first and if ref_1 is -2 that is one skip,
    //if ref_2 is -2, that is double skip,
    //if ref_2 and ref_3 are -2, that is triple skip.
    
    if(ref_1 == -1)
    {
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            if(ref_2 == -1 && ref_3 == -1)
                return From_arr[i];
            ref_1 =  From_arr[i];
            break;
        }
    }
    if(ref_1 == -2)
    {
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            ref_1 =  From_arr[i];
            break;
        }
    }
    if(ref_2 != -1 && ref_3 != -1 && ref_2 != -2 && ref_3 != -2){
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            if(!(From_arr[i] == ref_1 || From_arr[i] == ref_2 || From_arr[i] == ref_3))
                return From_arr[i];
        }
    }
    else if(ref_2 != -1 && ref_2 != -2 && ref_3 != -2){
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            if(!(From_arr[i] == ref_1 || From_arr[i] == ref_2))
                return From_arr[i];
        }
    }
    else if(ref_2 == -2 && ref_3 == -2){
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            if(!(From_arr[i] == ref_1 || From_arr[i] == Ext_Array(From_arr, ref_1, -1, -1, size) || From_arr[i] == Ext_Array(From_arr, ref_1, -2, -1, size)))
                return From_arr[i];
        }
    }
    else if(ref_2 == -2){
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            if(!(From_arr[i] == ref_1 || From_arr[i] == Ext_Array(From_arr, ref_1, -1, -1, size)))
                return From_arr[i];
        }
    }
    else{
        for(int i = 0; i < size; i++){
            if(From_arr[i] == -1)
                continue;
            if(From_arr[i] != ref_1)
                return From_arr[i];
                
        }
    }
}

void Drawer(int h_size, int nPlayer, int (*posHand)[h_size], int *Draw_Start)
{
    for(int i = 0; i < h_size; i++)
        for(int j = 0; j < nPlayer; j++){
            posHand[j][i] = *Draw_Start;
            (*Draw_Start)++;
        }
}

void Hand_cat(Card *Hands, Card *cat_1, Card *cat_2, int cat_1_size, int cat_2_size)
{
    for(int i = 0; i < cat_1_size; i++)
        Hands[i] = cat_1[i];
    for(int i = 0; i < cat_2_size; i++)
        Hands[cat_1_size + i] = cat_2[i];
}

bool Status_Same(int *status, int size, int excluding)
{
    for(int i = 0; i < size - 1; i++)
    {
        if(status[i] == status[i + 1] || status[i] == excluding)
            continue;
        return false;
    }
    return true;
}

int Hand_Cmp(Player * players, int size, int h_size)
{
    int Hand_rank[PLAYER] = {};
    for(int i = 0; i < PLAYER; i++)
        if(players[i].Error == 5)
            Hand_rank[i] = -1;
    int counter = 0;
    int nMax_Pos = 0;
    for(int i = 1; i < size; i++){
        if(Hand_rank[i] == -1)
            continue;
        if(players[nMax_Pos].Hand_id.hand >= players[i].Hand_id.hand || Hand_rank[nMax_Pos] == -1){
            if(players[nMax_Pos].Hand_id.hand == players[i].Hand_id.hand){
                nMax_Pos = i;
                counter++;
            }
            else{
                Hand_rank[nMax_Pos] = -1;
                nMax_Pos = i;
                counter = 0;
            }
        }
        else{
            Hand_rank[i] = -1;
        }
    }
    if(counter == 0)
        return nMax_Pos;
    counter = 0;
    nMax_Pos = 0;
    for(int i = 1; i < size; i++){
        if(Hand_rank[i] == -1)
            continue;
        if(players[nMax_Pos].Hand_id.ref_1 <= players[i].Hand_id.ref_1 || Hand_rank[nMax_Pos] == -1){
            if(players[nMax_Pos].Hand_id.ref_1 == players[i].Hand_id.ref_1){
                nMax_Pos = i;
                counter++;
            }
            else{
                Hand_rank[nMax_Pos] = -1;
                nMax_Pos = i;
                counter = 0;
            }
        }
        else{
            Hand_rank[i] = -1;
        }
    }
    if(counter == 0)
        return nMax_Pos;
    counter = 0;
    nMax_Pos = 0;
    for(int i = 1; i < size; i++){
        if(Hand_rank[i] == -1)
            continue;
        if(players[nMax_Pos].Hand_id.ref_2 <= players[i].Hand_id.ref_2 || Hand_rank[nMax_Pos] == -1){
            if(players[nMax_Pos].Hand_id.ref_2 == players[i].Hand_id.ref_2){
                nMax_Pos = i;
                counter++;
            }
            else{
                Hand_rank[nMax_Pos] = -1;
                nMax_Pos = i;
                counter = 0;
            }
        }
        else{
            Hand_rank[i] = -1;
        }
    }
    if(counter == 0)
        return nMax_Pos;
    counter = 0;
    nMax_Pos = 0;
    for(int i = 1; i < size; i++){
        if(Hand_rank[i] == -1)
            continue;
        if(players[nMax_Pos].Hand_id.ref_3 <= players[i].Hand_id.ref_3 || Hand_rank[nMax_Pos] == -1){
            if(players[nMax_Pos].Hand_id.ref_3 == players[i].Hand_id.ref_3){
                nMax_Pos = i;
                counter++;
            }
            else{
                Hand_rank[nMax_Pos] = -1;
                nMax_Pos = i;
                counter = 0;
            }
        }
        else{
            Hand_rank[i] = -1;
        }
    }
    if(counter == 0)
        return nMax_Pos;
    counter = 0;
    nMax_Pos = 0;
    
    int face[PLAYER][FACE] = {};

    for(int i = 0; i < PLAYER; i++){
        if(Hand_rank[i] == -1)
            continue;
        for(int j = 0; j < h_size; j++)
            face[i][players[i].Hands[j].num % FACE]++;
    }
    
    int nMax = -1;
    
    for(int i = FACE - 1; i >= 0; i--){
        int on = 0;
        for(int j = 0; j < PLAYER; j++){
            if(Hand_rank[j] == -1)
                continue;
            if((face[j][i] != 0 && on == 0) || face[j][i] > on)
                on = face[j][i];
        }
        if(on == 0)
            continue;
        for(int j = 0; j < PLAYER; j++){
            if(Hand_rank[j] == -1)
                continue;
            if(face[j][i] == on){
                counter++;
                nMax = j;
            }
            else
                Hand_rank[j] = -1;
        }
        if(counter == 1)
            return nMax;
        nMax = -1;
        counter = 0;
        on = 0;
    }
    
    return -1;
    
}

void saveGameData(Game *game, char* fname, int* key, int *Next)
{
    FILE *cfPtr;
    
    if((cfPtr = fopen(fname, "wb")) == NULL)
    {
        puts("The file could not be opened");
        fclose(cfPtr);
        return;
    }
    else{
        fwrite(key, sizeof(int), 1, cfPtr);
        fwrite(Next, sizeof(int), 1, cfPtr);
        fwrite(game, sizeof(Game), 1, cfPtr);
    }
    fclose(cfPtr);
}

int loadGameData(Game *game, char* fname, int *key, int *Next)
{
    FILE *cfPtr;
    int temp = *key;
    if((cfPtr = fopen(fname, "rb")) == NULL)
    {
        puts("The file could not be opened");
        fclose(cfPtr);
        return 0;
    }
    else{
        fread(key, sizeof(int), 1, cfPtr);
        fread(Next, sizeof(int), 1, cfPtr);
        if(temp != *key || *key == 0){
            fread(game, sizeof(Game), 1, cfPtr);
            fclose(cfPtr);
            return 1;
        }
    }
    fclose(cfPtr);
    return 0;
}

void BG_Draw(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int DrawFromX, int DrawFromY, int letter_size, int h_size, int Draw_Height)
{
    char string_field[1600];
    for(int i = 0; i < Draw_Height; i++) // 5 is letter hight
    {
        memset(string_field, 0, 1600);
        
        for(int j = 0; j < h_size; j++)
                strcat(string_field, fontData[53 * letter_size + i]);
        strcpy(&field[DrawFromY + i][DrawFromX], string_field);
//        strcpy(&field[DrawFromY + i][DrawFromX], string_field);
    }
}

void shuffleDeck_From(Card * wDeck, int size_excld)
{
    for(int i = size_excld; i < CARD; i++)
    {
        size_t j = rand() % (CARD - size_excld) + size_excld;
        Card temp = wDeck[i];
        wDeck[i] = wDeck[j];
        wDeck[j] = temp;
    }
}

void fillDeck_From(Card * wDeck, char * wFace[], char * wSuit[], int *Hand_Pos, int size_hand, int size_table, int size_excld)
{
    int pos = 0;
    for(int i = 0; i < size_hand; i++){
        wDeck[i].face = wFace[Hand_Pos[i] % FACE];
        wDeck[i].suit = wSuit[Hand_Pos[i] / FACE];
        wDeck[i].num = Hand_Pos[i];
    }
    for(int i = 0; i < size_excld - size_hand; i++){
        wDeck[i + size_hand].face = wFace[Hand_Pos[i + size_hand] % FACE];
        wDeck[i + size_hand].suit = wSuit[Hand_Pos[i + size_hand] / FACE];
        wDeck[i + size_hand].num = Hand_Pos[i + size_hand];
    }
    for(int i = size_excld; i < CARD; i++)
    {
        while(Excld_Hand(pos, Hand_Pos, size_excld) == false)
            pos++;

        wDeck[i].face = wFace[pos % FACE];
        wDeck[i].suit = wSuit[pos / FACE];
        wDeck[i].num = pos;
        pos++;
    }
}

bool Excld_Hand(int input, int *excld_Hands, int size_excld)
{
    for(int i = 0; i < size_excld; i++)
    {
        if(input == excld_Hands[i])
            return false;
    }
    return true;
}

float Prob_Cal(Card *Hands, int size_player, int size_hand, int size_table, int size_excld, int *Hand_Counter, int precision)
{
    Card deck[CARD];

    Player players[PLAYER];
    int posHand[PLAYER][TBLHD] = {};
    int posTable[TBL] = {};
    float won = 0;
    for(int i = 0 ; i < size_hand + size_table; i++)//TBLHD
        posHand[0][i] = Hands[i].num;
    fillDeck_From(deck, wFace, wSuit, posHand[0],size_hand, size_table, size_excld); //HADN value is needed
    shuffleDeck_From(deck, size_excld);
    for(int i = 1; i < size_player; i++)
        for(int j = 0 ; j < size_hand  + size_table; j++) //TBLHD
            posHand[i][j] = j <  size_hand ? j + size_hand * i + size_table : j;
    for(int i = 0; i < size_hand + size_table; i++)
        posHand[0][i] = i;
    for(int i = 0; i < size_player; i++)
        Cpy_Hnds(deck, players[i].Hands, size_hand + size_table, posHand[i]);
    
    memset(Hand_Counter, 0, sizeof(int) * 10);
    
    for(int i = 0; i < precision; i++){ //100000
        for(int i = 0; i < size_player; i++)
            players[i].Hand_id = Hnds_Idntfy(players[i].Hands, size_hand + size_table);
        
        Hand_Counter[players[0].Hand_id.hand]++;
        if(Hand_Cmp(players, size_player, size_hand) == 0)
            won++;
        shuffleDeck_From(deck, size_excld);
            
        for(int i = 0; i <size_player; i++)
            Cpy_Hnds(deck, players[i].Hands, size_hand + size_table, posHand[i]);
    //        printf("*");
    }
    return won / (precision / 100);
}

void Odds_Prob_Draw_float(int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], float *data, float *data_Ext, int DrawFromX, int DrawFromY, int letter_size)
{
    char print_field[300];

    for(int i = 0; i < 10; i++)
    {
        memset(print_field, 0, 300);
        sprintf(print_field, "%-18s%.2f (%.2f)", Hands_Rank_Ext[i], data[i], data_Ext[i]);
        Letter_Draw_Normal(fontDataWidth, fontData, fieldWidth, field, DrawFromX, DrawFromY + letter_size * i, print_field, letter_size);
    }
}

int Status_count(Player *players, int size, int status_number)
{
    int counter = 0;
    for(int i = 0; i < size; i++)
    {
        if(players[i].Error == status_number)
            counter++;
    }
    return counter;
}

char kb_m_Input(Box *Clicks, int *x, int *y, int fieldWidth, char (*field)[fieldWidth], int fieldWidth_display, int displayWidth, int displayHeight_Shift, int displayWidth_Shift, int *cnt, int *pre_cnt, int size_box)
{
    char returnChar[BOX] = " crf";
    while(1){
		if(GetAsyncKeyState(VK_LBUTTON)){
			(*cnt)++;
		}
        if((*cnt) != *pre_cnt){
            Get_Mouse_Pos(x, y);
            for(int p = 0; p < size_box; p++){
                if(Click_Box(*x, *y, displayWidth_Shift, displayHeight_Shift, Clicks[p].Start_X, Clicks[p].Start_Y, Clicks[p].Lenght_X, Clicks[p].Lenght_Y, displayWidth / (float)fieldWidth_display) == true){
                    *pre_cnt = (*cnt);
                    return returnChar[p];
                }
            }
        }
        if(_kbhit())
            return _getch();
	}
}

bool Click_Box(int m_x, int m_y, int m_x_Sht, int m_y_Sht, int Start_X, int Start_Y, int Lenght_X, int Lenght_Y, float Coe)
{
    if(Start_X * Coe < m_x - m_x_Sht && m_x - m_x_Sht < (Start_X + Lenght_X) * Coe && Start_Y * Coe * 2 < m_y - m_y_Sht && m_y - m_y_Sht < (Start_Y + Lenght_Y) * Coe * 2) 
        return true;
    return false;
}

Box Box_Init(int Start_X, int Start_Y, int Lenght_X, int Lenght_Y)
{
    Box box;
    box.Start_X = Start_X;
    box.Start_Y = Start_Y;
    box.Lenght_X = Lenght_X;
    box.Lenght_Y = Lenght_Y;
    
    return box;
}

void Get_Mouse_Pos(int *x, int *y)
{
	POINT pt;
	GetCursorPos(&pt);
    *x = pt.x;
    *y = pt.y;
}

void Box_Draw(Box *Clicks, int fontDataWidth, char (*fontData)[fontDataWidth], int fieldWidth, char (*field)[fieldWidth], int size_box, int letter_size)
{
    for(int p = 0; p < size_box; p++)
    {
        for(int i = Clicks[p].Start_X; i < Clicks[p].Lenght_X + Clicks[p].Start_X; i++){
            field[Clicks[p].Start_Y][i] = '*';
            field[Clicks[p].Lenght_Y + Clicks[p].Start_Y][i] = '*';
        }
        for(int i = Clicks[p].Start_Y; i < Clicks[p].Lenght_Y + Clicks[p].Start_Y; i++){
            field[i][Clicks[p].Start_X] = '*';
            field[i][Clicks[p].Lenght_X + Clicks[p].Start_X] = '*';
        }
        Letter_Draw_Normal(fontDataWidth, fontData, fieldWidth, field, Clicks[p].Start_X, Clicks[p].Start_Y, status_Stack[p], letter_size);
    }
}