//recipe.h
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//September 3, 2014
///////////////////////////////////

#ifndef RECIPE_H
#define RECIPE_H

struct recipeStruct
{
	int one;
	int two;
	int three;
	int result;
	recipeStruct() {one=0; two=0; three=0; result=0;}
};

class RecipeManager
{
public:
	RecipeManager();
	~RecipeManager();

	std::vector<recipeStruct> cookbook;
	void addRecipe(int result, int a, int b, int c);
	recipeStruct getRecipe(int index);
	bool isValidRecipe(int index);
};


#endif//RECIPE_H