/*

	submitted by:
				Shubham 	204101054
				Rahul Ballani   204101017

*/

#include<iostream>
#include <string>

using namespace std;

//structur of INODE
struct Inode{
	
	public:
	char name[8];
	int size;
	int used;
	int blockPointers[8];
	
	Inode(){
		used = 0;
	}
};


//class for superblock
class superBlock{
	
	public:
	short int freeBlockList[128];  //free block list
	Inode node[16]; //inode
	
	superBlock(){ //constructure
		
		freeBlockList[0] = 1;
		for(int i=1; i<128; i++){
			
			freeBlockList[i] = 0;
		}
		
		for(int i=0; i<16; i++){
			node[i].used = 0;
		}
	}
	
} ;


class Disk{
	
	public:
	int totalFile = 0;  //total file in system currently
	int freeblocks = 127; //total number ofsh free blocks
	
	char block[128][1024];
	
	
	superBlock sprblock;
	Disk(){  //constructor
		
		sprblock.freeBlockList[0] = 1;
		for(int i=1; i<128; i++){
			
			sprblock.freeBlockList[i] = 0;
		}
		
		for(int i=0; i<16; i++){
				sprblock.node[i].used = 0;
			}
		
		for(int i = 0; i< 128; i++){
			for(int j = 0; j<1024; j++)
				block[i][j] = '\0';
		}
	
	}
	
	void create(char fname[8], int fsize){  //Function to create file
		
		string str = fname;
		if(str.length() > 8){ //when file name is more than 8 character..
			str = str.substr(0,8);
		}
		
		//Checking If file is already there with the given name...
		for(int j=0; j<= 16; j++){
			
			if(sprblock.node[j].used == 1){
				
				string temp = sprblock.node[j].name;
				
				if(temp.length() > 8){
					temp = temp.substr(0,8);
				}
				if(str == temp){
					cout << "\nFile already there with " << str <<" name. Please change the file name\n";
					return;
				}
			
			}
			
		}
		
		if(freeblocks < fsize || totalFile == 16){  //when disk is full......
			
			cout <<"\n Not Enough Space.......\n";
		}
		else{
			
			int index = 0;
			for(int j=0; j <= 16; j++){
			
				if(sprblock.node[j].used == 0){ //when Inode is free...
					
					index = j;
					break;
				}
			
			}
			
			
			//write name of the file
			
			int k = 0;
			while(k < str.length()){
				sprblock.node[index].name[k] = str[k];
				k++;
			}
			sprblock.node[index].name[k] = '\0';
			
		
			if(str.length() < 8){
				sprblock.node[index].name[str.length()] = '\0';
			}
			sprblock.node[index].size = fsize;
			sprblock.node[index].used = 1;
			//cout << "\nfile created...." << sprblock.node[index].used << endl;
			//cout <<"IN LOOP\n";
			for(int i=0, j=1; i < fsize && j < 128; j++){
				
				if(sprblock.freeBlockList[j] == 0){
					//cout << "\nfile created...." << sprblock.node[index].used << endl;
					sprblock.node[index].blockPointers[i] = j;
					i++;
					freeblocks--;
					
				}
			}
			//cout <<"OUT LOOP\n";
			cout << "\n" << str << " created.....\n" << endl;
			totalFile++;
			
		}
		
	}
	
	void deleteFile(char fname[]){
		
		string str = fname;
		cout <<"\nFile to be deleted : " << str;
		
		if(str.length() > 8){
			str = str.substr(0,8);
		}
		
		int j=0;
		
		if(totalFile == 0){
			
			cout << "\nNo file in system currently\n\n";
			return;	
		}
		for(; j < 16; j++){
			
			string temp = sprblock.node[j].name;
			
			if(temp.length() > 8){
				temp = temp.substr(0,8);
			}
			
			if(str == temp){
				
				int size = sprblock.node[j].size;
				for(int i=0; i<size; i++){
					
					int index = sprblock.node[j].blockPointers[i];
					sprblock.freeBlockList[index] = 0;
					
				}
				totalFile--;
				sprblock.node[j].used = 0;
				cout << "\n" << str << " deleted.......\n\n";
				return;
			}
			
		}
		
		if(j == 16){
			cout <<"\n No Such File Exist\n\n";
		}
		
	}
	
	void read(char fname[], int blockNum, char buff[]){
		
		string str = fname;
		if(str.length() > 8){
			str = str.substr(0,8);
		}
		int j=0;
		for(; j< totalFile; j++){
			
			string temp = sprblock.node[j].name;
			if(temp.length() > 8){
				temp = temp.substr(0,8);
			}
			if(str == temp){
				
				if(sprblock.node[j].size <= blockNum){
					cout << "\nInvalid Block number....\n";
					break;
				}
				
				int index = sprblock.node[j].blockPointers[blockNum];
				cout <<"-------" << str <<": "<< "Block No: "<< blockNum <<"  ---------:\n";
				int i = 0;
				char c = block[index][i];
				
				do{
					c = block[index][i];
					cout << c;
					buff[i] = c;
					i++;
				}while(c != '\0' && i < 1024);
				cout <<"\n---------------------\n";
				break;
			}
			
		}
		
		if(j == totalFile){
			cout <<"\nNo Such File Exist\n\n";
		}
		
	}
	
	void write(char fname[], int blockNum, char buff[]){
		
		string str = fname;
		if(str.length() > 8){
			str = str.substr(0,8);
		}
		int j=0;
		for(; j< totalFile; j++){
			
			string temp = sprblock.node[j].name;
			if(temp.length() > 8){
				temp = temp.substr(0,8);
			}
			if(str == temp){
				
				if(sprblock.node[j].size <= blockNum){
					cout << "\nInvalid Block number....\n";
					break;
				}
				
				int index = sprblock.node[j].blockPointers[blockNum];
				//cout <<"File: " << str <<" :\n";
				int i = 0;
				char c ;
				
				do{
					block[index][i] = buff[i];
					c = buff[i];
					i++;
				}while(c != '\0' && i < 1024);
				
				cout << "\nData written Successfully........\n";
				
				break;
			}
			
		}
		
		if(j == totalFile){
			cout <<"\nNo Such File Exist\n\n";
		}
		
	}
	
	void is(){
		
		int f = 0;
		cout << "Total files is system currently: " << totalFile << endl;
		if(totalFile == 0){
			cout << "No file in system currently\n\n";
			return;
		}
		//cout << "\nfile created...." << sprblock.node[0].used << endl;
		cout <<"Files in system:\n";
		for(int j=0; j< 16; j++){
			
			//cout <<"\nzzzz " << j <<"    sprblock.node[j].used:  "<< sprblock.node[j].used ;
			if(sprblock.node[j].used == 1){
				
				//cout <<"\n File found.....\n";
				f=1;
				string str = sprblock.node[j].name;
				
				if(str.length() > 8){
					str = str.substr(0,8);
					//cout << "\n str: " << str <<endl;
				}
				
				cout << "\nFile name: " << str << endl;
				cout << "File size: " << sprblock.node[j].size << endl;
				
			
			}
			
		}
		cout <<"\n";
		
		
	}
	
};

int main(){
	
	
	Disk d;
	//int ch;
	
	while(1){
		
		char name[8];
		int blockNum;
		int size;
		char buff[1024];
		string str;
		int ch;
		
		cout <<"Enter your choice:\n1. Create File\n2. Delete File\n3. Read a File\n4. Write a File\n5. List all files in system\n6. Exit\n";
		cin >> ch;
		
		switch(ch){
			
			case 1: str.clear();
					cin.ignore();
					cout <<"\nEnter new File Name(without space): " ;
					cin.getline (name, 8, '\n');
					if(name[0] == '\0'){
						cout << "File name cannot be blank...\n";
						break;
					}
					/*
					cin >> str;
					
					for(int i = 0; i<8; i++){
						if(i < str.length())
							name[i] = str[i];
						else	
							name[i] = '\0';
					}
					*/
					cout << "Enter size: ";
					cin >> size;
					if(size > 8){
						cout << "Maximun file size allowed is 8 blocks....Try Again.....\n";
						break;
					}
					d.create(name, size);
					break;
					
			case 2: str.clear();
					cout <<"\nEnter the File Name to be deleted: " ;
					cin >> str;
					for(int i = 0; i<8; i++){
						if(i < str.length())
							name[i] = str[i];
						else	
							name[i] = '\0';
					}
					d.deleteFile(name);
					break;
			
			case 3: str.clear();
					cout <<"\nEnter File Name: " ;
					cin >> str;
					for(int i = 0; i<8; i++){
						if(i < str.length())
							name[i] = str[i];
						else	
							name[i] = '\0';
					}
					cout <<"\nEnter block number: " ;
					cin >> blockNum;
					d.read(name, blockNum, buff);
					break;
					
			case 4: str.clear();
					cout <<"\nEnter File Name: " ;
					cin >> str;
					for(int i = 0; i<8; i++){
						if(i < str.length())
							name[i] = str[i];
						else	
							name[i] = '\0';
					}
					cout <<"\nEnter block number: " ;
					cin >> blockNum;
					cout <<"Enter data(Give '$' as EOF):\n";
					cin.getline (buff, 1024, '$');
					/*
					cin >> str;
					for(int i=0; i < str.length() && i < 1024; i++){
						buff[i]=str[i];
					}
					*/
					d.write(name, blockNum, buff);
					break;
					
			case 5: d.is();
					break;
			
			case 6: exit(1);
			
			default: cout << "Invalid Input....\n";
						break;
					
					
		}
		
	}
	
	
	return 0;
}
