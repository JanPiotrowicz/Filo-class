#include <iostream>
#include "Filo.h"

using namespace std;

int main()
{
	{
		Filo<int> stos;
		stos.push_many(1, 2, 3, 4, 5);

		stos.print();

		stos.pop();
		stos.pop();

		stos.emplace(6);
		stos.emplace(7);

		for (auto& it : stos)
			cout << it << ' ';

		cout << "\n\n";

		while (!stos.empty())
		{
			cout << stos << '\n';
			stos.pop();
		}

	}

	return 0;
}
