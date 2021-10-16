//
//  main.cpp
//  Assignment #2
//
//  Created by Rameen Mahmood on 11/10/2019.
//  Copyright © 2019 ENGR-AD101. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;
#define SIZE 7 //size of the array

int printMenu();
void encodeMessageStream(string);
void generateHammingCode(bool [], int);
void decodeMessageStream(string);
int checkMessageParity(bool [], int);
int main()
{
    string encoded_file;
    string decoded_file;
    int choice=0;

    do
    {
        choice = printMenu(); //getting the value from the function into the variable choice
    if (choice == 1)
    {
        cout<<"Enter File Name for the messages to be encoded"<<endl;
         cin>>encoded_file;
         encodeMessageStream(encoded_file); //passing the filename into the funciton
        cout<<"Messages are successfully encoded, check encoded messages file"<<endl;
    }
    else if (choice == 2)
    {
        cout<<"Enter File Name for the messages to be decoded"<<endl;
        cin>>decoded_file;
        decodeMessageStream(decoded_file);
        cout<<"Messages are successfully decoded, check decoded messages file"<<endl;
    }

    }while (choice!=3);
    cout<<"Program terminating"<<endl;
    exit(0);

    return 0;
}
int printMenu()
{
    int selection=0;
       cout<<"Select from the following menu"<<endl;
       cout<<"1) Encode a message"<<endl;
       cout<<"2) Decode a message"<<endl;
       cout<<"3) Exit"<<endl;
    cout<<"Please make a selection: ";
       cin>>selection;

    while((selection !=  1) && (selection !=2) && (selection != 3)) //validating the user input
    {
        cout<<"Invalid entry, enter again"<<endl;
        cin>>selection;
    }
       return selection;
}
void encodeMessageStream(string filename) //to encode each message
{

    bool message[SIZE];
    char c;
    ifstream encoded_file; //to read the file
    encoded_file.open(filename);
    if (encoded_file.fail())
    {
        cerr<<"File failed to open"<<endl;
        exit(-1);
    }
    ofstream newfile; //creating and opening new file to write
    newfile.open("encodedMessages.txt");
    if (newfile.fail())
    {
        cerr<<"File failed to open"<<endl;
        exit(-2);
    }

    while (!encoded_file.eof()) //reading the file into the array
    {
        for (int j=0;j<=4;j++) //storing 4 bits each time from the file
        {
            if (j<4)
            {
                message[j]=encoded_file.get()-48; //subtracting the ascii value of zero as .get() will get the ascii value of the contents of the file
            }
            if (j==4)
            {
                c=encoded_file.get();
                c=encoded_file.get(); // to get the eof character
            }
            if (j==3)
            {
                message[j+1]=message[j]; // message[4]=message[3]
            }
        }
        generateHammingCode(message, SIZE); // pass the array and its size to generate values for the rest of the indexes of the array (parity bits)
        cout<<"Encoded message: ";
    for (int i=0;i<SIZE;i++) //to output the array and its contents
    {
        cout << message[i];
        newfile << message[i]; // writing the file with the contents of the array
    }
        cout<<endl; //to produce a new line after 7 bits

}
    encoded_file.close(); // closing the reading file
    newfile.close(); //closing the writing file
}
void generateHammingCode (bool array[], int size) //to calculate the parity bits
{
    array[3]=array[0]^array[1]^array[2]; // calculating p1 using XOR
    array[5]=array[0]^array[1]^array[4]; // calculating p2
    array[6]=array[0]^array[2]^array[4]; //calculating p3
}

void decodeMessageStream(string filename)
{
    bool encodemessage [SIZE];
    bool decodemessage [SIZE];
    char c;
    ifstream encoded_file;
    int message_status;
    ofstream output_file;
    output_file.open("decodedMessages.txt");
    if (output_file.fail())
    {
        cerr<<"Error opening the  file"<<endl;
        exit(-1);
    }
    encoded_file.open(filename);
    if (encoded_file.fail())
    {
        cerr<<"Error in opening the file"<<endl;
        exit(-2);
    }
    cout<<"Encoded|Corrected|Decoded|Status"<<endl;
    while(!encoded_file.eof())
    {
        for (int i=0;i<=SIZE;i++) // reading the file into the array
        {

               if (i<SIZE)
                {
                encodemessage[i]=encoded_file.get()-48;
                cout<<encodemessage[i]; //outputting the array
                }
                else if (i==SIZE)
                {
                    c=encoded_file.get(); //getting the eof character
                }

        }
      message_status = checkMessageParity(encodemessage, SIZE); //getting the bitposition

    if ((message_status != -1) && (encodemessage[message_status-1]==1)) //we do -1 as message status tells us the bit position, so for index we will have to minus 1.
       {
           encodemessage[message_status-1]=0;
           output_file << encodemessage[message_status-1];
       }
       else if ((message_status !=-1) && (encodemessage[message_status-1]==0))
       {
           encodemessage[message_status-1]=1;
           output_file << encodemessage[message_status-1];
       } // these if statements are to correct the wrong codes at the particular bit positions
        cout<<"|";
        for (int h=0;h<SIZE;h++)
        {
            cout<<encodemessage[h];
            output_file << encodemessage[h]; //writing the contents of the array into the file
        }
        cout<<"|";
        output_file<<"|";
        for (int i=0;i<4;i++) //decoding the messages
        {

         if (i==3)
            {
                decodemessage[i]=encodemessage[i+1]; //decodemessage[3]=decodemessage[4]
            }
         else if (i!=3)
         {
            decodemessage[i]=encodemessage[i];
         }
            output_file << decodemessage[i];
            cout<<decodemessage[i];
        }

    if (message_status==-1)
    {
        cout<<"|";
        cout<<"NO_ERROR"<<endl;
        output_file<<"|"<<"NO_ERROR"<<endl; //writing the message status into the file

    }
    else if (message_status!=-1)
    {
        cout<<"|";
        cout<<"ERROR_"<<message_status<<endl;
        output_file<<"|"<<"ERROR_"<<message_status<<endl;
    }
    }
}

int checkMessageParity(bool array[], int size) //verifying if there are any wrong bits and getting the bit positions of the wrong bits
{
    int p1,p2,p3;
    int bit_position;

    p1=array[6]^array[0]^array[2]^array[4]; //now we will also use the parity bit we calculated before to verify the results
    p2=array[5]^array[0]^array[1]^array[4];
    p3=array[3]^array[0]^array[1]^array[2];
    bit_position = p1 + p2*2 +p3*4; // to calculate the decimal value we will add the values of 2 raise to the power 0,1 and 2.

    if (bit_position==0) // if there are no errors, bit position will be 1
    {
        bit_position=-1; //to differentiate the bit position of the right message from the wrong message
    }

    return bit_position;
}
