#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4];
FILE *fp;

void init(){
	int i, j;
	game_over = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i][j] = 0;
}


void rotate(int b[4][4]){
	/* user code */ 
	int temp[4][4];
	int i,j;
	
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			temp[i][j] = b[i][j];
		}
	}

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			b[j][3 - i] = temp[i][j];
		}
	}
}

int set_board(int dir, int b[4][4]){
	/* user code init */

	int i, j, k;
	int temp[4][4];
	int count = 0;

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			temp[i][j] = b[i][j];
		}
	}

	for(i = 0; i < dir; i++){
		rotate(b);
	}

	for(i = 0; i <= 3; i++){
		for(j = 3; j > 0;){
			count++;
			if (count > 4) {
				j--;
				count = 0;
			}
			if (b[i][j] == 0){
				for(k = j; k > 0; k--){
					b[i][k] = b[i][k - 1];
			}
				b[i][0] = 0;
			}
		}
	}
	for(i = 0; i <= 3; i++){
		for(j = 3; j > 0; j--){
				if (b[i][j] == b[i][j-1]){
					b[i][j] += b[i][j-1];
					for(k = j - 1; k > 0; k--){
						b[i][k] = b[i][k - 1];
					}	
				b[i][0] = 0;
				}
			}
		}


	for(i = 0; i < 4 - dir; i++){
		rotate(b);
	}

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(temp[i][j] != b[i][j]){
				return 1;
			}
		}
	}
	return 0;
}

int is_game_over(){
	/* user code */

	/* if game over return 0, else then return 1 */
	
	int i, j;
	int temp[4][4];

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			temp[i][j] = b[i][j];
		}
	}
	if(set_board(0,temp) + set_board(1,temp) + set_board(2,temp) + set_board(3,temp) == 0){
		return 1;
	}
	else return 0;

}

void draw_board(int tot, int command){
	int i, j, k, c[8][8], score, sum = 0;
	/* console clear */
	system("clear");

	score = 0;
	/* user code */
	
	/* calculate score & check sum of all block equasls variable tot */
	//calculate score
	
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(b[i][j] > score){
				score = b[i][j];
			}
		}
	}
	//check sum & tot
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			sum += b[i][j];
		}
	}

	printf("    Score : %d\n", score);
	fprintf(fp, "%d %d\n", score, command);

	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			c[i][j] = 32;
			for(k=0; k<50; k++){
				if( b[i][j] == (1<<k) ){
					c[i][j] = 32 + (k%6);
					break;
				}
			}

		}
	}
	for(i=0; i<4; i++){
		for(j=0; j<4; j++)
			fprintf(fp, "%d ", b[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<4; i++){
		puts("");
		puts("");
		for(j=0; j<4; j++)
			printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
		puts("");
		puts("");
	}
	if(sum != tot){
	system("clear");
	printf("ERROR!\n");
	}
}

int make_two_or_four(){

	/* user code */

	/* return 2 or 4 that makes in this times */

	/* if can not make two or four, then return 0 */
	
	int b1, b2;
	int i, j, sum = 0;
	
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if (b[i][j] != 0){
				sum += 1;
			}
		}
	}

	srand(time(NULL));

	if (sum == 16){
		return 0;
	}

	do{
		b1 = rand() % 4;
		b2 = rand() % 4;	
	}while(b[b1][b2] != 0);
	
	if(rand() % 3 == 0){
		b[b1][b2] = 4;

		return 4;
	}
	else {
		b[b1][b2] = 2;

		return 2;
	}
}


int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}

	return -1;
}


int main() {
	int command = -1;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));

	int tot = 0;
	/* make init board */
	/* user code */
	 int i,j;

	 for(i = 0; i < 2; i++){
	 	tot += make_two_or_four();
	 }
	draw_board(tot, -1);

	
	do {
		command = GetCommand();

		if (command != -1) {
			if( set_board(command, b) ){

				/* user code */
				make_two_or_four();
					
				for(tot = 0, i = 0; i < 4; i++){
					for(j = 0; j < 4; j++){
						tot += b[i][j];
					}
				}

				draw_board(tot, command);
			}
		}
		game_over = is_game_over();
	} while (!game_over);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}
