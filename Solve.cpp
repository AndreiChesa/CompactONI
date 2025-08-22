#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <set>

using namespace std;

ifstream f("compact.in");
ofstream g("compact.out");

struct val
{
	int minim;
	int maxim;
	int last_index;
	int first_index;
};
const int NMAX = 1000005;
int n, m;
val aint[4 * NMAX];
int vec[NMAX];
int grup[NMAX];
unordered_set<int>s;
set<int>ans;
int stg[NMAX];
int drp[NMAX]; 
int rez2[NMAX];
pair<int, int> rez[NMAX];

void build(int nod, int st, int dr)
{
	if (st == dr)
	{
		aint[nod].minim = vec[st];
		aint[nod].maxim = vec[st];
		return;
	}
	
	int mij = (st + dr) / 2;

	build(nod * 2, st, mij);
	build(nod * 2 + 1, mij + 1, dr);

	aint[nod].minim = min(aint[2 * nod].minim, aint[2 * nod + 1].minim);
	aint[nod].maxim = max(aint[2 * nod].maxim, aint[2 * nod + 1].maxim);
}

void build2(int nod, int st, int dr)
{
	if (st == dr)
	{
		aint[nod].first_index = stg[st];
		aint[nod].last_index = drp[st];
		return;
	}


	int mij = (st + dr) / 2;

	build2(nod * 2, st, mij);
	build2(nod * 2 + 1, mij + 1, dr);

	aint[nod].first_index = min(aint[2 * nod].first_index, aint[2 * nod + 1].first_index);
	aint[nod].last_index = max(aint[nod * 2 + 1].last_index, aint[2 * nod].last_index);
}

bool cmp(pair<int,int> a, pair<int,int> b)
{
	if (a.first == b.first)
		return a.second > b.second;
	return a.first < b.first;
}

pair<int, int>MinMaxQuery(int nod, int st, int dr, int x, int y)
{
	if (st >= x && dr <= y)
	{
		return { aint[nod].minim, aint[nod].maxim };
	}
	else {
		int mij = (st + dr) / 2;

		pair<int, int>ans = { 1e9,0 };
		pair<int, int>stanga = { 1e9, 0 };
		pair<int, int>dreapta = { 1e9,0 };
		if(x <= mij)
		{
			stanga = MinMaxQuery(2 * nod, st, mij, x, y);
		}
		if(y > mij) {
			dreapta = MinMaxQuery(2 * nod + 1, mij + 1, dr, x, y);
		}
		ans.first = min(stanga.first, dreapta.first);
		ans.second = max(stanga.second, dreapta.second);

		return ans;
	}
}

pair<int, int> LeftRightQuery(int nod, int st, int dr, int x, int y)
{
	if (st >= x && dr <= y)
	{
		return { aint[nod].first_index, aint[nod].last_index };
	}
	else {
		int mij = (st + dr) / 2;

		pair<int, int>ans = { 1e9,0 };
		pair<int, int>stanga = { 1e9, 0 };
		pair<int, int>dreapta = { 1e9,0 }; 

		if (x <= mij)
		{
			stanga = LeftRightQuery(2 * nod, st, mij, x, y);
		}
		if (y > mij) {
			dreapta = LeftRightQuery(2 * nod + 1, mij + 1, dr, x, y);
		}

		ans.first = min(stanga.first, dreapta.first);
		ans.second = max(stanga.second, dreapta.second);

		return ans;
	}
}

int main()
{

	f >> n >> m;
	for (int i = 1; i <= n; i++)
	{
		f >> vec[i];
		if (stg[vec[i]] == 0)
		{
			stg[vec[i]] = i;
		}
		drp[vec[i]] = i;
	}

	build(1, 1, n);
	build2(1, 1, m);

	int pozitie = 1;
	int st = 1;
	int dr = 1;
	int nr = 0;
	while (pozitie <= n)
	{
		bool ok = true;
		st = pozitie;
		dr = drp[vec[st]];
		while (ok)
		{
			ok = 0;
			pair<int, int> minMax = MinMaxQuery(1, 1, n, st, dr);
			pair<int, int>capete = LeftRightQuery(1, 1,m, minMax.first, minMax.second);
			int l = capete.first;
			int r = capete.second;

			if (st > l || dr < r)
			{
				ok = 1;
				st = l;
				dr = r;
			}
		}
		nr++;
		rez[nr] = { st,dr };
		dr++;
		pozitie = dr;
	}
	sort(rez + 1, rez + nr + 1, cmp);
	pozitie = 1;
	int k = 0;
	for (int i = 1; i <= nr; i++)
	{
		if (rez[i].first == pozitie)
		{
			pozitie = rez[i].second;
			rez2[++k] = pozitie;
			pozitie++;
		}
	}
	g << k << '\n';
	for (int i = 1; i <= k; i++)
		g << rez2[i] << " ";
}