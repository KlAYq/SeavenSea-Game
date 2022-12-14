#include <iostream>
#include <unistd.h> // for sleep()
#include <cstdlib> // for rand() srand()
#include <time.h> // for time()
using namespace std;

#define MAX 100

int size = 11;
char waterIcon = '~', shipIcon = 'X', enemyIcon = 'A', deathIcon = '#', obstacleIcon = '!', bulletIcon = '.', portalIcon = '@'; // icon game
int shipPosX = 6, shipPosY = 9; // vi tri ban dau cua tau
int shipStatus = 1; // 1, 9 = / -> 1
				 	// 2, 8 = _  -> 2
				 	// 3, 7 = \  -> 3
					// 4, 6 = |  -> 4
				
char map[MAX][MAX];
bool visited[MAX][MAX]; int visitedNum;

void initMap(char ch){
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			map[i][j] = ch;
}

void updateMap(){
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++)
			cout << map[i][j] << " ";
		cout << endl << endl;
	}
}

int CheckWinCondition(){
	if (map[shipPosX][shipPosY] == deathIcon)
		return -1; 
		
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (map[i][j] == enemyIcon)
				return 0;
	
	return 1;
}

void clear(){
	   system("cls");
}

// Check xem co di chuyen ra ngoai map khong
bool isInMap(int x, int y){
	if (x >= 0 && x < size && y >= 0 && y < size)
		return true;
	return false;
}

//Nhap gia tri cua nguoi choi
void getInput(int &x, int &y){
		int input;
		cout << "Your joystick :" << endl;
		cout << "7 8 9" << endl; // upper left  | up          | upper right
		cout << "4 5 6" << endl; // left        | fire cannon | right
		cout << "1 2 3" << endl; // bottom left | bottom      | bottom right 
		cout << "Your move : ";
		cin >> input; 
		
		switch(input)
		{
			case 7:{
				x = -1;
				y = -1;
				shipStatus = 3;
				break;
			}
			case 8:{
				x = -1;
				y = 0;	
				shipStatus = 2;
				break;
			}
			case 9:{
				x = -1;
				y = 1;
				shipStatus = 1;
				break;
			}
			case 6:{
				x = 0;
				y = 1;
				shipStatus = 4;
				break;
			}		
			case 4:{
				x = 0;
				y = -1;
				shipStatus = 4;
				break;
			}
			case 1:{
				x = 1;
				y = -1;
				shipStatus = 1;
				break;
			}
			case 2:{
				x = 1;
				y = 0;
				shipStatus = 2;
				break;
			}
			case 3:{
				x = 1;
				y = 1;
				shipStatus = 3;
				break;
			}
			default:{
				x = 100;
				break;
			}
		}
}

//Di chuyen tu vi tri (PosX, PosY) sang vi tri (PosX + x, PosY + y) 
void enemyMove(int posX, int posY, int x, int y){
	// check xem vi tri di chuyen toi co vat can gi khong
	if (map[posX + x][posY + y] != waterIcon){
		if (map[posX + x][posY + y] == enemyIcon){
			if (visited[posX + x][posY + y] == true){
				map[posX][posY] = waterIcon;
				map[posX + x][posY + y] = deathIcon;
			}
			else {
				visited[posX][posY] = false;
				visitedNum--;
			}	
		}
		else {
			map[posX][posY] = waterIcon;
			map[posX + x][posY + y] = deathIcon;	
		}		
	}
	else{
		map[posX][posY] = waterIcon;		
		map[posX + x][posY + y] = enemyIcon;
		if (visited[posX+x][posY+y] != true){
			visited[posX+x][posY+y] = true;	
			visitedNum++;
		}	
		
	}
		
}

void bulletMove(int x, int y){
	int temp;
	int bullet1X = shipPosX, bullet2X = shipPosX,
	    bullet1Y = shipPosY, bullet2Y = shipPosY;
	for (int i = 0; i < 3; i++){
		if (map[bullet1X][bullet1Y] != shipIcon && map[bullet1X][bullet1Y] != deathIcon)
			map[bullet1X][bullet1Y] = waterIcon;
		if (map[bullet1X + x][bullet1Y + y] != waterIcon){	
			map[bullet1X + x][bullet1Y + y] = deathIcon;
		}
		else
			map[bullet1X + x][bullet1Y + y] = bulletIcon;
		bullet1X += x;
		bullet1Y += y;
		
		
		if (map[bullet2X][bullet2Y] != shipIcon && map[bullet2X][bullet2Y] != deathIcon)
			map[bullet2X][bullet2Y] = waterIcon;	
		if (map[bullet2X - x][bullet2Y - y] != waterIcon){	
			map[bullet2X - x][bullet2Y - y] = deathIcon;
		}
		else
			map[bullet2X - x][bullet2Y - y] = bulletIcon;
		bullet2X -= x;
		bullet2Y -= y;
		clear();
		updateMap();
		sleep(1);
	}
	// clear the last bullet
	if (map[bullet1X][bullet1Y] != shipIcon && map[bullet1X][bullet1Y] != deathIcon)
			map[bullet1X][bullet1Y] = waterIcon;
	if (map[bullet2X][bullet2Y] != shipIcon && map[bullet2X][bullet2Y] != deathIcon)
			map[bullet2X][bullet2Y] = waterIcon;	
}

void fireCannon(){
	int x, y;
	switch(shipStatus){
		case 1:{
			x = -1;
			y = -1;
			break;
		}
		case 2:{
			x = 0;
			y = 1;
			break;
		}
		case 3:{
			x = 1;
			y = -1;
			break;
		}
		case 4:{
			x = -1;
			y = 0;
			break;
		}
	}
	bulletMove(x, y);
}

void playerTurn(){
	int x, y;
	getInput(x, y);
	if (x == 100)
		fireCannon();
	else
		if (isInMap(shipPosX + x, shipPosY + y)){
			map[shipPosX][shipPosY] = waterIcon;
			if (map[shipPosX + x][shipPosY + y] != portalIcon){
				if (map[shipPosX + x][shipPosY + y] != waterIcon)
					map[shipPosX + x][shipPosY + y] = deathIcon;
				else
					map[shipPosX + x][shipPosY + y] = shipIcon;			
				shipPosX += x;
				shipPosY += y;
			}
			else { // tele
				srand(time(0));
				int tempx, tempy;
				while (true){
					tempx = rand() % 11;
					tempy = rand() % 11;
					if (map[tempx][tempy] == waterIcon)
						break;
				}
				shipPosX = tempx;
				shipPosY = tempy;
				map[shipPosX][shipPosY] = shipIcon;
			}
		}			
}

void enemyTurn(){
	int x, y;
	visitedNum = 0;
	for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				visited[i][j] = false;
				
	while (visitedNum < size*size){
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if (visited[i][j] == false)
				{
					visited[i][j] = true;
					visitedNum++;
					if (map[i][j] == enemyIcon){
						
						// so sanh vi tri dich tai (i, j) so voi tau cua minh de co huong di toi uu nhat
						if (i == shipPosX)
							x = 0;
						else if (i < shipPosX)
							x = 1;
						else 
							x = -1;
							
						if (j == shipPosY)
							y = 0;
						else if (j < shipPosY)
							y = 1;
						else 
							y = -1;
							
						enemyMove(i, j, x, y);	
					}
				}
			}
	}
}

int main(){
	
	initMap(waterIcon);
	// manual set up
	map[shipPosX][shipPosY] = shipIcon;
	
	map[0][0] = portalIcon;
	map[0][10] = portalIcon;
	map[10][0] = portalIcon;
	map[10][10] = portalIcon;
	
	map[6][1] = enemyIcon;
	map[6][2] = enemyIcon;
	map[2][5] = enemyIcon;
	map[10][9] = enemyIcon;
	map[1][2] = enemyIcon;

	
	map[5][9] = obstacleIcon;
	map[1][9] = obstacleIcon;
	map[5][4] = obstacleIcon;
	
	bool win = false;
	
	while (true){
		clear();
		updateMap(); //  Cap nhat map sau moi lan ve xong 
		
		// check win/ lose condition 
		if (CheckWinCondition() == 1){
			win = true;
			break;
		} 
		else if (CheckWinCondition() == -1){
			win = false;
			break;	
		}
		playerTurn();
		
		clear();
		updateMap();
		sleep(1);
		
		enemyTurn();
		
	}
	
	if (win)
		cout << "YOU WIN !!!";
	else 
		cout << "GAME OVER !!!";  
	return 0;
}

















