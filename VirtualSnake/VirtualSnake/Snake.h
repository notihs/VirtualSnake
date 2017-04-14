#pragma once

typedef struct Snake {
	int id;
	int size;
	int score;
	Effects effects;
};

typedef struct Effects {
	bool vodka;
	bool oil;
	bool glue;
};