#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <pqxx/pqxx>
using namespace std;

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct TRAN 
{
    string type;
    double value;
    TRAN* next;

    TRAN(string t, double v) : type(t), value(v), next(nullptr) {}
};

static string answer;
static string typeOfTran;
static double amount;
static double btcAdd;
static double ethAdd;
static double rightsAdd;
static double stocksAdd;
static int sumToHundred = 0;
static int heirsCounter;

void addTran(TRAN*&, string, double&);
void displayBtc(TRAN*, double&);
void displayEth(TRAN*, double&);
void displayRights(TRAN*, double&);
void displayStocks(TRAN*, double&);
void assets( string&, string&, double&, double&, double&, double&, pqxx::connection*, double&, double&, double&, double&);
void will( int, int, pqxx::connection* conn);

#endif