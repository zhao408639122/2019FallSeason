#ifndef __sparseMatrix_H_
#define __sparseMatrix_H_

#include <iostream>
#include <stdexcept>
#include <arrayList.h>
using namespace std;

template<typename T>
struct matrixTerm
{
	int index;
	T value;
	bool operator!=(const matrixTerm<T>& x) const { return !(index == x.index && value == x.value); }
};

template<typename T>
class sparseMatrix
{
public:
	sparseMatrix(int theRow, int theCol) :term(theRow*theCol/5), row(theRow), col(theCol) {};

	T get(int, int) const;
	void set(int, int, const T&);
	void print(ostream&) const;

	sparseMatrix<T> transpose() const;
	sparseMatrix<T> operator+(const sparseMatrix<T>&) const;
	sparseMatrix<T> operator*(const sparseMatrix<T>&) const;
protected:
	int row, col;
	arrayList<matrixTerm<T> > term;
};

template<typename T>
T sparseMatrix<T>::get(int theRow, int theCol) const
{
	if (theRow < 1 || theRow > row || theCol < 1 || theCol > col)	throw out_of_range("the index is out of range");
	int index = (theRow - 1)*col + theCol;
	int l = 0, r = term.size(), mid;
	while (l < r)
	{
		mid = (l + r) / 2;
		if (index > term[mid].index)	l = mid + 1;
		else r = mid;
	}
	if (l == term.size())	return 0;
	else if (term[l].index == index)	return term[l].value;
	else return 0;
}

template<typename T>
void sparseMatrix<T>::set(int theRow, int theCol, const T& theValue)
{
	if (theRow < 1 || theRow > row || theCol < 1 || theCol > col)	throw out_of_range("the index is out of range");
	int index = (theRow - 1)*col + theCol;
	int l = 0, r = term.size(), mid;
	while (l < r)
	{
		mid = (l + r) / 2;
		if (index > term[mid].index)	l = mid + 1;
		else r = mid;
	}
	if (l == term.size())
	{
		if(theValue != 0)	term.insert(l, { index, theValue });
	}
	else if (term[l].index == index)
	{
		if (theValue == 0)	term.erase(l);
		else term[l].value = theValue;
	}
	else
	{
		if (theValue != 0)	term.insert(l, { index, theValue });
	}
}

template<typename T>
void sparseMatrix<T>::print(ostream& out) const
{
	int n = term.size();
	int k = 0;
	int index;
	T value;
	for (int i = 1; i <= row; ++i)
	{
		for (int j = 1; j <= col; ++j)
		{
			index = (i - 1)*col + j;
			if (k >= n || index < term[k].index)	value = 0;
			else value = term[k++].value;
			out << value << ' ';
		}
		out << endl;
	}
}

template<typename T>
sparseMatrix<T> sparseMatrix<T>::transpose() const
{
	sparseMatrix<T> res(col, row);
	for (int i = 0; i < this->term.size(); ++i)
	{
		int index = term[i].index;
		int value = term[i].value;
		int c = index % col;
		int r = index / col + 1;
		if (c == 0)
		{
			c = col;
			r--;
		}
		res.set(c, r, value);
	}
	return res;
}

template<typename T>
sparseMatrix<T> sparseMatrix<T>::operator+(const sparseMatrix<T>& a) const
{//¾ØÕó¼Ó
	if (row != a.row || col != a.col)	throw logic_error("No matching of row and column");
	sparseMatrix<T> ans(row, col);
	matrixTerm<T> tmp;
	int i = 0, j = 0;
	while (i < term.size() && j < a.term.size())
	{
		if (term[i].index < a.term[j].index)
			tmp = term[i++];
		else if (term[i].index > a.term[j].index)
			tmp = a.term[j++];
		else
		{
			tmp = { term[i].index, term[i].value + a.term[j].value };
			i++; j++;
		}
		ans.term.push_back(tmp);
	}
	while (i < term.size())	ans.term.push_back(term[i++]);
	while (j < a.term.size())	ans.term.push_back(a.term[j++]);
	return ans;
}

template<typename T>
sparseMatrix<T> sparseMatrix<T>::operator*(const sparseMatrix<T>& a) const
{
	if (col != a.row)	throw logic_error("No matching of row and column");
	sparseMatrix<T> tmp(row, a.col);
	sparseMatrix<T> ans(row, a.col);
	arrayList<T> item(a.col + 1);
	int lastR = 0;
	for (int i = 0; i < term.size(); ++i)
	{
		int c = term[i].index % col;
		int r = term[i].index / col + 1;
		if (c == 0)
		{
			c = col;
			r--;
		}
		for (int j = 0; j < a.term.size(); ++j)
		{
			int ac = a.term[j].index % a.col;
			int ar = a.term[j].index / a.col + 1;
			if (ac == 0)
			{
				ac = a.col;
				ar--;
			}
			if (c == ar)	tmp.set(r, ac, tmp.get(r, ac) + term[i].value*a.term[j].value);
		}
	}
	for (int i = 0; i < tmp.term.size(); ++i)
		if (tmp.term[i].value != 0)
			ans.term.push_back(tmp.term[i]);
	return ans;
}


#endif //__sparseMatrix_H_