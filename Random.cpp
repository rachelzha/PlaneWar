#include "stdafx.h"
#include"Random.h"
#include<iostream>
using namespace std;


int random(int a, int b){
	int c = a;
	if(a!=b)c = (rand() % (b-a)) + a;
	return c;
}