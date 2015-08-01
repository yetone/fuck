#include <vector>
#include <string>

#include "headers/parser.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/keywords.h"

using namespace std;

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int indent, int* totalend, vector<wstring> keywords) {
	unsigned int end = *i;

	wstring line;

	vector<Chunk> chunks;

	Chunk *current = nullptr;

	while (end < method->getlines().size()) {
		wstring temp = method->getlines()[end];

		int s = temp.find_first_not_of('\t');

		bool word = false;

		for (wstring str : keywords) {
			if (startswith(trim(temp), get_kw(str))) {
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
				wstring cond = line.substr(line.find_first_of(L" ") + 1);

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
