#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "graph.h"

class Doublet {
	typedef graph<std::string> GRAPH;

public:
	Doublet() {
		std::ifstream is("C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\knuth.txt");
		if (is.fail())
			exit(1);

		// Build point cloud graph
		std::size_t length = 5757;
		std::vector<std::string> words;

		std::string temp;

		for (std::size_t i = 0; i < length; i++) {
			is >> temp;
			words.push_back(temp);
		}
		//is >> _words;

		// Utility
		int common_letters;
		std::string word1, word2;

		// Build edges
		//for (auto word1 : _words.vertices()) {
		for (std::size_t j = 0; j < length; j++) {
			word1 = words[j];
			_words.addVertex(word1);
			//for (auto word2 : _words.vertices()) {
			for (std::size_t m = j+1; m < length; m++) {
				word2 = words[m];
				_words.addVertex(word2);

				common_letters = 0;

				if (word1 == word2)
					continue;

				if (!_words.isEdge(word1, word2)) {

					for (size_t k = 0; k < 5; k++) {
						if (word1[k] == word2[k])
							common_letters++;
					}

					if (common_letters == 4) {
						_words.addEdge(word1, word2);
					}
				}
			}
		}

	}

	GRAPH words() {
		return _words;
	}

	GRAPH::Path find_path(std::string origin, std::string dest) {
		GRAPH::Path route = _words.shortest_path(origin, dest);
		return route;
	}

private:
	GRAPH _words;

};