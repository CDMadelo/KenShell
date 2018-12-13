#include <conio.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

/*
*	Author : Madelo, Candace D.
*	A simulation of a Windows Command Line Written in C.
*
*/

//--------------------MAIN PROGRAM-------------------------

int main()	{
	os_version();
	run();
	return 0;
}
//--------------------------------------------------------

//--------------------------------------------------------
//This functions runs the program.
void run() {
	char command[1000];

	display_currDirectory(0);
	input_receiver(command);
	checker(command);
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function displays the current system's OS version.
void os_version() {
	DWORD version = 0;
	DWORD version1 = 0;
	DWORD version2 = 0;
	DWORD build = 0;
	int constant = 400;
	int constant1 = 4;
	int constant2 = 2;
	int constant3 = 5436;

	//gets the current Windows version.
	version = GetVersion();
	version1 = (DWORD)(LOBYTE(LOWORD(version))); 
	version2 = (DWORD)(HIBYTE(LOWORD(version))); 

	if(version < 0x80000000) {
		build = (DWORD)(HIWORD(version)) + constant;
	}
	printf("Microsoft Windows [Version 10.0.17134.407]\n", version1+constant1, version2-constant2, build+constant3);
	printf("(c) 2018 Microsoft Corporation. All rights reserved.\n");
	printf("\n");
	
}
//--------------------------------------------------------

//--------------------------------------------------------
// This function stores the input in a character array.
void input_receiver(char cmd[]) {
	gets(cmd);
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function clears out the screen in the command prompt.
void clear_screen(){
	HANDLE handleConsole;
	handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// This gets the number of character cells in the buffer.
 	if( !GetConsoleScreenBufferInfo( handleConsole, &csbi )){
    	return;
 	}

  	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// This fills the entire screen with blanks.
	if( !FillConsoleOutputCharacter(handleConsole,(TCHAR)' ',dwConSize,coordScreen,&cCharsWritten ) ){
	   return;
	}

	// This gets the text attribute.
	if( !GetConsoleScreenBufferInfo(handleConsole, &csbi ))	{
		return;
	}

	// This sets the buffer's attributes accordingly.
	if( !FillConsoleOutputAttribute(handleConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten ) ) {
		return;
	}

	// This puts the cursor at its correct coordinates.
	SetConsoleCursorPosition(handleConsole, coordScreen );
}
//--------------------------------------------------------

//===================================DATE FUNCTIONS================================================

//--------------------------------------------------------
//This function returns 1 if the date entered is valid, else
//returns 0 if not.
int date_format(char date[]) {
	char d[20], m[20], y[20];
	int day, month, year;
	int count;

	if(strstr(date, "/") != NULL) {
		if(3 != sscanf(date, "%d/%d/%d", &day, &month, &year)){
			return 0;
		}
	}else{
		if(3 != sscanf(date, "%d-%d-%d", &day, &month, &year)){
			return 0;
		}
	}

	if(strstr(date, "-") == NULL && strstr(date, "/") == NULL){
		return 0;
	}

	if(strlen(date) == 0)
		return 2;

	return (((day >= 1) && (day <= 31)) &&
		((month >= 1) && (month <= 12)) &&
		((year >= 0) && (year <= 9999)));
}
//--------------------------------------------------------

//--------------------------------------------------------
// This function gets the current date.
void current_Date(char *date)	{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("Current date: %d/%d/%d \n",  tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	printf("Enter new date: [DD-MM-YY] ");
	gets(date);

	enterdate(date);
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function lets the user enter date.
void enterdate(char *date) {
	if(strlen(date) != 0) {
		while(strlen(date) != 0 && 	date_format(date) != 1) {
			printf("Invalid date.\n");
			printf("Please enter the new date: [DD-MM-YY]: ");
			gets(date);
		}

		if(date_format(date) == 1) {
			printf("A required privilege is not held by the client.\n\n");
		}else{
			printf("\n");
		}
	}else{
		printf("\n");
	}
}
//--------------------------------------------------------
//=================================================================================================


//===================================TIME FUNCTIONS================================================

//--------------------------------------------------------
//This function returns 1 if the time entered is valid, else
//returns 0.
int time_format(char time[]) {
	int hr = 0;
	int min = 0;
	int sec = 0;
	int ms = 0;

	if(strstr(time, ":") != NULL) {
    sscanf( time, "%d:%d:%d.%d", &hr, &min, &sec, &ms );
    if(has_letters(time)){
      return 0;
    }
	}else{
		if(!check_number(time)){
			return 0;
		}
	}

  if(strlen(time) == 0)
		return 2;

	if(ms >= 0) {
		return (((hr >= 0) && (hr < 24)) &&
		((min >= 0) && (min < 60)) &&
		((sec >= 0) && (sec < 60)));
	} else {
		return 0;
	}	
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function sets the time.
void set_time(char *time) {
	while(strlen(time) != 0 && time_format(time) != 1) {
      printf("The system cannot accept the time entered.\n");
      printf("Please enter the new time:");
      gets(time);
    }

    if(time_format(time) != 2) {
        printf("A required privilege is not held by the client.\n\n");
    }else{
    	printf("\n");
	}	
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function displays the current time.
void get_time(char *time) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	printf("Current time is: %d:%d:%d.%02d\n", st.wHour, st.wMinute,st.wSecond,st.wMilliseconds);
  	printf("Enter the new time:");
  	gets(time);

  	set_time(time);

}
//--------------------------------------------------------
//=================================================================================================


//===================================FILE FUNCTIONS================================================

//--------------------------------------------------------
//This function opens a file displays the contents.
void file_open(char f_name[], int tok) {
	if(exist_file(f_name)) {
		FILE *ptr_file;
		char buf[1000];

		if(tok > 2){
			printf("%s\n",f_name);
			printf("\n\n");
		}

		ptr_file =fopen(f_name,"r");
		if (!ptr_file) {
//			return 1;
		}

		while (fgets(buf,1000, ptr_file)!=NULL)	{
				printf("%s",buf);
		}
		fclose(ptr_file);
		printf("\n");
	} else {
		char directory[1024];
		getcwd(directory, sizeof(directory));
		strcat(directory, "\\");
		strcat(directory, f_name);
		if(is_Directory(directory)){
			printf("Access is denied.\n\n");
		}else{
			if(tok > 2){
				printf("The system cannot find the specific file.\n");
				printf("Error while processing: %s\n\n",f_name);
			}else{
				printf("The system cannot find the specific file.\n\n");
			}
		}
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function return 1 if the file exists, else 0.
int exist_file(char f_name[]) {
	DIR *directory;
	struct dirent *dir;
	directory = opendir(".");
	int exists = 0;
	if (directory)	{
		while ((dir = readdir(directory)) != NULL)	{
	      if(strcmp(f_name, dir->d_name) == 0) {
	      	exists = 1;
	      }
	    }
	}
	closedir(directory);
	return exists;	
}
//--------------------------------------------------------

//-------------------------------------------------------
//This function renames a files.
void rename_file(char *original, char *new_name){
	if(exist_file(original)) {
		rename(original, new_name);
	}else{
		printf("The system cannot find specific file.\n");
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function deletes a file.
void delete_file(char *file, int tokens) {
	char directory[1024];
	char path[1024];
	char choice[100];
	int done = 0;
	getcwd(directory, sizeof(directory));
	getcwd(path, sizeof(directory));
	strcat(path, "\\");
	strcat(path, file);

	if(is_Directory(path)){
		strcat(directory, "\\");
		strcat(directory, file);
		strcat(directory, "\\");
		strcat(directory, "*");
		while(done == 0)	{
			printf("%s, Are you sure (Y/N)? ",directory);
			input_receiver(choice);
			strlwr(choice);
			if(strcmp(choice, "y") == 0 || strcmp(choice, "n") == 0){
				done = 1;
			}
		}
	}else{
		if(exist_file(file)){
			remove(file);
		}else{
			int result = remove(file);
			if(result != 0) {
				strcat(directory, "\\");
				strcat(directory, file);
				printf("Could Not Find %s\n", directory);
			}
		}
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function copies a file to another file.
void copy(char command[]) {
	FILE *file_pointer1;
	FILE *file_pointer2;
	int tocopy = 1, ask = 1, current_token = 2, files_copied = 0;
	int nTokens = no_of_tokens(command) - 1;
  	char *temp;
	char c;
	char cwd[1024];
	char input[1024];
	char nthToken[strlen(command)];
	char filename[strlen(command)];
  	getcwd(cwd, sizeof(cwd));

	if(nTokens == 2) {
		nth_token(command, filename, 2);
		file_pointer1 = fopen(filename, "r");

		if(file_pointer1 == NULL) {
			printf("The system cannot find the specific file.\n\n");
		} else {
			nth_token(command, nthToken, 3);
			file_pointer2 = fopen(nthToken, "r");
			if(file_pointer2 == NULL) {
				if(is_Directory(nthToken)) {
					strcat(cwd, "\\");
					strcat(cwd, nthToken);
					chdir(cwd);
					file_pointer2 = fopen(filename, "r");

					if(file_pointer2) {
						strcat(temp, nthToken);
						strcat(temp, "\\");
						strcat(temp, filename);
						while(ask){
							printf("Overwrite %s? (Yes/No/All): ", temp);
							gets(input);

							if(strstr(input, "yes") != NULL ||
						   		strstr(input, "y")  != NULL ||
						   		strstr(input, "all") != NULL||
						   		strstr(input, "a") != NULL) {
								tocopy = 1;
								ask = 0;
								break;
							}else if(strstr(input, "no") != NULL ||
								strstr(input, "n") != NULL) {
								tocopy = 0;
								ask = 0;
								break;
							} else {
								continue;
							}
						}
					}

					if(tocopy){
						file_pointer2 = fopen(filename, "w+");
						while((c = getc(file_pointer1)) != EOF) {
							fputc(c, file_pointer2);
						}
						files_copied++;
					}
					printf("\t%d file(s) copied.\n\n", files_copied);
					trim_path(cwd);
					change_directory(cwd);
				} else{
					nth_token(command, filename, 3);
					file_pointer2 = fopen(filename, "w+");
					while( (c = getc(file_pointer1)) != EOF) {
						fputc(c, file_pointer2);
					}
					files_copied++;
					printf("\t%d file(s) copied.\n\n", files_copied);
				}
			} else {
				while(ask){
					printf("Overwrite %s? (Yes/No/All): ");
					gets(input);

					if(strstr(input, "yes") != NULL ||
				   		strstr(input, "y")  != NULL ||
				   		strstr(input, "all") != NULL||
				   		strstr(input, "a") != NULL) {
						tocopy = 1;
						ask = 0;
						break;
					}else if(strstr(input, "no") != NULL ||
						strstr(input, "n") != NULL) {
						tocopy = 0;
						ask = 0;
						break;
					} else {
						continue;
					}
				}

				if(tocopy){
					nth_token(command, filename, 3);
					file_pointer2 = fopen(filename, "w+");
					while((c = getc(file_pointer1)) != EOF) {
						fputc(c, file_pointer2);
					}
					files_copied++;
				}
				printf("\t%d file(s) copied.\n\n", files_copied);

			}
		}
	}
	fclose(file_pointer1);
	fclose(file_pointer2);
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function moves a file to another file.
void move(char command[]) {
	FILE *file_pointer1;
	FILE *file_pointer2;
	int tocopy = 1, ask = 1, current_token = 2, files_copied = 0;;
	int nTokens = no_of_tokens(command) - 1;
  	char *temp;
	char c;
	char cwd[1024];
	char input[1024];
	char nthToken[strlen(command)];
	char filename[strlen(command)];
 	getcwd(cwd, sizeof(cwd));

	nth_token(command, filename, 2);
	file_pointer1 = fopen(filename, "r");

	if(file_pointer1 == NULL) {
		printf("The system cannot find the file specified.\n\n");
		return;
	}
	if(nTokens == 1) {
		files_copied++;
		printf("\t%d file(s) moved.\n\n", files_copied);
	} else {
		nth_token(command, nthToken, 3);
		file_pointer2 = fopen(nthToken, "r");

		if(file_pointer2 == NULL) {
			if(is_Directory(nthToken)) {
				strcat(cwd, "\\");
				strcat(cwd, nthToken);
				chdir(cwd);
				file_pointer2 = fopen(filename, "r");

					if(file_pointer2) {
						strcat(temp, nthToken);
						strcat(temp, "\\");
						strcat(temp, filename);
						while(ask){
							printf("Overwrite %s? (Yes/No/All): ", temp);
							gets(input);

							if(strstr(input, "yes") != NULL ||
						   		strstr(input, "y")  != NULL ||
						   		strstr(input, "all") != NULL||
						   		strstr(input, "a") != NULL) {
								tocopy = 1;
								ask = 0;
								break;
							}else if(strstr(input, "no") != NULL ||
								strstr(input, "n") != NULL) {
								tocopy = 0;
								ask = 0;
								break;
							} else {
								continue;
							}
						}
					}

					if(tocopy){
						file_pointer2 = fopen(filename, "w+");
						while((c = getc(file_pointer1)) != EOF) {
							fputc(c, file_pointer2);
						}
						files_copied++;
					}
					fclose(file_pointer1);
					trim_path(cwd);
					change_directory(cwd);
					remove(filename);
					printf("\t%d file(s) moved.\n\n", files_copied);

			} else {
				nth_token(command, filename, 3);
				file_pointer2 = fopen(filename, "w+");
				while( (c = getc(file_pointer1)) != EOF) {
					fputc(c, file_pointer2);
				}
				files_copied++;
				nth_token(command, filename, 2);
				fclose(file_pointer1);
				remove(filename);
				printf("\t%d file(s) moved.\n\n", files_copied);

			}
		} else {
			while(ask){
					printf("Overwrite %s? (Yes/No/All): ");
					gets(input);

					if(strstr(input, "yes") != NULL ||
				   		strstr(input, "y")  != NULL ||
				   		strstr(input, "all") != NULL||
				   		strstr(input, "a") != NULL) {
						tocopy = 1;
						ask = 0;
						break;
					}else if(strstr(input, "no") != NULL ||
						strstr(input, "n") != NULL) {
						tocopy = 0;
						ask = 0;
						break;
					} else {
						continue;
					}
				}

				if(tocopy){
					nth_token(command, filename, 3);
					file_pointer2 = fopen(filename, "w+");
					while((c = getc(file_pointer1)) != EOF) {
						fputc(c, file_pointer2);
					}
					files_copied++;
					fclose(file_pointer1);
					nth_token(command, filename, 2);
					remove(filename);
				}
				printf("\t%d file(s) moved.\n\n", files_copied);
		}
	}
	fclose(file_pointer2);
}
//--------------------------------------------------------
//==================================================================================================

//===================================STRING FUNCTIONS================================================

//--------------------------------------------------------
//This function checks if the string contains numbers only.
int check_number(char *num) {
	while(*num) {
		if(isdigit(*num++) == 0) {
			return 0;
		}
	}
	return 1;
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function checks if the string contains letters.
int has_letters(char *val) {
  while (*val) {
    if (isalpha(*val++) != 0){
    		return 1;
    }
  }
  return 0;
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function converts the string to lowercase.
void to_lower(char cmd[], char lc_COMMAND[]) {
	char s[strlen(cmd)];
	char* token;
	int c = 0;

	strcpy(s, cmd);
	token = strtok(s, " ");

  while (token) {
		if(c == 0){
			strcpy(lc_COMMAND, token);
			strlwr(lc_COMMAND);
		}
    token = strtok(NULL, " ");
		c++;
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function stores the token in string nthToken
void nth_token(char cmd[], char nthToken[], int n){
  char s[strlen(cmd)];
	char* token;
	int c = 1;

	strcpy(s, cmd);
	token = strtok(s, " ");

	while (token ) {
		if(c == n){
			strcpy(nthToken, token);
		}
    token = strtok(NULL, " ");
		c++;
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This functins counts the number of tokens in a string.
int no_of_tokens(char cmd[]) {
	char s[strlen(cmd)];
	char* token;
	int count = 0;

	strcpy(s, cmd);
	token = strtok(s, " ");
	while (token ) {
    token = strtok(NULL, " ");
		count++;
	}
	return count;
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function counts the number of occurences of a ch
//in a string.
int num_of_occurences(char *source, char c) {
	char *del = &source[strlen(source)]-1;
	int count = 0;

	 while (del > source){
			if( *del == c){
				count++;
			}
        del--;
    }
	return count;
}
//--------------------------------------------------------

//--------------------------------------------------------
//This functions checks if strins contains a ".".
int contains_dots(char *s) {
	while (*s) {
		if (*s++ != '.')
				return 0;
	}
	return 1;
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function deletes the last string in the given source
void trim_path (char* source){
		char *del = &source[strlen(source)];
		int slash_count = num_of_occurences(source, '\\');

    while (del > source && *del != '\\'){
        del--;
    }

    if (*del== '\\'){
				if(slash_count == 1){
					*del++;
				}
        *del= '\0';
    }
    return;
}
//--------------------------------------------------------
//===================================================================================================

//=================================DIRECTORY FUNCTIONS================================================

//--------------------------------------------------------
// This function displays the current working directory.
// The int choice value can either be 0 or 1. If 0, directory
// is displayed with '>'. If 1, the current directory is displayed.
void display_currDirectory(int choice) {
	char dir[1024];
	if (getcwd(dir, sizeof(dir)) != NULL){
			if(choice == 1)
				fprintf(stdout, "%s\n\n", dir);
			else
				fprintf(stdout, "%s>", dir);
   }
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function checks whether it is a directory or not.
int is_Directory(char target[]) {
	 struct stat statbuf;
   stat(target, &statbuf);
   return S_ISDIR(statbuf.st_mode);
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function changes the directory.
void change_directory(char path[]){
	if(chdir(path) != 0)	{
  	printf("The system cannot find the specific path.\n\n");
  }else{
    printf("\n");
  }
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function creates a new directory.
void make_directory(char *directory, int tokens) {
	if(_mkdir(directory) != 0){
		printf("A subdirectory or file %s already exists.\n",directory);
		if(tokens > 2){
		    printf("Error occured while processing: %s\n", directory);
		}
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This function deletes a directory.
void remove_directory(char *directory, int tokens) {
	if(rmdir(directory) != 0){
		if(_mkdir(directory) != 0) {
			printf("Directory is not empty.\n");
		}else{
			rmdir(directory);
			printf("The system cannot find  specific file.\n");
		}
	}
}
//--------------------------------------------------------

//--------------------------------------------------------
//This functions displays the files found in the given
//directory.
void display_directory(char *directory) {
	struct dirent *directory_entry;
	struct stat info;
	char path[1024];
	char *dayofWeek, *month, *day, *time, *year;
	int fsize = 0;
	int dsize = 0;
	int fileCount = 0;
	int dirCount = 0;
	strcpy(path, directory);
	DIR *dr = opendir(".");

	printf(" Volume in drive C is OS\n");
	printf(" Volume Serial Number is 98C8-FA90\n\n");
	printf(" Directory of %s\n\n",path);
	if (dr == NULL) {
			printf("Could not open current directory" );
	}

	while ((directory_entry = readdir(dr)) != NULL){
			char date[20];
			char time[20];
			int size;
			strcpy(path, directory);
			strcat(path, "\\");
			strcat(path, directory_entry->d_name);
			stat(path, &info);
			strftime(date, 20, "%d/%m/%y", localtime(&(info.st_ctime)));
			strftime(time, 20, "%I:%M %p", localtime(&(info.st_ctime)));

			if(is_Directory(path)){
				dirCount++;
				printf("%s  %s   <DIR> \t  %s\n",date,time,directory_entry->d_name);
				struct dirent *sub_directory_entry;
				DIR *sub_dr = opendir(path);
				if(sub_dr == NULL){

				}else{

				}
				dsize += info.st_size;
			}else{
				fileCount++;
				size = info.st_size;
				fsize += size;
				printf("%s  %s         %6d %s\n",date,time,size,directory_entry->d_name);
			}
	}
	printf("\t     %d File(s)    %d bytes \n", fileCount, fsize);
	printf("\t     %d Dir(s)     %d bytes free\n\n", dirCount, dsize);

	closedir(dr);
//	return 0;
}
//--------------------------------------------------------

//==========================================EVALUATION=================================================
//--------------------------------------------------------
//This function checks the command inputted by the user.
void checker(char command[]) {
	char  lc_COMMAND [500];
	int valid_command = 0;
	int tokens = no_of_tokens(command);
	to_lower(command,lc_COMMAND);

	//change directory (cd)
	if(strcmp(lc_COMMAND, "cd") == 0 || strcmp(lc_COMMAND, "cd..") == 0) {
		valid_command = 1;
		char *temp;
		char directory[1024];
		int dirCount = sscanf(lc_COMMAND, "cd%s;", &temp);

		getcwd(directory, sizeof(directory));
		if(tokens == 1 ){
			if(dirCount == 1)	{
				trim_path(directory);
				change_directory(directory);
			}else{
				display_currDirectory(1);
			}
		}else {
			char nthToken[strlen(command)];
			nth_token(command, nthToken, 2);
			if(tokens == 2 ) {
				if(strcmp(nthToken, "..") == 0 && dirCount != 1){
					trim_path(directory);
					change_directory(directory);
				}else{
					strcat(directory, "\\");
					strcat(directory, nthToken);
					change_directory(directory);
				}
			} else {
				strcat(directory, "\\");
				strcat(directory, nthToken);
				char nthToken[strlen(command)];
				int validNextTokens;
				int current_n = 2;
				nth_token(command, nthToken, 2);
				validNextTokens = contains_dots(nthToken);
				current_n++;

				if(validNextTokens == 0) {
					while(current_n <= tokens && validNextTokens == 1){
						nth_token(command, nthToken, current_n);
						validNextTokens = contains_dots(nthToken);
						current_n++;
					}
					if(validNextTokens == 1) {
						change_directory(directory);
					} else {
						printf("The system cannot find the specific path.\n\n");
					}
				}else {
					printf("\n");
				}
			}
		}
	}

	//change directory (chdir)
	if(strcmp(lc_COMMAND,"chdir") == 0 || strcmp(lc_COMMAND, "chdir..") == 0) {
		valid_command = 1;
		char *temp;
		char directory[1024];
		int chdirCount = sscanf(lc_COMMAND, "chdir%s;", &temp);
		getcwd(directory, sizeof(directory));
		if(tokens == 1 ){
			if(chdirCount == 1)	{
				trim_path(directory);
				change_directory(directory);
			}else{
				display_currDirectory(1);
			}
		}else {
			char nthToken[strlen(command)];
			nth_token(command, nthToken, 2);
			if(tokens == 2 ) {
				if(strcmp(nthToken, "..") == 0 && chdirCount != 1){
					trim_path(directory);
					change_directory(directory);
				}else{
					strcat(directory, "\\");
					strcat(directory, nthToken);
					change_directory(directory);
				}
			} else {
				strcat(directory, "\\");
				strcat(directory, nthToken);
				int validNextTokens = 1;
				int current_n = 2;
				nth_token(command, nthToken, 2);
				validNextTokens = contains_dots(nthToken);
				current_n++;

				if(validNextTokens == 0) {
					while(current_n <= tokens && validNextTokens == 1){
						nth_token(command, nthToken, current_n);
						validNextTokens = contains_dots(nthToken);
						current_n++;
					}
					if(validNextTokens == 1) {
						change_directory(directory);
					} else {
						printf("The system cannot find the specific path.\n\n");
					}
				}else {
					printf("\n");
				}
			}
		}
	}

	//clear screen (cls)
	if(strcmp(lc_COMMAND,"cls") == 0) {
		valid_command = 1;
		clear_screen();
		printf("\n");
	}

	//command (cmd)
	if(strcmp(lc_COMMAND,"cmd") == 0) {
		valid_command = 1;
		os_version();
	}

	//date
	if(strcmp(lc_COMMAND,"date") == 0) {
		valid_command = 1;
		char date[10];
		if(tokens <= 2) {
				if(tokens == 1){
					current_Date(date);
				}else{
					char nthToken[strlen(command)];
					nth_token(command,nthToken, 2);

					if(date_format(nthToken) == 0){
						enterdate(nthToken);
					}else{
						printf("A required privilege is not held by the client.\n\n");
					}

				}
		}
	}

	//time
	if(strcmp(lc_COMMAND,"time") == 0) {
		valid_command = 1;
		char time[24];

		if(tokens <= 2) {
				if(tokens == 1){
					get_time(time);
				}else{
					char nthToken[strlen(command)];
					nth_token(command,nthToken, 2);

					if(time_format(nthToken) == 0){
						set_time(nthToken);
					}else{
						printf("A required privilege is not held by the client.\n\n");
					}

				}
		}
	}

	//mkdir
	if(strcmp(lc_COMMAND,"mkdir") == 0) {
		char nthToken[strlen(command)];
		valid_command = 1;
		if(tokens >= 2) {
			int nTokens = tokens - 1;
			int current_token = 2;
			while(nTokens != 0){
				nth_token(command, nthToken, current_token);
        make_directory(nthToken,tokens);
				nTokens--;
				current_token++;
			}
		} else {
			if(tokens == 1){
				printf("The syntax of the command is incorrect.\n\n");
			}
		}
		printf("\n");
	}

	//rmdir
	if(strcmp(lc_COMMAND,"rmdir") == 0) {
		char nthToken[strlen(command)];
		valid_command = 1;
		if(tokens >= 2) {
			int nTokens = tokens - 1;
			int current_token = 2;
			while(nTokens != 0){
				nth_token(command, nthToken, current_token);
        remove_directory(nthToken,tokens);
				nTokens--;
				current_token++;
			}
		} else {
			if(tokens == 1){
				printf("The syntax of the command is incorrect.\n\n");
			}
		}
		printf("\n");
	}

	//delete
	if(strcmp(lc_COMMAND,"del") == 0) {
		char nthToken[strlen(command)];
		valid_command = 1;
		if(tokens >= 2) {
			int nTokens = tokens - 1;
			int current_token = 2;
			while(nTokens != 0){
				nth_token(command, nthToken, current_token);
        delete_file(nthToken,tokens);
				nTokens--;
				current_token++;
			}
		} else {
			if(tokens == 1){
				printf("The syntax of the command is incorrect.\n\n");
			}
		}
		printf("\n");
	}

	//rename
	if(strstr(lc_COMMAND,"rename") != NULL) {
		char original[strlen(command)];
		char new_name[strlen(command)];
		char *temp;
		int renameCount = (lc_COMMAND, "rename%s;", &temp);
		valid_command = 1;
		if(tokens == 3) {
			nth_token(command,original, 2);
			nth_token(command,new_name, 3);
			rename_file(original, new_name);
		} else {
			printf("The syntax of the command is incorrect.\n\n");
		}
		printf("\n");
	}

	//directory (dir)
	if(strstr(lc_COMMAND, "dir") != NULL) {
		valid_command = 1;
		char *temp;
		char directory[1024];
		int dirCount = sscanf(lc_COMMAND, "dir%s;", &temp);

		getcwd(directory, sizeof(directory));
		if(tokens == 1 ){
			if(strcmp(lc_COMMAND, "dir..") == 0 || strcmp(lc_COMMAND, "dir") == 0){
				display_directory(directory);
			}else{
				printf(" Directory of %s\n");
				printf("File Not Found\n");
			}
			if(dirCount == 1)	{
				trim_path(directory);
				change_directory(directory);
			}else{
				display_currDirectory(1);
			}
		}
	}

	//copy
	if(strstr(lc_COMMAND, "copy") != NULL) {
		valid_command = 1;
		char *temp;
		int copyCount = sscanf(lc_COMMAND, "copy%s;", &temp);

		if(tokens == 1 && copyCount != -1) {
			printf("The syntax of the command is incorrect.\n");
		}

		if(tokens == 2) {
		  printf("The file cannot be copied onto itself.\n");
		} else {
		  copy(command);
		}
	}

	//move
	if(strstr(lc_COMMAND, "move") != NULL) {
		valid_command = 1;
		char *temp;
		int moveCount = sscanf(lc_COMMAND, "move%s;", &temp);

		if(tokens == 1 && moveCount != -1) {
			printf("The syntax of the command is incorrect.\n");
		} else {
		  move(command);
		}
	}

	//type
	if(strcmp(lc_COMMAND,"type") == 0) {
		valid_command = 1;
		if(tokens >= 2) {
			int nTokens = tokens - 1;
			int nthToken = 2;
			while(nTokens != 0){
				char file_name[strlen(command)];

				nth_token(command,file_name, nthToken);
				file_open(file_name, tokens);
				nTokens--;
				nthToken++;
			}
		} else {
			if(tokens == 1){
				printf("The syntax of the command is incorrect.\n\n");
			}
		}
	}

	//Exit
	if(strcmp(lc_COMMAND,"exit") == 0) {
		valid_command = 1;
		return 0;
	}

	if(valid_command !=	 1){
		printf("'%s' is not recognized as an internal or external command,\noperable program or batch file.\n\n", lc_COMMAND);
	}
	run();
}
//--------------------------------------------------------























