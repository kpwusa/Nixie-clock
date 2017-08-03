

/*Nixie Display Program for Arduino
Written by: Tim Anderson
Started: May 7, 2010

Description: Takes serial input string and displays digits 0-9 and decimals on 6 nixie-tube display.

Look for string "RETURN:" in comments for things that need to be updated later.

*/

#define NUM_TUBES 6
#define NUM_DIGITS 11
#define MAX_OUTPUT_LENGTH 20
#define TP_DELAY 500
#define DELAY_TIME_ON 300
#define DELAY_TIME_OFF 100


//declare arduino pin names:
int t1_pin = 11;  //3
int t2_pin = 9;   //5
int t3_pin = 5;   //6
int t4_pin = 3;   //9
int t5_pin = 6;  //10
int t6_pin = 10;  //11

int n1_pin = 2;  //4
int n2_pin = A5;  //7
int n3_pin = A4;   //8
int n4_pin = A3;  //12
int n5_pin = A2;  //13
int n6_pin = A1;  //15
int n7_pin = 13;  //16
int n8_pin = 12;  //17
int n9_pin = 8;  //18
int n0_pin = 7;  //19
int dec_pin = 4;  //2
int switch_pin = A0; //14


//declare arrays for tubes and digits.
int tubes[NUM_TUBES+1] = {t1_pin, t2_pin, t3_pin, t4_pin, t5_pin, t6_pin};
int digits[NUM_DIGITS+1] = {n0_pin, n1_pin, n2_pin, n3_pin, n4_pin, n5_pin, n6_pin, n7_pin, n8_pin, n9_pin, dec_pin};


//define test strings
char tp_string0[MAX_OUTPUT_LENGTH+1] = "012345\r\n";
char tp_string1[MAX_OUTPUT_LENGTH+1] = ".1.2.3.4.5.6\r\n";
char tp_string2[MAX_OUTPUT_LENGTH+1] = "234567\r\n";
char tp_string3[MAX_OUTPUT_LENGTH+1] = ".3.4.5.6.7.8\r\n";
char tp_string4[MAX_OUTPUT_LENGTH+1] = "456789\r\n";
char tp_string5[MAX_OUTPUT_LENGTH+1] = ".5.6.7.8.9.0\r\n";
char tp_string6[MAX_OUTPUT_LENGTH+1] = "678901\r\n";
char tp_string7[MAX_OUTPUT_LENGTH+1] = ".7.8.9.0.1.2\r\n";
char tp_string8[MAX_OUTPUT_LENGTH+1] = "890123\r\n";
char tp_string9[MAX_OUTPUT_LENGTH+1] = ".9.0.1.2.3.4\r\n";

//array to hold the incoming serial data
char serial_string[MAX_OUTPUT_LENGTH+1];

//a pointer to hold the location of the string that will be printed
char * pnt_output_string;

//a time variable for the test pattern
unsigned long last_tp_change;
int current_tp = 0;

void setup(){
	//initialize serial
	Serial.begin(9600);

	//declare tube annode pins as outputs, set LOW
	for(int i=0; i<NUM_TUBES; i++){
		pinMode(tubes[i], OUTPUT);
		digitalWrite(tubes[i], LOW);
	}
	
	//declare tube digit pins as outputs, set LOW
	for(int i=0; i<NUM_TUBES; i++){
		pinMode(digits[i], OUTPUT);
		digitalWrite(digits[i], LOW);
	}
	
	//declare switch input
	pinMode(switch_pin, INPUT);
	digitalWrite(switch_pin, LOW); //activate pullup resistor
	
	pnt_output_string = tp_string1;
} 


void loop(){
	
	//Check the demo switch
	if(digitalRead(switch_pin)){
		test_pattern();
	}
	else{
		update_serial();
	}
	//Serial.println(output_string);
	
	display_digits();
}

void test_pattern(){
	//only change every TP_DELAY milliseconds
	if( (millis() - last_tp_change) > TP_DELAY){
		last_tp_change = millis();
		if(current_tp == 0){
			pnt_output_string = tp_string0;
			current_tp++;
		}
		else if(current_tp == 1){
			pnt_output_string = tp_string1;
			current_tp++;
		}
		else if(current_tp == 2){
			pnt_output_string = tp_string2;
			current_tp++;
		}
		else if(current_tp == 3){
			pnt_output_string = tp_string3;
			current_tp++;
		}
		else if(current_tp == 4){
			pnt_output_string = tp_string4;
			current_tp++;
		}
		else if(current_tp == 5){
			pnt_output_string = tp_string5;
			current_tp++;
		}
		else if(current_tp == 6){
			pnt_output_string = tp_string6;
			current_tp++;
		}
		else if(current_tp == 7){
			pnt_output_string = tp_string7;
			current_tp++;
		}
		else if(current_tp == 8){
			pnt_output_string = tp_string8;
			current_tp++;
		}
		else{
			pnt_output_string = tp_string9;
			current_tp = 0;
		}
		
	}
}

void update_serial(){
	char temp_char;
	int string_position = 0;
	bool read_incoming = false;
	if(Serial.available() > 0){
		while(Serial.available() > 0){ //read till the letter 'A' is found
			temp_char = Serial.read();
			if(temp_char == 'A'){ 
				read_incoming = true; 
				break; 
			}
		}
		while(read_incoming){
			temp_char = Serial.read();
			if( (string_position > MAX_OUTPUT_LENGTH) || ((temp_char == 'Z')) ){  //break when the letter 'Z' is found or if the string is too long.
				read_incoming = false; 
			}
			if( (temp_char == '.') || (temp_char == ' ') || ( (temp_char >= 48) && (temp_char <= 57) ) ){
				serial_string[string_position] = temp_char;
				string_position++;
			}
		}
		Serial.flush();
		pnt_output_string = serial_string;
		Serial.println(pnt_output_string);
	}
}

//prints a string, or rather the first 6 digits of a string. Don't send it two decimal points in a row unless you want a blank tube with the decimal
void display_digits(){
	
	bool decimal_state = false; //for keeping track of wether or not to turn of the decimal off
	int char_position = 0; //reset the position in the output string being read each time through the tubes
	
	for(int i=0; i<NUM_TUBES; i++){//cycle through the tubes
		
		//turn things on:
		if(pnt_output_string[char_position] != ' '){
			digitalWrite(tubes[i], HIGH);//activate the tube
		}
		if(pnt_output_string[char_position] == '.'){//write a decimal if the decimal is on.
			digitalWrite(digits[10], HIGH);
			decimal_state = true;
			char_position++;//move to the next character
		}
		if( (pnt_output_string[char_position] >= 48) && (pnt_output_string[char_position] <= 57) ){ //only if not a space
			digitalWrite(digits[pnt_output_string[char_position]-48], HIGH); //write the number to high
		}
		
		//wait:
		delayMicroseconds(DELAY_TIME_ON);
		
		//tun things off:
		digitalWrite(tubes[i], LOW);//turn off the tube
		if(decimal_state = true){//turn off the decimal if it was turned on.
			digitalWrite(digits[10], LOW);
			decimal_state = false;//reset decimal state to avoid unnecessary writes
		}
		digitalWrite(digits[pnt_output_string[char_position]-48], LOW); //write the number to low
		delayMicroseconds(DELAY_TIME_OFF);
		char_position++;//move to the next character
	}
}
