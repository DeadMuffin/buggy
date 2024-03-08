#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

void logo() {
	std::cout
		<< std::endl
		<< "***************************" << std::endl
		<< "Philipps und Carstens Buggy" << std::endl
		<< "Embedded - SS 2023"			 << std::endl
		<< "***************************" << std::endl
		<< std::endl;
}

void hauptmenue() {
	std::cout
		<< "Um den jeweiligen Befehl aufzurufen,"		<< std::endl
		<< "die Nummer aus der folgenden Liste tippen." << std::endl

		<< "0. Starte Gerade bis ein Hindernis kommt"	<< std::endl
		<< "1. Rechteck - Rechts"						<< std::endl
		<< "2. Rechteck - Links"						<< std::endl
		<< "3. Selbst steuern"							<< std::endl
		<< "4. Befehlsliste nochmal ausgeben"			<< std::endl
		<< "5. Team"									<< std::endl;		
}

void steuerungsmenue() {
	std::cout
		<< "w. Vorwaerts"								<< std::endl
		<< "a. Links"									<< std::endl
		<< "s. Rueckwaerts"								<< std::endl
		<< "d. Rechts"									<< std::endl
		<< "x. Stopp"									<< std::endl
		<< "M. Befehlsliste nochmal ausgeben"			<< std::endl;
}

void shrek() {
	std::vector <std::string> shreks = {
		"What are you doing in my swamp?",
		"I'm an ogre! You know, grab your torch and pitchforks!",
		"Ogres are not like cakes.",
		"It's the donkey that turned me in!"
		"Ogres are like onions, they have layers."
	};

	std::srand(static_cast <unsigned int> (std::time(nullptr)));
	int Zufallszahl = std::rand() % shreks.size();
	std::string Zitat = shreks[Zufallszahl];

	std::cout 
		<< "\"" << Zitat << "\"" << std::endl
		<< "    -- Shrek"		 << std::endl
								 << std::endl;
}