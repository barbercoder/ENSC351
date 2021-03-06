#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std;
void printsingleE();
void trace_flush();
ofstream fileoutput;
//make sure to include the chrono library to allow the program to tell the computer clock
//also have to research how to use chrono

struct singleE
{
	string nameE;
	string cat;
	string ph;
	int pid;
	int tid;
	unsigned long long ts;
	string arg;
	string key;	// this is for the counter key name
	string value;	//this is for the counter key value
	string s_value;
	void* location;
};

singleE buffer[10000];
int i = 0;

void trace_start(string filename)
{
	fileoutput.open(filename.c_str()); //replace the example with actual filename
	// may want to check if file is empty and, empty if there is anything in file
	if (fileoutput.fail())
	{
		fileoutput.close();
		cout << "file didn't open properly";
	}
	fileoutput << "[ ";
}


void trace_end()
{
	//will need to iterate through the stack of singleEs to actually make the file content
	//then output to fileoutput using a while loop as long as buffer not empty
	// do this later
	printsingleE();
	fileoutput << "]";
	fileoutput.close();
}

void trace_event_start(string  name, string categories, string arguments)
{
	singleE part;
	part.nameE = name;
	part.cat = categories;
	part.ph = "B";
	part.pid = 1;
	part.tid = 1;
	part.ts = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	part.arg = arguments;
	if(i >= 9999)
	{
		trace_flush();
	}
	buffer[i] = part;
	i++;
}

void trace_event_end(string arguements)
{
	singleE part2;
	part2.ph = "E";
	part2.pid = 1;
	part2.tid = 1;
	part2.ts = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	part2.arg = arguements;
	if(i >= 9999)
	{
		trace_flush();
	}
	buffer[i] = part2;
	i++;
}

void trace_counter(string name, string key1, string value1)
{
	singleE part3;
	part3.nameE = name;
	part3.ph = "C";
	part3.pid = 1;
	part3.tid = 1;
	part3.ts = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	part3.key = key1;
	part3.value = value1;
	if(i >= 9999)
	{
		trace_flush();
	}
	buffer[i] = part3;
	i++;
}

void trace_intstant_global(string name)
{
	singleE part4;
	part4.nameE = name;
	part4.ph = "i";
	part4.pid = 1;
	part4.tid = 1;
	part4.ts = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	part4.s_value = "g";
	if(i >= 9999)
	{
		trace_flush();
	}
	buffer[i] = part4;
	i++;
}

void trace_object_new(string name, void* obj_pointer)
{
	singleE part5;
	part5.nameE = name;
	part5.ph = "N";
	part5.pid = 1;
	part5.tid = 1;
	part5.ts = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	part5.location = obj_pointer;
	if(i >= 9999)
	{
		trace_flush();
	}
	buffer[i] = part5;
	i++;
}

void trace_object_gone(string name, void* obj_pointer)
{
	singleE part6;
	part6.nameE = name;
	part6.ph = "D";
	part6.pid = 1;
	part6.tid = 1;
	part6.ts = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	part6.location = obj_pointer;
	if(i >= 9999)
	{
		trace_flush();
	}
	buffer[i] = part6;
	i++;
}

void printsingleE()
{
	int k = 0;
	while (k < 10000)
	{
		singleE temp = buffer[k];
		if (temp.ph == "B")
		{
			fileoutput << "{\"name\": \"" << temp.nameE << "\",\"cat\": \"" << temp.cat << "\",";
			fileoutput << "\"ph\":\"" << temp.ph << "\", \"pid\":" << temp.pid << ",";
			fileoutput << "\"tid\":" << temp.tid << ", \"ts\":" << temp.ts << ", \"args\":" << temp.arg << "}," << endl;
		}
		else if (temp.ph == "C")
		{
			fileoutput << "{\"name\": \"" << temp.nameE << "\",\"cat\": \"" << temp.cat << "\",";
			fileoutput << "\"ph\":\"" << temp.ph << "\", \"pid\":" << temp.pid << ",";
			fileoutput << "\"tid\":" << temp.tid << ", \"ts\":" << temp.ts << ", \"args\":{\"" << temp.key << "\": ";
			fileoutput << temp.value <<"}}" << endl;
			if (k != i - 1)
			{
				fileoutput << ",";
			}
			fileoutput << endl;
		}
		else if (temp.ph == "E")
		{
			fileoutput << "{\"ph\":\"" << temp.ph << "\", \"pid\":" << temp.pid << ",";
			fileoutput << "\"tid\":" << temp.tid << ", \"ts\":" << temp.ts << ", \"args\":" << temp.arg <<"}";
			if (k != i - 1)
			{
				fileoutput << ",";
			}
			fileoutput << endl;
		}
		else if (temp.ph == "i")
		{
			fileoutput << "{\"name\": \"" << temp.nameE << "\",\"ph\": \"" << temp.ph << "\",";
			fileoutput << "\"ts\":" << temp.ts << ", \"pid\":" << temp.pid << ",";
			fileoutput << "\"tid\":" << temp.tid << ", \"s\":" << temp.s_value << "}";
			if (k != i - 1)
			{
				fileoutput << ",";
			}
			fileoutput << endl; 
		}
		else if (temp.ph == "N" || temp.ph == "D")
		{
			fileoutput << "{\"name\": \"" << temp.nameE <<"\", \"ph\": "<< temp.ph <<"\", \"id\": \"";
			fileoutput << temp.location <<"\", \"ts\": "<<temp.ts<<", \"pid\": "<<temp.pid;
			fileoutput << ", \"tid\": "<<temp.tid<<"}";
			if (k != i - 1)
			{
				fileoutput << ",";
			}
			fileoutput<< endl;
		}
	k++;
	}
	i = 0;
}
void trace_flush()
{
	printsingleE();
}
int main()
{
	int x =1;
	trace_start("output.txt");
	trace_event_start("Hello", "greeting", "bah humbug");
	trace_object_new("test Object", &x);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	trace_object_gone("test Object", &x);
	trace_event_end("bah humbug");
	trace_end();
	return 0;
}
