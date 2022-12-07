#include "main.h"


using namespace std;

char output[MAX_DATA_LENGTH]; //MAX_DATA_LENGTH fra .h-filen
char port[] = "\\\\.\\COM8"; //skfit til rigtig com-port
char incoming[MAX_DATA_LENGTH];

Morser testSender;
uint8_t* testSenderAllAttrPtr;
const uint8_t* messPtr = nullptr;

int main()
{
    system("Color 0A");

    int i = 0;

    char selection = 0;
    int speed = 8; //8 er 1 i sekundet
    string message = "besked her";
    int lowIntensity = 4;
    int highIntensity = 24;
    int tSpeed = 0;
    string allAttributes;

/*
    Morser mSpeed;
    Morser mMessage;
    Morser mOnVel;
    Morser mOffVel;
    Morser mAllAttrPtr;
    */


    //char dummy[] = { 1,0,0,1,0,0,1,1,1,0,0,1,1 };
    //int i = 0; //sizeof(dummy) / sizeof(dummy[0])

    do
    {
        system("CLS");
        cout << "  JUNO Communication 0.3 - COM8\n";
        cout << "                                   _________________\n";
        cout << "  --------------------------------| Current Values: |\n";
        cout << "  1.  Select speed                |" << speed << "         |" << endl;
        cout << "  2.  Select message              |" << message << "       |" << endl;
        cout << "  3.  Select intensity            |" << lowIntensity << "  " << highIntensity << endl;
        cout << "  4.  Show current attributes\n";
        cout << "  5.  Update attributes\n";
        cout << "\n";
        cout << "  6.  Exit\n";
        cout << "  7.  Test area\n";
        cout << "  ------------------------------------\n";
        cout << "  Enter your selection: ";
        cin >> selection;
        cout << endl;

        switch (selection)
        {
        case '1':
            system("CLS");
            cout << "Select speed:\n";
            cout << "Range from 1-31, 8 is once per second\n";

            cin >> speed; 
            testSender.setSpeed(speed);
            //mSpeed.setSpeed(speed); //virker det her?

            cout << "\n";
            break;

        case '2':
            system("CLS");
            cout << "Select message:\n";

            getline(cin >> ws, message); //remove whitespace for blank spaces in message
            testSender.setMessage(message);
            //mMessage.setMessage(message);

            cout << "\n";
            break;

        case '3':
            system("CLS");
            cout << "Select low intensity:\n";
            cout << "Range from 1-31\n";
                
            cin >> lowIntensity;
            testSender.setOffIntensity(lowIntensity);
            //mOffVel.setOffIntensity(lowIntensity);
            //setOffIntensity(lowIntensity);
            
            cout << "\n";
            
            cout << "Select high intensity:\n";
            cout << "Range from 1-31\n";
            
            cin >> highIntensity;
            testSender.setOnIntensity(highIntensity);
            //mOnVel.setOnIntensity(highIntensity);
            //setOnIntensity(highIntensity);
            
            cout << "\n";

                //here we check if intensity values are correct, low should be lower than high
                while (lowIntensity > highIntensity) { 
                    cout << "Intensity values are not corret, try again\n";
                    cout << "Select low intensity:\n";
                    cin >> lowIntensity;
                    testSender.setOffIntensity(lowIntensity);
                    cout << "\n";
                    cout << "Select high intensity:\n";
                    testSender.setOnIntensity(highIntensity);
                    cin >> highIntensity;
                    cout << "\n";
                }
            break;
        
        case '4':
            system("CLS");
            cout << "Speed:  " << speed << endl << "Message:  " << message << endl << "Low Intensity:  " 
                << lowIntensity << endl << "High Intensity:  " << highIntensity << endl << endl;
            /*
                        cout << "Speed:  " << getSpeed << endl << "Message:  " << getMessage << endl << "Low Intensity:  " 
                << getLowIntensity << endl << "High Intensity:  " << getHighIntensity << endl << endl;
            */
            cout << "Returning in 5 seconds..." << endl;
            Sleep(5000);
            break;

        case '5':
            system("CLS");

            allAttributes = to_string(speed) + message + to_string(lowIntensity) + to_string(highIntensity);

            cout << "Connecting to Arduino...\n" << endl;

            //cout << "Arduino will receive:\n" << allAttributes << endl;

            toArd(allAttributes);


/*
            do {
                attrToArd(testSenderAllAttrPtr[i]);
                i++;
            } while (testSenderAllAttrPtr[i] != NULL);
            */


            //toArd(allAttributes); //testSenderAllAttrPtr

            //toArd("OFF"); //TIL AT TESTE toArd FUNKTION MED CUSTOM BESKED!

            //allAttrPtr = getAllAttrPtr();
            //toArd(allAttrPtr);
            // ELLER:
            //toArd(*getallattr());

            //toArd(mAllAttrPtr.getAllAttrPtr);

            break;

        case '6':
            cout << "Goodbye.\n";
            Sleep(2000);
            exit(1);
            break;

        case '7':
            cout << "Test area\n";
            //tSpeed = mSpeed.getSpeed;

            //toArd("00000000");

            testSender.getMessageInMorse(messPtr);
            testSender.getAllAttrPtr(testSenderAllAttrPtr);

            cout << endl << endl << "All data sent (as wrong int):  " << (int*)testSenderAllAttrPtr << endl;

            cout << endl << endl << "Arduino will receive: " << endl;
            for (unsigned int i = 0; testSenderAllAttrPtr[i] != 0; i++)
            {
                cout << "  0b"; //til at skrive det pænt op, 0b kommer ikke videre til Arduino
                for (uint8_t j = 0; j < 8; j++) {
                    cout << (((char)testSenderAllAttrPtr[i] & (0b10000000 >> j)) ? '1' : '0');
                }
            }
            cout << "  0b00000000";

            Sleep(5000);


        default: cout << selection << " is not a valid menu item.\n";

            cout << endl;
        }

    } while (selection != 7);

    return 0;
}

int toArd(string command) {

    SerialPort arduino(port);
    if (arduino.isConnected()) {
        cout << "Connection established to Arduino\n" << endl;
    }
    else {
        cout << "Error in port name";
    }
    //while (arduino.isConnected()) {
        //string command;
        //cout << "Besked til Arduino:\n";
        //cin >> command;
        //}
        char* charArray = new char[command.size() + 1];
        copy(command.begin(), command.end(), charArray);
        charArray[command.size()] = '\n';

        testSender.getMessageInMorse(messPtr);
        testSender.getAllAttrPtr(testSenderAllAttrPtr);

        //cout << "Arduino will receive:\n" << (int*)testSenderAllAttrPtr << endl;

        
        for (unsigned int i = 0; testSenderAllAttrPtr[i] != 0; i++)
        {
            cout << "sent byte " << endl;
            arduino.writeSerialPort((char*)testSenderAllAttrPtr[i], MAX_DATA_LENGTH);
        }

        //arduino.writeSerialPort(charArray, MAX_DATA_LENGTH); //original kode foruden for-loopet

        arduino.readSerialPort(output, MAX_DATA_LENGTH);

        cout << "Arduino Output: " << output << endl; //output fra Arduino bliver cout'et her, måske ikke nødvendigt

        cout << "\nAttributes updated to Arduino!" << endl;

        delete[] charArray;

        Sleep(5000);

    return 0;
}
/*
int attrToArd(uint8_t intCommand) {

    string command = to_string(intCommand);

    SerialPort arduino(port);
    if (arduino.isConnected()) {
        cout << "Connection established to Arduino\n" << endl;
    }
    else {
        cout << "Error in port name";
    }
    //while (arduino.isConnected()) {
        //string command;
        //cout << "Besked til Arduino:\n";
        //cin >> command;
        //}
    char* charArray = new char[command.size() + 1];
    copy(command.begin(), command.end(), charArray);
    charArray[command.size()] = '\n';

    arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);
    arduino.readSerialPort(output, MAX_DATA_LENGTH);

    cout << "\nAttributes updated to Arduino!" << endl;

    cout << "Arduino Output: " << output << endl; //output fra Arduino bliver cout'et her, måske ikke nødvendigt

    delete[] charArray;

    Sleep(3000);

    return 0;
}
*/
