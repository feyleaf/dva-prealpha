//recipe.cpp
//feyleaf game studio
//by Benjamin C Watt (@feyleafgames)
//September 3, 2014
////////////////////////

#include "globals.h"

RecipeManager::RecipeManager()
{
	cookbook.clear();
}

RecipeManager::~RecipeManager()
{
	cookbook.clear();
}

void RecipeManager::addRecipe(int result, int a, int b, int c)
{
	recipeStruct dummy;
	dummy.one=a;
	dummy.two=b;
	dummy.three=c;
	dummy.result=result;
	if(result>0) cookbook.push_back(dummy);
}

recipeStruct RecipeManager::getRecipe(int index)
{
	if(isValidRecipe(index))
		return cookbook[index];
	return recipeStruct();
}

bool RecipeManager::isValidRecipe(int index)
{
	if(index<0) return false;
	if(index>=int(cookbook.size())) return false;
	if(cookbook[index].result<=0) return false;
	if(cookbook[index].one<0 || cookbook[index].two<0 || cookbook[index].three<0) return false;
	if((cookbook[index].one+cookbook[index].two+cookbook[index].three)<1) return false;

	return true;
}