#pragma once
#include <iostream>
#include "Lexer.h"
#include "GlobalHelper.h"
#include "Env.h";

using namespace std;

Env* global_env = new Env();
Env* current_env = new Env();