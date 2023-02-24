#include <stdio.h>
#include <stdlib.h>
#include <string.h>
					//Muhammed Ali LALE - 20011045

void getInputs(char**, int**, int);
int getIndex(char**, char*, int);
void printMatrix(char**, int**, int);
int checkRowsAbove(int**, int, int);
void print(int**, int);
void shift(int*, int);
int backtrack(int**, char**, int, int, int*, char);

int main(){
	int i, j, N;
	char **colors, mode;
	int  **numbers, *shiftCount;
	printf(" Please enter the size of the color matrix: ");
	scanf("%d",&N);
	while(N<3 || N>8){
		printf(" Please enter the size of the color matrix between 3-8: ");
		scanf("%d",&N);
	}
	
	printf(" Please select mode \n");
	printf("   1.Normal mode\n");
	printf("   2.Detail mode\n\t> ");
	scanf("%*c%c", &mode);	//%*c for enter character
	while(mode!='1' && mode!='2'){
		printf(" PLease select 1 or 2: ");
		scanf("%*c%c", &mode); //%*c for enter character
	}
	
	colors  = (char**)malloc(N*sizeof(char*));	//char array for the first row
	if(colors==NULL){
		printf("Allocation Error! "); exit(1);	//allocation check
	}
	
	numbers = (int**) malloc(N*sizeof(int*));	//integer matrix for colors as numbers
	if(numbers==NULL){
		printf("Allocation Error! "); exit(1);	//allocation check
	}
	
	shiftCount = (int*)calloc(N,sizeof(int));	//array for shift count for every row
	
	getInputs(colors, numbers, N);	//get inputs from the user
	
	printf("\n The given matrix is: \n");	//print the given matrix
		printMatrix(colors,numbers,N);
		
	//go to backtrack function to solve the problem
	//we check with the return value if the problem could solve or not
	if(backtrack(numbers,colors,N,1,shiftCount,mode)){
		printf("\n  Result: \n");
		printMatrix(colors,numbers,N);
	}else{
		printf("\n  No results");
	}
	
	return 0;
}

void getInputs(char** colors, int** numbers, int N){	//get inputs from the user
	char input[20];
	int i,j;

	for(i=0; i<N; i++){
		colors [i] = (char*)malloc(20*sizeof(char));
		if(colors[i]==NULL){
			printf("Allocation Error! "); exit(1);
		}
		numbers[i] = (int*) malloc(N*sizeof(int));
		if(numbers[i]==NULL){
			printf("Allocation Error! "); exit(1);
		}
	}
	//get the first row and save it in colors array
	printf(" Please enter the colors with spaces between them:\n");
	printf(" Line 1: ");
	for(i=0; i<N; i++){
		scanf("%s", colors[i]);
		numbers[0][i] = i;
	}
	
	//the rows other than the first row will be in integer form
	//compare the given input with the colors in colors[] array and match them with the indexes of the colors
	for(i=1; i<N; i++){
		printf(" Line %d: ", i+1);	//get the inputs line by line
		for(j=0; j<N; j++){
			scanf("%s", &input);
			numbers[i][j] = getIndex(colors,input,N);	//take the index of color and keep it in the numbers matrix
			if(numbers[i][j]==-1){
				printf("  You entered a wrong input!\n");
				i--; j=N; //to take the current row again
			}
		}
	}
}

int getIndex(char** colors, char* input, int N){	
	int i;
	//compare the input with all the colors  in array
	for(i=0; i<N; i++){
		if(!strcmp(colors[i],input))
			return i;	//return the index of the color
	}
	return -1;	//if the input is not in the array return -1
}

void printMatrix(char** colors, int** numbers, int N){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			printf("  %s",colors[numbers[i][j]]);
		}
		printf("\n");
	}
}

void shift(int* row, int N){	//shift the given row right
	int i, last;
	last = row[N-1];	//save the last element
	for (i = N; i > 0; i--)
		row[i] = row[i-1];
		
	row[0] = last;	//the first element will take the value of last
}

int checkRowsAbove(int** numbers, int N, int row){
	int i, j;
	//check if there is any same value in same column
	for(i=0; i<N; i++){
		for(j=row-1; j>=0; j--){
			if(numbers[j][i] == numbers[row][i])
				return 0;	//there is a match
		}
	}
	return 1;	//all the columns have different values
}

int backtrack(int** numbers, char** colors, int N, int row, int* shiftCount, char mode){
	int flag=1, returnValue;
	
	//if the function pass the last row the problem is solved
	if(row>=N){	
		return 1;
	}
	//if the function return to the first row the problem doesn't have any solution
	if(row == 0){
		return 0;
	}
	
	while(shiftCount[row]<N && flag){	//if the row dit not shifted N times and flag==1
		if(!checkRowsAbove(numbers,N,row)){	//if the current row has no same values at the rows above
			shiftCount[row]++;
			shift(numbers[row],N);		//shift the row
		}else{//then the row is well set
			flag=0;	//clear the flag to break the loop	
			shiftCount[row+1]=0; 
			//if the detail mode is selected print the matrix in every step
			if(mode=='2'){	
				printf("\n\t* Row %d *\n\n",row+1);
				printMatrix(colors,numbers,N);
			}
			returnValue = backtrack(numbers,colors,N,row+1,shiftCount,mode);//go to the next row
		}
	}
	
	if(flag){	//if the loop ended for shifted N times it should return the previous row
		//if the detail mode is selected print the matrix in every step
		if(mode=='2'){	
			printf("\n\t* Row %d *\n\n",row+1);
			printMatrix(colors,numbers,N);
		}
		shift(numbers[row-1],N);
		shiftCount[row-1]++;
		returnValue = backtrack(numbers,colors,N,row-1,shiftCount,mode);
	}
	return returnValue;
}
