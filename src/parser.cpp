#include <vector>
#include <string>

#include "headers/parser.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/keywords.h"

using namespace std;

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int indent, int* totalend, vector<string> keywords) {
	unsigned int end = *i;

	string line;

	vector<Chunk> chunks;

	Chunk *current = nullptr;

	while (end < method->getlines().size()) {
		string temp = method->getlines()[end];

		int s = temp.find_first_not_of('\t');

		bool word = false;

		for (string s : keywords) {
			if (startswith(trim(temp), get_kw(s))) {
				word = true;
			}
		}

		if (s == indent && word) {
			if (current != nullptr) {
				current->end = end - 1;
				chunks.push_back(*current);
			}

			current = new Chunk;
			current->start = end;
			*i = end;
		} else if (s == indent && startswith(trim(temp), get_kw(KW_END))) {
			if (current != nullptr) {
				line = method->getlines()[current->start];
				string cond = line.substr(line.find_first_of(" ") + 1);

				current->end = end - 1;
				chunks.push_back(*current);
				*i = end;
				*totalend = end;
				break;
			}
		}

		end++;
	}

	return chunks;
}
