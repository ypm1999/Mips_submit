#ifndef _Expections
#define _Expections


#include <iostream>
#include <string>
using std::string;
using std::cerr;
using std::endl;

class empty_label {
public:
	empty_label(string name = "Unknown") noexcept {
		cerr << "Error! Lebal " << name << " is empty" << endl;
	}
};

class command_not_found {
public:
	command_not_found(string name = "Unknown") noexcept {
		cerr << "Error! Command " << name << " is not found" << endl;
	}
};

class memory_used_up {
public:
	memory_used_up() noexcept {
		cerr << "Error! Memory used up!!" << endl;
	}
};

class write_out_of_bound {
public:
	write_out_of_bound(string name = "Unknown") noexcept {
		cerr << "Erroe! Write " << name << " at somewhere have not applied!" << endl;
	}
};

class without_main {
public:
	without_main() noexcept {
		cerr << "Error! Code don't have main!!" << endl;
	}
};

class function_not_defined {
public:
	function_not_defined(string name = "Unknown") noexcept {
		cerr << "Error! Function " << name << " have not defined!!" << endl;
	}
};

class run_command_error {
public:
	run_command_error(string name = "Unknown") noexcept {
		cerr << "Error! Run " << name << " command error!!" << endl;
	}
};

class command_address_error {
public:
	command_address_error(int ad) noexcept {
		cerr << "Error! Command address("<< ad <<") error!!" << endl;
	}
};

class read_out_of_bound {
public:
	read_out_of_bound(string name = "Unknown") noexcept {
		cerr << "Erroe! Write " << name << " at somewhere have not applied!" << endl;
	}
};

class without_text_or_data {
public:
	without_text_or_data() noexcept {
		std::cerr << "ERROR! Code don't have .text or .data at beginning!" << endl;
	}
};




#endif // !_Expections
