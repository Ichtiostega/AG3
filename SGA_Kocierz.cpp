/******************************************************************************/
/* Ten plik, kazdy moze sobie zorganizowac po swojemu                         */
/* Prosze tylko ustawic ponizsze dwie definicje:                              */
/*  - pierwsza defincja to nazwisko osoby lub nazwiska grupy piszacej kod     */
/*  - druga definicja to nazwa cwiczenia i numeracja wersji kodu              */
/******************************************************************************/

#define NAME "Karol Kocierz"
#define VERSION "SGA 0.1"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include "SGA_Kocierz.h" // Tu trzeba zainkludowac wlasny plik
#include "random.h"
using namespace std;

/******************************************************************************/
/*                                                          OPERATORY WYJSCIA */
/* Dla wygody, stworzylem sobie operatory wyjscia dla kazdej klasy.           */
/******************************************************************************/

ostream &operator<<(ostream &out, genotype g)
{
	for (int i = 0; i < max_chrom_size; i++)
		if (g.chromosome[i])
			out << "1";
		else
			out << "0";
	return out;
}

ostream &operator<<(ostream &out, phenotype p)
{
	out << p.A << "\t" << p.B;
	return out;
}

/******************************************************************************/
/*                                                 IMPLEMENTACJE KLASY FACADE */
/******************************************************************************/

char *My_SGA::Authors_Names() { return NAME; };
char *My_SGA::Version() { return VERSION; };

void My_SGA::Init_Population(int n)
{
	pop.pop_size = n;
	pop.init_generation();
}

float My_SGA::Individual_Fitness(int i)
{
	return pop.main_pop[i].fitness;
}

int My_SGA::Individual_A(int i, int j)
{
	return pop.main_pop[i].p.A;
}

int My_SGA::Individual_B(int i, int j)
{
	return pop.main_pop[i].p.B;
}

int My_SGA::Next_Generation()
{
	return pop.next_generation();
}

void My_SGA::Any_Rapport(ostream &out)
{
	// Tu wpisujemy dowolny kod, ktory bedzie sie wykonywal w kazdym pokoleniu
	// Algorytmu Genetycznego. W szczegolnosci mozemy tutaj zapisywac rozne dane
	// do pliku z logami, aby latwiej analizowac prace programu.
	// Ponizej tylko przykladowy kod, ktory mozna w calosci wykomentowac.
	out << "Pokolenie: " << Gen_Num() << endl;
	for (int i = 0; i < 10; i++)
	{
		out << i << ". Fenotyp:\t" << pop.main_pop[i].p;
		out << "\tGenotyp:\t" << pop.main_pop[i].g << endl;
	}
}

/******************************************************************************/
/*                                             IMPLEMENTACJE KLASY POPULATION */
/******************************************************************************/

void population::init_generation()
{
	gen_num = 0;
	for (int i = 0; i < pop_size; i++)
		main_pop[i].init();
}

int population::next_generation()
{
	int i;
	n_cross = 0;
	n_mutation = 0;
	sum_fitness = 0;

	for (i = 0; i < pop_size; i++)
		sum_fitness += main_pop[i].fitness;

	for (i = 0; i < pop_size; i++)
		temp_pop[i] = select();

	for (i = 0; i < pop_size; i = i + 2)
		n_cross += temp_pop[i].cross_over(temp_pop[i + 1], p_cross);

	for (i = 0; i < pop_size; i++)
		n_mutation += temp_pop[i].mutation(p_mutation);

	for (i = 0; i < pop_size; i++)
		main_pop[i] = temp_pop[i];

	for (i = 0; i < pop_size; i++)
		main_pop[i].calculate_fitness();
	gen_num++;
	return gen_num;
}

individual population::select()
{
	// ????????????????????????????????????????????????????????????????????????? //
	// ?? TA METODE NALEZY ZAIMPLEMENTOWAC. Funkcja zwraca pojedynczego       ?? //
	// ?? osobnika wylosowanego metoda ruletki, z prawdopodobienstwem         ?? //
	// ?? proporcjonalnym do wartosci jego funkcji dostosowania.              ?? //
	// ?? W chwili obecnej funkcja zwraca przypadkowego osobnika.             ?? //
	// ????????????????????????????????????????????????????????????????????????? //
	double sum = 0;
	double fin = rnd_float();
	int i = 0;
	while (sum < fin)
	{
		i++;
		sum += this->main_pop[i].p.fitness_function() / this->sum_fitness;
	}

	return main_pop[i];
}

/******************************************************************************/
/*                                             IMPLEMENTACJE KLASY INDIVIDUAL */
/******************************************************************************/

void individual::init()
{
	g.random_init();
	calculate_fitness();
}

bool individual::cross_over(individual &ind, float p_cross)
{
	// ????????????????????????????????????????????????????????????????????????? //
	// ?? TA METODE NALEZY ZAIMPLEMENTOWAC. Funkcja zwraca true jesli doszlo  ?? //
	// ?? do krzyzowania i false w przypadku przeciwnym.                      ?? //
	// ?? Wewnatrz funkcji nalezy dokonac krzyzowania jednopunktowego.        ?? //
	// ?? Rodzicami sa osobnik wskazywany przez this oraz osobnik wskazywany  ?? //
	// ?? przez ind. Po krzyzowaniu te same wskazniki wskazuja na dzieci.     ?? //
	// ????????????????????????????????????????????????????????????????????????? //
	if (rnd_float() < p_cross)
	{
		int split = rnd_int(max_chrom_size - 1, 0);
		for (int i = 0; i <= split; i++)
		{
			bool tmp;
			tmp = this->g.chromosome[i];
			this->g.chromosome[i] = ind.g.chromosome[i];
			ind.g.chromosome[i] = tmp;
		}
		return true;
	}
	else
		return false;
}

void individual::calculate_fitness()
{
	p.decode(g);
	fitness = p.fitness_function();
}

/******************************************************************************/
/*                                               IMPLEMENTACJE KLASY GENOTYPE */
/******************************************************************************/

void genotype::random_init()
{
	int i;
	for (i = 0; i < max_chrom_size; i++)
		chromosome[i] = rnd_bool(0.5);
}

int genotype::mutation(float p_mut)
{
	// ????????????????????????????????????????????????????????????????????????? //
	// ?? TA METODE NALEZY ZAIMPLEMENTOWAC. Funkcja mutuje kazdy z genow      ?? //
	// ?? chromosomu z prawdopodobienstwem p_mut. Funkcja zwraca liczbe       ?? //
	// ?? mutacji do jakich doszlo w danym chromosomie.                       ?? //
	// ????????????????????????????????????????????????????????????????????????? //
	int n_mut = 0;
	for (int i = 0; i < max_chrom_size; i++)
	{
		if (rnd_float() < p_mut)
		{
			n_mut++;
			this->chromosome[i] = !this->chromosome[i];
		}
	}
	return n_mut;
}

/******************************************************************************/
/*                                              IMPLEMENTACJE KLASY PHENOTYPE */
/******************************************************************************/

// Funkcja dekoduje liczbe rzeczywista zapisana w kodzie dwojkowym na
// max_chrom_size pozycjach na liczbe zapisana w kodzie dziesietnym i wpisuje
// otrzymana wartosc do zmiennej x.
void phenotype::decode(genotype g)
{
	A = 0;
	B = 1;
	for(int i=max_chrom_size-1; i>=0; i--)
	{
		if(g.chromosome[i])
		{
			A += i+1;
		}
		else
		{
			B *= i+1;
		}
	}
}

// Funkcja dostosowania. UWAGA bedzie zmieniana. Prawdopodobnie zmieni sie
// rowniez dostep do niej.
float phenotype::fitness_function()
{
	return A_max / abs(A - A_0) + B_max / (B - B_0);
}
