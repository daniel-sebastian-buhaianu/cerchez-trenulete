#include <fstream>

using namespace std;

ifstream fin("tren.in");
ofstream fout("tren.out");

int main()
{
	int n, k, i, j, x, st, dr, mij, lg, p, pot, ok, y;

	fin >> n >> k;

	int NR_MAX_COZI = k+2;
	int LGMAX_REZ = n*k;

	int inc[NR_MAX_COZI], sf[NR_MAX_COZI], nr[NR_MAX_COZI], cd[NR_MAX_COZI][n], v[n];
	int X[LGMAX_REZ], V[LGMAX_REZ], L[LGMAX_REZ], P[LGMAX_REZ];
	
	// initializez inc[i], nr[i] si sf[i],
	// unde inc[i], nr[i], sf[i] reprezinta
	// inceputul, nr. de elemente si sfarsitul cozii i
	for (i = 0; i <= k+1; i++)
	{
		inc[i] = nr[i] = 0, sf[i] = -1;
	}
	
	for (lg = i = 0; i < n; i++)
	{
		// citesc numarul vagonului
		fin >> x;
		
		// il adaug in coada 0 - linia de intrare
		cd[0][++sf[0]] = x, nr[0]++;
	
		// caut pozitia lui x in vectorul sortat crescator v
		// folosind cautare binara
		st = -1, dr = lg;

		while (dr-st > 1)
		{
			mij = st + (dr-st)/2;

			if (v[mij] < x)
			{
				st = mij;
			}
			else
			{
				dr = mij;
			}
		}

		// mut elementele din vector cu o pozitie spre dreapta
		for (j = lg; j > dr; j--)
		{
			v[j] = v[j-1];
		}
		
		// inserez pe x in vectorul sortat v la pozitia dr
		v[dr] = x, lg++;
	}

	fin.close();
	
	for (p = i = 0; i < lg;)
	{
		// retin nr. vagonului pe care trebuie sa il plasez
		// pe linia de iesire (cd[k+1])
		x = v[i];

		// daca vagonul x este primul vagon de pe oricare linie de manevra
		// atunci il pot muta pe linia de iesire
		pot = 0;
		for (j = 1; j <= k && !pot; j++)
		{
			if (nr[j] && cd[j][inc[j]] == x )
			{
				pot = 1;
				
				// mut vagonul x pe linia de iesire
				cd[k+1][++sf[k+1]] = x;
				nr[k+1]++;
				inc[j] = (inc[j]+1) % n;
				nr[j]--;
				
				// actualizez vectorii de rezultat
				X[p] = 'O';
				V[p] = x;
				L[p] = j;
				P[p++] = ++i;
			}
		}
		
		// daca vagonul x nu indeplineste criteriul anterior,
		// atunci incerc sa il scot de pe linia de intrare
		// pe o linie de manevra
		while(!pot)
		{
			// retin nr. primului vagon de pe linia de intrare
			y = cd[0][inc[0]++];
			
			ok = 0; // nu pot muta pe y pe linia de manevra

			for (j = 1; j <= k && !ok; j++)
			{
				// pot muta vagonul y pe linia de manevra j?
				if (!nr[j]
					|| (nr[j] <= n && y > cd[j][sf[j]]))
				{
					ok = 1; // pot
					
					// mut vagonul y pe linia de manevra j
					sf[j] = (sf[j]+1) % n;
					cd[j][sf[j]] = y;
					nr[j]++;
					nr[0]--;
					
					// actualizez vectorii de rezultat
					X[p] = 'M';
					V[p] = y;
					L[p] = j;
					P[p++] = sf[j] >= inc[j] ? sf[j]-inc[j]+1
							       : n-inc[j]+1 + sf[j]+1;
				}
			}
			
			// nu am muta vagonul y, deci am epuizat toate liniile de manevra
			// prin urmare vagoanele nu se pot muta in ordinea ceruta
			if (!ok)
			{
				fout << "VAGOANELE NU SE POT MUTA\n" << y << '\n';
				return 0;
			}
			else if (y == x)
			{
				pot = 1; // pot muta vagonul x pe linia de iesire
			}
		}
	}
	
	// afisez solutia
	for (i = 0; i < p; i++)
	{
		fout << (char)X[i] << ' ' << V[i] << ' ' << L[i] << ' ' << P[i] << '\n';
	}

	fout.close();

	return 0;
}
