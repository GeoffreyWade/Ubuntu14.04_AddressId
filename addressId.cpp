//*****************************************************************************
// This addressId.cpp class is created to input the output from   
// the netstat -tanp command and create a shell script that will be executed
// performing the ip address look up from the output of the 
// netstat -tanp command.  
// author:GeoffWade
//*****************************************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstdlib>
#include <locale>

using namespace std;

void LogAddress(list<string> addressList);

int main()
{

	// create string variables
	string line;
	string holdWord = "";

	// create string variables to be used as test vars in regs
	string theTcp = "tcp";
	string theSix = "tcp6";
	string firstLn = "0.0.0.0";
	 
	// create list<string> container object to hold intaking strings
	list<string> addressList;

	// create integer variables
	int index;
	int cardex;
	int count = 0;
	
	// use the system function to execute the netstat command and the data
	system("sudo netstat -tanp > /usr/local/addressid/Ntstat.txt");
	
	// create an ifstream object 
	ifstream inFile("/usr/local/addressid/Ntstat.txt");
	
	while ( getline( inFile, line ) ) {
		
		if ( !line.compare(44,7,firstLn) == 0 ) {

			if ( !line.compare(0,4,theSix) == 0 ) {

				if ( line.compare(0,3,theTcp) == 0 ) {
			
					// find the index position of the line at which point it ends 
					cardex = line.find('\0');

					// store the substring of the line based on index positions
					string intermStr = line.substr(44,cardex);

					// locate the index position of the substring at which a colon
					// resides
					index = intermStr.find(":");

					// create a second substr out of the first substring.
					holdWord = intermStr.substr(0,index);
					addressList.push_back(holdWord);

					// null the holdWord var and increment the count integer
					holdWord = "";				
					count++;
				}

			}
		
		}		
		
	}
	
	inFile.close();
	cout << "The data has been inputted to the program!" << endl;
			
	// date the log file
	system("date >> /var/log/addressid/addressid.log");
		
	// call the LogAddress function
	LogAddress(addressList);

	// clear the list as verification
	addressList.clear();

    // zero out the count var
	count = 0;

	// output a statement to the user of the binary
	cout << "The operation has completed successfully!" << endl;
	return 0;

}

void LogAddress(list<string> addressList) {

	int listsiz = 0; // create an integer var for the terminating condition
	listsiz += addressList.size();

	string wordHold = "";
	
	for (int j = 0; j < listsiz; j++) {

		string outHold = addressList.front();
		addressList.pop_front();
		wordHold = outHold;
		
		system(("whois "+wordHold+" >> /var/log/addressid/addressid.log").c_str()); 
			   
		wordHold = "";

	}
	listsiz = 0;

}
/**
	The system admin must create the /usr/local/addressid and the
	/var/log/addressid directories, and place the binary in
	a directory of systems path.
*/
