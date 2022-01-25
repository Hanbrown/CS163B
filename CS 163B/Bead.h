#pragma once
#ifndef BEAD_H
#define BEAD_H

class Bead {

public:
	Bead() {
		_first = 0;
		_second = 0;
	}

	Bead(const Bead& src) {
		_first = src._first;
		_second = src._second;
	}

	// Methods
	int first() {
		return _first;
	}

	int second() {
		return _second;
	}

	void setFirst(int a) {
		_first = a;
	}

	void setSecond(int b) {
		_second = b;
	}

	bool canTouch(const Bead& other) {
		return _first == other._second || _second == other._first;
	}

private:
	int _first, _second;

};

#endif // !Bead
