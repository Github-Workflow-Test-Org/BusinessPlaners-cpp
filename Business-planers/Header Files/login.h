#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <pqxx/pqxx>
using namespace std;

static string username;
static string password;
static string passwordVer;
static char loginOrSignup;
static bool haveAccount = 1;

void login(string, string, string, bool, char, pqxx::connection*);
void sendToEcxcel(string, string);