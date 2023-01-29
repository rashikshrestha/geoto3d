#include "ncqtsignalhandler.h"

ncQtSignalHandler::ncQtSignalHandler() {
	block_count = 0;
}

ncQtSignalHandler::~ncQtSignalHandler() {

}

void ncQtSignalHandler::input() {
	(*this)();
}

void ncQtSignalHandler::operator()() {
	if (block_count==0)
		output();
}

ncQtSignalHandler& ncQtSignalHandler::operator++() {
	block_count++;
	return *this;
}
 
void ncQtSignalHandler::operator++ (int) {
	++(*this);
}

ncQtSignalHandler& ncQtSignalHandler::operator--() {
	block_count--;
	(*this)();
	return *this;
}
 
void ncQtSignalHandler::operator-- (int) {
	--(*this);
}