#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

class empty_label {
public:
	empty_label(string name = "Unknown") {
		cerr << "Error! Lebal " << name << " is empty" << endl;
	}
};

class command_not_found {
public:
	command_not_found(string name = "Unknown") {
		cerr << "Error! Command " << name << " is not found" << endl;
	}
};

class memory_used_up {
public:
	memory_used_up() {
		cerr << "Error! Memory used up!!" << endl;
	}
};

class write_out_of_bound {
public:
	write_out_of_bound(string name = "Unknown") {
		cerr << "Erroe! Write " << name << " at somewhere have not applied!" << endl;
	}
};

class without_main {
public:
	without_main() {
		cerr << "Error! Code don't have main!!" << endl;
	}
};

class function_not_defined {
public:
	function_not_defined(string name = "Unknown") {
		cerr << "Error! Function " << name << " have not defined!!" << endl;
	}
};

class run_command_error {
public:
	run_command_error(string name = "Unknown") {
		cerr << "Error! Run " << name << " command error!!" << endl;
	}
};

class command_address_error {
public:
	command_address_error(int ad) {
		cerr << "Error! Command address("<< ad <<") error!!" << endl;
	}
};

class read_out_of_bound {
public:
	read_out_of_bound(string name = "Unknown") {
		cerr << "Erroe! Write " << name << " at somewhere have not applied!" << endl;
	}
};
#endif
