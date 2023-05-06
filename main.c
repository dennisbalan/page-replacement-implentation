#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//main.c contains the code for a program that does FIFO and LRU page replacement and prints out the number of page references,page misses,page miss time units and page writeback units
//page_container represents the frame and has count for amount of times it was used and name to represent its name
typedef struct page_container{
	int count;
	int name;
}page_container;
//functions are explained in detail later
//FIFO takes a char filename and int size and does FIFO on filename
void FIFO(char *,int);
//LRU takes a char filename and int size and does LRU on filename
void LRU(char *,int);
//search finds an int element in page_container *
int search(page_container *,int,int);
//insert inserts an element into page_container[int]
page_container insert(page_container *,int,int);
//find least_used finds the biggest count in page_container array and returns its index
int find_least_used(page_container *, int);
//main takes the 3 command line arguments (method, size,filename) and decides which page method to use based on the command line arguments
int main(int argc, char *argv[]){
	//index takes the command line argv argument for index as a string
	char index[255];
	//takes the command line argument for method in a string
	char method[255];
	//take command line arguemnt for filename in a string
	char filename[255];
	//string idnex will be converted to this variable. It is the size 
	int size = 0;
	//if number of elements in the command line equals 4, set all the correct variables above to their needed values from the command line (reminder command line args are method,size and filename).
	if(argc == 4){
		strcpy(method,argv[1]);
		strcpy(index,argv[2]);
		size = atoi(index);
		strcpy(filename,argv[3]);
	}
	//error arg
	else{
		printf("Not enough or too many command line arguments\n");
		return 0;
	}
	//do FIFO if method is FIFO and input filename and size into the FIFO function
	if(strcmp(method,"FIFO") == 0){
		FIFO(filename,size);
	}
	//do LRU if method is LRU and input filename and size into the LRU function
	if(strcmp(method,"LRU") == 0){
		LRU(filename,size);
	}
	

}
//FIFO does FIFO page replacment method and takes an inut of filename to read the file and size to find the number of frames, and prints the number of page references, misses, miss time and page writeback time
void FIFO(char *filename,int size){
	//misses is the number of misses, miss-time is the time_unit count for misses, dirty_time is the page writeback time and hits is the number of page references. All of these vars are set to 0
	int misses,hits,miss_time,dirty_time;
	misses = 0;
	hits = 0;
	miss_time = 0;
	dirty_time = 0;
	//open filename as fp and create buffer,chars and len to read the file line by line
	FILE *fp = fopen(filename, "r");
	char *buffer = NULL;
	size_t len = 0;
	int chars = 0;
	//index counter keeps track of the frame/position you are in during traversal
	int index_counter = 0;
	//create a dynamic array of page_container structs named array (how original of a name). set the name to -1 to signify empty spot in the frame,and count is 1000 for empty frames bevuase this is a number of counts I am confident wont happen
	page_container *array = malloc(sizeof(page_container) * size);
	for(int n = 0;n < size;n++){
		array[n].count = 5000;
		array[n].name = -1;
	}
	//read each line by line and save the line in the buffer until you hit EOF
	while((chars = getline(&buffer,&len,fp))!= -1){
		//check to make sure chars is not -1
		if(chars != -1){
			//operation and name take the name of the opaeration(R,W) and name(number after teh op) repsectively
			//operation determines the time units to be added if age misses
			char *operation;
			char *name;
			//split the string by space and store the first split string into operation and the second one into name
			operation = strtok(buffer, " ");
			name = strtok(NULL," ");
			//convert name to int element for easier searching
			int element = atoi(name);
			//search for element in array of size size. if element is not found, insert element into array at index_counter location and increment misses,index_counter,and hits by 1 and miss_time by 5
			if(search(array,size,element) == -1){
				insert(array,index_counter,element);
				misses++;
				index_counter++;
				miss_time = miss_time+5;
				//if operation is not R (it will be W) incrmeent miss_time and dirty_time by 10
				if(strcmp(operation,"R") != 0){
					miss_time = miss_time+10;
					dirty_time = dirty_time+10;
				}
				hits++;
			}
			//if element was found, just increment hits by 1
			else{
				hits++;
			}

		}
		//if index_counter becomes size, reset index_counter to 0 to mantain FIFO
		if(index_counter == size){
			index_counter = 0;
		}
	}
	//free everything that needs to be freed
	free(array);
	fclose(fp);
	free(buffer);
	//print the required stuff
	printf("%d\n",hits);
	printf("%d\n",misses);	
	printf("%d\n",miss_time);
	printf("%d\n",dirty_time);	
}
//LRU does LRU page replacment method and takes an inut of filename to read the file and size to find the number of frames, and prints the number of page references, misses, miss time and page writeback time. It is mostly a copy-paste of FIFO, so comments are a bit minimal or copy-paste
void LRU(char *filename,int size){
	//misses is the number of misses, miss-time is the time_unit count for misses, dirty_time is the page writeback time and hits is the number of page references. All of these vars are set to 0
	int misses,hits,miss_time,dirty_time;
	misses = 0;
	hits = 0;
	miss_time = 0;
	dirty_time = 0;
	//open filename as fp and create buffer,chars and len to read the file line by line
	FILE *fp = fopen(filename, "r");
	char *buffer = NULL;
	size_t len = 0;
	int chars = 0;
	//create a dynamic array of page_container structs named array (how original of a name). set the name to -1 to signify empty spot in the frame,and count is 1000 for empty frames bevuase this is a number of counts I am confident wont happen
	page_container *array = malloc(sizeof(page_container) * size);
	for(int n = 0;n < size;n++){
		array[n].count = 1000;
		array[n].name = -1;
	}
	//empty_space is the variable coutner that is checked until size is reached and a new pahse begins
	int empty_space = 0;
	//read each line by line and save the line in the buffer until you hit EOF
	while((chars = getline(&buffer,&len,fp))!= -1){
		if(chars != -1){
			//operation and name take the name of the opaeration(R,W) and name(number after teh op) repsectively
			//operation determines the time units to be added if age misses
			char *operation;
			char *name;
			//split the string by space and store the first split string into operation and the second one into name
			operation = strtok(buffer, " ");
			name = strtok(NULL," ");
			//convert name into int element
			int element = atoi(name);
			//search_index is the int that captures the position needs to be in
			int search_index;
			//while empty_space < size, treat empty_space as size, else use size as size. else statement wont be commented as it is very similar to the if block
			if(empty_space < size){
				//search for the element in array in a for loop of (empty_space + 1) size. If search is -1, find least recently used frame in array using the find_least_used function and insert into element into the array[find_least_used return value] and sets it count to 0 to imply it was recently used. increment the count in every array frame that exists and increment hits, misses by 1 and miss_time by 5
				if((search_index = search(array,empty_space+1,element)) == -1){
					//find the least recently used value here and return the index with it into least
					int least = find_least_used(array,empty_space+1);
					//insert element into array at least index
					insert(array,least,element);
					//increment the count of LRU
					for(int i = 0;i < (empty_space+1);i++){
						array[i].count++;
					}
					misses++;
					miss_time = miss_time+5;
					//if operation is W, incmrement miss_time and dirty_time by 10
					if(strcmp(operation,"W") == 0){
						miss_time = miss_time+10;
						dirty_time = dirty_time+10;
					}
					if(empty_space < size){
						empty_space++;
					}
					hits++;
				}
				//if you found element in array,set the count to 0 to indicate that the array[search_index] was just recently used. increment the hits too
				else{
					array[search_index].count = 0;
					hits++;		
				}
			}
			//same as if statement block, except size replaces size-1
			else{
				if((search_index = search(array,size,element)) == -1){
					int least = find_least_used(array,size);
					insert(array,least,element);
					for(int i = 0;i < size;i++){
						array[i].count++;
					}
					misses++;
					miss_time = miss_time+5;
					if(strcmp(operation,"W") == 0){
						miss_time = miss_time+10;
						dirty_time = dirty_time+10;
					}
					hits++;
				}
				else{
					array[search_index].count = 0;
					hits++;		
				}
			}		
		}
	}
	//free everything
	fclose(fp);
	free(array);
	free(buffer);
	//print everything that is needed
	printf("%d\n",hits);
	printf("%d\n",misses);
	printf("%d\n",miss_time);
	printf("%d\n",dirty_time);
}
//search takes an page_container array of array and while looping for size times, tries to find name. If name is found, return the index where name is located,else return -1 to indicate not found
int search(page_container *array, int size, int name){
	for(int i = 0;i < size;i++){
		if(array[i].name == name){
			return i;
		}
	}
	return -1;
}
//page_container insert function insert element name into array[index].name and sets array[index].count to 0, returing array[0] in the process
page_container insert(page_container *array, int index, int name){
	array[index].count = 0;
	array[index].name = name;
	return array[index];	
}
//int find_least_used finds the largest count in array of page_containers of size size and return the index that contains the largest count in array
int find_least_used(page_container *array, int size){
	//largest is the largest count, index is the default index
	int largest = 0;
	int index = 0;
	//while i is less than size find the largest count. if array[i].count saved in elemet is larger than element set index to i and largest to elemet
	for(int i = 0; i < size; i++){
		int element = array[i].count;
		if(element > largest){
			largest = element;
			index = i;
		}
	}
	return index;	
}

