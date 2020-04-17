#pragma once
#include <iostream>
#include "Lexer.h"
#include "GlobalHelper.h"
#include "Env.h";

using namespace std;

vector<Env*> envs;

Env* global_env = new Env(Global);
Env* current_env = new Env();