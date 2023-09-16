# include <stdlib.h>
# include <string.h>
# include <string>
# include <fstream>
# include <vector>
# include <iomanip>
# include <iostream>
using namespace std ;
class DS06_tree{
	protected :
		struct DATA{
			int i ;                 //�ĴX�����
			int ID ;    			//�s��
			char Name[50] ;         // �W�r
			char Type1[20] ;        // ����1
			int HP ;                // �ͩR��
			int Attack ;            // ����
			int Defense ;           // ���m
			char others[50] ;
			DATA * left = NULL ;
			DATA * right = NULL ;
			DATA * next = NULL ;
		};
		
	private :
		DATA data ;
		DATA * head = NULL ;
		DATA * walk = head ;

	public:
		bool Is_num ( int &num ) {                                             // command���b (�ư��Ʀr�H�~����J)
   			string numstr ;
   			cin >> numstr ;
			if ( numstr == "0" ) {                                             // ��J������0
				num = 0 ;
	 			return true ;
 			} // if

			num = atoi( numstr.c_str() ) ;
  			if ( num == 0 ) return false ;
  			else return true ;
		} // Is_num()
		
		void PrintOne( DATA * temp ) {                                                         // �L�@��
			cout << "        #       Name                            Type 1          HP      Attack  Defense" << endl ;
			cout << "[" << right << setw(2) << temp->i << "]\t" ;
			cout << left << setw(8) << temp->ID ;
			cout << left << setw(32) << temp->Name ;
			cout << left << setw(16) << temp->Type1 ;
			cout << left << setw(8) << temp->HP ;
			cout << left << setw(8) << temp->Attack ;
			cout << temp->Defense << endl ;
			cout << endl ;
		} // PrintOne()
		
		void Print() {                                                         // �L����
			walk = head ;
			cout << endl << "        #       Name                            Type 1          HP      Attack  Defense" << endl ;
			while ( walk != NULL ) {
				cout << "[" << right << setw(2) << walk->i << "]\t" ;        // seyw ���
				cout << left << setw(8) << walk->ID ;
				cout << left << setw(32) << walk->Name ;
				cout << left << setw(16) << walk->Type1 ;
				cout << left << setw(8) << walk->HP ;
				cout << left << setw(8) << walk->Attack ;
				cout << walk->Defense << endl ;
				walk = walk->next ;
			} // while

			cout << endl ;
		} // Print()
		
		void DataGet( string filename, bool & success ) { // �N��Ƥ@�����s�Jnext���w
			delete head ;                                 // �C��get data ���e�A�����M��head���w
			fstream file ;
			int i = 1 ;
			char str[500] ;
			char ch ;
			file.open( filename.c_str(), ios::in ) ;
			if ( !file ) {
				cout << "### " << filename << " does not exist! ###" << endl ;
				return ;
			} // if
			success = true ;
            file.getline( str, sizeof(str) ) ;
            DATA * temp = new DATA ;
            temp->next = NULL ;
			while ( file >> temp->ID ) {
				temp->i = i ;
				file >> temp->Name ;
				ch = file.get() ;
				if ( ch == ' ' ) {
					file >> temp->others ;
					strcat( temp->Name, " " ) ;
					strcat( temp->Name, temp->others ) ;
				} // if
				file >> temp->Type1 ;
				file.get() ;                                         //Ū���U�@��char
				if ( file.peek() != '\t' ) file >> temp->others ;    //�Y�O�U�@�Ӥ��O\t�A�~�i�HŪ�@����ƶiothers
				file.get() ;
				if ( file.peek() != '\t' ) file >> temp->others ;
				file >> temp->HP ;
				file >> temp->Attack ;
				file >> temp->Defense ;
				file.getline( str, sizeof(str) ) ;

				if ( head == NULL ) {
					head = temp ;
					walk = head ;
				} // if
				else {
					walk->next = temp ;
					walk = walk->next ;
				} // else
				temp = new DATA ;
				temp->next = NULL ;
				i++ ;
			} // while
			file.close() ;
		} // DataGet

		void Build_HP_tree( int num, int now, int &max, int height ) { // �إߥ�left�Mright���w�զ����G���j�M��(HP<-�P�_)
			if ( num < now && walk->left != NULL ) {
				walk = walk->left ;
				height++ ;
				now = walk->HP ;
				Build_HP_tree( num, now, max, height ) ;
			} // if
			else if ( num >= now && walk->right != NULL ) {
				walk = walk->right ;
				height++ ;
				now = walk->HP ;
				Build_HP_tree( num, now, max, height ) ;
			} // else if
			else if ( num < now && walk->left == NULL ) {
				walk->left = head->next ;
				head->next = head->next->next ;
				walk->left->next = NULL ;
				height++ ;
				if ( max < height ) max = height ;
			} // else if
			else if ( num >= now && walk->right == NULL ) {
				walk->right = head->next ;
				head->next = head->next->next ;
				walk->right->next = NULL ;
				height++ ;
				if ( max < height ) max = height ;
			} // else if
		} // Build_HP_tree()
		
		DATA * Min( DATA * walk ) {
			DATA * temp = walk ;
			while ( temp->left != NULL ) {
				temp = temp->left ;
			} // while

			return temp ;
		} // Min()
		
		DATA * Max( DATA * walk ) {
			DATA * temp = walk ;
			while ( temp->right != NULL ) {
				temp = temp->right ;
			} // while

			return temp ;
		} // Max()

		bool Mission1( bool & success, string &filename ) {
			cout << endl << "Input a file number [0: quit]: "  ;
			cin >> filename ;
			if ( filename.compare( "0" ) == 0 ) {
				success = true ;
				return false  ;
			} // if
			filename = "input" + filename + ".txt" ;
			head = NULL ;
			DataGet( filename, success ) ;
			if ( !success ) return true ;
			Print() ;
			int max = 1 ;
			walk = head ; // �^��head�A�qhead�}�l��
			do {
				Build_HP_tree( head->next->HP, walk->HP, max, 1 ) ; // head/walk do
				walk = head ;
			} while ( walk->next != NULL ) ;

			cout << "HP tree height = " << max << endl << endl ;
			
			DATA * temp = Min( head ) ;
			cout << "Leftmost node:" << endl ;
			PrintOne( temp ) ;
			
			
			temp = Max( head ) ;
			cout << "Rightmost node:" << endl ;
			PrintOne( temp ) ;
			
			return true ;
		} // Mission1

}; // class DS06_tree

class DS06_heap : protected DS06_tree {
	private :
		vector <DATA> heap ;
		DATA data ;
		
	public :
		
		void Change( int i, int j ) {                                          // �洫����
			heap[i].i = j ;
			heap[j].i = i ;
			swap( heap[i], heap[j] ) ;
		} // change()
		
		void Dataget( string filename ) {
			heap.clear() ;                                                     //�C�� get data���e�A�����M��heap
			fstream file ;
			int i = 0 ;
			char str[500] ;
			char ch ;
			file.open( filename.c_str(), ios::in ) ;
			if ( !file ) {
				cout << "### " << filename << " does not exist! ###" << endl ;
				return ;
			} // if
            file.getline( str, sizeof(str) ) ;

			while ( file >> data.ID ) {
				data.i = i ;
				file >> data.Name ;
				ch = file.get() ;
				if ( ch == ' ' ) {
					file >> data.others ;
					strcat( data.Name, " " ) ;
					strcat( data.Name, data.others ) ;
				} // if
				file >> data.Type1 ;
				file.get() ;
				if ( file.peek() != '\t' ) file >> data.others ;
				file.get() ;
				if ( file.peek() != '\t' ) file >> data.others ;
				file >> data.HP ;
				file >> data.Attack ;
				file >> data.Defense ;
				file.getline( str, sizeof(str) ) ;
				heap.push_back( data ) ;
				i++ ;
			} // while
			file.close() ;

		} // Dataget()
		
		void Heap( int fa , string filename ) {
			int child1 = (fa+1)*2 ;                             //���`�I�k�䪺�l�`�I
			int child2 = child1-1 ;                             //���`�I���䪺�l�`�I
			if ( child1 >= heap.size() && child2 >= heap.size() ) return ;               //�Y�O�S���l�`�I�A�N����
			if ( filename != "input606.txt" ) {
				if ( heap[fa].HP < heap[child1].HP && heap[child1].HP >= heap[child2].HP ) { // �H�U��X�T�̳̤j����Ʃ�J���`�I�A�m���û��j
				   Change( fa, child1 ) ;
				   Heap( child1, filename ) ;
				} // if
				else if ( heap[fa].HP < heap[child2].HP ) {
					Change( fa, child2 ) ;
					Heap( child2, filename ) ;
				} // else if
			} // if
			else {
				if ( heap[fa].HP < heap[child2].HP && heap[child2].HP >= heap[child1].HP ) { // �H�U��X�T�̳̤j����Ʃ�J���`�I�A�m���û��j
				   Change( fa, child2 ) ;
				   Heap( child2, filename ) ;
				} // if
				else if ( heap[fa].HP < heap[child1].HP ) {
					Change( fa, child1 ) ;
					Heap( child1, filename ) ;
				} // else if
			} // else
			

		} // Heap()
		
		void Print_Heap( int num1, int num2 ) {
			cout << endl << "        #       Name                            Type 1          HP      Attack  Defense" << endl ;
			for ( int j = num1; j < num2; j++ ) {
        		cout << "[" << right << setw(2) << heap[j].i << "]\t" ;
				cout << left << setw(8) << heap[j].ID ;
				cout << left << setw(32) << heap[j].Name ;
				cout << left << setw(16) << heap[j].Type1 ;
				cout << left << setw(8) << heap[j].HP ;
				cout << left << setw(8) << heap[j].Attack ;
				cout << heap[j].Defense << endl ;
			} // for

		} // Print_Heap()
		
		bool Mission2( string filename, int command ) {
			int left = 0, i = 0 ;
			if ( command != 3 ) {
				Dataget( filename ) ;
				for ( int i = heap.size() -1 ; i >= 0 ; i-- ) Heap( i, filename ) ; // ��Max_Heap
			} // if
			else Heap(0, filename ) ;

			
			Print_Heap( 0, heap.size() ) ;
			int j = heap.size() ;
			int h = 0 ;
			for ( i = 1 ; j > 0 ; i = i*2 ) { // �p��� 
				j = j - i ;
				left = left + i ;
				h++ ;
			} // for
			
			left = left - i/2 ;
			cout << "HP heap height = " << h << endl ;
			cout << endl << "Leftmost node: "  ;
			Print_Heap( left, left+1 ) ;
			cout << endl << "Bottom: " ;
			Print_Heap( heap.size()-1 , heap.size() ) ;
			cout << endl << endl ;
			return true ;
		} // Mission2()

		void Mission3 ( string filename, bool & do2 ) {
			cout << "The removed root:" << endl ;
			cout << "        #       Name                            Type 1          HP      Attack  Defense" << endl ;
			cout << "\t" << left << setw(8) << heap[0].ID ;
			cout << left << setw(32) << heap[0].Name ;
			cout << left << setw(16) << heap[0].Type1 ;
			cout << left << setw(8) << heap[0].HP ;
			cout << left << setw(8) << heap[0].Attack ;
			cout << heap[0].Defense << endl ;
			Change( 0, heap.size()-1 ) ;
			heap.erase( heap.end() ) ;
			if ( heap.size() > 0 ) {
				for ( int i = 0 ; i < heap.size() ; i++ ) heap[i].i = i ;
				Mission2( filename, 3 ) ;
			} // if
			else {
		        cout << endl ;
				do2 = false ;
			} // else
			
		} // Mission3()

} ;

int main() {
	DS06_tree ds06 ;
	DS06_heap ds061 ;
	bool do1 = false ;
	bool do2 = false ;
	bool success = false ;
	string filename ;
	while( true ) {
		cout << "***** Pokemon Tree and Heap *****" << endl ;
		cout << "* 0. QUIT                       *" << endl ;
		cout << "* 1. Read a file to build BST   *" << endl ;
		cout << "* 2. Transform it into Max Heap *" << endl ;
		cout << "*********************************" << endl ;
		cout << "* 3. Delete max from Max Heap   *" << endl ;
		cout << "*********************************" << endl ;
		cout << "Input a choice(0, 1, 2, 3): " ;

		int command ;
  	  	if ( !ds06.Is_num( command ) ) cout << "Command does not exist!" << endl << endl ;
  	 	else if ( command > 3 || command < 0 ) cout << "Command does not exist!" << endl << endl ;
 		else if ( command == 0 ) break ;
  	  	else if ( command == 1 ) {
  	  		do do1 = ds06.Mission1( success, filename ) ;
  	  		while ( !success ) ;
		} // else if
  	  	else if ( command == 2 ) {
  	  		if ( !do1 ) cout << endl << "----- Execute Mission 1 first! -----" << endl << endl ;
  	  		else do2 = ds061.Mission2( filename, 2 ) ;
		} // else if()

  	  	else if ( command == 3 ) {
  	  		if ( !do1 ) cout << endl << "----- Execute Mission 1 first! -----" << endl << endl ;
  	  		else if ( !do2 ) cout << endl << "----- Execute Mission 2 first! -----" << endl << endl ;
  	  		else ds061.Mission3( filename, do2 ) ;
		} // else if

	} // while
} // main()
