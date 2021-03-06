#pragma once		// used for error prevention
#include "pch.h"		
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include "TLogger.h"	// the logging libary

using namespace std;
extern thread_names g_thread_names;

// Functions
int read_DNA_file();
void create_DNA_map();

// Values
enum amino_acid		// These are the amino acids made by DNA and RNA
{
	Histidine,		// H, HIS		R = C3N2H4 = Imidazole
	Glutamine,		// Q			R = 
	Proline,		// P			R = 
	Arginine,		// R			R = 
	Leucine,		// L			R = 
	AsparticAcid,	// D			R = 
	GlutamicAcid,	// E			R = 
	Alanine,		// A, ALA		R = H3C = methyl group
	Glycine,		// G			R = 
	Valine,			// V			R = 
	Tyrosine,		// Y			R = 
	Selenocysteine,	// U	Stop	R = 
	Pyrrolysine,	// O	Stop	R = 
	Serine,			// S			R = 
	Cysteine,		// C			R = 
	Typtophan,		// W			R = 
	Phenylalnine,	// F			R = 
	Asparagine,		// N			R = 
	Lysine,			// K			R = 
	Threonine,		// T			R = 
	Isoleucine,		// I			R = 
	Methionine		// M	Start	R = 
};
logging logger;

/*
	always,			// Most basic of a log
	basic,			// Means we are logging actual information veruses debugging logs
	info,			// Can mean anything other then something has happened
	debug,			// Means something has happened   - (good or bad)
	success,		// Means something good has happened
	warning,		// Means something bad might happen
	error,			// Means something bad has happened but will not crash the program
	critical		// Means something bad has happened and will crash the program
*/
int main()
{
	// This is to start the logging process
	g_thread_names.name_the_thread("main thread");
	logger.addline("logging process has started", always);
	logger.addline("logging process has started", basic);
	logger.addline("logging process has started", info);
	logger.addline("logging process has started", debug);
	logger.addline("logging process has started", success);
	logger.init();
	logger.addline("logging process has been initated", always);
	logger.addline("logging process has been initated", basic);
	logger.addline("logging process has been initated", info);
	logger.addline("logging process has been initated", debug);

	// This is the code for this function
	map<string, amino_acid> data_value;
	read_DNA_file();
	logger.addline("Started creation of DNA Map Thread", debug);
	thread file_reading_thread(read_DNA_file);
	logger.addline("Started creation of DNA Map Thread", debug);
	thread dna_map_thread(create_DNA_map);
	for (int i = 0; i < 10; i++)
	{
		cout << "i = " << i << endl;
	}

	// Ending all of my threads
	file_reading_thread.join();
	dna_map_thread.join();

	// finishes all the logging before ending the program.
	logger.addline("The logging process ended no more logs will be recorded", success);
	logger.kill();
	cout << "press enter three times to end the program";
	cin.clear(); // reset any error flags
	cin.ignore(32767, '\n'); // ignore any characters in the input buffer until we find an enter character
	cin.get(); // get one more char from the user


	return 0;
}

int read_DNA_file()
{
	g_thread_names.name_the_thread("reading DNA file thread");
	logger.addline("created the reading from file thread", success);
	// first step to find the promoter
	// right now I am using a very basic concept of a promoter and * will be the promoter
	string DNA = "AUAUAUAUGCGCAU*AGCAAAAGCAGGGGATAATAAAAACAACCAGAATGAAAGTAAAACTACTGGTCCTGTTATGCACATTTACAGCTACATATGCAGACACAATATGTATAGGCTACCATGCTAACAACTCGACCGACACTGTTGACACAGTACTTGAAAAGAATGTGACAGTGACACACTCTGTCAACCTGCTTGAGAACAGTCACAATGGAAAACTATGTCTATTAAAAGGAATAGCCCCACTACAATTGGGTAATTGCAGCGTTGCCGGGTGGATCTTAGGAAACCCAGAATGCGAATTACTGATTTCCAAGGAGTCATGGTCCTACATTGTAGAAAAACCAAATCCTGAGAATGGAACATGTTACCCAGGGCATTTCGCTGACTATGAGGAACTGAGGGAGCAATTGAGTTCAGTATCTTCATTTGAGAGGTTCGAAATATTCCCCAAAGAAAGCTCATGGCCCAACCACACCGTAACCGGAGTGTCAGCATCATGCTCCCATAATGGGGAAAGCAGTTTTTACAGAAATTTGCTATGGCTGACGGGGAAGAATGGTTTGTACCCAAACCTGAGCAAGTCCTATGCAAACAACAAAGAAAAAGAAGTCCTTGTACTATGGGGTGTTCATCACCCGCCAAACATAGGTATCCAAAAGGCCCTCTATCATACAGAAAATGCTTATGTCTCTGTAGTGTCTTCACATTATAGCAGAAAATTCACCCCAGAAATAGCCAAAAGACCCAAAGTAAGAGATCAAGAAGGAAGAATCAATTACTACTGGACTCTGCTTGAACCCGGGGATACAATAATATTTGAGGCAAATGGAAATCTAATAGCGCCAAGATATGCTTTCGCACTGAGTAGAGGCTTTGGATCAGGAATCATCAACTCAAATGCACCAATGGATAAATGTGATGCGAAGTGCCAAACACCTCAGGGAGCTATAAACAGCAGTCTTCCTTTCCAGAACGTACACCCAGTCACAATAGGAGAGTGTCCAAAGTATGTCAGGAGTGCAAAATTAAGGATGGTTACAGGACTAAGGAACATCCCATCCATTCAATCCAGAGGTTTGTTTGGAGCCATTGCCGGTTTCATTGAAGGGGGGTGGACTGGAATGGTAGATGGTTGGTATGGTTATCATCATCAGAATGAGCAAGGATCTGGCTATGCTGCAGATCAAAAAAGCACACAAAATGCCATTAATGGGATTACAAACAAGGTGAATTCTGTAATTGAGAAAATGAACACTCAATTCACAGCTGTGGGCAAAGAATTCAACAAATTGGAAAGAAGGATGGAAAACTTGAATAAAAAAGTTGATGATGGGTTTATAGACATTTGGACATATAATGCAGAACTGTTGGTTCTACTGGAAAATGAAAGGACTTTGGATTTCCATGACTCCAATGTGAAGAATCTGTATGAGAAAGTAAAAAGCCAGTTAAAGAATAATGCTAAAGAAATAGGAAATGGGTGTTTTGAATTCTATCACAAGTGTAACGATGAATGCATGGAGAGTGTAAAGAATGGAACTTATGACTATCCAAAATATTCCGAAGAATCAAAGTTAAACAGGGAGAAAATTGATGGAGTGAAATTGGAATCAATGGGAGTCTATCAGATTCTGGCGATCTACTCAACAGTCGCCAGTTCTCTGGTTCTTTTGGTCTCCCTGGGGGCAATCAGCTTCTGGATGTGTTCCAATGGGTCTTTACAGTGTAGAATATGCATCTAAGACCAGAATTTCAGAAATATAAGGAAAAACACCCTTGTTTCTACTA";
	int promoter;	// the location of the promoter in the given string.
	for (int i = 0; i < sizeof(DNA); i++)
	{
		if (DNA.at(i) == '*')
		{
			promoter = i;
			break;
		}// if
	}// for loop

	// removing the unnessary DNA and starting at the promoter
	promoter++;	// to remove the protmoer as well 
	DNA.substr(promoter, DNA.length() - promoter);	// does the editing and removing
	//cout << DNA.substr(promoter, DNA.length() - promoter);

	return 0;
}

void create_DNA_map()
{
	g_thread_names.name_the_thread("DNA Map thread");
	logger.addline("created the mapping thread", success);
	for (int j = 0; j < 10; j++)
	{
		cout << "j = " << j << endl;
	}

}
